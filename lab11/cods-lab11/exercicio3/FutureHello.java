/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;

import java.lang.Math;

//classe runnable
class MyCallable implements Callable<Long> {
  //construtor
  MyCallable() {
  }
 
  //método para execução
  public Long call() throws Exception {
    long s = 0;
    for (long i=1; i<=100; i++) {
      s++;
    }
    return s;
  }
}

class VerificaPrimo implements Callable<Integer> {
	private final long inicio;
	private final long fim;

	public VerificaPrimo(long inicio, long fim) {
		this.inicio = inicio;
		this.fim = fim;
	}

	private boolean ehPrimo(long n) {
		if (n <=1) return false;
		if (n==2) return true;
		if (n % 2 == 0) return false;
		for (long i = 3; i<= Math.sqrt(n); i += 2) {
			if (n % i == 0) return false;
		}
		return true;
	}

	@Override
	public Integer call() throws Exception {
		int count = 0;
		for(long i = inicio; i <= fim; i++) {
			if(ehPrimo(i)) {
				count++;
			}
		}
		return count;
	}
}

public class FutureHello {
	private static final long N = 1_000_000L;
	private static final int NTHREADS = 10;
	private static final int NTASKS = 100;

	public static void main(String[] args) {
		long start_time = System.currentTimeMillis();

		ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
		List<Future<Integer>> lista_futuros = new ArrayList<>();

		long tamanho_bloco = N / NTASKS;
		long inicio = 1;
		long fim;

		for(int i = 0; i < NTASKS; i++) {
			fim = inicio + tamanho_bloco - 1;
			if (i == NTASKS - 1) {
				fim = N;
			}

			Callable<Integer> tarefa = new VerificaPrimo(inicio, fim);
			Future<Integer> futuro = executor.submit(tarefa);
			lista_futuros.add(futuro);

			inicio = fim + 1;
		}

		long total_primos = 0;
		for (Future<Integer> futuro : lista_futuros) {
			try {
				total_primos += futuro.get();
			} catch (InterruptedException | ExecutionException e) {}
		}

		executor.shutdown();

		long end_time = System.currentTimeMillis();
		long duration = end_time - start_time;

		System.out.println("quantidade de primos encontrados: " + total_primos);
		System.out.println("tempo de execucao: " + duration + " ms");
	}
}

//classe do método main
/*
public class FutureHello  {
  private static final int N = 3;
  private static final int NTHREADS = 10;

  public static void main(String[] args) {
    //cria um pool de threads (NTHREADS)
    ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
    //cria uma lista para armazenar referencias de chamadas assincronas
    List<Future<Long>> list = new ArrayList<Future<Long>>();

    for (int i = 0; i < N; i++) {
      Callable<Long> worker = new MyCallable();
      Future<Long> submit = executor.submit(worker);
      list.add(submit);
    }

    System.out.println(list.size());
    //pode fazer outras tarefas...

    //recupera os resultados e faz o somatório final
    long sum = 0;
    for (Future<Long> future : list) {
      try {
        sum += future.get(); //bloqueia se a computação nao tiver terminado
      } catch (InterruptedException e) {
        e.printStackTrace();
      } catch (ExecutionException e) {
        e.printStackTrace();
      }
    }
    System.out.println(sum);
    executor.shutdown();
  }
}
*/
