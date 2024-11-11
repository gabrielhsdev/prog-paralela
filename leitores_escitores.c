#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int read_count = 0;
int data = 0; // Recurso compartilhado

void *writer(void *arg) {
    pthread_mutex_lock(&rw_mutex);
    data++;
    printf("Escritor alterou o valor para %d\n", data);
    pthread_mutex_unlock(&rw_mutex);
    return NULL;
}

void *reader(void *arg) {
    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1) {
        pthread_mutex_lock(&rw_mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("Leitor leu o valor %d\n", data);

    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) {
        pthread_mutex_unlock(&rw_mutex);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t readers[5], writers[5];

    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
        pthread_create(&writers[i], NULL, writer, NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }
    return 0;
}
