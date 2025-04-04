//"Hello world" concorrente usando a biblioteca pthread de C

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//estrutura para receber mais de um argumento na funcao da thread
typedef struct {
   int id, nthreads;
} t_Args;

//funcao executada pela thread
void * print_hello(void *arg) {
   t_Args *args; //identificador da thread na aplicacao e numero de threads
   args = (t_Args*) arg; //typecasting de ponteiros
   printf("Sou a thread %d de %d threads.\n", args->id, args->nthreads);
   pthread_exit(NULL); //funcao de termino da thread, para retorno de valores
}

//funcao principal
int main(int argc, char* argv[]) {
   //variaveis do programa
   pthread_t *tid; //vetor para guardar os identificadores das threads no sistema
   int nthreads; //numero de threads

   //verificar argumentos de entrada
   if(argc < 2 ) {
      printf("ERRO de entrada de parametros: Digite %s <numero de threads>\n", argv[0]);
      return 1;
   }
   //armazenar o numero de threads
   nthreads = atoi(argv[1]); 

   //alocar espaco para o vetor tid
   tid = (pthread_t*) malloc (sizeof(pthread_t) * nthreads);
   if(tid==NULL){
      printf("ERRO de alocacao de memoria\n");
      return 2;
   }   
   
   //criar os fluxos de execução (threads)
   for(long int i=0; i<nthreads; i++) {
      //aloca espaco de memoria para a estrutura com argumentos para a funcao da thread
      t_Args *args = (t_Args*) malloc (sizeof(t_Args));
      if(args==NULL){
         printf("ERRO de alocacao de memoria\n");
         return 2;
      }   
      //preenche os campos da estrutura para a thread i
      args->id = i;
      args->nthreads = nthreads; 
      //cria a thread     
      int ret = pthread_create(&tid[i], NULL, print_hello, (void*) args);
      if(ret) { //retorna 0 quando ocorre com sucesso e diferente de 0 caso contrario
         printf("ERRO %d na criacao da thread %ld\n", ret, i);
         return 3;
      } 
   }

   //esperar os fluxos terminarem suas execucoes
   for(int i=0; i<nthreads; i++) {
      int ret = pthread_join(tid[i], NULL);
      if(ret) { //retorna 0 quando ocorre com sucesso e diferente de 0 caso contrario
         printf("ERRO %d na espera da thread %d\n", ret, i);
         return 4;
      }   
   }
   
   printf("FIM\n");
   //finalizar o programa
   return 0;
}
