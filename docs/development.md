# Développement

Ce document décrira comment préparer, construire, exécuter et déboguer PunkOS.
Il est volontairement incomplet tant que la chaîne d’outils n’a pas été choisie
et vérifiée sur notre environnement.

## Cycle de travail prévu

```text
comprendre un mécanisme
        ↓
définir un petit résultat observable
        ↓
concevoir l’interface et l’algorithme
        ↓
écrire le code
        ↓
compiler avec des avertissements stricts
        ↓
tester dans QEMU
        ↓
observer ou déboguer
        ↓
documenter ce qui a été appris
```

## Outils prévus

- Git pour l’historique local ;
- GitHub comme dépôt distant officiel ;
- un compilateur capable de produire du code freestanding pour x86_64 ;
- un assembleur compatible avec les fichiers retenus ;
- un éditeur de liens et des outils d’inspection binaire ;
- Make pour orchestrer les commandes initiales ;
- Limine pour charger le noyau ;
- QEMU pour l’émulation ;
- GDB pour le débogage.

## Environnement de référence

L’environnement initial vérifié le 13 octobre 2026 est le suivant :

| Composant | Version ou choix |
| --- | --- |
| Système hôte | Ubuntu 26.04.1 LTS, x86_64 |
| Éditeur | Visual Studio Code |
| Compilateur du noyau | Clang 21.1.8 |
| Éditeur de liens | LLD 21.1.8 |
| Assembleur | NASM 3.01 |
| Construction | GNU Make 4.4.1 |
| Émulation | QEMU 10.2.1 |
| Débogage | GDB 17.1 |
| Création d’images ISO | xorriso 1.5.6 |
| Outils pour images FAT | mtools 4.0.49 |

Clang a été testé avec la cible `x86_64-unknown-none-elf` et les options
freestanding initiales. Le résultat produit est bien un objet ELF 64 bits x86-64.

La chaîne Clang/LLD évite de lier accidentellement le noyau aux bibliothèques
d’Ubuntu sans imposer, dès le premier jalon, la construction complète d’un GCC
croisé. Construire et étudier une chaîne GCC `x86_64-elf` reste une possibilité
pour une phase ultérieure.

### Installation sur l’hôte de référence

```sh
sudo apt-get install qemu-system-x86 xorriso nasm clang lld
```

Git, Make, GDB et mtools étaient déjà installés. Limine ne sera pas installé
globalement : une version déterminée sera intégrée de manière reproductible au
processus de construction du projet.

## Règles de progression

- Un nouveau fichier de code doit avoir une responsabilité identifiable.
- Une fonction doit être écrite après définition de son entrée, de sa sortie et
  de ses erreurs possibles.
- Un comportement invisible doit recevoir un moyen d’observation : trace série,
  assertion, compteur, test ou inspection avec GDB.
- Les avertissements du compilateur ne sont pas ignorés silencieusement.
- Les nombres et adresses « magiques » doivent être expliqués.
- Une optimisation ne précède pas une implémentation comprise et mesurable.
- Les changements importants mettent à jour la documentation correspondante.

## Stratégie de débogage

Dans un noyau, un bug peut arrêter toute la machine virtuelle. Nous construirons
donc les moyens de diagnostic très tôt :

1. sortie série capturée par le terminal hôte ;
2. symboles de débogage dans les constructions de développement ;
3. connexion de GDB à QEMU ;
4. gestionnaires d’exceptions affichant le contexte disponible ;
5. fonction `panic` centralisant les erreurs irrécupérables ;
6. assertions réservées aux invariants du noyau.

## Reproductibilité

À terme, la construction devra distinguer au moins :

- les fichiers générés des fichiers sources ;
- une construction de développement d’une construction optimisée ;
- l’image amorçable du noyau brut ;
- le disque virtuel de test des données persistantes.

Les fichiers générés ne seront pas ajoutés au dépôt sauf justification explicite.

## Vérification initiale effectuée

Avant le premier fichier C, nous avons vérifié :

- le système d’exploitation hôte ;
- le compilateur et l’éditeur de liens retenus ;
- la production d’un objet freestanding x86-64 ;
- les versions de QEMU, GDB, NASM, xorriso et mtools.

Le protocole Limine est désormais épinglé comme sous-module Git. Il reste à
épingler les binaires du bootloader et à automatiser leur récupération dans le
processus de construction.

Pour récupérer le projet avec ses dépendances après un clonage :

```sh
git clone --recurse-submodules <URL_DU_DEPOT>
```

Pour initialiser les sous-modules dans un clone déjà existant :

```sh
git submodule update --init --recursive
```
