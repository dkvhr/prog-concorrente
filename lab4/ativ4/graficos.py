import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os.path
import sys

def main():
    csv_file = 'resultados_primos.csv'
    if not os.path.isfile(csv_file):
        print(f"o arquivo {csv_file} não foi encontrado.")
        sys.exit(1)
    
    df = pd.read_csv(csv_file)
    
    if len(df) < 1:
        print("o arquivo csv esta incompleto")
        sys.exit(1)
    if not os.path.exists('graficos'):
        os.makedirs('graficos')
    
    valores_N = sorted(df['N'].unique())
    valores_threads = sorted(df['threads'].unique())
    plt.figure(figsize=(12, 8))
    for n in valores_N:
        df_n = df[df['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['tempo_concorrente'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['tempo_concorrente'], marker='o', label=f'N={n}')
    
    plt.title('tempo de execucao x numero de threads')
    plt.xlabel('numero de threads')
    plt.ylabel('tempo de execucao (segundos)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos/tempo_execucao_vs_threads.png')
    plt.figure(figsize=(12, 8))
    
    for n in valores_N:
        df_n = df[df['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['aceleracao'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['aceleracao'], marker='o', label=f'N={n}')
    
    max_threads = max(valores_threads)
    plt.plot([1, max_threads], [1, max_threads], 'k--', label='aceleracao ideal')
    
    plt.title('aceleracao x numero de threads')
    plt.xlabel('numero de threads')
    plt.ylabel('aceleracao')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos/aceleracao_vs_threads.png')
    plt.figure(figsize=(12, 8))
    
    for n in valores_N:
        df_n = df[df['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['eficiencia'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['eficiencia'], marker='o', label=f'N={n}')
    
    plt.axhline(y=1, color='k', linestyle='--', label='eficiencia ideal')
    
    plt.title('eficiencia x numero de threads')
    plt.xlabel('numero de threads')
    plt.ylabel('eficiencia')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos/eficiencia_vs_threads.png')
    plt.figure(figsize=(12, 8))
    
    for t in valores_threads:
        if t > 1:
            df_t = df[df['threads'] == t]
            if len(df_t) > 1:
                grouped = df_t.groupby('N')['tempo_concorrente'].mean().reset_index()
                plt.plot(grouped['N'], grouped['tempo_concorrente'], marker='o', label=f'{t} threads')

    df_seq = df[df['threads'] > 0]
    grouped_seq = df_seq.groupby('N')['tempo_sequencial'].mean().reset_index()
    plt.plot(grouped_seq['N'], grouped_seq['tempo_sequencial'], marker='o', label='Sequencial (1 thread)')
    
    plt.title('tempo de execucao vs N')
    plt.xlabel('N')
    plt.ylabel('tempo de execucao (segundos)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos/tempo_execucao_vs_N.png')
    plt.figure(figsize=(12, 8))
    
    grouped = df.groupby('threads')['eficiencia'].mean().reset_index()
    
    plt.bar(grouped['threads'], grouped['eficiencia'], color='skyblue')
    plt.axhline(y=1, color='r', linestyle='--', label='eficiencia ideal')
    
    plt.title('eficiencia media por numero de threads')
    plt.xlabel('numero de threads')
    plt.ylabel('eficiencia media')
    plt.grid(True, axis='y')
    plt.legend()
    plt.savefig('graficos/eficiencia_media_barras.png')
    
    print("graficos gerados em 'graficos/'")

if __name__ == "__main__":
    main()
