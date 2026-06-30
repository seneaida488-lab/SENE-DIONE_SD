import subprocess
import sys
import os

scripts = [
    "courbe_lineaire.py",
    "courbe_log.py",
    "courbe_distribution.py",
    "courbe_comparaison.py",
]

# Se placer dans le dossier graphs
os.chdir(os.path.dirname(os.path.abspath(__file__)))

print("============================================")
print("   GENERATION DE TOUTES LES COURBES")
print("   Projet SD")
print("============================================\n")

for script in scripts:
    print(f"--- Lancement de {script} ---")
    result = subprocess.run([sys.executable, script])
    if result.returncode != 0:
        print(f"ERREUR dans {script}")
        sys.exit(1)
    print()

print("============================================")
print("   TOUTES LES COURBES ONT ETE GENEREES")
print("   Fichiers dans le dossier graphs/")
print("============================================")