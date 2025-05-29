Os argumentos são lidos pela entrada padrão ao invés de argumentos da CLI.

Para a corretude, testei com alguns primos:

Para até 10:

```
$ ./primos 
10 2 2
total de primos encontrados: 4
thread consumidora vencedora: 2 (encontrou 3 primos)

$ ./primos
10 1 1
total de primos encontrados: 4
thread consumidora vencedora: 1 (encontrou 4 primos)
```

Para até 100:

```
$ ./primos
100 1 1
total de primos encontrados: 25
thread consumidora vencedora: 1 (encontrou 25 primos)

$ ./primos
100 1 2
total de primos encontrados: 25
thread consumidora vencedora: 1 (encontrou 25 primos)

$ ./primos
100 1 2
total de primos encontrados: 25
thread consumidora vencedora: 2 (encontrou 22 primos)

$ ./primos
100 2 2
total de primos encontrados: 25
thread consumidora vencedora: 2 (encontrou 18 primos)

$ ./primos
100 2 2
total de primos encontrados: 25
thread consumidora vencedora: 1 (encontrou 19 primos)
```

Para até 10000

```
$ ./primos
10000 1 1
total de primos encontrados: 1229
thread consumidora vencedora: 1 (encontrou 1229 primos)

$ ./primos
10000 2 2
total de primos encontrados: 1229
thread consumidora vencedora: 2 (encontrou 690 primos)

$ ./primos
10000 4 1
total de primos encontrados: 1229
thread consumidora vencedora: 1 (encontrou 1229 primos)

$ ./primos
10000 4 8 
total de primos encontrados: 1229
thread consumidora vencedora: 6 (encontrou 177 primos)
```
