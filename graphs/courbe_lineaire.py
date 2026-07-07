import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os

fichier = "resultats_benchmark.csv"

if not os.path.exists(fichier):
    print(f"Erreur : {fichier} introuvable.")
    print("Lance d'abord le benchmark : ./bench")
    exit(1)

df = pd.read_csv(fichier, sep=";")

structures  = df["structure"].unique()
operations  = df["operation"].unique()
tailles     = sorted(df["n"].unique())

couleurs = {
    "TAB_STAT" : "#378ADD",   # bleu
    "TAB_DYN"  : "#1D9E75",   # vert
    "LISTE"    : "#D85A30",   # orange
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
        # Filtrer : opération + structure + distribution uniforme
        sous_df = df[
            (df["operation"]    == op)     &
            (df["structure"]    == struct) &
            (df["distribution"] == "Uniforme")
        ].sort_values("n")

        if sous_df.empty:
            continue

        moy  = sous_df["temps_moy(s)"]   # convertir en ms
        ecar = sous_df["ecart_type(s)"] 
        ns   = sous_df["n"]

        couleur = couleurs.get(struct, "gray")
        style   = styles.get(struct, "-o")
        nom     = noms.get(struct, struct)

        ax.errorbar(
            ns, moy,
            yerr=ecar,
            fmt=style,
            color=couleur,
            label=nom,
            linewidth=1.8,
            markersize=6,
            capsize=4,
            capthick=1.2,
            elinewidth=1
        )

    ax.set_title(
        f"Temps d'exécution — opération : {op}\n(distribution uniforme, échelle linéaire)",
        fontsize=12, fontweight="bold", pad=12
    )
    ax.set_xlabel("Taille n (nombre d'enregistrements)", fontsize=11)
    ax.set_ylabel("Temps moyen (ms)", fontsize=11)
    ax.legend(fontsize=10, loc="upper left")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.xaxis.set_major_formatter(ticker.FuncFormatter(
        lambda x, _: f"{int(x):,}".replace(",", " ")
    ))

    plt.tight_layout()
    nom_fichier = f"graphs/lineaire_{op}.png"
    plt.savefig(nom_fichier, dpi=150, bbox_inches="tight")
    plt.close()
    print(f"Courbe sauvegardée : {nom_fichier}")

print("Toutes les courbes linéaires générées.")