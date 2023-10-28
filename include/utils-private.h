/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#ifndef UTILS_PRIVATE_H_GUARD
#define UTILS_PRIVATE_H_GUARD

#include "memory.h"
#include "main.h"
#include "synchronization.h"
#include <time.h>
#include <stdio.h>

// Constantes para definir diferentes códigos de saída dependendo do tipo de erro associado.
#define INVALID_ARG_COUNT_EXIT  1
#define INVALID_ARG_TYPE_EXIT   2
#define FORK_FAILED_EXIT        3
#define SHM_LINK_FAILED_EXIT    4
#define FTRUNCATE_FAILED_EXIT   5
#define MMAP_FAILED_EXIT        6
#define MUNMAP_FAILED_EXIT      7
#define SHM_UNLINK_FAILED_EXIT  8
#define SAVE_TIME_FAILED_EXIT   9
#define OPEN_FILE_FAILED_EXIT   10
#define CLOSE_FILE_FAILED_EXIT  11
#define SEM_CREATE_FAILED_EXIT  12
#define SEM_CLOSE_FAILED_EXIT   13
#define SEM_UNLINK_FAILED_EXIT  14
#define SEM_WAIT_FAILED_EXIT    15
#define SEM_POST_FAILED_EXIT    16
#define SIGACTION_FAILED_EXIT   17

/* Função que escreve uma operação no buffer de memória partilhada.
* A operação é escrita numa posição livre do buffer, 
* tendo em conta o tipo de buffer e as regras de escrita em buffers desse tipo.
* Se não houver nenhuma posição livre, não escreve nada.
*/
void write_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op);

/* Função que inicia um novo processo empresa através da função fork do SO. O novo
* processo irá executar a função dada por referência, fazendo exit do retorno.
* O processo pai devolve o pid do processo criado.
*/
int launch_process(int id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems, int(*execute_func)(int, struct comm_buffers*, struct main_data*, struct semaphores*));

// Função que escreve no stdout o menu de ajuda do programa.
void print_help_menu();

// Função que devolve a string formatada do tipo YYYY-MM-DD HH:MM:SS.MMM da estrutura de tempo dada.
void fprint_time(FILE* file, struct timespec time_for_str);

//Função que salva na estrutura dada o tempo corrente.
void save_time(struct timespec* time);

#endif