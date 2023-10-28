/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#include "aptime.h"
#include "utils-private.h"

void save_start_time(struct operation* op)
{
    save_time(&(op->start_time));
}

void save_client_recieve_time(struct operation* op)
{
    save_time(&(op->client_time));
}

void save_intermediary_recieve_time(struct operation* op)
{
    save_time(&(op->intermed_time));
}

void save_enterprise_recieve_time(struct operation* op)
{
    save_time(&(op->enterp_time));
}