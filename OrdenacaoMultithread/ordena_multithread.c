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

//Debug Mode: Printa na tela os passos da execução (divisão/ordenação)
//Útil para demonstrar paralelismo das threads
//Para ativar, alterar valor de debugMode para 1
int debugMode = 0;

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
pthread_t *create_thread_main(unsigned int i_a, unsigned int t_a, unsigned int i_b, unsigned int t_b){
    
    //Alocação de memória para passagem de parâmetros
    args_main *data = (args_main*) malloc(sizeof(args_main));
    if(data == NULL){
        printf("Falha na alocação de memória para passagem\n");
        printf("de parâmetros para thread_main.\n");
        return 0;
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
        return 0;
    }

    //Criação da thread propriamente dita
    int criacao_thread = pthread_create(t_main, NULL, thread_main, (void *) data);
    if(criacao_thread != 0){
        printf("Falha na criação de thread_main.\n");
        free(t_main);
        free(data);
        return 0;
    }

    //Função retorna endereço de alocação da thread (ponteiro)
    return(t_main);
}
//Função auxiliar para facilitar criação e gestão de thread para executar thread_divide
pthread_t *create_thread_divide(unsigned int i, unsigned int t){

    //Alocação de memória para passagem de parâmetros
    args_divide *data = (args_divide*) malloc(sizeof(args_divide));
    if(data == NULL){
        printf("Falha na alocação de memória para passagem\n");
        printf("de parâmetros para thread_divide.\n");
        return 0;
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
        return 0;
    }

    //Criação da thread propriamente dita
    int criacao_thread = pthread_create(t_divide, NULL, thread_divide, (void *) data);
    if(criacao_thread != 0){
        printf("Falha na criação de thread_divide.\n");
        free(t_divide);
        free(data);
        return 0;
    }

    //Função retorna endereço de alocação da thread (ponteiro)
    return(t_divide);
}

//Definição de função thread_main
void *thread_main(void *args){
    
    //Leitura de parâmetros
    args_main *data = (args_main*) args;

    //Declaração de variáveis locais auxiliares
    pthread_t *thread_a, *thread_b;
    int tmp_a = data->indice_a;
    int tmp_b = data->indice_b;
    int cont = 0;

    //Declaração de vetor auxiliar de ordenação
    long long aux[(data->tamanho_a + data->tamanho_b)];

    //Enquanto o tamanho dos subvetores a processar for maior que 1,
    //os vetores são subdivididos, de acordo com a estratégia de
    //merge sort.
    if(data->tamanho_a > 1){
        thread_a = create_thread_divide(data->indice_a, data->tamanho_a); 
    }

    if(data->tamanho_b > 1){
        thread_b = create_thread_divide(data->indice_b, data->tamanho_b);
    }

    if(thread_a != NULL){
        pthread_join(*thread_a, NULL);
    }
    if(thread_b != NULL){
        pthread_join(*thread_b, NULL);
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
    if(debugMode)
        printf("ORDENACAO: ");

    for(cont = 0; cont < (data->tamanho_a + data->tamanho_b); cont++){
        vetor[data->indice_a + cont] = aux[cont];
        if(debugMode)
            printf("%lld ", aux[cont]);
    }
    if(debugMode)
        printf("\n");

    //Libera memória alocada para parâmetros
    free(data);

    return 0;
}

//Definição de função thread_divide
void *thread_divide(void *args){
    
    //Leitura de parâmetros
    args_divide *data = (args_divide*) args;

    //Declaração de variáveis locais auxiliares
    pthread_t *thread;
    unsigned int x, y;
    unsigned int tam_x, tam_y;

    //Cálculo de índices e tamanhos para a subdivisão do subvetor
    x = data->indice;
    tam_x = data->tamanho - ((data->tamanho)/2);
    y = x + tam_x;
    tam_y = (data->tamanho)/2;

    if(debugMode)
        printf("DIVISAO: %d %d %d %d\n", x, tam_x, y, tam_y);

    //Nova thread_main com subvetores divididos
    thread = create_thread_main(x, tam_x, y, tam_y);
    pthread_join(*thread, NULL);

    //Libera memória alocada para parâmetros
    free(data);

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
    pthread_t *thread = create_thread_main(0, tamanho_vetor, 0, 0);
    pthread_join(*thread, NULL);

    //Caso não haja nenhuma intercorrência na gestão das threads e na
    //execução do programa, o vetor estará ordenado e será
    //printado na tela, com seus valores separados por espaço
    if(thread != 0){
        for(count_input = 0; count_input < tamanho_vetor; count_input++)
            printf("%lld ", vetor[count_input]);  
        printf("\n"); 
    }else printf("ERRO.\n");

    //Liberação de memória alocada para o vetor
    free(vetor);

    return 0;
}