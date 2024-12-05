import matplotlib.pyplot as plt

# Datos
n_points = [10, 100, 1000]
parallel_time_ms = [376, 1567, 5560]
sequential_time_ms = [114, 908, 10682]

# Crear el gráfico
plt.figure(figsize=(10, 6))

# Agregar las dos líneas
plt.plot(n_points, parallel_time_ms, marker='o', linestyle='-', label='Parallel Time (ms)')
plt.plot(n_points, sequential_time_ms, marker='s', linestyle='--', label='Sequential Time (ms)')

# Configurar la escala logarítmica para el eje X
plt.xscale('log')

# Agregar etiquetas y título
plt.xlabel('N Points (log scale)')
plt.ylabel('Time (ms)')
plt.title('Parallel vs Sequential Time (Logarithmic Scale)')
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend()

# Mostrar el gráfico
plt.savefig("comparison_few_points.jpg")