import matplotlib.pyplot as plt
import numpy as np

# Exemple pour les 4 tailles : diviser le pire temps par le meilleur temps
tailles_labels = ['N=100', 'N=1000', 'N=10000', 'N=100000']
facteur_ralentissement = [1.2, 2.5, 5.8, 13.7] # (Temps Pire / Temps Meilleur)

plt.figure(figsize=(7, 5))
plt.bar(tailles_labels, facteur_ralentissement, color='purple', width=0.4)
plt.title("Facteur de ralentissement (Pire cas / Meilleur cas)")
plt.ylabel("Facteur multiplicateur de ralentissement")
plt.axhline(y=1, color='r', linestyle='--', label="Aucun ralentissement")
plt.grid(axis='y', linestyle='--')

plt.show()