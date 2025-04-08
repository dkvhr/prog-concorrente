/* Disciplina: Programação Concorrente */
/* Profa.: Silvana Rossetto */
/* Lab2:  */
/* Codigo: Soma todos os elementos de um vetor de floats */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define VERSOES

//variaveis globais
//tamanho do vetor
long int n;
//vetor de elementos
float * vet1, * vet2;
//numero de threads
int nthreads;

void *dot_product(void *tid) {
	long int id = (long int) tid;
	int ini, fim, bloco;
	float *ret;
	float produtos_locais = 0;

	bloco = n / nthreads;
	ini = id * bloco;
	fim = ini + bloco;
	if(id == (nthreads - 1))
		fim = n;

	for(int i = ini; i < fim; i++) {
		produtos_locais += vet1[i] * vet2[i];
	}
	ret = (float *) malloc(sizeof(float));
	if(ret != NULL)
		*ret = produtos_locais;
	else
		printf("ERRO: malloc() na thread\n");
	pthread_exit((void *) ret);
}


double var_relativa(double ref, float comparado) {
	double var = (ref - comparado) / ref;
	return var;
}


//funcao principal do programa
int main(int argc, char * argv[]) {
        FILE *arq; //arquivo de entrada
        size_t ret; //retorno da funcao de leitura no arquivo de entrada
	double prod_normal, prod_invertido, prod_blocos;
        float prod_interno_par_global = 0; //resultado da soma concorrente
        float *soma_retorno_threads; //auxiliar para retorno das threads

        pthread_t *tid_sistema; //vetor de identificadores das threads no sistema

        //valida e recebe os valores de entrada
        if (argc < 3) {
                printf("Use: %s <arquivo de entrada> <numero de threads> \n", argv[0]);
                exit(-1);
        }

        //abre o arquivo de entrada com os valores para serem somados
        arq = fopen(argv[1], "rb");
        if (arq == NULL) {
                printf("--ERRO: fopen()\n");
                exit(-1);
        }

        //le o tamanho do vetor (primeira linha do arquivo)
        ret = fread(&n, sizeof(long int), 1, arq);
        if (!ret) {
                fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
                return 3;
        }

        //aloca espaco de memoria e carrega o vetor de entrada
        vet1 = (float *) malloc(sizeof(float) * n);
        if (vet1 == NULL) {
                printf("--ERRO: malloc()\n");
                exit(-1);
        }
        ret = fread(vet1, sizeof(float), n, arq);
        if (ret < n) {
                fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
                return 4;
        }

	vet2 = (float *) malloc(sizeof(float) * n);
	if(vet2 == NULL) {
		printf("ERRO: malloc()\n");
		exit(-1);
	}
	ret = fread(vet2, sizeof(float), n, arq);
	if(ret < n) {
		fprintf(stderr, "erro de leitura dos elementos do vetor\n");
		return 5;
	}

	// lendo os produtos sequenciais do arquivo!
	ret = fread(&prod_normal, sizeof(double), 1, arq);
	if(ret != 1) {
		fprintf(stderr, "erro na leitura do prod_normal\n");
		return 6;
	}
	ret = fread(&prod_invertido, sizeof(double), 1, arq);
	if(ret != 1) { 
		fprintf(stderr, "Erro na leitura do prod_invertido\n"); 
		return 7;
	}
	ret = fread(&prod_blocos, sizeof(double), 1, arq);
	if(ret != 1) {
		fprintf(stderr, "Erro na leitura do prod_blocos\n"); 
		return 8; 
	}

	fclose(arq);

        //le o numero de threads da entrada do usuario 
        nthreads = atoi(argv[2]);
        //limita o numero de threads ao tamanho do vetor
        if (nthreads > n) nthreads = n;

        //aloca espaco para o vetor de identificadores das threads no sistema
        tid_sistema = (pthread_t * ) malloc(sizeof(pthread_t) * nthreads);
        if (tid_sistema == NULL) {
                printf("--ERRO: malloc()\n");
                exit(-1);
        }

        //cria as threads
        for (long int i = 0; i < nthreads; i++) {
                if (pthread_create( & tid_sistema[i], NULL, dot_product, (void * ) i)) {
                        printf("--ERRO: pthread_create()\n");
                        exit(-1);
                }
        }

        //espera todas as threads terminarem e calcula a soma total das threads
        //retorno = (float*) malloc(sizeof(float));
        for (int i = 0; i < nthreads; i++) {
                if (pthread_join(tid_sistema[i], (void * ) &soma_retorno_threads)) {
                        printf("--ERRO: pthread_join()\n");
                        exit(-1);
                }
                prod_interno_par_global += * soma_retorno_threads;
                free(soma_retorno_threads);
        }

	printf("\nResultados Sequenciais (lidos do arquivo):\n");
	printf("Produto normal    = %.26f\n", prod_normal);
	printf("Produto invertido = %.26f\n", prod_invertido);
	printf("Produto blocos    = %.26f\n", prod_blocos);

	printf("\nProduto Concorrente = %.26f\n", prod_interno_par_global);
	printf("\nVariacao relativa (normal)    = %.26f\n", var_relativa(prod_normal, prod_interno_par_global));
	printf("Variacao relativa (invertido) = %.26f\n", var_relativa(prod_invertido, prod_interno_par_global));
	printf("Variacao relativa (blocos)    = %.26f\n", var_relativa(prod_blocos, prod_interno_par_global));

        //desaloca os espacos de memoria
        free(vet1);
	free(vet2);
        free(tid_sistema);
        return 0;
}
