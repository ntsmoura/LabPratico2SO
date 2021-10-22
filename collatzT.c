//Autores: Ariel Lima, Andre Paiva, Natan Moura, Pedro Augusto
//Implementacao de Collatz com Threads
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
//Corpo da Thread filha, para calculo da sequencia de collatz
void *thread_init (void* num)
{
   long int numero = (long int) num ;
   printf("%ld ",numero);
   while (numero!=1){
       if (numero%2!=0){
           numero = 3*numero + 1;
       }
       else{
           numero = numero/2;
       }
       printf("%ld ",numero);
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
   if(numero <= 0){
       printf("Entrada inválida (Digite um inteiro positivo). Encerrando...");
   }
   else{
    //Criacao da thread filha e passagem de parametros como corpo e valor para collatz
    status = pthread_create (&thread, NULL, thread_init,(void*) numero) ;
    if (status)
    {
        perror ("Falha ao criar thread filha") ;
        exit (1) ;
    }
    else{
        //Join na thread filha criada
        pthread_join(thread, NULL);
        printf("\nOperação concluída. Encerrando...");
    }
    pthread_exit (NULL) ;
   }
}

