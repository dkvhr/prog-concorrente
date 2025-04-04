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
	float produtos_locais = 0, *ret;

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


double var_relativa(double *val_seq, float *val_conc) {
	double var = (*val_seq - *val_conc) / *val_seq;
	return var;
}


//funcao principal do programa
int main(int argc, char * argv[]) {
        FILE * arq; //arquivo de entrada
        size_t ret; //retorno da funcao de leitura no arquivo de entrada
        double soma_ori; //soma registrada no arquivo
        #ifdef VERSOES
        float prod_seq, prod_seq2, prod_seq2_2; //resultados das somas adicionais
	float soma_seq_blocos;
        #endif
        float prod_interno_par_global; //resultado da soma concorrente
        float * soma_retorno_threads; //auxiliar para retorno das threads

        pthread_t * tid_sistema; //vetor de identificadores das threads no sistema

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
        ret = fread( & n, sizeof(long int), 1, arq);
        if (!ret) {
                fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
                return 3;
        }

        //aloca espaco de memoria e carrega o vetor de entrada
        vet1 = (float * ) malloc(sizeof(float) * n);
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

        #ifdef VERSOES
        // sequencial de traz para frente
        prod_seq = 0;
        for (int t = n - 1; t >= 0; t--) {
                prod_seq += vet1[t] * vet2[t];
        }
        // sequencial bloco (== soma com 2 threads)
        prod_seq2 = 0;
        for (int t = 0; t < n / 2; t++) {
                prod_seq2 += vet1[t] * vet2[t];
        }
        prod_seq2_2 = 0;
        for (int t = n / 2; t < n; t++) {
                prod_seq2_2 += vet1[t] * vet2[t];
        }
        soma_seq_blocos = prod_seq2 + prod_seq2_2;
        #endif

        //espera todas as threads terminarem e calcula a soma total das threads
        //retorno = (float*) malloc(sizeof(float));
        prod_interno_par_global = 0;
        for (int i = 0; i < nthreads; i++) {
                if (pthread_join(tid_sistema[i], (void * ) &soma_retorno_threads)) {
                        printf("--ERRO: pthread_join()\n");
                        exit(-1);
                }
                prod_interno_par_global += * soma_retorno_threads;
                free(soma_retorno_threads);
        }

        //imprime os resultados
        printf("\n");
        #ifdef VERSOES
        printf("soma_seq (invertida)         = %.26f\n\n", prod_seq);
        printf("soma_seq_blocos (2 blocos)   = %.26f\n\n", soma_seq_blocos);
        #endif
        printf("soma_concorrente             = %.26f\n", prod_interno_par_global);
        //le o somatorio registrado no arquivo
        ret = fread( &soma_ori, sizeof(double), 1, arq);
        printf("\nSoma-ori                   = %.26lf\n", soma_ori);

	printf("\nVariacao relativa          = %.26lf\n", var_relativa(&soma_ori, &prod_interno_par_global));

        //desaloca os espacos de memoria
        free(vet1);
	free(vet2);
        free(tid_sistema);
        //fecha o arquivo
        fclose(arq);
        return 0;
}
