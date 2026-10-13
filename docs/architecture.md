# Architecture initiale

Ce document sépare les choix déjà effectués des questions encore ouvertes. Il
sera enrichi à mesure que les composants existeront.

## Choix actés

- PunkOS est un système éducatif écrit principalement en C.
- La première architecture ciblée est x86_64.
- Une quantité limitée d’assembleur sera utilisée lorsque le C ne suffit pas.
- Limine assurera initialement le chargement du noyau.
- Un bootloader PunkOS remplacera Limine dans une phase ultérieure.
- QEMU sera la plateforme de développement principale.
- Les tests sur matériel réel n’arriveront qu’après stabilisation en émulation.
- La CLI initiale sera intégrée au noyau, puis remplacée par un shell utilisateur.
- Le noyau ELF x86-64 est lié dans la moitié haute à partir de
  `0xffffffff80000000`.
- La révision de base 6 du protocole Limine est demandée pour le premier démarrage.

## Vue logique cible

```text
Firmware
   │
   ▼
Limine, puis bootloader PunkOS
   │  informations de démarrage
   ▼
Noyau PunkOS
   ├── démarrage et architecture x86_64
   ├── diagnostics et console
   ├── interruptions et horloge
   ├── mémoire physique et virtuelle
   ├── pilotes de périphériques
   ├── stockage et systèmes de fichiers
   ├── tâches et ordonnanceur
   └── interface des appels système
              │
              ▼
       Programmes utilisateur
          └── shell PunkOS
```

Cette vue est une cible conceptuelle. Elle ne préjuge pas encore de l’organisation
exacte des sources ni des interfaces entre modules.

## Contrat futur du bootloader

Le noyau ne devrait pas dépendre inutilement des détails internes de Limine. Une
couche de démarrage devra convertir les informations reçues en structures propres
à PunkOS. Le futur bootloader devra au minimum :

- charger l’image du noyau à l’emplacement attendu ;
- placer le processeur dans un état documenté ;
- fournir une carte de la mémoire utilisable et réservée ;
- fournir les informations nécessaires à l’affichage retenu ;
- fournir l’emplacement des éventuels modules de démarrage ;
- transférer le contrôle au point d’entrée du noyau.

Le détail précis de ce contrat sera écrit avant d’implémenter le bootloader.

L’intégration actuelle de Limine constitue le premier exemple concret de ce
contrat. Le noyau émet une requête framebuffer et attend en retour une description
de la mémoire vidéo. Le futur bootloader PunkOS devra fournir une abstraction
équivalente ou une couche d’adaptation devra convertir son propre protocole vers
les structures internes du noyau.

## Chaîne de compilation initiale

Le noyau sera initialement compilé avec Clang pour la cible
`x86_64-unknown-none-elf` et lié avec LLD. Ce choix permet de produire du code
freestanding sans dépendre des bibliothèques de la machine Ubuntu hôte. NASM est
disponible pour les portions qui nécessiteront explicitement de l’assembleur.

Ce choix décrit l’outil initial, pas une dépendance de l’architecture du noyau :
le code devra éviter autant que possible les extensions propres à un compilateur.

## Questions ouvertes

- format exact du noyau et modèle de chargement ;
- framebuffer, console texte ou combinaison des deux ;
- stratégie de gestion des interruptions matérielles ;
- organisation du code et conventions de nommage ;
- modèle de processus et politique d’ordonnancement ;
- premier contrôleur de stockage pris en charge ;
- format du premier système de fichiers ;
- ABI des appels système ;
- BIOS, UEFI ou les deux pour le bootloader PunkOS.

Une question ouverte ne doit pas être tranchée uniquement parce qu’une solution
est populaire. Nous documenterons le problème, les contraintes et la raison du
choix retenu.

## Dépendances externes

Les outils de développement peuvent être externes. Le noyau, lui, sera compilé
en environnement *freestanding* : il ne pourra pas supposer la présence de la
bibliothèque standard de la machine hôte.

Limine constitue une dépendance de démarrage temporaire et explicite. Son retrait
est planifié, contrairement aux outils de construction tels que le compilateur,
Make, QEMU ou GDB, qui restent des outils utilisés sur la machine de développement.
