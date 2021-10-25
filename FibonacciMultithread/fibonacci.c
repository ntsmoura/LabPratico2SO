//Autores: Ariel Lima, Andre Paiva, Natan Moura, Pedro Augusto
//Implementacao de Fibonacci com Threads
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int *sequenciaglobal;

//Corpo da Thread filha, para calculo da sequencia de Fibonacci
void *thread_init (void* num) {
    long int numero = (long int) num ;
    for (int i = 0; i < numero; i++) {
        if (i == 0) {
            sequenciaglobal[i] = 0;
        } else if (i == 1) {
            sequenciaglobal[i] = 1;
        } else {
            sequenciaglobal[i] = sequenciaglobal[i-1] + sequenciaglobal[i-2];
        }
    }
   pthread_exit (NULL) ;
}
 
//Main da aplicacao
int main (int argc, char *argv[])
{
   //Definicao de string de entrada e variaveis
   pthread_t thread;
   char entrada[10];
   long int numero, status;

   //Entrada de dados
   fgets (entrada, 10, stdin);
   //Conversao da entrada para long int, retorna 0 se entrada nao inteira
   numero = atol(entrada);

    //Checa validade da entrada
   if (numero <= 0) {
       printf("Entrada inválida (Digite um inteiro maior ou igual a zero). Encerrando...");
   } else {
    sequenciaglobal = malloc(sizeof(int) * numero);

    //Criacao da thread filha com o N de Fibonacci e o array onde se guardará os resultados da operação
    status = pthread_create (&thread, NULL, thread_init, (void *) numero);
    if (status) {
        perror ("Falha ao criar thread filha") ;
        exit (1) ;
    } else {
        //Join na thread filha criada
        pthread_join(thread, NULL);
        printf("\nOperação concluída. Imprimindo a sequência de Fibonacci: \n");
        for (int i = 0; i < numero; i++) {
            printf("%d\n", sequenciaglobal[i]);
        }
    }
    pthread_exit (NULL) ;
   }
}

