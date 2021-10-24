/*
Autores: Ariel Lima, Andre Paiva, Natan Moura, Pedro Augusto
Implementacao de Ordenação com Threads
*/

//Importação de bibliotecas
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Declaração de ponteiro do vetor e variável indicadora de tamanho
long long *vetor;
unsigned int tamanho_vetor;

//Declaração de funções de threads
void *thread_main(void*);
void *thread_divide(void*);

//Criação de struct para passagem de argumentos para thread_divide
typedef struct{
    unsigned int indice;
    unsigned int tamanho;
} args_divide;

//Criação de struct para passagem de argumentos para thread_main
typedef struct{
    unsigned int indice_a;
    unsigned int tamanho_a;
    unsigned int indice_b;
    unsigned int tamanho_b;
} args_main;

//Função auxiliar para facilitar criação e gestão de thread para executar thread_main
int create_thread_main(unsigned int i_a, unsigned int t_a, unsigned int i_b, unsigned int t_b){
    
    //Alocação de memória para passagem de parâmetros
    args_main *data = (args_main*) malloc(sizeof(args_main));
    if(data == NULL){
        printf("Falha na alocação de memória para passagem\n");
        printf("de parâmetros para thread_main.\n");
        return 1;
    }

    //Configuração dos parâmetros
    data->indice_a = i_a;
    data->tamanho_a = t_a;
    data->indice_b = i_b;
    data->tamanho_b = t_b;

    //Alocação de memória para thread
    pthread_t *t_main = (pthread_t*)malloc(sizeof(pthread_t));
    if(t_main == NULL){
        printf("Falha na alocação de memória para\n");
        printf("criação de thread_main.\n");
        free(data);
        return 2;
    }

    //Criação da thread propriamente dita
    int criacao_thread = pthread_create(t_main, NULL, thread_main, (void *) data);
    if(criacao_thread != 0){
        printf("Falha na criação de thread_main.\n");
        free(t_main);
        free(data);
        return 3;
    }

    //Aguardar o término da execução da thread
    pthread_join(*t_main, NULL);

    //Liberação de memória
    free(t_main);
    free(data);

    return 0;
}
//Função auxiliar para facilitar criação e gestão de thread para executar thread_divide
int create_thread_divide(unsigned int i, unsigned int t){

    //Alocação de memória para passagem de parâmetros
    args_divide *data = (args_divide*) malloc(sizeof(args_divide));
    if(data == NULL){
        printf("Falha na alocação de memória para passagem\n");
        printf("de parâmetros para thread_divide.\n");
        return 1;
    }

    //Configuração dos parâmetros
    data->indice = i;
    data->tamanho = t;

    //Alocação de memória para thread
    pthread_t *t_divide = (pthread_t*)malloc(sizeof(pthread_t));
    if(t_divide == NULL){
        printf("Falha na alocação de memória para\n");
        printf("criação de thread_divide.\n");
        free(data);
        return 2;
    }

    //Criação da thread propriamente dita
    int criacao_thread = pthread_create(t_divide, NULL, thread_divide, (void *) data);
    if(criacao_thread != 0){
        printf("Falha na criação de thread_divide.\n");
        free(t_divide);
        free(data);
        return 3;
    }

    //Aguardar o término da execução da thread
    pthread_join(*t_divide, NULL);

    //Liberação de memória
    free(t_divide);
    free(data);
    
    return 0;
}

//Definição de função thread_main
void *thread_main(void *args){
    
    //Leitura de parâmetros
    args_main *data = (args_main*) args;

    //Declaração de variáveis locais auxiliares
    int status;
    int tmp_a = data->indice_a;
    int tmp_b = data->indice_b;
    int cont = 0;

    //Declaração de vetor auxiliar de ordenação
    long long aux[(data->tamanho_a + data->tamanho_b)];

    //Enquanto o tamanho dos subvetores a processar for maior que 1,
    //os vetores são subdivididos, de acordo com a estratégia de
    //merge sort.
    if(data->tamanho_a > 1){
        status = create_thread_divide(data->indice_a, data->tamanho_a);
        if(status != 0) return 0;
    }

    if(data->tamanho_b > 1){
        status = create_thread_divide(data->indice_b, data->tamanho_b);
        if(status != 0) return 0;
    }

    //Ordenação dos valores dos subvetores em aux
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

    //Sobrescrição de aux no vetor global
    for(cont = 0; cont < (data->tamanho_a + data->tamanho_b); cont++){
        vetor[data->indice_a + cont] = aux[cont];
    }

    return 0;
}

//Definição de função thread_divide
void *thread_divide(void *args){
    
    //Leitura de parâmetros
    args_divide *data = (args_divide*) args;

    //Declaração de variáveis locais auxiliares
    unsigned int x, y;
    unsigned int tam_x, tam_y;

    //Cálculo de índices e tamanhos para a subdivisão do subvetor
    x = data->indice;
    tam_x = data->tamanho - ((data->tamanho)/2);
    y = x + tam_x;
    tam_y = (data->tamanho)/2;

    //Nova thread_main com subvetores divididos
    create_thread_main(x, tam_x, y, tam_y);

    return 0;
}

//Função main
int main(int argc, char *argv[]){

    //Declaração de variável local auxiliar de contagem
    int count_input;

    //Leitura da quantidade de elementos do vetor
    scanf("%d", &tamanho_vetor);

    //Alocação de espaço na memória para armazenamento do vetor
    vetor = (long long*)malloc(tamanho_vetor * sizeof(long long));

    //Leitura de valores do vetor (valores desordenados)
    for(count_input = 0; count_input < tamanho_vetor; count_input++)
        scanf("%lld", &vetor[count_input]);

    //Criação da thread principal de ordenação do vetor
    int status = create_thread_main(0, tamanho_vetor, 0, 0);

    //Caso não haja nenhuma intercorrência na gestão das threads e na
    //execução do programa, o vetor estará ordenado e será
    //printado na tela, com seus valores separados por espaço
    if(status == 0){
        for(count_input = 0; count_input < tamanho_vetor; count_input++)
            printf("%lld ", vetor[count_input]);  
        printf("\n"); 
    }else printf("ERRO.\n");

    //Liberação de memória alocada para o vetor
    free(vetor);

    return 0;
}