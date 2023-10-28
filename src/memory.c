/*
 * Projeto - Fase 2 - Sistemas Operativos
 *
 * Grupo 47:
 * Rodrigo Correia   58180
 * Laura Cunha       58188
 * Guilherme Wind    58640
 */

#include "memory.h"
#include "utils-private.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

void *create_shared_memory(char *name, int size)
{
    char shmName[100];
    sprintf(shmName, "/%s_%d", name, getuid());
    int fd = shm_open(shmName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        fprintf(stderr, "Failed to create shared memory with full name %s\n", shmName);
        exit(SHM_LINK_FAILED_EXIT);
    }
    if (ftruncate(fd, size) == -1)
    {
        fprintf(stderr, "Failed to truncate shared memory %s\n", shmName);
        exit(FTRUNCATE_FAILED_EXIT);
    }
    void *memoryPtr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memoryPtr == MAP_FAILED)
    {
        fprintf(stderr, "Failed to map shared memory with full name %s\n", shmName);
        exit(MMAP_FAILED_EXIT);
    }
    memset(memoryPtr, 0, size);
    return memoryPtr;
}

void *create_dynamic_memory(int size)
{
    return calloc(1, size); // allocate memory and initializes all bytes allocated to zero
}

void destroy_shared_memory(char *name, void *ptr, int size)
{
    char shmName[100];
    sprintf(shmName, "/%s_%d", name, getuid());
    if (munmap(ptr, size) == -1)
    {
        fprintf(stderr, "Failed to unmap shared memory with full name %s\n", shmName);
        exit(MUNMAP_FAILED_EXIT);
    }
    if (shm_unlink(shmName) == -1)
    {
        fprintf(stderr, "Failed to unlink shared memory with full name %s\n", shmName);
        exit(SHM_UNLINK_FAILED_EXIT);
    }
}

void destroy_dynamic_memory(void *ptr)
{
    free(ptr);
}

void write_main_client_buffer(struct rnd_access_buffer *buffer, int buffer_size, struct operation *op)
{
    write_rnd_access_buffer(buffer, buffer_size, op);
}

void write_client_interm_buffer(struct circular_buffer *buffer, int buffer_size, struct operation *op)
{
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    int buffer_index = (in + 1) % buffer_size;
    if (buffer_index == out)
        return;
    buffer->buffer[in] = *op;
    buffer->ptrs->in = buffer_index;
}

void write_interm_enterp_buffer(struct rnd_access_buffer *buffer, int buffer_size, struct operation *op)
{
    write_rnd_access_buffer(buffer, buffer_size, op);
}

void read_main_client_buffer(struct rnd_access_buffer *buffer, int client_id, int buffer_size, struct operation *op)
{
    for (int i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] && buffer->buffer[i].requesting_client == client_id)
        {
            *op = buffer->buffer[i];
            buffer->ptrs[i] = 0;
            return;
        }
    }
    op->id = -1;
}

void read_client_interm_buffer(struct circular_buffer *buffer, int buffer_size, struct operation *op)
{
    int in = buffer->ptrs->in;
    int out = buffer->ptrs->out;
    if (in == out)
        op->id = -1;
    else
    {
        *op = buffer->buffer[out];
        buffer->ptrs->out = (out + 1) % buffer_size;
    }
}

void read_interm_enterp_buffer(struct rnd_access_buffer *buffer, int enterp_id, int buffer_size, struct operation *op)
{
    for (int i = 0; i < buffer_size; i++)
    {
        if (buffer->ptrs[i] && buffer->buffer[i].requested_enterp == enterp_id)
        {
            *op = buffer->buffer[i];
            buffer->ptrs[i] = 0;
            return;
        }
    }
    op->id = -1;
}
