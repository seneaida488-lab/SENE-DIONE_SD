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

complexites = {
    ("insertion", "TAB_STAT")     : ("O(n)",       lambda n, a: a * n),
    ("insertion", "TAB_DYN")      : ("O(1) amorti", lambda n, a: np.full_like(n, a, dtype=float)),
    ("insertion", "LISTE")        : ("O(1)",        lambda n, a: np.full_like(n, a, dtype=float)),
    ("recherche", "TAB_STAT")     : ("O(n)", lambda n, a: a * n),
    ("recherche", "TAB_DYN")      : ("O(n)", lambda n, a: a * n),
    ("recherche", "LISTE")        : ("O(n)", lambda n, a: a * n),
    ("suppression", "TAB_STAT")   : ("O(n)", lambda n, a: a * n),
    ("suppression", "TAB_DYN")    : ("O(n)", lambda n, a: a * n),
    ("suppression", "LISTE")      : ("O(n)", lambda n, a: a * n),
    ("tri_insertion", "TAB_STAT") : ("O(n²)", lambda n, a: a * n**2),
    ("tri_insertion", "TAB_DYN")  : ("O(n²)", lambda n, a: a * n**2),
    ("tri_insertion", "LISTE")    : ("O(n²)", lambda n, a: a * n**2),
    ("tri_rapide", "TAB_STAT")    : ("O(n log n)", lambda n, a: a * n * np.log2(n)),
    ("tri_rapide", "TAB_DYN")     : ("O(n log n)", lambda n, a: a * n * np.log2(n)),
}

structures = df["structure"].unique()
operations = df["operation"].unique()

couleurs_struct = {
    "TAB_STAT" : "#378ADD",
    "TAB_DYN"  : "#1D9E75",
    "LISTE"    : "#D85A30",
}
noms = {
    "TAB_STAT" : "Tab. statique",
    "TAB_DYN"  : "Tab. dynamique",
    "LISTE"    : "Liste chaînée",
}

for op in operations:
    if op not in complexites:
        continue

    label_theo, f_theo = complexites[op]

    fig, ax = plt.subplots(figsize=(9, 5))

    for struct in structures:
        sous_df = df[
            (df["operation"]    == op)     &
            (df["structure"]    == struct) &
            (df["distribution"] == "Uniforme")
        ].sort_values("n")

        if sous_df.empty:
            continue

        ns   = sous_df["n"].values.astype(float)
        moy  = sous_df["temps_moy(s)"]
        ecar = sous_df["ecart_type(s)"]

        couleur = couleurs_struct.get(struct, "gray")
        nom     = noms.get(struct, struct)

        # Courbe empirique
        ax.errorbar(
            ns, moy,
            yerr=ecar,
            fmt="-o",
            color=couleur,
            label=f"{nom} (mesuré)",
            linewidth=1.8,
            markersize=6,
            capsize=4
        )
        
        try:
            valeur_theo_1 = f_theo(ns[-1:], 1.0)[0]
            if valeur_theo_1 > 0:
                a = moy[-1] / valeur_theo_1
            else:
                a = 1.0
        except Exception:
            a = 1.0

        ns_dense  = np.linspace(ns[0], ns[-1], 200)
        theo_vals = f_theo(ns_dense, a)

        ax.plot(
            ns_dense, theo_vals,
            linestyle="--",
            color=couleur,
            alpha=0.5,
            linewidth=1.2,
            label=f"{nom} ({label_theo} théorique)"
        )

    ax.set_title(
        f"Empirique vs théorique — opération : {op}\n(distribution uniforme)",
        fontsize=12, fontweight="bold", pad=12
    )
    ax.set_xlabel("Taille n", fontsize=11)
    ax.set_ylabel("Temps moyen (ms)", fontsize=11)
    ax.legend(fontsize=9, loc="upper left")
    ax.grid(True, linestyle="--", alpha=0.5)

    plt.tight_layout()
    nom_fichier = f"graphs/comparaison_{op}.png"
    plt.savefig(nom_fichier, dpi=150, bbox_inches="tight")
    plt.close()
    print(f"Courbe sauvegardée : {nom_fichier}")

print("Toutes les courbes de comparaison générées.")