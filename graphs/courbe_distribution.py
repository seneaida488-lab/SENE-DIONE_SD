ssimport pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

fichier = "resultats_benchmark.csv"

if not os.path.exists(fichier):
    print(f"Erreur : {fichier} introuvable.")
    exit(1)

df = pd.read_csv(fichier, sep=";")

# On fixe n = 10000
N_FIXE = 10000

structures   = df["structure"].unique()
operations   = df["operation"].unique()
distributions = df["distribution"].unique()

couleurs = {
    "TAB_STAT" : "#378ADD",
    "TAB_DYN"  : "#1D9E75",
    "LISTE"    : "#D85A30",
}
noms = {
    "TAB_STAT" : "Tab. statique",
    "TAB_DYN"  : "Tab. dynamique",
    "LISTE"    : "Liste chaînée",
}

os.makedirs("graphs", exist_ok=True)

for op in operations:
    fig, ax = plt.subplots(figsize=(9, 5))

    x      = np.arange(len(distributions))
    width  = 0.25
    offset = -(len(structures) - 1) * width / 2

    for idx, struct in enumerate(structures):
        moyennes  = []
        erreurs   = []

        for dist in distributions:
            sous_df = df[
                (df["operation"]    == op)     &
                (df["structure"]    == struct) &
                (df["distribution"] == dist)   &
                (df["n"]            == N_FIXE)
            ]
            if sous_df.empty:
                moyennes.append(0)
                erreurs.append(0)
            else:
                moyennes.append(sous_df["temps_moy(s)"].values[0] )
                erreurs.append(sous_df["ecart_type(s)"].values[0] )

        ax.bar(
            x + offset + idx * width,
            moyennes,
            width,
            yerr=erreurs,
            label=noms.get(struct, struct),
            color=couleurs.get(struct, "gray"),
            alpha=0.85,
            capsize=4,
            error_kw={"elinewidth": 1.2}
        )

    ax.set_title(
        f"Temps d'exécution par distribution — opération : {op}\n(n = {N_FIXE:,} enregistrements)".replace(",", " "),
        fontsize=12, fontweight="bold", pad=12
    )
    ax.set_xlabel("Distribution des données", fontsize=11)
    ax.set_ylabel("Temps moyen (ms)", fontsize=11)
    ax.set_xticks(x)
    ax.set_xticklabels(distributions, fontsize=10)
    ax.legend(fontsize=10)
    ax.grid(True, axis="y", linestyle="--", alpha=0.5)

    plt.tight_layout()
    nom_fichier = f"graphs/distribution_{op}.png"
    plt.savefig(nom_fichier, dpi=150, bbox_inches="tight")
    plt.close()
    print(f"Courbe sauvegardée : {nom_fichier}")

print("Toutes les courbes par distribution générées.")