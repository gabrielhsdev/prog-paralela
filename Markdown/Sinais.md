
# Relatório Final: Exemplo 3 - Sinais

### De: Gabriel Henrique da Silva
### RA:22020864
``` 
Essa parte do projeto foi feita em markdown
```

## Introdução

**Sinais**  são usados para comunicação entre threads ou processos. Neste exemplo, usamos sinais para notificar uma thread sobre um evento específico, usando uma variável de condição (condition variable).

## Código Explicado

#### Declaração das Variáveis, Mutex e Variável de Condição

```
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t condition;
int ready = 0;
```

-   **mutex**: Controla o acesso ao estado compartilhado.
-   **condition**: Variável de condição usada para notificar a thread.
-   **ready**: Marca quando a condição foi sinalizada.

#### Função de Espera

```
void *waiter(void *arg) {
    pthread_mutex_lock(&mutex);
    while (!ready) {
        pthread_cond_wait(&condition, &mutex);  // Espera o sinal
    }
    printf("Thread esperando foi sinalizada!\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}
```

-   **Espera**: A thread espera que a condição seja sinalizada (`ready`  se torna  `1`).
-   **Notificação**:  `pthread_cond_wait`  libera o mutex e suspende a thread até o sinal.

#### Função de Sinalização
```
void *signaler(void *arg) {
    sleep(1);  // Simula processamento
    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_signal(&condition);  // Envia o sinal
    printf("Sinal enviado para a thread esperando.\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}
```

-   **Sinalização**: A thread sinalizadora altera  `ready`  e envia o sinal com  `pthread_cond_signal`.

#### Função Principal

```
int main() {
    pthread_t t_waiter, t_signaler;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    pthread_create(&t_waiter, NULL, waiter, NULL);
    pthread_create(&t_signaler, NULL, signaler, NULL);

    pthread_join(t_waiter, NULL);
    pthread_join(t_signaler, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}
```

-   **Inicialização**: Cria as threads e inicializa o mutex e a condição.
-   **Execução**: A thread  `signaler`  envia o sinal para que  `waiter`  continue.

### Conclusão

Este código usa sinais para sincronizar threads, com uma thread esperando um evento e outra notificando quando ele ocorre.

