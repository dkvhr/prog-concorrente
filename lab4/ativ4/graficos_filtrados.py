import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os.path
import sys

def main():
    csv_file = 'resultados_primos.csv'
    if not os.path.isfile(csv_file):
        print(f"o arquivo {csv_file} nao foi encontrado")
        sys.exit(1)
    
    df = pd.read_csv(csv_file)
    
    if len(df) < 1:
        print("o arquivo csv esta incompleto")
        sys.exit(1)
    
    df_filtered = df[df['N'] != 15000000]
    
    if len(df_filtered) < 1:
        print("nao ha dados apos a exclusao de N=15000000.")
        sys.exit(1)
    
    if not os.path.exists('graficos_filtrados'):
        os.makedirs('graficos_filtrados')
    
    valores_N = sorted(df_filtered['N'].unique())
    valores_threads = sorted(df_filtered['threads'].unique())
    
    plt.figure(figsize=(12, 8))
    for n in valores_N:
        df_n = df_filtered[df_filtered['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['tempo_concorrente'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['tempo_concorrente'], marker='o', label=f'N={n}')
    
    plt.title('tempo de execucao vs numero de threads (sem N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('tempo de execucao (segundos)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados/tempo_execucao_vs_threads.png')
    
    plt.figure(figsize=(12, 8))
    for n in valores_N:
        df_n = df_filtered[df_filtered['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['aceleracao'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['aceleracao'], marker='o', label=f'N={n}')
    
    max_threads = max(valores_threads)
    plt.plot([1, max_threads], [1, max_threads], 'k--', label='aceleracao ideal')
    plt.title('aceleracao vs numero de threads (sem N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('aceleracao')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados/aceleracao_vs_threads.png')
    
    plt.figure(figsize=(12, 8))
    for n in valores_N:
        df_n = df_filtered[df_filtered['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['eficiencia'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['eficiencia'], marker='o', label=f'N={n}')
    
    plt.axhline(y=1, color='k', linestyle='--', label='eficiencia ideal')
    plt.title('eficiencia vs numero de threads (sem N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('eficiencia')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados/eficiencia_vs_threads.png')
    
    plt.figure(figsize=(12, 8))
    for t in valores_threads:
        if t > 1:
            df_t = df_filtered[df_filtered['threads'] == t]
            if len(df_t) > 1:
                grouped = df_t.groupby('N')['tempo_concorrente'].mean().reset_index()
                plt.plot(grouped['N'], grouped['tempo_concorrente'], marker='o', label=f'{t} threads')
    
    df_seq = df_filtered[df_filtered['threads'] > 0]
    grouped_seq = df_seq.groupby('N')['tempo_sequencial'].mean().reset_index()
    plt.plot(grouped_seq['N'], grouped_seq['tempo_sequencial'], marker='o', label='sequencial (1 thread)')
    plt.title('tempo de execucao vs N (sem N=15000000)')
    plt.xlabel('N')
    plt.ylabel('tempo de execucao (segundos)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados/tempo_execucao_vs_N.png')
    
    plt.figure(figsize=(12, 8))
    grouped = df_filtered.groupby('threads')['eficiencia'].mean().reset_index()
    plt.bar(grouped['threads'], grouped['eficiencia'], color='skyblue')
    plt.axhline(y=1, color='r', linestyle='--', label='eficiencia ideal')
    plt.title('eficiencia media por numero de threads (sem N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('eficiencia media')
    plt.grid(True, axis='y')
    plt.legend()
    plt.savefig('graficos_filtrados/eficiencia_media_barras.png')
    
    plt.figure(figsize=(14, 8))
    bar_width = 0.8 / len(valores_N)
    for i, n in enumerate(valores_N):
        df_n = df_filtered[df_filtered['N'] == n]
        df_n = df_n[df_n['threads'] > 1]
        grouped = df_n.groupby('threads')['aceleracao'].mean().reset_index()
        positions = np.array(range(len(grouped['threads']))) + i * bar_width
        plt.bar(positions, grouped['aceleracao'], width=bar_width, label=f'N={n}')
        
    plt.xticks(np.array(range(len(valores_threads)-1)) + bar_width * (len(valores_N) / 2 - 0.5), 
               [t for t in valores_threads if t > 1])
    
    plt.title('comparacao da aceleracao por numero de threads para diferentes valores de N')
    plt.xlabel('numero de threads')
    plt.ylabel('aceleracao media')
    plt.grid(True, axis='y')
    plt.legend()
    plt.savefig('graficos_filtrados/comparacao_aceleracao_barras.png')
    
    print("graficos filtrados (sem N=15000000) gerados em 'graficos_filtrados/'")

if __name__ == "__main__":
    main()
