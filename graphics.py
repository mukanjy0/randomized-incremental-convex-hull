import matplotlib.pyplot as plt

# Datos
n_points = [10, 100, 1000, 10000, 100000, 1000000]
time_ms = [114, 908, 10682, 94758, 418419, 3826403]

# Crear el gráfico
plt.figure(figsize=(10, 6))
plt.plot(n_points, time_ms, marker='o', linestyle='-', label='Time vs N Points')

# Configurar la escala logarítmica para el eje X
plt.xscale('log')

# Agregar etiquetas y título
plt.xlabel('N Points (log scale)')
plt.ylabel('Time (ms)')
plt.title('Time vs N Points (Logarithmic Scale) - Sequential')
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.legend()

plt.savefig("Sequential_times.jpg")

# Mostrar el gráfico
#plt.show()
