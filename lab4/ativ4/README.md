# Atividade 4 do lab 4

## Especificações e considerações

Os programas dessa atividade foram executados em um processador i7-10750H com 6 núcleos e 12 threads.

Os valores de N foram: 1000, 10000, 100000, 1000000, 15000000

Para o número de threads, os valores foram: 1, 2, 4, 8, 12, 16

Cada configuração foi executada 50 vezes.

Usei o arquivo `script.sh` para isso.

Nos gráficos, considerei criar alguns separados sem o N = 15000000, já que os resultados para esse valor de N faziam com que alguns dos gráficos não ficassem visualmente bons.

Os gráficos foram gerados com o matplotlib.

## Avaliação dos gráficos

### Tempo de execução vs N

Olhando os primeiros gráficos de tempo de execução (tempo de execução vs. N), é possível ver algumas coisas interessantes:

Para tempo de execucao vs N:

![t_exec_vs_N_1](/graficos_filtrados/tempo_execucao_vs_N.png)

![t_exec_vs_N_2](/graficos/tempo_execucao_vs_N.png)

Os tempos de execução obviamente aumentam quanto maior o valor de N. No entanto, isso não parece seguir um comportamento linear.

### Tempo de execução vs Threads

Para o N = 15000000, o tempo de execução parece diminuir conforme aumentamos o número de threads. No entanto, esse tempo de execução não começa a diminuir tanto assim a partir de um certo número de threads.

Para o N = 1000000, o tempo de execução na verdade começa a aumentar!

![t_exec_vs_threads_1](/graficos_filtrados/tempo_execucao_vs_threads.png)

![t_exec_vs_threads_2](/graficos/tempo_execucao_vs_threads.png)

Parece que o overhead de criar mais threads passa a não valer mais a pena a partir de 8 threads nesse caso do N = 1000000.

Vamos conseguir olhar melhor esses dois comportamentos nos próximos gráficos de eficiência e aceleração.

### Aceleração vs Threads

![acel_vs_threads_1](/graficos_filtrados/aceleracao_vs_threads.png)

![acel_vs_threads_2](/graficos/aceleracao_vs_threads.png)

A aceleração não parece aumentar muito (na verdade ela diminui bem rapidamente) para valores pequenos de N.

Para N = 1000000, a aceleração parece aumentar no começo, mas ela para de escalar a partir de 4 threads. A partir de 8 threads ela começa a cair.

Para N = 15000000, a aceleração aumenta bastante até 4 threads. A partir de 8 ela não aumenta muito. Em 16 threads ela começa a cair.

Isso demonstra que aumentar o número de threads não começa a valer tanto a pena assim a partir de um determinado número.

Aqui um gráfico de barras demonstrando melhor esse comportamento (sem o N = 15000000):

![acel_barras](/graficos_filtrados/comparacao_aceleracao_barras.png)

### Eficiência vs Threads

Agora vamos olhar para a eficiência:

![efic_vs_threads_2](/graficos/eficiencia_vs_threads.png)

Antes de qualquer coisa, é possível ver que a eficiência está acima de 1 em alguns casos. Acredito que isso seja um problema de ponto flutuante e esses casos devem ser descartados.

Com o aumento do número de threads, a eficiência acaba diminuindo. Isso acaba acontecendo porque o overhead da criação de threads, somado ao overhead de usar mutexes acaba sendo maior do que o ganho esperado com o uso de mais threads. Dessa forma, apesar do tempo de execução (na maioria dos casos) cair com o aumento de threads, esse overhead faz com que ele não caia de forma ideal.