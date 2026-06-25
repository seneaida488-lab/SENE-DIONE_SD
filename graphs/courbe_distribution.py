import matplotlib.pyplot as plt

# Données d'exemple pour N = 100 000 à remplacer par vos mesures
distributions = ['Uniforme', 'Déjà trié', 'Inversement trié']
temps_n100k = [0.280, 0.045, 0.620] 

plt.figure(figsize=(7, 5))
plt.bar(distributions, temps_n100k, color=['gray', 'green', 'orange'], width=0.5)
plt.title("Temps d'exécution selon la distribution (pour N = 100 000)")
plt.ylabel("Temps (secondes)")
plt.grid(axis='y', linestyle='--')

plt.show()