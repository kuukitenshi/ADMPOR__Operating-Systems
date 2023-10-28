/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "memory.h"
#include "apsignal.h"
#include "process.h"
#include "client.h"
#include "intermediary.h"
#include "enterprise.h"
#include "utils-private.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

extern struct main_data* g_Data;
extern struct comm_buffers* g_Buffers;
extern struct semaphores* g_Sems;

void ignore_sigint_handler()
{
    struct sigaction sa = {0};
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("Failed to setup SIGINT signal handler.\n");
        exit(SIGACTION_FAILED_EXIT);
    }
}

void register_main_signal_handlers()
{
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("Failed to setup SIGINT signal handler.\n");
        exit(SIGACTION_FAILED_EXIT);
    }
    if (sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("Failed to setup SIGALRM signal handler.\n");
        exit(SIGACTION_FAILED_EXIT);
    }
    struct itimerval val = {0};
    val.it_interval.tv_sec = g_Data->alarm_time;
    val.it_value.tv_sec = 1;
    setitimer(ITIMER_REAL, &val, 0);
}

void print_stats()
{
    for(int i = 0; i < MAX_RESULTS && g_Data->results[i].id != -1; i++)
    {    
        semaphore_mutex_lock(g_Sems->results_mutex);
        struct operation op = g_Data->results[i];
        semaphore_mutex_unlock(g_Sems->results_mutex);
        printf("op:%d ", i);
        printf("status:%c ", op.status);
        printf("start_time:%ld ", op.start_time.tv_sec);
        printf("client:%d ", op.receiving_client);
        printf("client_time:%ld ", op.client_time.tv_sec);
        printf("intermediary:%d ", op.receiving_interm);
        printf("intermediary_time:%ld ", op.intermed_time.tv_sec);
        printf("enterprise:%d ", op.receiving_enterp);
        printf("enterprise_time:%ld\n", op.enterp_time.tv_sec);
    }
}


void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        stop_execution(g_Data, g_Buffers, g_Sems);
        exit(0);
    }
    else if (signal == SIGALRM)
    {
        print_stats();
    }
}