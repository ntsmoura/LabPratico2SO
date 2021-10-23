# Sodoku Multithread
Sodoku validator using multithreading in C.
The Sodoku validator multithread is a C program that verifies lines, columns and squares of a Sodoku matrix in order to validate it. 
The program contains:
- Main thread responsible for creating threads, receiving the Sodoku matrix as input and verifying the result from each thread.
- Threads for checking each line (9)
- Threads for checking each column (9)
- Threads for checking each square (9)
In a total of 27 threads for checking and one main thread.

# Compiling and running
```
gcc -Wall -std=c99 sodoku_multithread.c -o sodoku_multithread -lpthread
./sodoku_multithread < input1.txt 
```
# Output example for correct values (input1.txt)
```
---- Verificador de resolução - Sodoku ----
Insira a matriz de resolução do sodoku 9x9:
Resolução inserida:

8   2   7 | 1   5   4 | 3   9   6   
9   6   5 | 3   2   7 | 1   4   8   
3   4   1 | 6   8   9 | 7   5   2   
---------------------------------
5   9   3 | 4   6   8 | 2   7   1   
4   7   2 | 5   1   3 | 6   8   9   
6   1   8 | 9   7   2 | 4   3   5   
---------------------------------
7   8   6 | 2   3   5 | 9   1   4   
1   5   4 | 7   9   6 | 8   2   3   
2   3   9 | 8   4   1 | 5   6   7   

Thread ID = 00 / Scan = LINHA 1 / OK
Thread ID = 01 / Scan = LINHA 2 / OK
Thread ID = 02 / Scan = LINHA 3 / OK
Thread ID = 03 / Scan = LINHA 4 / OK
Thread ID = 04 / Scan = LINHA 5 / OK
Thread ID = 05 / Scan = LINHA 6 / OK
Thread ID = 06 / Scan = LINHA 7 / OK
Thread ID = 07 / Scan = LINHA 8 / OK
Thread ID = 08 / Scan = LINHA 9 / OK
Thread ID = 09 / Scan = COLUNA 1 / OK
Thread ID = 10 / Scan = COLUNA 2 / OK
Thread ID = 11 / Scan = COLUNA 3 / OK
Thread ID = 12 / Scan = COLUNA 4 / OK
Thread ID = 13 / Scan = COLUNA 5 / OK
Thread ID = 14 / Scan = COLUNA 6 / OK
Thread ID = 15 / Scan = COLUNA 7 / OK
Thread ID = 16 / Scan = COLUNA 8 / OK
Thread ID = 17 / Scan = COLUNA 9 / OK
Thread ID = 18 / Scan = SUBGRADE 0 0 / OK
Thread ID = 19 / Scan = SUBGRADE 0 3 / OK
Thread ID = 20 / Scan = SUBGRADE 0 6 / OK
Thread ID = 21 / Scan = SUBGRADE 3 0 / OK
Thread ID = 22 / Scan = SUBGRADE 3 3 / OK
Thread ID = 23 / Scan = SUBGRADE 3 6 / OK
Thread ID = 24 / Scan = SUBGRADE 6 0 / OK
Thread ID = 25 / Scan = SUBGRADE 6 3 / OK
Thread ID = 26 / Scan = SUBGRADE 6 6 / OK
Array de resultado das threads: [ 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 ]
Resolução correta!
```

# Output example for incorrect values (input2.txt)
```
---- Verificador de resolução - Sodoku ----
Insira a matriz de resolução do sodoku 9x9:
Resolução inserida:

8   2   7 | 1   5   4 | 3   9   6   
9   6   5 | 3   2   7 | 1   4   8   
3   4   1 | 6   8   9 | 7   5   2   
---------------------------------
5   9   3 | 4   6   8 | 2   7   1   
4   7   2 | 5   9   3 | 6   8   9   
6   1   8 | 1   7   2 | 4   3   5   
---------------------------------
7   8   6 | 2   3   5 | 9   1   4   
1   5   4 | 7   9   9 | 8   2   3   
2   3   9 | 8   4   1 | 5   6   7   

Thread ID = 00 / Scan = LINHA 1 / OK
Thread ID = 01 / Scan = LINHA 2 / OK
Thread ID = 02 / Scan = LINHA 3 / OK
Thread ID = 03 / Scan = LINHA 4 / OK
Thread ID = 04 / Scan = LINHA 5 / Número 9 repetido na linha 5.
Thread ID = 05 / Scan = LINHA 6 / Número 1 repetido na linha 6.
Thread ID = 06 / Scan = LINHA 7 / OK
Thread ID = 07 / Scan = LINHA 8 / Número 9 repetido na linha 8.
Thread ID = 08 / Scan = LINHA 9 / OK
Thread ID = 09 / Scan = COLUNA 1 / OK
Thread ID = 10 / Scan = COLUNA 2 / OK
Thread ID = 11 / Scan = COLUNA 3 / OK
Thread ID = 12 / Scan = COLUNA 4 / Número 1 repetido na coluna 4.
Thread ID = 13 / Scan = COLUNA 5 / Número 9 repetido na coluna 5.
Thread ID = 14 / Scan = COLUNA 6 / Número 9 repetido na coluna 6.
Thread ID = 15 / Scan = COLUNA 7 / OK
Thread ID = 16 / Scan = COLUNA 8 / OK
Thread ID = 17 / Scan = COLUNA 9 / OK
Thread ID = 18 / Scan = SUBGRADE 0 0 / OK
Thread ID = 19 / Scan = SUBGRADE 0 3 / OK
Thread ID = 20 / Scan = SUBGRADE 0 6 / OK
Thread ID = 21 / Scan = SUBGRADE 3 0 / OK
Thread ID = 22 / Scan = SUBGRADE 3 3 / OK
Thread ID = 23 / Scan = SUBGRADE 3 6 / OK
Thread ID = 24 / Scan = SUBGRADE 6 0 / OK
Thread ID = 25 / Scan = SUBGRADE 6 3 / Número 9 repetido na subgrade (6, 3).
Thread ID = 26 / Scan = SUBGRADE 6 6 / OK
Array de resultado das threads: [ 1 1 1 1 0 0 1 0 1 1 1 1 0 0 0 1 1 1 1 1 1 1 1 1 1 0 1 ]
Resolução incorreta!
```
