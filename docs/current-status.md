# État exact du projet

Ce document décrit uniquement ce qui existe et a été vérifié. Il évite de
confondre une intention de la feuille de route avec une fonctionnalité réelle.

Dernière mise à jour : 13 octobre 2026.

## Résultat atteint

PunkOS produit actuellement un fichier `build/kernel.elf` possédant les
caractéristiques vérifiées suivantes :

- format ELF 64 bits pour x86-64 ;
- type exécutable statiquement lié ;
- point d’entrée `kernel_main` à l’adresse virtuelle `0xffffffff80001000` ;
- aucun symbole externe non résolu ;
- segment de requêtes Limine en lecture/écriture ;
- segment de code en lecture/exécution ;
- requête de protocole Limine en révision de base 6 ;
- requête d’un framebuffer Limine ;
- code prévu pour remplir un framebuffer RGB 32 bits en magenta.

Le fichier ELF compile et se lie sans avertissement avec les options strictes
actuelles. Il n’a pas encore été chargé par Limine ni exécuté dans QEMU. Le rendu
magenta décrit donc un comportement implémenté mais encore non validé à
l’exécution.

## Code existant

### Point d’entrée

`src/kernel.c` définit `kernel_main`. Cette fonction :

1. vérifie que le bootloader accepte la révision de protocole demandée ;
2. vérifie qu’une réponse framebuffer et au moins un écran sont disponibles ;
3. sélectionne le premier framebuffer ;
4. accepte uniquement le modèle RGB avec des pixels de 32 bits ;
5. parcourt sa surface en respectant le pas mémoire `pitch` ;
6. écrit la valeur `0x00ff00ff` dans chaque pixel ;
7. appelle les quatre fonctions arithmétiques pédagogiques ;
8. entre dans une boucle infinie.

En cas d’échec d’une vérification, le noyau entre actuellement dans une boucle
infinie sans diagnostic. Cette solution est temporaire ; une sortie série et une
fonction de panique devront rendre ces échecs observables.

### Module arithmétique pédagogique

`src/math.c` implémente `add`, `sub`, `mult` et `divide`. Leur interface commune
se trouve dans `include/punkos/math.h`.

Ce module sert actuellement à démontrer la compilation séparée, les prototypes,
les headers, les symboles définis ou indéfinis et leur résolution par le linker.
Ce n’est pas encore une bibliothèque mathématique robuste. En particulier,
`divide` ne traite pas un diviseur nul.

### Liaison

`linker.ld` :

- sélectionne le format `elf64-x86-64` ;
- désigne `kernel_main` comme point d’entrée ;
- place le noyau dans la moitié haute à partir de `0xffffffff80000000` ;
- conserve et ordonne les marqueurs et requêtes Limine ;
- sépare les segments de requêtes, de code, de constantes et de données ;
- aligne les catégories principales sur des pages mémoire.

### Dépendance Limine

Le protocole officiel est intégré comme sous-module Git dans
`vendor/limine-protocol`. Le dépôt principal enregistre le commit exact utilisé,
actuellement `3a0526b700e356f0eac1b71a77697b3fd1c707a3`.

Le bootloader binaire lui-même n’est pas encore intégré.

## Ce qui ne fonctionne pas encore

- aucune image ISO ou disque amorçable ;
- aucun lancement QEMU de PunkOS ;
- aucun résultat visuel vérifié ;
- aucune sortie série ni fonction de panique ;
- aucune gestion des interruptions ;
- aucun pilote clavier ;
- aucune gestion de la mémoire ;
- aucune CLI ;
- aucun système de fichiers ;
- aucun processus ni espace utilisateur ;
- aucun bootloader propre à PunkOS.

## Prochain critère de réussite

Créer une image contenant Limine et `kernel.elf`, la démarrer avec QEMU et
observer le framebuffer magenta. Ce résultat prouvera toute la chaîne : firmware
virtuel, bootloader, chargement ELF, protocole Limine, entrée C et écriture en
mémoire vidéo.
