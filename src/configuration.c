/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "configuration.h"
#include "utils-private.h"
#include <stdlib.h>
#include <stdio.h>

FILE* open_configuration_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open configuration file! (%s)\n", filename);
        exit(OPEN_FILE_FAILED_EXIT);
    }
    return file;
}

void read_configuration_file(FILE* file, struct main_data* data)
{
    fscanf(file, " %d", &data->max_ops);
    fscanf(file, " %d", &data->buffers_size);
    fscanf(file, " %d", &data->n_clients);
    fscanf(file, " %d", &data->n_intermediaries);
    fscanf(file, " %d", &data->n_enterprises);
    fscanf(file, " %s", data->log_filename);
    fscanf(file, " %s", data->statistics_filename);
    fscanf(file, " %d", &data->alarm_time);
}

void close_configuration_file(FILE* file)
{
    if (fclose(file) == EOF)
    {
        perror("Failed to close configuration file!");
        exit(CLOSE_FILE_FAILED_EXIT);
    }
}

