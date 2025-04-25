#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>
#include "timer.h"

long long int N;
int nthreads;
long long int prox_num;
long long int num_primos = 0;

pthread_mutex_t mutex;

int ehPrimo(long long int n) {
	int i;
	if (n <= 1) return 0;
	if (n <= 2) return 1;
	if (n % 2 == 0) return 0;
	for(i = 3; i < sqrt(n) + 1; i += 2)
		if (n % i == 0) return 0;
	return 1;
}

typedef struct {
	int id;
} tArgs;

void *tarefa(void *arg) {
	long long int numero;
	int ehPrimo_local;
	long long int contador_local = 0;

	while(true) {
		pthread_mutex_lock(&mutex);
		numero = prox_num++;
		pthread_mutex_unlock(&mutex);

		if (numero > N) break;

		ehPrimo_local = ehPrimo(numero);
		if (ehPrimo_local)
			contador_local++;
	}

	pthread_mutex_lock(&mutex);
	num_primos += contador_local;
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int arquivo_existe(char *filename) {
	FILE *file = fopen(filename, "r");
	if(file) {
		fclose(file);
		return 1;
	}
	return 0;
}

double executa_contador(int n_threads) {
	pthread_t *tid;
	tArgs *args;
	double inicio, fim, delta;

	prox_num = 1;
	num_primos = 0;

	tid = (pthread_t *) malloc(sizeof(pthread_t) * n_threads);
	if (tid == NULL) {
		printf("Erro--malloc\n");
		exit(4);
	}

	args = (tArgs *) malloc(sizeof(tArgs) * n_threads);
	if (tid == NULL) {
		printf("Erro--malloc\n");
		exit(4);
	}
	args = (tArgs *) malloc(sizeof(tArgs) * n_threads);
	if (args == NULL) {
		printf("Erro--malloc\n");
		free(tid);
		exit(5);
	}

	GET_TIME(inicio);

	for(int i=0; i<n_threads; i++) {
		(args+i)->id = i;
		if(pthread_create(tid+i, NULL, tarefa, (void *) (args+i))) {
			printf("Erro--pthread_create\n");
			free(tid);
			free(args);
			exit(6);
		}
	}

	for(int i=0; i<n_threads; i++) {
		pthread_join(*(tid+i), NULL);
	}

	GET_TIME(fim);
	delta = fim - inicio;

	free(args);
	free(tid);

	return delta;
}

int main(int argc, char **argv) {
	double tempo_sequencial, tempo_concorrente;
	double aceleracao, eficiencia;
	FILE *csv;
	char *csv_name = "resultados_primos.csv";
	int arquivo_novo = 0;

	if (argc < 3) {
		printf("Digite: %s <valor de N> <numero de threads>\n", argv[0]);
		return 1;
	}

	N = atoll(argv[1]);
	nthreads = atoi(argv[2]);

	if (N <= 0) {
		printf("Erro: N deve ser maior que 0\n");
		return 2;
	}

	if (nthreads <= 0) {
		printf("Erro: o numero de threads deve ser maior que 0\n");
		return 3;
	}

	pthread_mutex_init(&mutex, NULL);

	printf("Execucao sequencial (1 thread)\n");
	tempo_sequencial = executa_contador(1);

	printf("Execucao concorrente (%d threads)\n", nthreads);
	tempo_concorrente = executa_contador(nthreads);

	aceleracao = tempo_sequencial / tempo_concorrente;
	eficiencia = aceleracao / nthreads;

	printf("valor de N: %lld\n", N);
	printf("Numero de threads: %d\n", nthreads);
	printf("Total de numeros primos encontrados: %lld\n", num_primos);
	printf("tempo de execucao sequencial: %lf segundos\n", tempo_sequencial);
	printf("tempo de execucao concorrente (%d threads): %lf segundos\n", nthreads, tempo_concorrente);
	printf("aceleracao: %lf\n", aceleracao);
	printf("eficiencia: %lf\n", eficiencia);

	arquivo_novo = !arquivo_existe(csv_name);

	csv = fopen(csv_name, "a");
	if (csv == NULL) {
		printf("Erro--Abertura do arquivo CSV\n");
		return 7;
	}

	//header
	if(arquivo_novo) {
		fprintf(csv, "N,threads,tempo_sequencial,tempo_concorrente,aceleracao,eficiencia,primos_encontrados\n");
	}

	fprintf(csv, "%lld,%d,%lf,%lf,%lf,%lf,%lld\n",
			N, nthreads, tempo_sequencial, tempo_concorrente, aceleracao, eficiencia, num_primos);

	fclose(csv);

	pthread_mutex_destroy(&mutex);

	return 0;
}
