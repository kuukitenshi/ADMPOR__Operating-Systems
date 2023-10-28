/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "utils-private.h"
#include "apsignal.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

void write_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op)
{ 
    for (int i = 0; i < buffer_size; i++)
    {
        if (!buffer->ptrs[i])
        {
            buffer->ptrs[i] = 1;
            buffer->buffer[i] = *op;
            break;
        }
    }
}

int launch_process(int id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems, int(*execute_func)(int, struct comm_buffers*, struct main_data*, struct semaphores*))
{
    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "Failed to create child process. %d\n", id);
        exit(FORK_FAILED_EXIT);
    }
    else if (pid == 0)
    {
        ignore_sigint_handler();
        int ret = execute_func(id, buffers, data, sems);
        exit(ret);
    }
    return pid;
}

void print_help_menu() 
{
    puts("<----------------------------- AdmPor Commands ----------------------------->");
    puts(" op <cliente> <empresa> - Cria um novo pedido por parte do cliente.");
    puts(" status <id>            - Consultar os estado do pedido especificado por id.");
    puts(" stop                   - Termina a execuçãodo sistema AdmPor.");
    puts(" help                   - Mostrar as informações de ajuda.\n");
}

void fprint_time(FILE* file, struct timespec time_for_str)
{
    time_t time_milisecs = time_for_str.tv_nsec / 1000000; //nano to miliseg
    struct tm* date_info = localtime(&time_for_str.tv_sec);
    fprintf(file, "%d-%d-%d %d:%d:%d.%03ld",
            1900 + date_info->tm_year, date_info->tm_mon+1, date_info->tm_mday, date_info->tm_hour, date_info->tm_min, date_info->tm_sec, time_milisecs);
}

void save_time(struct timespec* time)
{
    if(clock_gettime(CLOCK_REALTIME, time) == -1)
    {
        perror("Failed saving time.");
        exit(SAVE_TIME_FAILED_EXIT);
    }
}