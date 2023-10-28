/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "enterprise.h"
#include "aptime.h"
#include "synchronization.h"

int execute_enterprise(int enterp_id, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    struct operation op;
    int counter = 0;
    while (1)
    {
        enterprise_receive_operation(&op, enterp_id, buffers, data, sems);
        if (*data->terminate)
            break;
        if (op.id == -1)
            continue;
        enterprise_process_operation(&op, enterp_id, data, &counter, sems);
    }
    return counter;
}

void enterprise_receive_operation(struct operation *op, int enterp_id, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    if (*data->terminate)
        return;
    consume_begin(sems->intermed_enterp);
    read_interm_enterp_buffer(buffers->interm_enterp, enterp_id, data->buffers_size, op);
    if (op->id == -1)
    {
        semaphore_mutex_unlock(sems->intermed_enterp->mutex);
        semaphore_mutex_unlock(sems->intermed_enterp->full);
    }
    else
        consume_end(sems->intermed_enterp);
}

void enterprise_process_operation(struct operation *op, int enterp_id, struct main_data *data, int *counter, struct semaphores *sems)
{
    op->receiving_enterp = enterp_id;
    op->status = op->id >= data->max_ops ? 'A' : 'E';
    save_enterprise_recieve_time(op);
    (*counter)++;
    semaphore_mutex_lock(sems->results_mutex);
    data->results[op->id] = *op;
    semaphore_mutex_unlock(sems->results_mutex);
}

