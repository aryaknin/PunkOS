# Journal d’apprentissage

Ce journal conservera les concepts étudiés, les expériences réalisées et les
erreurs instructives. Il ne remplace ni l’historique Git ni la documentation de
référence : il raconte la progression du projet.

## 2026-10-13 — Lancement du projet

### Décisions

- Le projet porte le nom **PunkOS**.
- Il s’agit d’un système éducatif développé principalement en C et en assembleur.
- La première cible sera x86_64.
- QEMU servira de machine de test principale.
- Limine sera utilisé temporairement afin de commencer par un noyau observable.
- Un bootloader propre à PunkOS fait partie du projet et remplacera Limine plus
  tard.
- Le développement privilégiera la compréhension : l’auteur du projet écrira
  chaque fonction et sera guidé par des explications, algorithmes et revues.

### Pourquoi utiliser temporairement Limine ?

Le démarrage, le format du noyau et le noyau lui-même constituent plusieurs
problèmes différents. Limine permet d’obtenir d’abord un noyau connu et testable.
Lorsque nous écrirons notre bootloader, ce noyau servira de référence : si le
remplacement ne démarre pas, la recherche pourra se concentrer sur le chemin
d’amorçage et son contrat avec le noyau.

### Prochaine étape

Créer le dépôt officiel, inventorier l’environnement de développement puis choisir
et vérifier la chaîne de compilation avant d’écrire le premier code du noyau.

## 2026-10-13 — Préparation de l’environnement

### Outils installés et vérifiés

- QEMU pour exécuter la machine virtuelle x86-64 ;
- xorriso pour construire les futures images ISO ;
- NASM pour les portions écrites en assembleur ;
- Clang et LLD pour compiler et lier le noyau ;
- GDB pour le futur débogage ;
- mtools pour manipuler les futures images FAT.

Un test a produit avec succès un objet ELF 64 bits x86-64 pour la cible
`x86_64-unknown-none-elf` en mode freestanding.

### Décision sur la chaîne initiale

Clang/LLD est retenu pour commencer. Clang sait viser directement une cible sans
système d’exploitation, ce qui permet de ne pas confondre les bibliothèques de
l’hôte Ubuntu avec les fonctionnalités disponibles dans PunkOS. La construction
d’un GCC croisé pourra être étudiée plus tard sans bloquer le premier démarrage.

### Prochaine étape

Comprendre le passage d’un fichier source C à un objet, puis à un noyau ELF, et
préparer l’intégration reproductible d’une version déterminée de Limine.

## 2026-10-13 — Première chaîne noyau complète jusqu’à ELF

### Compilation séparée

Le premier `kernel.c` a défini `kernel_main` et une boucle infinie. Une première
inspection avec `file`, `readelf` et `nm` a montré qu’un fichier source non
enregistré peut produire un objet vide : `.text` avait une taille nulle et aucun
symbole `kernel_main` n’existait. Après enregistrement et recompilation, le symbole
est apparu avec le type `T`.

Un second fichier `math.c` a servi à étudier la compilation séparée. `kernel.o`
contenait une référence `U add`, tandis que `math.o` fournissait `T add`. Les
fonctions `sub`, `mult` et `divide` ont ensuite été ajoutées par le développeur.

### Prototypes et headers

Un appel placé avant la déclaration de `add` a été refusé par Clang. Une tentative
d’initialisation globale avec `add(2, 3)` a également été refusée, car une variable
globale exigeait ici une valeur calculable à la compilation. L’appel a été replacé
dans `kernel_main` et précédé de son contrat.

Le contrat a ensuite été déplacé vers `include/punkos/math.h`, protégé contre les
inclusions multiples et partagé par `kernel.c` et `math.c`. Le renommage incomplet
de `div` vers `divide` a démontré que déclaration, appel et définition doivent
rester cohérents.

### Liaison et organisation mémoire

LLD a d’abord relié les objets pour résoudre leurs symboles. Un script de liaison
a ensuite été écrit pour sélectionner ELF64 x86-64, désigner `kernel_main`,
placer le noyau dans la moitié haute et séparer les permissions de ses segments.

Le processus manuel a aussi montré la différence entre reconstruire un objet et
reconstruire le noyau final : un `kernel.elf` antérieur ne contient pas les
modifications d’un `kernel.o` plus récent tant que le linker n’est pas relancé.

### Protocole Limine et framebuffer

Le dépôt officiel `limine-protocol` a été intégré comme sous-module. Le noyau
contient désormais les marqueurs de début et de fin des requêtes, demande la
révision de base 6 et émet une requête framebuffer.

Le point d’entrée vérifie la réponse, sélectionne le premier framebuffer RGB de
32 bits, calcule chaque position avec le `pitch` et prévoit de remplir l’écran en
magenta. `<stddef.h>` a été ajouté explicitement pour obtenir la macro `NULL` et
`<stdint.h>` pour les types entiers à largeur déterminée.

### État de fin de session

La compilation et la liaison réussissent avec les avertissements transformés en
erreurs. `kernel.elf` ne possède aucun symbole non résolu et expose deux segments
non vides : requêtes Limine en lecture/écriture et code en lecture/exécution.

Le noyau n’a pas encore été placé dans une image amorçable ni exécuté. Le prochain
objectif est d’intégrer une version binaire de Limine, écrire sa configuration,
construire l’image et observer le framebuffer magenta dans QEMU.
