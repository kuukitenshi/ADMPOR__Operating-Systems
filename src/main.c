/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "main.h"
#include "configuration.h"
#include "log.h"
#include "synchronization.h"
#include "process.h"
#include "utils-private.h"
#include "aptime.h"
#include "stats.h"
#include "apsignal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static FILE* s_LogFile;

struct main_data* g_Data;
struct comm_buffers* g_Buffers;
struct semaphores* g_Sems;

int main(int argc, char *argv[])
{
    // init data structures
    g_Data = create_dynamic_memory(sizeof(struct main_data));
    g_Buffers = create_dynamic_memory(sizeof(struct comm_buffers));
    g_Buffers->main_client = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    g_Buffers->client_interm = create_dynamic_memory(sizeof(struct circular_buffer));
    g_Buffers->interm_enterp = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    // init semaphore data structure
    g_Sems = create_dynamic_memory(sizeof(struct semaphores));
    g_Sems->main_cli = create_dynamic_memory(sizeof(struct prodcons));
    g_Sems->cli_intermed = create_dynamic_memory(sizeof(struct prodcons));
    g_Sems->intermed_enterp = create_dynamic_memory(sizeof(struct prodcons));
    // execute main code
    main_args(argc, argv, g_Data);
    create_dynamic_memory_buffers(g_Data);
    create_shared_memory_buffers(g_Data, g_Buffers);
    create_semaphores(g_Data, g_Sems);
    launch_processes(g_Buffers, g_Data, g_Sems);
    register_main_signal_handlers(g_Data, g_Buffers, g_Sems);
    user_interaction(g_Buffers, g_Data, g_Sems);

    return 0;
}

void main_args(int argc, char *argv[], struct main_data *data)
{
    if (argc != 2)
    {
        perror("Invalid arguments, execute the program with the config file name as argument!\n");
        printf("Example: %s <config file>\n", argv[0]);
        exit(INVALID_ARG_COUNT_EXIT);
    }
    FILE* configFile = open_configuration_file(argv[1]);
    read_configuration_file(configFile, data);
    close_configuration_file(configFile);
    s_LogFile = open_log_file(data->log_filename);
}

void create_dynamic_memory_buffers(struct main_data *data)
{
    data->client_pids = create_dynamic_memory(data->n_clients * sizeof(int));
    data->intermediary_pids = create_dynamic_memory(data->n_intermediaries * sizeof(int));
    data->enterprise_pids = create_dynamic_memory(data->n_enterprises * sizeof(int));

    data->client_stats = create_dynamic_memory(data->n_clients * sizeof(int));
    data->intermediary_stats = create_dynamic_memory(data->n_intermediaries * sizeof(int));
    data->enterprise_stats = create_dynamic_memory(data->n_enterprises * sizeof(int));
}

void create_shared_memory_buffers(struct main_data *data, struct comm_buffers *buffers)
{
    buffers->main_client->ptrs = create_shared_memory(STR_SHM_MAIN_CLIENT_PTR, data->buffers_size * sizeof(int));
    buffers->client_interm->ptrs = create_shared_memory(STR_SHM_CLIENT_INTERM_PTR, sizeof(struct pointers));
    buffers->interm_enterp->ptrs = create_shared_memory(STR_SHM_INTERM_ENTERP_PTR, data->buffers_size * sizeof(int));

    buffers->main_client->buffer = create_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, data->buffers_size * sizeof(struct operation));
    buffers->client_interm->buffer = create_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, data->buffers_size * sizeof(struct operation));
    buffers->interm_enterp->buffer = create_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, data->buffers_size * sizeof(struct operation));

    data->results = create_shared_memory(STR_SHM_RESULTS, MAX_RESULTS * sizeof(struct operation));
    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));

    *data->terminate = 0;
    for (int i = 0; i < MAX_RESULTS; i++)
    {
        data->results[i].id = -1;
    }
    for (int i = 0; i < data->buffers_size; i++)
    {
        buffers->main_client->buffer[i].id = -1;
        buffers->client_interm->buffer[i].id = -1;
        buffers->interm_enterp->buffer[i].id = -1;
    }
}

void launch_processes(struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    for (int i = 0; i < data->n_clients; i++)
    {
        data->client_pids[i] = launch_client(i, buffers, data, sems);
    }
    for (int i = 0; i < data->n_intermediaries; i++)
    {
        data->intermediary_pids[i] = launch_interm(i, buffers, data, sems);
    }
    for (int i = 0; i < data->n_enterprises; i++)
    {
        data->enterprise_pids[i] = launch_enterp(i, buffers, data, sems);
    }
}

void user_interaction(struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    int op_counter = 0;
    char command[7] = "";
    print_help_menu();
    while (1)
    {
        printf("$> ");
        memset(command, 0, sizeof(command));
        if (scanf("%6s", command) == EOF ) {
            break;
        }
        if (!strcmp(command, "op"))
        {
            create_request(&op_counter, buffers, data, sems);
        }
        else if (!strcmp(command, "status"))
        {
            read_status(data, sems);
        }
        else if (!strcmp(command, "stop"))
        {
            write_log_file(s_LogFile, "stop");
            stop_execution(data, buffers, sems);
            break;
        }
        else if (!strcmp(command, "help"))
        {
            print_help_menu();
            write_log_file(s_LogFile, "help");
        }
        else
        {
            fprintf(stderr, "(!) Invalid command. Type 'help' to get the list of menus.\n");
        }
        // Limpar os restantes caracteres presentes no stdin não lidos pelo scanf
        int c;
        do
        {
            c = getchar();
        } while (c != EOF && c != '\n');
    }
}

void create_request(int *op_counter, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    int clientId;
    int enterpriseId;
    scanf(" %d %d", &clientId, &enterpriseId);
    char logOperation[100];
    sprintf(logOperation, "op %d %d", clientId, enterpriseId);
    write_log_file(s_LogFile, logOperation);
    if (*op_counter == MAX_RESULTS)
    {
        fprintf(stderr, "(!) You cannot create more than %d operations.\n", MAX_RESULTS);
        return;
    }
    if (clientId < 0 || clientId >= data->n_clients)
    {
        fprintf(stderr, "(!) Invalid client id (%d) the maximum number of clients is %d, therefore the id must be between 0 and %d\n", clientId, data->n_clients, data->n_clients - 1);
        return;
    }
    if (enterpriseId < 0 || enterpriseId >= data->n_enterprises)
    {
        fprintf(stderr, "(!) Invalid enterprise id (%d) the maximum number of enterprises is %d, therefore the id must be between 0 and %d\n", enterpriseId, data->n_enterprises, data->n_enterprises - 1);
        return;
    }
    struct operation op = {0};
    save_start_time(&op);
    op.status = 'M';
    op.id = *op_counter;
    op.requesting_client = clientId;
    op.requested_enterp = enterpriseId;
    op.receiving_client = -1;
    op.receiving_interm = -1;
    op.receiving_enterp = -1;
    semaphore_mutex_lock(sems->results_mutex);
    data->results[op.id] = op;
    semaphore_mutex_unlock(sems->results_mutex);
    produce_begin(sems->main_cli);
    write_main_client_buffer(buffers->main_client, data->buffers_size, &op);
    produce_end(sems->main_cli);
    printf("Created request with id #%d\n", op.id);
    (*op_counter)++;
}

void read_status(struct main_data *data, struct semaphores *sems)
{
    int numb;
    scanf(" %d", &numb);
    char logOperation[100];
    sprintf(logOperation, "status %d", numb);
    write_log_file(s_LogFile, logOperation);
    if (numb >= MAX_RESULTS)
    {
        fprintf(stderr, "(!) The maximum number of operations set by the system admin is %d, therefore the maximum possible id is %d\n", MAX_RESULTS, MAX_RESULTS - 1);
        return;
    }
    if (numb < 0)
    {
        fprintf(stderr, "(!) All operations ids need to be greater then or equal to 0.\n");
        return;
    }
    semaphore_mutex_lock(sems->results_mutex);
    struct operation op = data->results[numb];
    semaphore_mutex_unlock(sems->results_mutex);
    if (op.id == -1)
    {
        fprintf(stderr, "(!) No operation with id %d has been found. Insert a valid operation id.\n", numb);
        return;
    }
    printf("<------------------ Operation #%d Stats ------------------>\n", numb);
    printf("\tStatus: %c\n", op.status);
    printf("\tRequest client id: %d\n", op.requesting_client);
    printf("\tRequested enterprise id: %d\n", op.requested_enterp);
    int recivClient = op.receiving_client;
    if (recivClient != -1)
        printf("\tReceived client id: %d\n", recivClient);
    else
        puts("\tThe operation hasn't yet been received by the client!");
    int recivInterm = op.receiving_interm;
    if (recivInterm != -1)
        printf("\tReceived intermediary id: %d\n", recivInterm);
    else
        puts("\tThe operation hasn't yet been received by the intermediary!");
    int recivEnterp = op.receiving_enterp;
    if (recivEnterp != -1)
        printf("\tReceived enterprise id: %d\n", recivEnterp);
    else
        puts("\tThe operation hasn't yet been received by the enterprise!");
    puts("<--------------------------------------------------------->\n");
}

void stop_execution(struct main_data *data, struct comm_buffers *buffers, struct semaphores *sems)
{
    *data->terminate = 1;
    close_log_file(s_LogFile);
    wakeup_processes(data, sems);
    wait_processes(data);
    destroy_semaphores(sems);
    write_statistics(data);
    destroy_memory_buffers(data, buffers);
}

void wait_processes(struct main_data *data)
{
    for (int i = 0; i < data->n_clients; i++)
    {
        data->client_stats[i] = wait_process(data->client_pids[i]);
    }
    for (int i = 0; i < data->n_intermediaries; i++)
    {
        data->intermediary_stats[i] = wait_process(data->intermediary_pids[i]);
    }
    for (int i = 0; i < data->n_enterprises; i++)
    {
        data->enterprise_stats[i] = wait_process(data->enterprise_pids[i]);
    }
}

void write_statistics(struct main_data *data)
{
    FILE* statsFile = open_statistics_file(data->statistics_filename);
    write_statistics_file(statsFile, data);
    close_statistics_file(statsFile);
}

void destroy_memory_buffers(struct main_data *data, struct comm_buffers *buffers)
{
    // memoria dinâmica
    destroy_dynamic_memory(data->client_pids);
    destroy_dynamic_memory(data->intermediary_pids);
    destroy_dynamic_memory(data->enterprise_pids);
    destroy_dynamic_memory(data->client_stats);
    destroy_dynamic_memory(data->intermediary_stats);
    destroy_dynamic_memory(data->enterprise_stats);

    // memoria partilhada
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_PTR, buffers->main_client->ptrs, data->buffers_size * sizeof(int));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_PTR, buffers->client_interm->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_PTR, buffers->interm_enterp->ptrs, data->buffers_size * sizeof(int));

    destroy_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, buffers->main_client->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, buffers->client_interm->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, buffers->interm_enterp->buffer, data->buffers_size * sizeof(struct operation));

    destroy_shared_memory(STR_SHM_RESULTS, data->results, MAX_RESULTS * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));

    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_client);
    destroy_dynamic_memory(buffers->client_interm);
    destroy_dynamic_memory(buffers->interm_enterp);
    destroy_dynamic_memory(buffers);
    destroy_dynamic_memory(g_Sems->main_cli);
    destroy_dynamic_memory(g_Sems->cli_intermed);
    destroy_dynamic_memory(g_Sems->intermed_enterp);
    destroy_dynamic_memory(g_Sems);
}

void create_semaphores(struct main_data *data, struct semaphores *sems)
{
    sems->main_cli->full = semaphore_create(STR_SEM_MAIN_CLI_FULL, 0);
    sems->main_cli->empty = semaphore_create(STR_SEM_MAIN_CLI_EMPTY, data->buffers_size);
    sems->main_cli->mutex = semaphore_create(STR_SEM_MAIN_CLI_MUTEX, 1);

    sems->cli_intermed->full = semaphore_create(STR_SEM_CLI_INTERMED_FULL, 0);
    sems->cli_intermed->empty = semaphore_create(STR_SEM_CLI_INTERMED_EMPTY, data->buffers_size);
    sems->cli_intermed->mutex = semaphore_create(STR_SEM_CLI_INTERMED_MUTEX, 1);

    sems->intermed_enterp->full = semaphore_create(STR_SEM_INTERMED_ENTERP_FULL, 0);
    sems->intermed_enterp->empty = semaphore_create(STR_SEM_INTERMED_ENTERP_EMPTY, data->buffers_size);
    sems->intermed_enterp->mutex = semaphore_create(STR_SEM_INTERMED_ENTERP_MUTEX, 1);

    sems->results_mutex = semaphore_create(STR_SEM_RESULTS_MUTEX, 1);
}

void wakeup_processes(struct main_data *data, struct semaphores *sems)
{
    produce_end(sems->main_cli);
    for (int i = 0; i < data->n_clients; i++)
    {
        produce_end(sems->cli_intermed);
    }
    for (int i = 0; i < data->n_intermediaries; i++)
    {
        produce_end(sems->intermed_enterp);
    }
}

void destroy_semaphores(struct semaphores *sems)
{
    semaphore_destroy(STR_SEM_MAIN_CLI_FULL, sems->main_cli->full);
    semaphore_destroy(STR_SEM_MAIN_CLI_EMPTY, sems->main_cli->empty);
    semaphore_destroy(STR_SEM_MAIN_CLI_MUTEX, sems->main_cli->mutex);

    semaphore_destroy(STR_SEM_CLI_INTERMED_FULL, sems->cli_intermed->full);
    semaphore_destroy(STR_SEM_CLI_INTERMED_EMPTY, sems->cli_intermed->empty);
    semaphore_destroy(STR_SEM_CLI_INTERMED_MUTEX, sems->cli_intermed->mutex);

    semaphore_destroy(STR_SEM_INTERMED_ENTERP_FULL, sems->intermed_enterp->full);
    semaphore_destroy(STR_SEM_INTERMED_ENTERP_EMPTY, sems->intermed_enterp->empty);
    semaphore_destroy(STR_SEM_INTERMED_ENTERP_MUTEX, sems->intermed_enterp->mutex);

    semaphore_destroy(STR_SEM_RESULTS_MUTEX, sems->results_mutex);
}