import os
import pandas as pd
import matplotlib.pyplot as plt

os.makedirs("../data/plots", exist_ok=True)
csv_path = "../data/measurements/matrix_measurements.csv"

if not os.path.exists(csv_path):
    print(f"Error: no existe {csv_path}")
    exit(1)

df = pd.read_csv(csv_path)

# Configuración de estilo editorial para gráficos limpios
plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 10
plt.rcParams['axes.edgecolor'] = '#CCCCCC'
plt.rcParams['axes.linewidth'] = 1.0

# Algoritmos y paleta armónica en celeste y rosado pastel
algos = ['naive', 'strassen']
colores = {
    'naive': '#3A86FF',
    'strassen': '#E56B96'
}
markers = {
    'naive': 'o',
    'strassen': 's'
}

# 1. Gráficos de Tiempo por Tipo de Matriz (densa, diagonal, dispersa)
for tipo in df['tipo'].unique():
    subset = df[df['tipo'] == tipo]
    grouped = subset.groupby(['algoritmo', 'n'])['tiempo_us'].mean().reset_index()

    fig, ax = plt.subplots(figsize=(7.5, 4.8), dpi=300)
    for algo in algos:
        algo_data = grouped[grouped['algoritmo'] == algo].sort_values('n')
        if not algo_data.empty:
            ax.plot(
                algo_data['n'], 
                algo_data['tiempo_us'] / 1000.0, 
                marker=markers[algo], 
                color=colores[algo], 
                label=algo,
                linewidth=2.2, 
                markersize=7.0,
                markeredgewidth=1.2,
                markeredgecolor='white'
            )

    ax.set_title(f"Tiempo de Ejecución vs. Dimensión n ({tipo.capitalize()})", fontsize=12, pad=12, fontweight='bold', color='#2B2D42')
    ax.set_xlabel("Dimensión de Matriz (n × n)", fontsize=10, labelpad=8, color='#333333')
    ax.set_ylabel("Tiempo promedio (ms)", fontsize=10, labelpad=8, color='#333333')
    ax.set_xscale('log', base=2)
    ax.set_yscale('log')
    ax.grid(True, which="major", ls="--", color='#E0E0E0', alpha=0.8)
    ax.grid(True, which="minor", ls=":", color='#F0F0F0', alpha=0.5)
    
    legend = ax.legend(frameon=True, facecolor='white', edgecolor='#E0E0E0', fontsize=9)
    legend.get_frame().set_alpha(0.9)
    
    plt.tight_layout()
    plt.savefig(f"../data/plots/tiempo_matriz_{tipo}.png")
    plt.close()

# 2. Gráficos de Tiempo por Dominio (D0 vs D10) para matrices densas
for dom in df['dominio'].unique():
    subset = df[(df['tipo'] == 'densa') & (df['dominio'] == dom)]
    if not subset.empty:
        grouped = subset.groupby(['algoritmo', 'n'])['tiempo_us'].mean().reset_index()

        fig, ax = plt.subplots(figsize=(7.5, 4.8), dpi=300)
        for algo in algos:
            algo_data = grouped[grouped['algoritmo'] == algo].sort_values('n')
            if not algo_data.empty:
                ax.plot(
                    algo_data['n'], 
                    algo_data['tiempo_us'] / 1000.0, 
                    marker=markers[algo], 
                    color=colores[algo], 
                    label=algo,
                    linewidth=2.2, 
                    markersize=7.0,
                    markeredgewidth=1.2,
                    markeredgecolor='white'
                )

        ax.set_title(f"Tiempo vs. Dimensión n (Densa - Dominio {dom})", fontsize=12, pad=12, fontweight='bold', color='#2B2D42')
        ax.set_xlabel("Dimensión de Matriz (n × n)", fontsize=10, labelpad=8, color='#333333')
        ax.set_ylabel("Tiempo promedio (ms)", fontsize=10, labelpad=8, color='#333333')
        ax.set_xscale('log', base=2)
        ax.set_yscale('log')
        ax.grid(True, which="major", ls="--", color='#E0E0E0', alpha=0.8)
        ax.grid(True, which="minor", ls=":", color='#F0F0F0', alpha=0.5)
        
        legend = ax.legend(frameon=True, facecolor='white', edgecolor='#E0E0E0', fontsize=9)
        legend.get_frame().set_alpha(0.9)
        
        plt.tight_layout()
        plt.savefig(f"../data/plots/tiempo_matriz_dominio_{dom}.png")
        plt.close()

# 3. Gráfico de Consumo de Memoria Máxima
mem_grouped = df.groupby(['algoritmo', 'n'])['memoria_kb'].mean().reset_index()
fig, ax = plt.subplots(figsize=(7.5, 4.8), dpi=300)

for algo in algos:
    algo_data = mem_grouped[mem_grouped['algoritmo'] == algo].sort_values('n')
    if not algo_data.empty:
        ax.plot(
            algo_data['n'], 
            algo_data['memoria_kb'] / 1024.0, 
            marker=markers[algo], 
            color=colores[algo], 
            label=algo,
            linewidth=2.2, 
            markersize=7.0,
            markeredgewidth=1.2,
            markeredgecolor='white'
        )

ax.set_title("Pico de Memoria Máxima vs. Dimensión n (Matrices)", fontsize=12, pad=12, fontweight='bold', color='#2B2D42')
ax.set_xlabel("Dimensión de Matriz (n × n)", fontsize=10, labelpad=8, color='#333333')
ax.set_ylabel("Memoria máxima residente (MB)", fontsize=10, labelpad=8, color='#333333')
ax.set_xscale('log', base=2)
ax.grid(True, which="major", ls="--", color='#E0E0E0', alpha=0.8)
ax.grid(True, which="minor", ls=":", color='#F0F0F0', alpha=0.5)

legend = ax.legend(frameon=True, facecolor='white', edgecolor='#E0E0E0', fontsize=9)
legend.get_frame().set_alpha(0.9)

plt.tight_layout()
plt.savefig("../data/plots/memoria_matrices.png")
plt.close()

print("¡Todos los gráficos de matrices fueron generados con éxito en data/plots/!")