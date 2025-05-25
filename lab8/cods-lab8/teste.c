#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TAM_BUFFER 5
#define NUM_ITENS 10

int buffer[TAM_BUFFER];
int in = 0, out = 0;

sem_t empty; // conta espaços vazios
sem_t full;  // conta itens no buffer
pthread_mutex_t mutex;

void* produtor(void* arg) {
    for (int i = 0; i < NUM_ITENS; i++) {
        int item = rand() % 100; // cria item

        sem_wait(&empty); // espera espaço
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Produtor produziu %d na posição %d\n", item, in);
        in = (in + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&full); // indica item disponível

        sleep(1);
    }
    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < NUM_ITENS; i++) {
        sem_wait(&full); // espera item
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumidor consumiu %d da posição %d\n", item, out);
        out = (out + 1) % TAM_BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // indica espaço disponível

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, TAM_BUFFER);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, produtor, NULL);
    pthread_create(&cons, NULL, consumidor, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}

