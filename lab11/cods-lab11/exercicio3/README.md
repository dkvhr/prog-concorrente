# Exercício 4

## Atividade 1

Para a atividade 1: item 4, eu apenas adicionei a classe que verifica se um número é primo (também implementando a interface Runnable)

A partir daí, executo a função executando pelo pool de threads junto com a função de hello.
Isso quer dizer que vamos ter o dobro de funções a serem executadas dentro do loop.

O output ficou assim:

```
Hello da tarefa 0
Hello da tarefa 5
Hello da tarefa 1
Hello da tarefa 6
Hello da tarefa 2
Hello da tarefa 7
Hello da tarefa 3
Hello da tarefa 4
Hello da tarefa 8
Hello da tarefa 9
o numero: 1 nao eh primo
Hello da tarefa 10
o numero: 10 nao eh primo
Hello da tarefa 11
o numero: 2 eh primo
Hello da tarefa 12
o numero: 11 eh primo
Hello da tarefa 13
o numero: 6 nao eh primo
Hello da tarefa 14
o numero: 0 nao eh primo
o numero: 12 nao eh primo
o numero: 9 nao eh primo
Hello da tarefa 16
o numero: 7 eh primo
Hello da tarefa 17
o numero: 4 nao eh primo
Hello da tarefa 18
o numero: 3 eh primo
Hello da tarefa 19
o numero: 13 eh primo
Hello da tarefa 20
o numero: 5 eh primo
Hello da tarefa 21
o numero: 14 nao eh primo
Hello da tarefa 22
Hello da tarefa 15
Hello da tarefa 23
o numero: 8 nao eh primo
o numero: 15 nao eh primo
o numero: 16 nao eh primo
o numero: 17 eh primo
o numero: 18 nao eh primo
o numero: 19 eh primo
o numero: 20 nao eh primo
o numero: 21 nao eh primo
o numero: 22 nao eh primo
o numero: 23 eh primo
Hello da tarefa 24
o numero: 24 nao eh primo
Terminou
```

Acredito que no começo temos mais hellos sendo executados porque é uma função bem mais rápida de ser executada.

# Atividade 3

## Item 2

Adicionei a classe de verificação de primos `ehPrimo` como `VerificaPrimo`

## Item 3

Para não ter que alterar o nome do arquivo, comentei o antigo `FutureHello` e fiz a nova implementação usando o mesmo nome.

Separei a sequência de números em blocos e chamei a função de verificar primos (usando computação assíncrona) para o intervalo de cada um deles.

No final eu pego e somo o total de primos encontrados

Output:

```
quantidade de primos encontrados: 78498
tempo de execucao: 55 ms
```
