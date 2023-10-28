/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "synchronization.h"
#include "utils-private.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

sem_t *semaphore_create(char *name, int value)
{
    char idName[100];
    sprintf(idName, "/%s_%d", name, getuid());
    sem_t* result = sem_open(idName, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, value);
    if (result == SEM_FAILED) {
        fprintf(stderr, "Failed to create semaphore named %s.\n", name);
        exit(SEM_CREATE_FAILED_EXIT);
    }
    return result;
}

void semaphore_destroy(char *name, sem_t *semaphore)
{
    char idName[100];
    sprintf(idName, "/%s_%d", name, getuid());
    if (sem_close(semaphore) == -1) //apaga
    { 
        fprintf(stderr, "Failed to close semaphore named %s.\n", name);
        exit(SEM_CLOSE_FAILED_EXIT);
    }
    if (sem_unlink(idName) == -1) //liberta 
    {
        fprintf(stderr, "Failed to unlink semaphore named %s.\n", idName);;
        exit(SEM_UNLINK_FAILED_EXIT);
    }
}

void produce_begin(struct prodcons *pc)
{
    semaphore_mutex_lock(pc->empty);
    semaphore_mutex_lock(pc->mutex);
}

void produce_end(struct prodcons *pc)
{
    semaphore_mutex_unlock(pc->mutex);
    semaphore_mutex_unlock(pc->full);
}

void consume_begin(struct prodcons *pc)
{
    semaphore_mutex_lock(pc->full);
    semaphore_mutex_lock(pc->mutex);
}

void consume_end(struct prodcons *pc)
{
    semaphore_mutex_unlock(pc->mutex);
    semaphore_mutex_unlock(pc->empty);
}

void semaphore_mutex_lock(sem_t *sem)
{
    if (sem_wait(sem) == -1)
    {
        perror("Failed waiting for semaphore.");
        exit(SEM_WAIT_FAILED_EXIT);
    }
}

void semaphore_mutex_unlock(sem_t *sem)
{
    if(sem_post(sem) == -1){
        perror("Failed to post a semaphore.");
        exit(SEM_POST_FAILED_EXIT);
    }
}