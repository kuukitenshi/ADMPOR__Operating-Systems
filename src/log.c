/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "log.h"
#include "utils-private.h"
#include <time.h>
#include <stdlib.h>

FILE* open_log_file(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Failed to open log file! (%s)\n", filename);
        exit(OPEN_FILE_FAILED_EXIT);
    }
    return file;
}

void write_log_file(FILE* logFile, const char* operation)
{
    struct timespec now;
    if(clock_gettime(CLOCK_REALTIME, &now) == -1)
    {
        perror("Failed getting time for log file.");
        exit(SAVE_TIME_FAILED_EXIT);
    }
    fprint_time(logFile, now);
    fprintf(logFile, " %s\n", operation);
}

void close_log_file(FILE* file)
{
    if (fclose(file) == EOF)
    {
        perror("Failed to close log file!");
        exit(CLOSE_FILE_FAILED_EXIT);
    }
}