#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#define NUM_THREADS 27
#define SODOKU_SIZE 9

bool result[NUM_THREADS]; // Armazena o resultado das threads.

typedef enum {LINE, COLUMN, SQUARE} scan_type;
typedef struct {
    int thread_id;
    scan_type type;
    int index_line;
    int index_column;
} thread_args;

typedef unsigned short int sodoku_input; // Definindo tipo de valor de entrada do Sodoku

sodoku_input sodoku[SODOKU_SIZE][SODOKU_SIZE];

bool isResultAllTrue(bool result_array[], unsigned int size);

void *threadScan(void *args)
{
    thread_args *data = (thread_args*) args;
    int checkRepetition[SODOKU_SIZE] = {0}; 
    printf("Thread ID = %02d / ", data->thread_id);
    switch (data->type)
    {
    case LINE:
        printf("Scan = LINHA %d / ", data->index_line + 1);
        for (size_t i = 0; i < SODOKU_SIZE; i++)
        {
            int index = sodoku[data->index_line][i];
            checkRepetition[index-1]++; 
            if(checkRepetition[index-1] > 1)
            { 
                printf("Número %d repetido na linha %d.\n", index, data->index_line + 1);               
                pthread_exit (NULL);
                return 0;
            }
        }
        printf("OK\n");
        result[data->thread_id] = true;     
        break;
    case COLUMN:
        printf("Scan = COLUNA %d / ", data->index_column + 1);
        for (size_t i = 0; i < SODOKU_SIZE; i++)
        {
            int index = sodoku[i][data->index_column];
            checkRepetition[index-1]++; 
            if(checkRepetition[index-1] > 1)
            {          
                printf("Número %d repetido na coluna %d.\n", index, data->index_column + 1);       
                pthread_exit (NULL);
                return 0;
            }
        }
        printf("OK\n"); 
        result[data->thread_id] = true;  
        break;
    case SQUARE:
        printf("Scan = SUBGRADE %d %d / ", data->index_line, data->index_column);

        for (size_t i = data->index_line; i < data->index_line + 3; i++)
        {
            for (size_t j = data->index_column; j < data->index_column + 3; j++)
            {
                int index = sodoku[i][j];
                checkRepetition[index-1]++; 
                if(checkRepetition[index-1] > 1)
                {       
                    printf("Número %d repetido na subgrade (%d, %d).\n", index, data->index_line, data->index_column);         
                    pthread_exit (NULL);
                    return 0;
                }
            }            
        }
        printf("OK\n");
        result[data->thread_id] = true;  
        break;    
    default:
        break;
    }
    pthread_exit (NULL);
}

int main (int argc, char *argv[])
{
    printf("---- Verificador de resolução - Sodoku ----\n");
    printf("Insira a matriz de resolução do sodoku 9x9:\n");
    
    for(size_t i = 0; i < SODOKU_SIZE; i++) 
    {
        for (size_t j = 0; j < SODOKU_SIZE; j++)
        {
            scanf("%hu", &sodoku[i][j]);
        }        
    }

    printf("Resolução inserida:\n\n");
    for(size_t i = 0; i < SODOKU_SIZE; i++) 
    {
        for (size_t j = 0; j < SODOKU_SIZE; j++)
        {            
            if((j+1)%3 == 0 && j != SODOKU_SIZE - 1) 
                printf("%hu | ", sodoku[i][j]);
            else
                printf("%hu   ", sodoku[i][j]);
        }
        printf("\n");
        if((i+1)%3 == 0 && i != SODOKU_SIZE - 1) 
        {
            for(size_t k = 0; k < 33; k++) 
                printf("-");
            printf("\n");
        }    
    }
    printf("\n");

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;

    pthread_attr_init (&attr);
    pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);

    // Inicialização das threads de verificação
    for(size_t i = 0; i < NUM_THREADS; i++) 
    {
        long status;
        // Verifica linhas
        if(i < 9) 
        {
            thread_args *data = (thread_args*) malloc(sizeof(thread_args));
            data->thread_id = i;
            data->type = LINE;
            data->index_line = i;
            data->index_column = 0;
            status = pthread_create(&thread[i], &attr, threadScan, (void *) data);
        }
        // Verifica colunas
        else if (i < 18) 
        {
            thread_args *data = (thread_args*) malloc(sizeof(thread_args));
            data->thread_id = i;
            data->type = COLUMN;
            data->index_line = 0;
            data->index_column = i - 9;
            status = pthread_create(&thread[i], &attr, threadScan, (void *) data);
        }
        // Verifica subgrades 3x3
        else
        {
            thread_args *data = (thread_args*) malloc(sizeof(thread_args));
            data->thread_id = i;
            data->type = SQUARE;
            data->index_line = ((i - 18) / 3) * 3;
            data->index_column = ((i - 18) % 3) * 3;
            status = pthread_create(&thread[i], &attr, threadScan, (void *) data);
        }

        if (status)
        {
            perror ("pthread_create") ;
            exit (1) ;
        }        
    }

    // Espera pela execução de todas as threads
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        long status;
        status = pthread_join(thread[i], NULL);
        if (status)
        {
            perror("pthread_join");
            exit(1);
        }
    }

    printf("Array de resultado das threads: [ ");
    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        printf("%d ", result[i]);
    }   
    printf("]\n");

    if(isResultAllTrue(result, NUM_THREADS))
        printf("Resolução correta!\n");
    else
        printf("Resolução incorreta!\n");

    pthread_attr_destroy (&attr) ;
    pthread_exit (NULL) ;
}

// Verifica se o resultado de todas as threads é true.
bool isResultAllTrue(bool result_array[], unsigned int size) 
{
    unsigned int i;
    for (i = 0; i < size; i++) 
    {
        if(!result_array[i]) 
            break;
    }
    return (i == size);
}