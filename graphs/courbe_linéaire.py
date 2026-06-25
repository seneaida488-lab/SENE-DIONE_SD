import matplotlib.pyplot as plt
import numpy as np

# Remplacer par vos vrais temps en secondes (Profil Uniforme par exemple)
n = np.array([100, 1000, 10000, 100000])
temps = np.array([0.0001, 0.0015, 0.0210, 0.2800]) 

plt.figure(figsize=(12, 5))

# 1. Échelle Linéaire
plt.subplot(1, 2, 1)
plt.plot(n, temps, marker='o', color='blue', linestyle='-')
plt.title("Échelle Linéaire : Temps = f(n)")
plt.xlabel("Nombre d'étudiants (n)")
plt.ylabel("Temps (secondes)")
plt.grid(True)

# 2. Échelle Logarithmique
plt.subplot(1, 2, 2)
plt.loglog(n, temps, marker='s', color='red', linestyle='--')
plt.title("Échelle Logarithmique : Temps = f(n)")
plt.xlabel("Nombre d'étudiants (n)")
plt.ylabel("Temps (secondes)")
plt.grid(True, which="both", ls="-")

plt.tight_layout()
plt.show()