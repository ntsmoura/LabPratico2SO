//Autores: Ariel Lima, Andre Paiva, Natan Moura, Pedro Augusto
//Implementacao de Ordenação com Threads
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

long long *vetor;
unsigned int tamanho_vetor;

/*
typedef struct{
    int indice;
    int tamanho;
} args_divide;

typedef struct{
    int indice_a;
    int tamanho_a;
    int indice_b;
    int tamanho_b;
} args_main;

void *thread_main(void *args){

}

void *thread_divide(void *args){

}
*/

void dividir(int i, unsigned int size);
void ordenar(int a, unsigned int sizea, int b, unsigned int sizeb);

void dividir(int i, unsigned int size){
    int x, y;
    unsigned int sizex, sizey;
    x = i;
    sizex = size - (size/2);
    y = x + sizex;
    sizey = size/2;
    ordenar(x, sizex, y, sizey);
}

void ordenar(int a, unsigned int sizea, int b, unsigned int sizeb){
    int p = a;
    int q = b;
    int cont = 0;
    long long aux[sizea+sizeb];
    if(sizea > 1){
        dividir(a, sizea);
    }
    if(sizeb > 1){
        dividir(b, sizeb);
    }
    while(cont < sizea+sizeb){
        if(q >= b + sizeb){
            aux[cont] = vetor[p];
            p++;
        }else if(p >= a + sizea){
            aux[cont] = vetor[q];
            q++;
        }else if(vetor[p] <= vetor[q]){
            aux[cont] = vetor[p];
            p++;
        }else if(vetor[p] > vetor[q]){
            aux[cont] = vetor[q];
            q++;
        }
        cont++;
    }
    for(cont = a; cont < a+sizea+sizeb; cont++){
        vetor[cont] = aux[cont-a];
    }
}

int main(int argc, char *argv[]){
    int count_input;
    scanf("%d", &tamanho_vetor);
    vetor = (long long*)malloc(tamanho_vetor * sizeof(long long));
    for(count_input = 0; count_input < tamanho_vetor; count_input++)
        scanf("%lld", &vetor[count_input]);

    ordenar(0, tamanho_vetor, 0, 0);

    for(count_input = 0; count_input < tamanho_vetor; count_input++)
        printf("%lld ", vetor[count_input]);    
    free(vetor);
}