/*
* Projeto - Fase 2 - Sistemas Operativos
*
* Grupo 47:
* Rodrigo Correia   58180
* Laura Cunha       58188
* Guilherme Wind    58640
*/

#ifndef CLIENT_H_GUARD
#define CLIENT_H_GUARD

#include "memory.h"
#include "main.h"


/* Função principal de um Cliente. Deve executar um ciclo infinito onde em 
* cada iteração lê uma operação da main e se data->terminate ainda 
* for igual a 0, processa-a. Operações com id igual a -1 são ignoradas
* (op inválida) e se data->terminate for igual a 1 é porque foi dada ordem
* de terminação do programa, portanto deve-se fazer return do número de 
* operações processadas. Para efetuar estes passos, pode usar os outros
* métodos auxiliares definidos em client.h.
*/
int execute_client(int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems);


/* Função que lê uma operação do buffer de memória partilhada entre a main
* e clientes que seja direcionada a client_id. Antes de tentar ler a operação, deve
* verificar se data->terminate tem valor 1. Em caso afirmativo, retorna imediatamente da função.
*/
void client_get_operation(struct operation* op, int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems);


/* Função que processa uma operação, alterando o seu campo receiving_client para o id
* passado como argumento, alterando o estado da mesma para 'C' (client), e 
* incrementando o contador de operações. Atualiza também a operação na estrutura data.
*/
void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter, struct semaphores* sems);

/* Função que escreve uma operação no buffer de memória partilhada entre
* clientes e intermediários.
*/
void client_send_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems);

#endif
