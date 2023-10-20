#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3
#define MATRIX_SIZE 3

int matrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

int vector[MATRIX_SIZE] = {1, 2, 3};
int result[MATRIX_SIZE];

void *matrixVectorMultiply(void *threadId) {
    long rank = (long)threadId;
    int chunkSize = MATRIX_SIZE / NUM_THREADS;
    int start = rank * chunkSize;
    int end = (rank == NUM_THREADS - 1) ? MATRIX_SIZE : start + chunkSize;

    for (int i = start; i < end; i++) {
        result[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    for (t = 0; t < NUM_THREADS; t++) {
        printf("Criando thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, matrixVectorMultiply, (void *)t);
        if (rc) {
            printf("Erro ao criar a thread; código de retorno: %d\n", rc);
            return -1;
        }
    }

    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Resultado da multiplicação da matriz pelo vetor:\n");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%d\n", result[i]);
    }

    printf("Todas as threads terminaram. Programa encerrado.\n");
    pthread_exit(NULL);
}



