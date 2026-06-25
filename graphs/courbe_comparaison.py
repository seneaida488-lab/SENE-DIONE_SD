import matplotlib.pyplot as plt
import numpy as np

n = np.array([100, 1000, 10000, 100000])
temps_reel = np.array([0.0001, 0.0015, 0.0210, 0.2800]) # Vos temps réels

# Calcul de la courbe théorique ajustée (n * log(n))
theorique = n * np.log2(n)
# Ajustement d'échelle pour la superposition
theorique_ajuste = theorique * (temps_reel[-1] / theorique[-1])

plt.figure(figsize=(7, 5))
plt.plot(n, temps_reel, 'ro', label="Mesures réelles (Empirique)")
plt.plot(n, theorique_ajuste, 'b-', label="Courbe théorique attendue O(n log n)")
plt.title("Comparaison Théorie vs Pratique")
plt.xlabel("Nombre d'étudiants (n)")
plt.ylabel("Temps (secondes)")
plt.legend()
plt.grid(True)

plt.show()