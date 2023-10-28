/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include <stdio.h>

//Função que abre o ficheiro de log cujo nome é passado por parâmetro.
FILE* open_log_file(const char* filename);

//Função que escreve no ficheiro log a operação dada.
void write_log_file(FILE* logFile, const char* operation);

//Função que fecha o ficheiro log passado por parâmetro.
void close_log_file(FILE* file);

#endif