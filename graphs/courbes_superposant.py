import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

fichier = "resultats_benchmark.csv"

if not os.path.exists(fichier):
    print(f"Erreur : {fichier} introuvable.")
    exit(1)

df = pd.read_csv(fichier, sep=";")

os.makedirs("graphs", exist_ok=True)

# Une seule comparaison, sur l'operation tri_rapide (la plus illustrative,
# theorie en n log n), toutes structures superposees.
OPERATION_CIBLE = "tri_rapide"
LABEL_THEORIQUE = "O(n log n)"

def modele_theorique(n, a):
    return a * n * np.log2(n)

structures = df["structure"].unique()

couleurs = {
    "TAB_STAT" : "#378ADD",
    "TAB_DYN"  : "#1D9E75",
    "LISTE"    : "#D85A30",
}
styles = {
    "TAB_STAT" : "o",
    "TAB_DYN"  : "s",
    "LISTE"    : "^",
}
noms = {
    "TAB_STAT" : "Tableau statique",
    "TAB_DYN"  : "Tableau dynamique",
    "LISTE"    : "Liste chainee",
}

fig, ax = plt.subplots(figsize=(9, 5))

for struct in structures:
    sous_df = df[
        (df["operation"] == OPERATION_CIBLE) &
        (df["structure"] == struct)
    ].sort_values("n")

    if sous_df.empty:
        continue

    ns   = sous_df["n"].values.astype(float)
    moy  = sous_df["temps_moy(s)"].values
    ecar = sous_df["ecart_type(s)"].values

    couleur  = couleurs.get(struct, "gray")
    marqueur = styles.get(struct, "o")
    nom      = noms.get(struct, struct)

    # Courbe empirique (mesuree) - sans barres d'erreur
    ax.plot(
        ns, moy,
        marker=marqueur,
        linestyle="-",
        color=couleur,
        label=f"{nom} (mesure)",
        linewidth=1.8,
        markersize=9,
        markeredgecolor="black",
        markeredgewidth=0.6,
    )

    # Courbe theorique calee sur le dernier point mesure
    try:
        valeur_theo_1 = modele_theorique(ns[-1:], 1.0)[0]
        a = moy[-1] / valeur_theo_1 if valeur_theo_1 > 0 else 1.0
    except Exception:
        a = 1.0

    ns_dense  = np.linspace(ns[0], ns[-1], 200)
    theo_vals = modele_theorique(ns_dense, a)

    ax.plot(
        ns_dense, theo_vals,
        linestyle="--",
        color=couleur,
        alpha=0.5,
        linewidth=1.2,
        label=f"{nom} ({LABEL_THEORIQUE} theorique)"
    )

ax.set_title(
    f"Empirique vs theorique — operation : {OPERATION_CIBLE}",
    fontsize=12, fontweight="bold", pad=12
)
ax.set_xlabel("Taille n", fontsize=11)
ax.set_ylabel("Temps moyen (ms)", fontsize=11)
ax.legend(fontsize=9, loc="upper left")
ax.grid(True, linestyle="--", alpha=0.5)

plt.tight_layout()
nom_fichier = f"graphs/comparaison_{OPERATION_CIBLE}.png"
plt.savefig(nom_fichier, dpi=150, bbox_inches="tight")
plt.close()
print(f"Courbe sauvegardee : {nom_fichier}")