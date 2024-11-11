#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

pthread_mutex_t barrier_mutex;
pthread_cond_t barrier_cond;
int barrier_count = 0;

void barrier_wait() {
    pthread_mutex_lock(&barrier_mutex);
    barrier_count++;

    if (barrier_count == NUM_THREADS) {
        barrier_count = 0;  // Reset para próxima utilização da barreira
        pthread_cond_broadcast(&barrier_cond);  // Desbloqueia todas as threads
    } else {
        pthread_cond_wait(&barrier_cond, &barrier_mutex);  // Espera até todas as threads chegarem
    }

    pthread_mutex_unlock(&barrier_mutex);
}

void *worker(void *arg) {
    int thread_id = *((int *)arg);
    printf("Thread %d chegou na barreira\n", thread_id);
    barrier_wait();
    printf("Thread %d passou da barreira\n", thread_id);
    free(arg);  // Libera a memória alocada para o ID
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&barrier_mutex, NULL);
    pthread_cond_init(&barrier_cond, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        int *thread_id = malloc(sizeof(int));  // Aloca memória para o ID
        *thread_id = i;
        pthread_create(&threads[i], NULL, worker, (void *)thread_id);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&barrier_mutex);
    pthread_cond_destroy(&barrier_cond);
    return 0;
}
