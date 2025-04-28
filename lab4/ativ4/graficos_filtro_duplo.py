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
        print("o arquivo CSV esta incompleto")
        sys.exit(1)
    
    df_filtered = df[(df['N'] != 15000000) & (df['N'] != 10000000)]
    
    if len(df_filtered) < 1:
        print("nao ha dados apos a exclusão de N=10000000 e N=15000000")
        sys.exit(1)
    
    if not os.path.exists('graficos_filtrados_duplo'):
        os.makedirs('graficos_filtrados_duplo')
    
    valores_N = sorted(df_filtered['N'].unique())
    valores_threads = sorted(df_filtered['threads'].unique())
    
    plt.figure(figsize=(12, 8))
    
    for n in valores_N:
        df_n = df_filtered[df_filtered['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['tempo_concorrente'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['tempo_concorrente'], marker='o', label=f'N={n}')
    
    plt.title('tempo de execucao vs numero de threads\n(Sem N=10000000 e N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('tempo de execucao (segundos)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados_duplo/tempo_execucao_vs_threads.png')
    
    plt.figure(figsize=(12, 8))
    
    for n in valores_N:
        df_n = df_filtered[df_filtered['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['aceleracao'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['aceleracao'], marker='o', label=f'N={n}')
    
    max_threads = max(valores_threads)
    plt.plot([1, max_threads], [1, max_threads], 'k--', label='aceleracao ideal')
    
    plt.title('aceleracao vs numero de threads\n(Sem N=10000000 e N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('aceleracao')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados_duplo/aceleracao_vs_threads.png')
    
    plt.figure(figsize=(12, 8))
    
    for n in valores_N:
        df_n = df_filtered[df_filtered['N'] == n]
        if len(df_n) > 1:
            grouped = df_n.groupby('threads')['eficiencia'].mean().reset_index()
            plt.plot(grouped['threads'], grouped['eficiencia'], marker='o', label=f'N={n}')
    
    plt.axhline(y=1, color='k', linestyle='--', label='eficiencia ideal')
    
    plt.title('eficiencia vs numero de threads\n(Sem N=10000000 e N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('eficiencia')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados_duplo/eficiencia_vs_threads.png')
    
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
    
    plt.title('tempo de execucao vs N\n(Sem N=10000000 e N=15000000)')
    plt.xlabel('N')
    plt.ylabel('tempo de execucao (segundos)')
    plt.grid(True)
    plt.legend()
    plt.savefig('graficos_filtrados_duplo/tempo_execucao_vs_N.png')
    
    plt.figure(figsize=(12, 8))
    
    grouped = df_filtered.groupby('threads')['eficiencia'].mean().reset_index()
    
    plt.bar(grouped['threads'], grouped['eficiencia'], color='skyblue')
    plt.axhline(y=1, color='r', linestyle='--', label='eficiencia ideal')
    
    plt.title('eficiencia media por numero de threads\n(Sem N=10000000 e N=15000000)')
    plt.xlabel('numero de threads')
    plt.ylabel('eficiencia media')
    plt.grid(True, axis='y')
    plt.legend()
    plt.savefig('graficos_filtrados_duplo/eficiencia_media_barras.png')
    
    if len(valores_N) > 1:
        plt.figure(figsize=(14, 8))
        
        bar_width = 0.8 / len(valores_N)
        
        threads_para_barras = [t for t in valores_threads if t > 1]
        
        for i, n in enumerate(valores_N):
            df_n = df_filtered[df_filtered['N'] == n]
            df_n = df_n[df_n['threads'] > 1]  # Apenas threads > 1
            
            if len(df_n) > 0:
                grouped = df_n.groupby('threads')['aceleracao'].mean().reset_index()
                
                aceleracoes = []
                for t in threads_para_barras:
                    thread_data = grouped[grouped['threads'] == t]
                    if len(thread_data) > 0:
                        aceleracoes.append(thread_data['aceleracao'].values[0])
                    else:
                        aceleracoes.append(0)
                
                positions = np.array(range(len(threads_para_barras))) + i * bar_width
                
                plt.bar(positions, aceleracoes, width=bar_width, label=f'N={n}')
        
        plt.xticks(np.array(range(len(threads_para_barras))) + bar_width * (len(valores_N) / 2 - 0.5), 
                   threads_para_barras)
        
        plt.title('comparacao da aceleracao por numero de threads para diferentes valores de N\n(sem N=10000000 e N=15000000)')
        plt.xlabel('numero de threads')
        plt.ylabel('aceleracao media')
        plt.grid(True, axis='y')
        plt.legend()
        plt.savefig('graficos_filtrados_duplo/comparacao_aceleracao_barras.png')
    
    print("graficos filtrados (sem N=10000000 e N=15000000) gerados em 'graficos_filtrados_duplo/'")

if __name__ == "__main__":
    main()
