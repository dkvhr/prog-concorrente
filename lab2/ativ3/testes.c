#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX 1000
#define TEXTO

void gera_entrada(float *entrada, int n) {
	float elem;
	float soma = 0;
	for(long int i=0; i<n; i++) {
		elem = (rand() % MAX)/3.0;
		entrada[i] = elem;
		soma += elem;
	}
}

double dot_product(float *ent1, float *ent2, long int n) {
	double soma = 0;
	for(int i=0; i<n; i++) {
		soma += ent1[i] * ent2[i];
	}
	return soma;
}

int salva_resultado(long int n, float *ent1, float *ent2,
					const char *filename, double *res) {
	FILE *filp = fopen(filename, "wb");
	size_t ret;
	if(filp == NULL) {
		fprintf(stderr, "Erro ao abrir o arquivo!");
		return 3;
	}
	ret = fwrite(&n, sizeof(long int), 1, filp);
	ret = fwrite(ent1, sizeof(float), n, filp);
	if (ret < n) {
		fprintf(stderr, "erro de escrita da entrada 1 no arquivo!\n");
		return 4;
	}
	ret = fwrite(ent2, sizeof(float), n, filp);
	if (ret < n) {
		fprintf(stderr, "erro de escrita da entrada 2 no arquivo!\n");
		return 5;
	}

	*res = dot_product(ent1, ent2, n);
	ret = fwrite(&res, sizeof(double), 1, filp);
	if (ret < 1) {
		fprintf(stderr, "erro de escrita no arquivo!\n");
		return 6;
	}

	return 0;
}

int main(int argc, char **argv) {
	// pegando a entrada
	long int n;
	if (argc < 3) {
		fprintf(stderr, "Entrada: %s <dimensao> <nome arquivo de saida>\n", argv[0]);
		return 1;
	}
	n = atoi(argv[1]);
	
	// alocando vetores de entrada de dimensao n
	float *vetor1 = (float *) malloc(sizeof(float) * n);
	if(vetor1 == NULL) {
		fprintf(stderr, "erro ao alocar memoria do vetor\n");
		return 2;
	
	}
	float *vetor2 = (float *) malloc(sizeof(float) * n);
	if(vetor2 == NULL) {
		fprintf(stderr, "erro ao alocar memoria do vetor\n");
		return 2;

	}

	srand(time(NULL));
	gera_entrada(vetor1, n);
	gera_entrada(vetor2, n);

	double *res = (double *) malloc(sizeof(double));
	salva_resultado(n, vetor1, vetor2, argv[2], res);
	// imprime na saida padrao
	#ifdef TEXTO
	fprintf(stdout, "%ld\n", n);
	for(long int i=0; i<n; i++) {
		fprintf(stdout, "%f ", vetor1[i]);
	}
	fprintf(stdout, "\n");
	for(long int i=0; i<n; i++) {
		fprintf(stdout, "%f ", vetor2[i]);
	}
	fprintf(stdout, "\n");
	fprintf(stdout, "result: %f\n", *res);
	#endif

}
