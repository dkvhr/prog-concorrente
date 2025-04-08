# Avaliacao dos resultados encontrados

## Produto Sequencial

### Comparações entre os resultados

A partir dos resultados gerados pelo produto sequencial (normal, invertido e com blocos), é difícil perceber uma diferença significativa entre eles. Eu pude perceber uma pequena diferença quando são 65536 dimensões. Nesse caso, o produto usando blocos teve um resultado um pouco diferente dos outros produtos.

Acredito que a diferença não foi tão significativa porque a precisão de ponto flutuante é bem alta nesse caso. Além disso, como os termos dos vetores são gerados com valores relativamente controlados (com a mesma escala e alternância de sinal), os erros de arredondamento se cancelam ou se acumulam de forma muito semelhante em todos os métodos.

Então os valores estão dentro do esperado, com uma diferença bem pequena e inexistente em alguns casos.

## Produto concorrente

Quando usando threads, o resultado do produto possui bem menos casas decimais. Isso ocorre porque existe uma propagação menor de erro, já que você meio que faz o mesmo que o produto em blocos, mas dessa vez com `n` blocos (número de threads) ao invés de somente 2 blocos.

### Variação relativa

Os resultados absolutos são claramente bem diferentes entre si.

Quando comparando com o resultado do produto concorrente usando variação relativa, os resultados parecem bem similares. As diferenças começam a aparecer só lá para a sétima ou oitava casa decimal.

### Observação

Quando comecei a usar um número grande de threads (nesse caso 65536), o programa começou a demorar alguns segundos para executar. Isso provavelmente ocorre por conta do overhead de criar várias threads. Criar uma thread para cada elemento do vetor não parece compensar muito por conta desse overhead.

Isso se confirma quando você faz o produto concorrente com os vetores de 65536 dimensões e apenas 8 threads, que nesse caso não demora praticamente nada para executar.

No entanto, a variação relativa parece diminuir. Nesse caso eu acredito que o produto concorrente usando mais threads seja mais preciso. Dessa forma, diminuir a variação relativa significa estar mais perto de um resultado errado, então eu considero isso uma perda de precisão.
