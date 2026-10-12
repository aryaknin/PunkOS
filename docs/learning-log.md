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
