#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct {
	int id, inicio, fim;
	int *vetor;
} t_Args;

void *add_thread(void *arg) {
	t_Args *args = (t_Args *) arg;
	for(int i=args->inicio; i<args->fim; i++) {
		args->vetor[i] = args->vetor[i] + 1;
	}
	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("ERRO de entrada de parametros: Digite %s <N>\n", argv[0]);
		return -1;
	}
	int N = atoi(argv[1]);
	int total_elementos = 4 * N;
	int *vetor = (int *) malloc(total_elementos * sizeof(int));
	if(vetor == NULL) {
		printf("Erro: Falha na alocacao de memoria!\n");
		return 2;
	}

	for(int i=0; i<total_elementos; i++) {
		vetor[i] = 0;
	}

	int n_threads = 4;
	pthread_t tid[n_threads];
	t_Args args[n_threads];

	// bloco de cada thread
	int bloco = total_elementos / n_threads;

	// criar o fluxo de exec das threads
	// o tamanho do vetor eh 4N
	// vou fazer com que cada thread pegue 1N para somar 1
	// entao a thread 0 pega do 0 ao N
	// thread 1 do N ao 2N
	// thread 2 do 2N ao 3N
	// thread 3 do 3N ao 4N
	for(long int i=0; i<n_threads; i++) {
		args[i].id = i;
		args[i].inicio = i*bloco;
		args[i].fim = (i+1) * bloco;
		args[i].vetor = vetor;

		int ret = pthread_create(&tid[i], NULL, add_thread, (void *) &args[i]);
		if(ret) {
			printf("ERRO: nao deu para criar a thread\n");
			free(vetor);
			return -1;
		}
	}
	
	for(int i=0; i<n_threads; i++) {
		pthread_join(tid[i], NULL);
	}

	for(int i=0; i<total_elementos; i++) {
		printf("%d ", vetor[i]);
	}

	free(vetor);
	return 0;
}
