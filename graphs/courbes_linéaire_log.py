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

structures = df["structure"].unique()
operations = df["operation"].unique()

couleurs = {
    "TAB_STAT" : "#378ADD",   # bleu
    "TAB_DYN"  : "#1D9E75",   # vert
    "LISTE"    : "#D85A30",   # orange
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

os.makedirs("graphs", exist_ok=True)

for op in operations:
    # Une figure avec 2 sous-graphiques cote a cote : lineaire (gauche), log (droite)
    fig, (ax_lin, ax_log) = plt.subplots(1, 2, figsize=(14, 5))

    for struct in structures:
        sous_df = df[
            (df["operation"] == op) &
            (df["structure"] == struct)
        ].sort_values("n")

        if sous_df.empty:
            continue

        moy  = sous_df["temps_moy(s)"]
        ns   = sous_df["n"]

        couleur  = couleurs.get(struct, "gray")
        marqueur = styles.get(struct, "o")
        nom      = noms.get(struct, struct)

        # --- Graphique lineaire (gauche) ---
        ax_lin.plot(
            ns, moy,
            marker=marqueur,
            linestyle="-",
            color=couleur,
            label=nom,
            linewidth=1.8,
            markersize=9,
            markeredgecolor="black",
            markeredgewidth=0.6,
        )

        # --- Graphique logarithmique (droite) ---
        ax_log.plot(
            ns, moy,
            marker=marqueur,
            linestyle="-",
            color=couleur,
            label=nom,
            linewidth=1.8,
            markersize=9,
            markeredgecolor="black",
            markeredgewidth=0.6,
        )

    # Mise en forme du graphique lineaire
    ax_lin.set_title("Echelle lineaire", fontsize=12, fontweight="bold")
    ax_lin.set_xlabel("Taille n (nombre d'enregistrements)", fontsize=10)
    ax_lin.set_ylabel("Temps moyen (ms)", fontsize=10)
    ax_lin.legend(fontsize=9, loc="upper left")
    ax_lin.grid(True, linestyle="--", alpha=0.5)
    ax_lin.xaxis.set_major_formatter(ticker.FuncFormatter(
        lambda x, _: f"{int(x):,}".replace(",", " ")
    ))

    # Mise en forme du graphique logarithmique
    ax_log.set_title("Echelle logarithmique", fontsize=12, fontweight="bold")
    ax_log.set_xlabel("Taille n (nombre d'enregistrements)", fontsize=10)
    ax_log.set_ylabel("Temps moyen (ms)", fontsize=10)
    ax_log.set_xscale("log")
    ax_log.set_yscale("log")
    ax_log.legend(fontsize=9, loc="upper left")
    ax_log.grid(True, which="both", linestyle="--", alpha=0.5)

    fig.suptitle(
        f"Temps d'execution — operation : {op} (toutes structures)",
        fontsize=13, fontweight="bold"
    )

    plt.tight_layout()
    nom_fichier = f"graphs/courbe_{op}.png"
    plt.savefig(nom_fichier, dpi=150, bbox_inches="tight")
    plt.close()
    print(f"Courbe sauvegardee : {nom_fichier}")

print("Toutes les courbes (lineaire + log) generees.")