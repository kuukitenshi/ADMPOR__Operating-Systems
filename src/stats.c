/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "stats.h"
#include "utils-private.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

FILE *open_statistics_file(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open statistics file! (%s)\n", filename);
        exit(OPEN_FILE_FAILED_EXIT);
    }
    return file;
}

void write_statistics_file(FILE *file, struct main_data *data)
{
    fputs("Process Statistics: \n", file);
    for (int i = 0; i < data->n_clients; i++)
    {
        if(data->client_stats[i] > 0)
            fprintf(file, "\tClient %d received %d operation(s) ! \n", i, data->client_stats[i]);
    }
    for (int i = 0; i < data->n_intermediaries; i++)
    {
        if(data->intermediary_stats[i] > 0)
            fprintf(file, "\tIntermediary %d prepared %d operation(s) ! \n", i, data->intermediary_stats[i]);
    }
    for (int i = 0; i < data->n_enterprises; i++)
    {
        if(data->enterprise_stats[i] > 0)
            fprintf(file, "\tEnterprise %d executed %d operation(s) ! \n", i, data->enterprise_stats[i]);
    }
    fputs("\nRequest Statistics: \n", file);
    for (int i = 0; i < MAX_RESULTS && data->results[i].id != -1; i++)
    {
        struct timespec start_time = data->results[i].start_time;
        struct timespec enterp_time = data->results[i].enterp_time;
        long seg_dif = enterp_time.tv_sec - start_time.tv_sec;
        long milis_dif = ((enterp_time.tv_nsec - start_time.tv_nsec) / 1000000) % 1000;

        fprintf(file, "Request: %d\n", i);
        fprintf(file, "Status: %c\n", data->results[i].status);
        fprintf(file, "Intermediary id: %d\n", data->results[i].receiving_interm);
        fprintf(file, "Enterprise id: %d\n", data->results[i].receiving_enterp);
        fprintf(file, "Client id: %d\n", data->results[i].receiving_client);

        fputs("Start time: ", file);
        fprint_time(file, start_time);
        fputs("\n", file);

        fputs("Client time: ", file);
        fprint_time(file, data->results[i].client_time);
        fputs("\n", file);

        fputs("Intermediary time: ", file);
        fprint_time(file, data->results[i].intermed_time);
        fputs("\n", file);

        fputs("Enterprise time: ", file);
        fprint_time(file, data->results[i].enterp_time);
        fputs("\n", file);

        fprintf(file, "Total time: %ld.%03ld\n\n", seg_dif, milis_dif);
    }
}

void close_statistics_file(FILE *file)
{
    if (fclose(file) == EOF)
    {
        perror("Failed to close statistics file!");
        exit(CLOSE_FILE_FAILED_EXIT);
    }
}
