//Autores: Ariel Lima, Andre Paiva, Natan Moura, Pedro Augusto
//Implementacao de Ordenação com Threads
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

long long *vetor;
unsigned int tamanho_vetor;

void *thread_main(void*);
void *thread_divide(void*);

typedef struct{
    unsigned int indice;
    unsigned int tamanho;
} args_divide;

typedef struct{
    unsigned int indice_a;
    unsigned int tamanho_a;
    unsigned int indice_b;
    unsigned int tamanho_b;
} args_main;

int create_thread_main(unsigned int i_a, unsigned int t_a, unsigned int i_b, unsigned int t_b){
    args_main *data = (args_main*) malloc(sizeof(args_main));
    if(data == NULL){
        return 1;
    }
    data->indice_a = i_a;
    data->tamanho_a = t_a;
    data->indice_b = i_b;
    data->tamanho_b = t_b;

    pthread_t *t_main = (pthread_t*)malloc(sizeof(pthread_t));
    if(t_main == NULL){
        free(data);
        return 2;
    }
    int criacao_thread = pthread_create(t_main, NULL, thread_main, (void *) data);
    if(criacao_thread != 0){
        free(t_main);
        free(data);
        return 3;
    }
    pthread_join(*t_main, NULL);
    free(t_main);
    free(data);
    return 0;
}

int create_thread_divide(unsigned int i, unsigned int t){
    args_divide *data = (args_divide*) malloc(sizeof(args_divide));
    if(data == NULL){
        return 1;
    }
    data->indice = i;
    data->tamanho = t;

    pthread_t *t_divide = (pthread_t*)malloc(sizeof(pthread_t));
    if(t_divide == NULL){
        free(data);
        return 2;
    }
    int criacao_thread = pthread_create(t_divide, NULL, thread_divide, (void *) data);
    if(criacao_thread != 0){
        free(t_divide);
        free(data);
        return 3;
    }
    pthread_join(*t_divide, NULL);
    free(t_divide);
    free(data);
    return 0;
}

void *thread_main(void *args){
    args_main *data = (args_main*) args;
    int status;
    int tmp_a = data->indice_a;
    int tmp_b = data->indice_b;
    int cont = 0;
    long long aux[(data->tamanho_a + data->tamanho_b)];
    if(data->tamanho_a > 1){
        status = create_thread_divide(data->indice_a, data->tamanho_a);
        if(status != 0){
            printf("ERRO.\n");
            return 0;
        }
    }
    if(data->tamanho_b > 1){
        status = create_thread_divide(data->indice_b, data->tamanho_b);
        if(status != 0){
            printf("ERRO.\n");
            return 0;
        }
    }
    while(cont < (data->tamanho_a + data->tamanho_b)){
        if(tmp_b >= data->indice_b + data->tamanho_b){
            aux[cont] = vetor[tmp_a];
            tmp_a++;
        }else if(tmp_a >= data->indice_a + data->tamanho_a){
            aux[cont] = vetor[tmp_b];
            tmp_b++;
        }else if(vetor[tmp_a] <= vetor[tmp_b]){
            aux[cont] = vetor[tmp_a];
            tmp_a++;
        }else if(vetor[tmp_a] > vetor[tmp_b]){
            aux[cont] = vetor[tmp_b];
            tmp_b++;
        }
        cont++;
    }
    for(cont = 0; cont < (data->tamanho_a + data->tamanho_b); cont++){
        vetor[data->indice_a + cont] = aux[cont];
    }
    return 0;
}

void *thread_divide(void *args){
    args_divide *data = (args_divide*) args;
    unsigned int x, y;
    unsigned int tam_x, tam_y;
    x = data->indice;
    tam_x = data->tamanho - ((data->tamanho)/2);
    y = x + tam_x;
    tam_y = (data->tamanho)/2;
    int status = create_thread_main(x, tam_x, y, tam_y);
    if(status != 0) printf("ERRO.\n");
    return 0;
}

int main(int argc, char *argv[]){
    int count_input;
    scanf("%d", &tamanho_vetor);
    vetor = (long long*)malloc(tamanho_vetor * sizeof(long long));
    for(count_input = 0; count_input < tamanho_vetor; count_input++)
        scanf("%lld", &vetor[count_input]);

    int status = create_thread_main(0, tamanho_vetor, 0, 0);

    if(status == 0){
        for(count_input = 0; count_input < tamanho_vetor; count_input++)
            printf("%lld ", vetor[count_input]);  
        printf("\n"); 
    }else printf("ERRO.\n");
    free(vetor);
}