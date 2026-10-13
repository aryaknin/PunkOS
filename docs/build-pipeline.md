# De la source C au noyau ELF

Ce document explique la chaîne de construction réellement utilisée durant la
première session de PunkOS.

## Vue générale

```text
src/kernel.c ── compilation ──> build/kernel.o ──┐
                                                 ├── liaison ──> build/kernel.elf
src/math.c   ── compilation ──> build/math.o   ──┘       ↑
                                                        │
                                                    linker.ld
```

Le compilateur et le linker ont des responsabilités différentes. Clang comprend
le langage C et produit du code machine dans des fichiers objets. LLD rassemble
les objets, résout leurs références et leur attribue une organisation finale.

## Mode freestanding

Un programme ordinaire est construit pour s’exécuter sous un système existant.
Un noyau doit au contraire fournir cet environnement. PunkOS est donc compilé en
mode *freestanding* : il ne suppose ni fonction `main`, ni libc, ni terminal, ni
allocation dynamique, ni processus hôte.

Options utilisées :

| Option | Rôle actuel |
| --- | --- |
| `--target=x86_64-unknown-none-elf` | Produit du code x86-64 sans OS cible |
| `-ffreestanding` | Active le modèle C freestanding |
| `-fno-stack-protector` | Évite une dépendance à un runtime de protection de pile |
| `-mno-red-zone` | Réserve la pile sous son pointeur aux futurs usages du noyau |
| `-Wall -Wextra -Werror` | Rend les diagnostics stricts |
| `-I include` | Rend les headers PunkOS accessibles |
| `-I vendor/limine-protocol/include` | Rend `limine.h` accessible |
| `-c` | S’arrête après la création du fichier objet |

Commande vérifiée pour le point d’entrée :

```sh
clang --target=x86_64-unknown-none-elf \
  -ffreestanding -fno-stack-protector -mno-red-zone \
  -Wall -Wextra -Werror \
  -I include -I vendor/limine-protocol/include \
  -c src/kernel.c -o build/kernel.o
```

Commande vérifiée pour le module arithmétique :

```sh
clang --target=x86_64-unknown-none-elf \
  -ffreestanding -fno-stack-protector -mno-red-zone \
  -Wall -Wextra -Werror \
  -I include -I vendor/limine-protocol/include \
  -c src/math.c -o build/math.o
```

## Fichiers objets et symboles

Un `.o` est un fichier ELF relogeable. Il contient du code machine, des sections,
une table de symboles et les références que le linker devra résoudre.

Avant la liaison, `kernel.o` définit `kernel_main` mais demande les fonctions
arithmétiques :

```text
U add
U divide
T kernel_main
U mult
U sub
```

`T` indique un symbole défini dans la section de code `.text`. `U` signifie que
le symbole est utilisé mais non défini par cet objet.

`math.o` fournit les définitions correspondantes :

```text
T add
T divide
T mult
T sub
```

Le prototype présent dans le header permet à Clang de vérifier l’appel. Il ne
fournit aucune implémentation. Le linker associe ensuite les références `U` aux
définitions `T` de même nom.

## Headers communs

`include/punkos/math.h` représente le contrat partagé entre l’utilisateur des
fonctions et leur implémentation. `kernel.c` et `math.c` incluent le même fichier,
ce qui permet au compilateur de détecter les signatures incompatibles.

La protection `PUNKOS_MATH_H` empêche le contenu du header d’être traité plusieurs
fois dans une même unité de compilation.

Les headers `<stddef.h>` et `<stdint.h>` utilisés par le noyau sont des headers C
compatibles avec le contexte freestanding. Ils fournissent respectivement `NULL`
et les types entiers à largeur déterminée comme `uint32_t` ou `uint64_t` ; ils ne
fournissent pas une bibliothèque d’exécution.

## Liaison

Commande vérifiée :

```sh
ld.lld -T linker.ld -static -z max-page-size=0x1000 \
  build/kernel.o build/math.o -o build/kernel.elf
```

`-T linker.ld` sélectionne notre plan mémoire. `-static` interdit une dépendance
à un chargeur dynamique. `-z max-page-size=0x1000` fixe la taille d’alignement
maximale à 4096 octets.

Le résultat vérifié possède deux segments actuellement non vides :

```text
LOAD RW  0xffffffff80000000  .limine_requests
LOAD R E 0xffffffff80001000  .text
```

Les segments `rodata` et `data` sont décrits par le script mais omis du résultat
actuel parce que leurs sections sont vides. Aucun symbole ne reste indéfini après
la liaison.

## Limite du processus manuel

La construction manuelle a montré qu’un objet peut devenir plus ancien que son
source, ou que `kernel.elf` peut devenir plus ancien que ses objets. Le futur
Makefile encodera ces dépendances pour reconstruire automatiquement uniquement ce
qui est nécessaire.
