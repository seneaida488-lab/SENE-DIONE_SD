# Projet LMI ASD 2026 — Système de Gestion de Scolarité

**Filière :** Licence 2 Mathématiques–Informatique (LMI 2)  
**Université :** Iba Der Thiam de Thiès  
**Enseignant :** Dr Abdoulaye DIALLO  
**Année universitaire :** 2025–2026  
**Binôme :** AIDA — COURA  

---

## Description

Ce projet implémente et compare trois structures de données pour
un système de gestion de scolarité (étudiants, notes, inscriptions) :

- **Tableau statique** — allocation fixe à la compilation
- **Tableau dynamique de pointeurs** — redimensionnement géométrique (×2)
- **Liste doublement chaînée** — avec sentinelles tête et queue

Chaque structure supporte : insertion, recherche, suppression,
mise à jour, tri, agrégations statistiques et persistance binaire.

---

## Organisation du dépôt

```
SENE-DIONE_SD/
├── include/                  # Fichiers d'en-tête (.h)
│   ├── structure.h           # Types : Date, Cours, Note, Inscription, Etudiant
│   ├── tableau_statique.h    # Tableau statique
│   ├── tableau_dynamique.h   # Tableau dynamique
│   └── liste_chainee.h       # Liste doublement chaînée
│
├── src/                      # Code source (.c)
│   ├── structure.c           # Fonctions de base des structures
│   ├── tableau_statique.c    # Implémentation tableau statique
│   ├── tableau_dynamique.c   # Implémentation tableau dynamique
│   └── liste_chainee.c       # Implémentation liste chaînée
│
├── benchmark/                # Programmes principaux
│   ├── main_structure.c      # Menu interactif général
│   ├── main_statique.c       # Test tableau statique
│   ├── main_dynamique.c      # Test tableau dynamique
│   ├── main_liste.c          # Test liste doublement chaînée 
│   ├──main.principal.c      # Test tout les mains.c  
│   ├── generateur_donnee.h   # Générateur de jeux de données
│   ├── generateur_donnee.c   # Distributions : uniforme, gaussienne, triée
│   └── main-bench.c                # Benchmark complet (mesures de temps)
│
├── graphs/                   # Scripts Python pour les courbes
│   ├── main.py               # Lance tous les scripts de courbes
│   ├── courbe_lineaire.py    # Courbes temps vs n (échelle linéaire)
│   ├── courbe_log.py         # Courbes temps vs n (échelle logarithmique)
│   ├── courbe_distribution.py# Courbes par distribution des données
│   └── courbe_comparaison.py # Empirique vs théorique
│
├── data/                     # Jeux de données générés (.bin)
├── gitignore
├── rapport/
    ├── Latex # Rapport LaTeX
    ├── Manuel # Rapport Manuel
    ├── PDF      
├── Makefile                  # Compilation du projet
└── README.md                 # Ce fichier
```

---

## Compilation

### Prérequis

- Compilateur `gcc` avec support C99
- `make`
- Python 3 avec `pandas`, `matplotlib`, `numpy` (pour les courbes)

### Compiler tout le projet

```bash
make all
```

### Compiler une cible spécifique
main_principal: gcc benchmark/main_principal.c benchmark/generateur_donnee.c src/structure.c src/tableau_statique.c src/tableau_dynamique.c src/liste_chainee.c -o benchmark/main_principal.exe
main_bench: gcc -Wall -Wextra -o bench src/structure.c src/tableau_statique.c src/tableau_dynamique.c src/liste_chainee.c benchmark/generateur_donnee.c benchmark/main_bench.c -I include -I benchmark -lm ; if ($?) { .\bench.exe }
### Avec Makefile
```bash
make structure     # menu interactif
make bench          # benchmark
make main_statique  # test tableau statique
make main_dynamique # test tableau dynamique
make main_chainee     # test liste chaînée
make main_principal  # Test tout les mains.c
```

### Nettoyer les fichiers générés

```bash
make clean
```

---

## Exécution
main_principal: benchmark/main_principal.exe
main_bench: .\bench.exe
### Menu interactif (système de scolarité)

```bash
./structure
```

Lance un menu complet permettant d'ajouter, rechercher,
modifier, supprimer des étudiants et de sauvegarder les données.

### Tester chaque structure séparément

```bash
./main_statique    # test tableau statique
./main_dynamique   # test tableau dynamique
./main_chainee       # test liste doublement 
./main_structure   # menu interactif
```

### Lancer le benchmark

```bash
./bench.exe
```

Génère automatiquement `resultats_benchmark.csv` avec les temps
d'exécution pour toutes les opérations, tailles et distributions.

### Générer les courbes

```bash
make courbes
```

Ou manuellement :

```bash
./bench
cd graphs
python main.py
```

Les courbes PNG sont sauvegardées dans `graphs/`.

---

## Structures de données implémentées

### 1. Tableau statique (`tableau_statique`)

- Capacité fixe : `TAILLEMAX = 100000`
- Accès direct par indice en O(1)
- Insertion triée par matricule

### 2. Tableau dynamique (`tableau_dynamique`)

- Capacité initiale : 4 places
- Redimensionnement géométrique ×2 via `realloc`
- Facteur de croissance documenté : 4 → 8 → 16 → ...

### 3. Liste doublement chaînée (`liste_chainee`)

- Sentinelles tête et queue
- Insertion en tête et en queue en O(1)
- Parcours avant et arrière grâce aux pointeurs `prec` et `suiv`

---

## Opérations supportées

| Opération | Tableau statique | Tableau dynamique | Liste chaînée |
|---|---|---|---|
| Insertion | ✅ | ✅ | ✅ tête + queue |
| Recherche clé | ✅ | ✅ | ✅ |
| Recherche intervalle | ✅ | ✅ | ✅ |
| Recherche préfixe | ✅ | ✅ | ✅ |
| Suppression | ✅ | ✅ | ✅ |
| Mise à jour | ✅ | ✅ | ✅ |
| Tri insertion | ✅ | ✅ | ✅ |
| Tri rapide / bulles | ✅ | ✅ | ✅ |
| Agrégations | ✅ | ✅ | ✅ |
| Persistance binaire | ✅ | ✅ | ✅ |

---

## Benchmark

Le benchmark mesure les temps d'exécution selon :

- **Tailles :** n ∈ {100, 1000, 10 000, 100 000}
- **Distributions :** Uniforme, Gaussienne, Triée
- **Répétitions :** 10 fois par mesure
- **Résultats :** moyenne + écart-type en secondes

---

## Références bibliographiques

1. Cormen T. H. et al., *Introduction to Algorithms*, 4e éd., MIT Press, 2022
2. Sedgewick R., Wayne K., *Algorithms*, 4e éd., Addison-Wesley, 2011
3. Kernighan B. W., Ritchie D. M., *The C Programming Language*, 2e éd.
GIP LIEN :
https://github.com/seneaida488-lab/SENE-DIONE_SD
