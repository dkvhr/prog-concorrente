#!/usr/bin/bash

valores_N=(1000 10000 100000 1000000 15000000)

valores_threads=(1 2 4 8 12 16)

repeticoes=50

for n in "${valores_N[@]}"; do
	for t in "${valores_threads[@]}"; do
		echo "[+] executando com N=$n e threads=$t"
		for (( i=1; i<=$repeticoes; i++ )); do
			echo "[+] repeticao $i de $repeticoes"
			./primo $n $t
			sleep 0.2
		done
	done
done

echo "done!"
