import os
import pandas as pd
import matplotlib.pyplot as plt

os.makedirs("../data/plots", exist_ok=True)
csv_path = "../data/measurements/sorting_measurements.csv"

if not os.path.exists(csv_path):
    print(f"Error: no existe {csv_path}")
    exit(1)

df = pd.read_csv(csv_path)

plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['font.size'] = 10
plt.rcParams['axes.edgecolor'] = '#CCCCCC'
plt.rcParams['axes.linewidth'] = 1.0

algos = ['sort', 'quickSort', 'mergeSort', 'patienceSort']
colores = {
    'sort': '#3A86FF',
    'quickSort': '#48CAE4',
    'mergeSort': '#B5838D',
    'patienceSort': '#E56B96'
}
markers = {
    'sort': 'o',
    'quickSort': 's',
    'mergeSort': '^',
    'patienceSort': 'D'
}

# 1. Gráficos de Tiempo por Tipo de Arreglo (aleatorio, ascendente, descendente)
for tipo in df['tipo'].unique():
    subset = df[df['tipo'] == tipo]
    grouped = subset.groupby(['algoritmo', 'n'])['tiempo_us'].mean().reset_index()

    fig, ax = plt.subplots(figsize=(7.5, 4.8), dpi=300)
    for algo in algos:
        data = grouped[grouped['algoritmo'] == algo].sort_values('n')
        if not data.empty:
            ax.plot(
                data['n'], 
                data['tiempo_us'] / 1000.0, 
                marker=markers[algo], 
                color=colores[algo], 
                label=algo,
                linewidth=2.2, 
                markersize=6.5,
                markeredgewidth=1.2,
                markeredgecolor='white'
            )

    ax.set_title(f"Tiempo de Ejecución vs. Tamaño n ({tipo.capitalize()})", fontsize=12, pad=12, fontweight='bold', color='#2B2D42')
    ax.set_xlabel("Tamaño del Arreglo (n)", fontsize=10, labelpad=8, color='#333333')
    ax.set_ylabel("Tiempo promedio (ms)", fontsize=10, labelpad=8, color='#333333')
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.grid(True, which="major", ls="--", color='#E0E0E0', alpha=0.8)
    ax.grid(True, which="minor", ls=":", color='#F0F0F0', alpha=0.5)
    
    legend = ax.legend(frameon=True, facecolor='white', edgecolor='#E0E0E0', fontsize=9)
    legend.get_frame().set_alpha(0.9)
    
    plt.tight_layout()
    plt.savefig(f"../data/plots/tiempo_sorting_{tipo}.png")
    plt.close()

# 2. Gráficos de Tiempo por Dominio (D1 vs D7) en caso aleatorio
for dom in df['dominio'].unique():
    subset = df[(df['tipo'] == 'aleatorio') & (df['dominio'] == dom)]
    grouped = subset.groupby(['algoritmo', 'n'])['tiempo_us'].mean().reset_index()

    fig, ax = plt.subplots(figsize=(7.5, 4.8), dpi=300)
    for algo in algos:
        data = grouped[grouped['algoritmo'] == algo].sort_values('n')
        if not data.empty:
            ax.plot(
                data['n'], 
                data['tiempo_us'] / 1000.0, 
                marker=markers[algo], 
                color=colores[algo], 
                label=algo,
                linewidth=2.2, 
                markersize=6.5,
                markeredgewidth=1.2,
                markeredgecolor='white'
            )

    ax.set_title(f"Tiempo vs. Tamaño n (Aleatorio - Dominio {dom})", fontsize=12, pad=12, fontweight='bold', color='#2B2D42')
    ax.set_xlabel("Tamaño del Arreglo (n)", fontsize=10, labelpad=8, color='#333333')
    ax.set_ylabel("Tiempo promedio (ms)", fontsize=10, labelpad=8, color='#333333')
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.grid(True, which="major", ls="--", color='#E0E0E0', alpha=0.8)
    ax.grid(True, which="minor", ls=":", color='#F0F0F0', alpha=0.5)
    
    legend = ax.legend(frameon=True, facecolor='white', edgecolor='#E0E0E0', fontsize=9)
    legend.get_frame().set_alpha(0.9)
    
    plt.tight_layout()
    plt.savefig(f"../data/plots/tiempo_sorting_dominio_{dom}.png")
    plt.close()

# 3. Gráfico de Consumo de Memoria
mem_grouped = df.groupby(['algoritmo', 'n'])['memoria_kb'].mean().reset_index()
fig, ax = plt.subplots(figsize=(7.5, 4.8), dpi=300)

for algo in algos:
    data = mem_grouped[mem_grouped['algoritmo'] == algo].sort_values('n')
    if not data.empty:
        ax.plot(
            data['n'], 
            data['memoria_kb'] / 1024.0, 
            marker=markers[algo], 
            color=colores[algo], 
            label=algo,
            linewidth=2.2, 
            markersize=6.5,
            markeredgewidth=1.2,
            markeredgecolor='white'
        )

ax.set_title("Pico de Memoria Máxima vs. Tamaño n (Sorting)", fontsize=12, pad=12, fontweight='bold', color='#2B2D42')
ax.set_xlabel("Tamaño del Arreglo (n)", fontsize=10, labelpad=8, color='#333333')
ax.set_ylabel("Memoria máxima residente (MB)", fontsize=10, labelpad=8, color='#333333')
ax.set_xscale('log')
ax.grid(True, which="major", ls="--", color='#E0E0E0', alpha=0.8)
ax.grid(True, which="minor", ls=":", color='#F0F0F0', alpha=0.5)

legend = ax.legend(frameon=True, facecolor='white', edgecolor='#E0E0E0', fontsize=9)
legend.get_frame().set_alpha(0.9)

plt.tight_layout()
plt.savefig("../data/plots/memoria_sorting.png")
plt.close()

print("¡Todos los gráficos de sorting fueron generados con éxito en data/plots/!")