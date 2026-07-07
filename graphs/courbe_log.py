import pandas as pd
import matplotlib.pyplot as plt
import os

fichier = "resultats_benchmark.csv"

if not os.path.exists(fichier):
    print(f"Erreur : {fichier} introuvable.")
    exit(1)

df = pd.read_csv(fichier, sep=";")

structures = df["structure"].unique()
operations = df["operation"].unique()

couleurs = {
    "TAB_STAT" : "#378ADD",
    "TAB_DYN"  : "#1D9E75",
    "LISTE"    : "#D85A30",
}
styles = {
    "TAB_STAT" : "-o",
    "TAB_DYN"  : "-s",
    "LISTE"    : "-^",
}
noms = {
    "TAB_STAT" : "Tableau statique",
    "TAB_DYN"  : "Tableau dynamique",
    "LISTE"    : "Liste doublement chaînée",
}

os.makedirs("graphs", exist_ok=True)

for op in operations:
    fig, ax = plt.subplots(figsize=(9, 5))

    for struct in structures:
        sous_df = df[
            (df["operation"]    == op)     &
            (df["structure"]    == struct) &
            (df["distribution"] == "Uniforme")
        ].sort_values("n")

        if sous_df.empty:
            continue

        moy  = sous_df["temps_moy(s)"] 
        ecar = sous_df["ecart_type(s)"] 
        ns   = sous_df["n"]

        ax.errorbar(
            ns, moy,
            yerr=ecar,
            fmt=styles.get(struct, "-o"),
            color=couleurs.get(struct, "gray"),
            label=noms.get(struct, struct),
            linewidth=1.8,
            markersize=6,
            capsize=4,
            capthick=1.2,
            elinewidth=1
        )

    # Échelle log-log
    ax.set_xscale("log")
    ax.set_yscale("log")

    ax.set_title(
        f"Temps d'exécution — opération : {op}\n(distribution uniforme, échelle logarithmique)",
        fontsize=12, fontweight="bold", pad=12
    )
    ax.set_xlabel("Taille n (log)", fontsize=11)
    ax.set_ylabel("Temps moyen (ms, log)", fontsize=11)
    ax.legend(fontsize=10, loc="upper left")
    ax.grid(True, which="both", linestyle="--", alpha=0.5)

    plt.tight_layout()
    nom_fichier = f"graphs/log_{op}.png"
    plt.savefig(nom_fichier, dpi=150, bbox_inches="tight")
    plt.close()
    print(f"Courbe sauvegardée : {nom_fichier}")

print("Toutes les courbes logarithmiques générées.")