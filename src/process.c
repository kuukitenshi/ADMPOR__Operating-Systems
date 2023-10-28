/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "process.h"
#include "client.h"
#include "intermediary.h"
#include "enterprise.h"
#include "utils-private.h"
#include <sys/wait.h>

int launch_client(int client_id, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    return launch_process(client_id, buffers, data, sems, execute_client);
}

int launch_interm(int interm_id, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    return launch_process(interm_id, buffers, data, sems, execute_intermediary);
}

int launch_enterp(int enterp_id, struct comm_buffers *buffers, struct main_data *data, struct semaphores *sems)
{
    return launch_process(enterp_id, buffers, data, sems, execute_enterprise);
}

int wait_process(int process_id)
{
    int status;
    waitpid(process_id, &status, 0);
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    return -1;
}