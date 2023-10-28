/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "client.h"
#include "aptime.h"
#include "synchronization.h"

int execute_client(int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems)
{
    struct operation op;
    int counter = 0;
    while (1)
    {
        client_get_operation(&op, client_id, buffers, data, sems);
        if (*data->terminate)
            break;
        if (op.id == -1)
            continue;
        client_process_operation(&op, client_id, data, &counter, sems);
        client_send_operation(&op, buffers, data, sems);
    }
    return counter;
}

void client_get_operation(struct operation *op, int client_id, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    if (*data->terminate)
        return;
    consume_begin(sems->main_cli);
    read_main_client_buffer(buffers->main_client, client_id, data->buffers_size, op);
    if (op->id == -1)
    {
        semaphore_mutex_unlock(sems->main_cli->mutex);
        semaphore_mutex_unlock(sems->main_cli->full);
    }
    else
        consume_end(sems->main_cli);
}

void client_process_operation(struct operation *op, int client_id, struct main_data *data, int *counter, struct semaphores *sems)
{
    op->receiving_client = client_id;
    op->status = 'C';
    save_client_recieve_time(op);
    (*counter)++;
    semaphore_mutex_lock(sems->results_mutex);
    data->results[op->id] = *op;
    semaphore_mutex_unlock(sems->results_mutex);
}

void client_send_operation(struct operation *op, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    produce_begin(sems->cli_intermed);
    write_client_interm_buffer(buffers->client_interm, data->buffers_size, op);
    produce_end(sems->cli_intermed);
}