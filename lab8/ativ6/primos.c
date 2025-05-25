#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include<semaphore.h>

int *primo_counter;
long long *buffer;
int M, C;
long long N;
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex_buf;

int ehPrimo(long long int n) {
	int i;
	if (n<=1) return 0;
	if (n==2) return 1;
	if (n%2==0) return 0;
	for (i=3; i<sqrt(n)+1; i+=2)
		if(n%i==0) return 0;
	return 1;
}

void *produtor(void *arg) {
	for(long long int i = 1; i <= N; i++) {
		sem_wait(&empty);
		pthread_mutex_lock(&mutex_buf);
		buffer[in] = i;
		in = (in + 1) % M;
		pthread_mutex_unlock(&mutex_buf);
		sem_post(&full);
	}

	for(long long int j = 0; j < C; j++) {
		sem_wait(&empty);
		pthread_mutex_lock(&mutex_buf);
		buffer[in] = -1;
		in = (in + 1) % M;
		pthread_mutex_unlock(&mutex_buf);
		sem_post(&full);
	}

	return NULL;
}

void *consumidor(void *arg) {
	int idx = *(int *) arg;
	long long val;
	int cnt = 0;
	while(1) {
		sem_wait(&full);
		pthread_mutex_lock(&mutex_buf);
		val = buffer[out];
		out = (out + 1) % M;
		pthread_mutex_unlock(&mutex_buf);
		sem_post(&empty);
		if(val == -1) {
			break;
		}
		if(ehPrimo(val)) {
			cnt++;
		}
	}
	primo_counter[idx] = cnt;
	return NULL;
}

int main(void) {
	if(scanf("%lld %d %d", &N, &M, &C) != 3) {
		fprintf(stderr, "esperado 3 valores: N, M, C\n");
		return 1;
	}

	buffer = malloc(sizeof(long long) *M);
	primo_counter = malloc(C * sizeof(int));
	sem_init(&empty, 0, M);
	sem_init(&full, 0, 0);
	pthread_mutex_init(&mutex_buf, NULL);

	pthread_t prod;
	pthread_t cons[C];
	int indices[C];

	if(pthread_create(&prod, NULL, produtor, NULL) != 0) {
		perror("pthread_create produtr");
		exit(1);
	}

	for(int i = 0; i < C; i++) {
		indices[i] = i;
		if(pthread_create(&cons[i], NULL, consumidor, &indices[i]) != 0) {
			perror("pthread_create consumidor");
			exit(1);
		}
	}

	pthread_join(prod, NULL);
	for(int i = 0; i < C; i++) {
		pthread_join(cons[i], NULL);
	}

	int total_primos = 0;
	int vencedor = 0;
	for(int i = 0; i < C; i++) {
		total_primos += primo_counter[i];
		if(primo_counter[i] > primo_counter[vencedor]) {
			vencedor = i;
		}
	}

	printf("total de primos encontrados: %d\n", total_primos);
	printf("thread consumidora vencedora: %d (encontrou %d primos)\n", 
		vencedor + 1, primo_counter[vencedor]);

	free(buffer);
	free(primo_counter);
	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_mutex_destroy(&mutex_buf);

	return 0;
}

