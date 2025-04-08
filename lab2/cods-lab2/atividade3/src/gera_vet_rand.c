/*
Programa auxiliar para gerar um vetor de floats 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000 //valor maximo de um elemento do vetor
//descomentar o define abaixo caso deseje imprimir uma versao do vetor gerado no formato texto
#define TEXTO 

double dot_product(float *ent1, float *ent2, long int n) {
	double soma = 0;
	for(int i=0; i<n; i++) {
		soma += ent1[i] * ent2[i];
	}
	return soma;
}

int main(int argc, char*argv[]) {
	float *vetor1, *vetor2; //vetor que será gerada
	long int n; //qtde de elementos do vetor
	float elem; //valor gerado para incluir no vetor
	int fator=1; //fator multiplicador para gerar números negativos
	FILE * descritorArquivo; //descritor do arquivo de saida
	size_t ret; //retorno da funcao de escrita no arquivo de saida

	//recebe os argumentos de entrada
	if(argc < 3) {
		fprintf(stderr, "Digite: %s <dimensao> <nome arquivo saida>\n", argv[0]);
		return 1;
	}
	n = atoi(argv[1]);

	//aloca memoria para o vetor
	vetor1 = (float*) malloc(sizeof(float) * n);
	if(!vetor1) {
		fprintf(stderr, "Erro de alocacao da memoria do vetor 1\n");
		return 2;
	}

	vetor2 = (float*) malloc(sizeof(float) * n);
	if(!vetor2) {
		fprintf(stderr, "erro de alocacao da memoria do vetor 2\n");
		return 3;
	}
      

	//preenche o vetor com valores float aleatorios
	srand(time(NULL));
	for(long int i=0; i<n; i++) {
		elem = (rand() % MAX)/3.0 * fator;
		vetor1[i] = elem;
		fator*=-1;
	}
	for(long int i=0; i<n; i++) {
		elem = (rand() % MAX)/3.0 * fator;
		vetor2[i] = elem;
		fator *= -1;
	}

	// calculos dos produtos internos
	double prod_normal = dot_product(vetor1, vetor2, n);

	double prod_invertido = 0;
	for(long int i = n - 1; i >= 0; i--) {
		prod_invertido += vetor1[i] * vetor2[i];
	}

	double prod_bloco1 = 0, prod_bloco2 = 0;
	long int meio = n/2;
	for(long int i = 0; i < meio; i++) {
		prod_bloco1 += vetor1[i] * vetor2[i];
	}
	for(long int i = meio; i < n; i++) {
		prod_bloco2 += vetor1[i] * vetor2[i];
	}
	double prod_blocos = prod_bloco1 + prod_bloco2;

	//imprimir na saida padrao o vetor gerado
#ifdef TEXTO
	fprintf(stdout, "%ld\n", n);
	for(long int i=0; i<n; i++) {
		fprintf(stdout, "%f ",vetor1[i]);
	}
	fprintf(stdout, "\n");
	for(long int i=0; i<n; i++) {
		fprintf(stdout, "%f ", vetor2[i]);
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "Resultados sequenciais:\n");
	fprintf(stdout, "Produto normal   = %.26f\n", prod_normal);
	fprintf(stdout, "Produto invertido= %.26f\n", prod_invertido);
	fprintf(stdout, "Produto blocos   = %.26f\n", prod_blocos);
#endif

	//escreve o vetor no arquivo
	//abre o arquivo para escrita binaria
	descritorArquivo = fopen(argv[2], "wb");
	if(!descritorArquivo) {
		fprintf(stderr, "Erro de abertura do arquivo\n");
		return 3;
	}
	//escreve a dimensao
	ret = fwrite(&n, sizeof(long int), 1, descritorArquivo);
	//escreve os elementos do vetor
	ret = fwrite(vetor1, sizeof(float), n, descritorArquivo);
	if(ret < n) {
		fprintf(stderr, "Erro de escrita no  arquivo\n");
		return 4;
	}
	ret = fwrite(vetor2, sizeof(float), n, descritorArquivo);
	if(ret < n) {
		fprintf(stderr, "erro de escrita no arquivo\n");
		return 5;
	}
	//escreve os resultadso
	ret = fwrite(&prod_normal, sizeof(double), 1, descritorArquivo);
	ret = fwrite(&prod_invertido, sizeof(double), 1, descritorArquivo);
	ret = fwrite(&prod_blocos, sizeof(double), 1, descritorArquivo);

	//finaliza o uso das variaveis
	fclose(descritorArquivo);
	free(vetor1);
	free(vetor2);
	return 0;
} 
