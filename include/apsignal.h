/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#ifndef APSIGNAL_H_GUARD
#define APSIGNAL_H_GUARD

#include "memory.h"
#include "main.h"
#include <semaphore.h>

#endif

/*
 * Função que guarda as estuturas passadas por parâmetro em variáveis estáticas e regista os sinais
 * de alarm e de ctrlC.
 */
void register_main_signal_handlers();

/*
 * Escreve para o ecrã o estado atual de todos os pedidos, incluindo: 
 * (i) os que já foram finalizados pelas empresas e (ii) os que ainda estão em andamento ou agendados.
*/
void print_stats();

/* Função que dependente do sinal recebido apresenta os seguintes comportamentos:
 * - termina o programa caso receba o sinal SIGINT.
 * - escreve as estatisticas no terminal caso receba o sinal SIGALARM.
 */
void signal_handler(int signal);

/* Função que ignora o sinal SIGINT para ser herdada pelos processos filhos, fazendo 
 * com que apenas a função main reaja ao sinal SIGINT.
 */
void ignore_sigint_handler();
