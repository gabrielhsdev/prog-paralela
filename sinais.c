#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int flag = 0;

void *signal_thread(void *arg) {
    sleep(2);
    pthread_mutex_lock(&mutex);
    flag = 1;
    printf("Sinal enviado\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *wait_thread(void *arg) {
    pthread_mutex_lock(&mutex);
    while (flag == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Sinal recebido\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, wait_thread, NULL);
    pthread_create(&t2, NULL, signal_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
