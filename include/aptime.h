/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#ifndef APTIME_H_GUARD
#define APTIME_H_GUARD

#include "memory.h"

// Função que guarda o tempo de inicio na estrutura operation.
void save_start_time(struct operation* op);

// Função que guarda o instante em que o cliente recebeu o pedido na estrutura operation.
void save_client_recieve_time(struct operation* op);

// Função que guarda o instante em que o intermediário recebeu o pedido na estrutura operation.
void save_intermediary_recieve_time(struct operation* op);

// Função que guarda o instante em que a empresa recebeu o pedido na estrutura operation.
void save_enterprise_recieve_time(struct operation* op);

#endif