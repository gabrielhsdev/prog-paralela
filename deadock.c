#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Adicionando o cabeçalho correto

#define NUM_THREADS 2

pthread_mutex_t lock1, lock2;

void* thread1_func(void* arg) {
    pthread_mutex_lock(&lock1);
    printf("Thread 1: lock1 adquirido\n");
    sleep(1);  // Dormir por 1 segundo para aumentar as chances de deadlock
    pthread_mutex_lock(&lock2);
    printf("Thread 1: lock2 adquirido\n");
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void* thread2_func(void* arg) {
    pthread_mutex_lock(&lock2);
    printf("Thread 2: lock2 adquirido\n");
    sleep(1);  // Dormir por 1 segundo para aumentar as chances de deadlock
    pthread_mutex_lock(&lock1);
    printf("Thread 2: lock1 adquirido\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&threads[0], NULL, thread1_func, NULL);
    pthread_create(&threads[1], NULL, thread2_func, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
