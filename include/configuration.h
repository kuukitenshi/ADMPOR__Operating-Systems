/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#ifndef CONFIGURATION_H_GUARD
#define CONFIGURATION_H_GUARD

#include "main.h"
#include <stdio.h>

//Função que abre o ficheiro de configuração cujo nome é passado por parâmetro.
FILE* open_configuration_file(const char* filename);

//Função que le o ficheiro de configuração e guarda o seu conteúdo nas respetivas estruturas.
void read_configuration_file(FILE* file, struct main_data* data);

//Função que fecha o ficheiro de configuração passado por parâmetro.
void close_configuration_file(FILE* file);

#endif