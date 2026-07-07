import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

fichier = "resultats_benchmark.csv"

if not os.path.exists(fichier):
    print(f"Erreur : {fichier} introuvable.")
    exit(1)

df = pd.read_csv(fichier, sep=";")

# On fixe n et la distribution pour la comparaison
N_FIXE    = 10000
DIST_FIXE = "Uniforme"

structures = df["structure"].unique()
operations = df["operation"].unique()

noms = {
    "TAB_STAT" : "Tab. statique",
    "TAB_DYN"  : "Tab. dynamique",
    "LISTE"    : "Liste chaînée",
}

os.makedirs("graphs", exist_ok=True)

ops_valides   = []
facteurs      = []
meilleure_lbl = []
pire_lbl      = []

for op in operations:
    sous_df = df[
        (df["operation"]    == op) &
        (df["n"]            == N_FIXE) &
        (df["distribution"] == DIST_FIXE)
    ]

    if sous_df.empty:
        continue

    # On ignore les temps nuls (mesure trop rapide pour la résolution de l'horloge)
    sous_df = sous_df[sous_df["temps_moy(s)"] > 0]

    if sous_df.empty or len(sous_df) < 2:
        continue

    ligne_min = sous_df.loc[sous_df["temps_moy(s)"].idxmin()]
    ligne_max = sous_df.loc[sous_df["temps_moy(s)"].idxmax()]

    facteur = ligne_max["temps_moy(s)"] / ligne_min["temps_moy(s)"]

    ops_valides.append(op)
    facteurs.append(facteur)
    meilleure_lbl.append(noms.get(ligne_min["structure"], ligne_min["structure"]))
    pire_lbl.append(noms.get(ligne_max["structure"], ligne_max["structure"]))

if not ops_valides:
    print("Aucune donnée exploitable pour l'histogramme de ralentissement.")
    exit(0)

fig, ax = plt.subplots(figsize=(9, 5))

x = np.arange(len(ops_valides))
barres = ax.bar(x, facteurs, color="#D85A30", alpha=0.85)

# Annotation : structure la plus rapide vs la plus lente, au-dessus de chaque barre
for i, (rect, meilleur, pire) in enumerate(zip(barres, meilleure_lbl, pire_lbl)):
    hauteur = rect.get_height()
    ax.annotate(
        f"{meilleur}\nvs\n{pire}",
        xy=(rect.get_x() + rect.get_width() / 2, hauteur),
        xytext=(0, 5),
        textcoords="offset points",
        ha="center", va="bottom",
        fontsize=8
    )

ax.set_title(
    f"Facteur de ralentissement (pire / meilleure structure)\n"
    f"n = {N_FIXE:,} | distribution = {DIST_FIXE}".replace(",", " "),
    fontsize=12, fontweight="bold", pad=12
)
ax.set_xlabel("Opération", fontsize=11)
ax.set_ylabel("Facteur de ralentissement (×)", fontsize=11)
ax.set_xticks(x)
ax.set_xticklabels(ops_valides, fontsize=10, rotation=20)
ax.grid(True, axis="y", linestyle="--", alpha=0.5)

plt.tight_layout()
nom_fichier = "graphs/histogramme_ralentissement.png"
plt.savefig(nom_fichier, dpi=150, bbox_inches="tight")
plt.close()
print(f"Histogramme sauvegardé : {nom_fichier}")