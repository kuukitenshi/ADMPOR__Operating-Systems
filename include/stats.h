/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "main.h"
#include <stdio.h>

// Função que abre, e cria caso não exista, o ficheiro de estatísticas cujo nome é passado por parâmetro.
FILE* open_statistics_file(const char* filename);

// Escreve as estatísticas no ficheiro de estatísticas.
void write_statistics_file(FILE* file, struct main_data* data);

// Função que fecha o ficheiro de estatísticas passado por parâmetro.
void close_statistics_file(FILE* file);

#endif

