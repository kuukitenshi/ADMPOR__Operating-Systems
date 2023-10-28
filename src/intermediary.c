/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "intermediary.h"
#include "aptime.h"
#include "synchronization.h"

int execute_intermediary(int interm_id, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    struct operation op;
    int counter = 0;
    while (1)
    {
        intermediary_receive_operation(&op, buffers, data, sems);
        if (*data->terminate)
            break;
        if (op.id == -1)
            continue;
        intermediary_process_operation(&op, interm_id, data, &counter, sems);
        intermediary_send_answer(&op, buffers, data, sems);
    }
    return counter;
}

void intermediary_receive_operation(struct operation *op, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    if (*data->terminate)
        return;
    consume_begin(sems->cli_intermed);
    read_client_interm_buffer(buffers->client_interm, data->buffers_size, op);
    consume_end(sems->cli_intermed);
}

void intermediary_process_operation(struct operation *op, int interm_id, struct main_data *data, int *counter, struct semaphores *sems)
{
    op->receiving_interm = interm_id;
    op->status = 'I';
    save_intermediary_recieve_time(op);
    (*counter)++;
    semaphore_mutex_lock(sems->results_mutex);
    data->results[op->id] = *op;
    semaphore_mutex_unlock(sems->results_mutex);
}

void intermediary_send_answer(struct operation *op, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    produce_begin(sems->intermed_enterp);
    write_interm_enterp_buffer(buffers->interm_enterp, data->buffers_size, op);
    produce_end(sems->intermed_enterp);
}