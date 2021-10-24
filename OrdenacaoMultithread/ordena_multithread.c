//Autores: Ariel Lima, Andre Paiva, Natan Moura, Pedro Augusto
//Implementacao de Ordenação com Threads
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int *vetor;
int tamanho_vetor;

int main(){
    int count_input;
    scanf("%d", &tamanho_vetor);
    vetor = (int*)malloc(tamanho_vetor * sizeof(int));
    for(count_input = 0; count_input < tamanho_vetor; count_input++)
        scanf("%d", &vetor[count_input]);
    
    free(vetor);
}