# Feuille de route

Cette feuille de route décrit une direction, pas un calendrier. Un jalon est
terminé uniquement lorsque son fonctionnement est compris, démontrable et
documenté.

## Phase 0 — Fondation du projet

Objectif : disposer d’un dépôt reproductible et d’une méthode de travail claire.

- initialiser Git et le dépôt GitHub ;
- documenter la vision et le périmètre ;
- choisir la chaîne de compilation ;
- installer et vérifier QEMU, GDB, Make et les outils binaires ;
- définir les conventions de code et de commits ;
- automatiser les vérifications élémentaires.

Critère de sortie : une nouvelle machine de développement peut être préparée à
partir de la documentation, sans connaissance implicite du projet.

## Phase 1 — Premier démarrage

Objectif : faire charger par Limine un noyau minimal et obtenir une preuve visible
de son exécution.

- comprendre le rôle du compilateur, de l’assembleur et de l’éditeur de liens ;
- créer le point d’entrée du noyau ;
- définir le script de l’éditeur de liens ;
- produire une image amorçable ;
- démarrer l’image avec QEMU ;
- écrire un message sur la console série ou dans le framebuffer ;
- arrêter proprement le processeur lorsque le noyau n’a plus rien à exécuter.

État au 13 octobre 2026 : le noyau ELF, son point d’entrée, son organisation
mémoire et sa requête framebuffer sont implémentés et vérifiés statiquement.
L’image amorçable, le lancement QEMU et la preuve visuelle restent à réaliser.

Critère de sortie : QEMU démarre de manière reproductible et affiche un message
provenant du code de PunkOS.

## Phase 2 — Fondations du noyau

Objectif : obtenir un environnement C minimal, observable et suffisamment sûr
pour poursuivre.

- types entiers et conventions propres au noyau ;
- fonctions mémoire indispensables ;
- journalisation sur port série ;
- gestion explicite des erreurs fatales (`panic`) ;
- GDT et IDT ;
- exceptions du processeur avec diagnostic lisible ;
- premières interruptions matérielles et temporisation.

Critère de sortie : les erreurs du processeur ne donnent plus seulement un écran
noir ; elles produisent un diagnostic exploitable.

## Phase 3 — Mémoire

Objectif : connaître et contrôler la mémoire utilisée par le noyau.

- lire la carte mémoire transmise par Limine ;
- gérer les pages physiques disponibles ;
- comprendre et gérer les tables de pagination ;
- créer un allocateur dynamique pour le noyau ;
- détecter les erreurs courantes d’allocation.

Critère de sortie : le noyau peut réserver, utiliser et libérer de la mémoire sans
écraser ses propres structures.

## Phase 4 — Console et CLI du noyau

Objectif : interagir directement avec PunkOS.

- pilote clavier ;
- gestion élémentaire de l’affichage texte ;
- tampon de saisie et édition d’une ligne ;
- analyse des commandes et arguments ;
- commandes initiales : `help`, `clear`, `echo`, `meminfo`, `reboot`, `shutdown` ;
- historique et traitement cohérent des erreurs.

Cette CLI sera d’abord un moniteur exécuté dans le noyau. Elle ne deviendra un
véritable shell qu’après l’apparition de programmes en espace utilisateur.

## Phase 5 — Stockage et fichiers

Objectif : rendre les données structurées et persistantes.

- accès à un disque virtuel ;
- abstraction en périphériques bloc ;
- système de fichiers minimal ou prise en charge d’un format simple ;
- chemins, répertoires et fichiers ;
- commandes d’inspection et de manipulation.

Le format de disque ne sera choisi qu’après étude des compromis et des pilotes
nécessaires.

## Phase 6 — Tâches et espace utilisateur

Objectif : séparer les programmes du noyau et exécuter plusieurs activités.

- sauvegarde et restauration du contexte processeur ;
- ordonnanceur initial ;
- séparation des espaces d’adressage ;
- niveaux de privilège ;
- appels système ;
- chargement de programmes ;
- communication et synchronisation élémentaires.

## Phase 7 — Shell et programmes

Objectif : remplacer le moniteur du noyau par un environnement utilisateur.

- processus d’initialisation ;
- entrée et sortie standard ;
- arguments et codes de retour ;
- shell PunkOS ;
- programmes fondamentaux ;
- redirections et tubes lorsque le modèle de processus le permettra.

Le but n’est pas de reproduire immédiatement Bash. Les fonctionnalités seront
introduites à partir des primitives réellement fournies par PunkOS.

## Phase 8 — Bootloader PunkOS

Objectif : remplacer Limine par un chargeur conçu et écrit dans le projet.

- définir précisément le contrat bootloader–noyau ;
- étudier BIOS, UEFI et le format de l’image de démarrage ;
- charger le noyau depuis le support ;
- obtenir la carte mémoire et les informations d’affichage ;
- préparer le mode d’exécution attendu par le noyau ;
- transférer le contrôle au noyau ;
- conserver Limine comme implémentation de référence durant la transition.

Cette phase arrivera lorsque le noyau sera suffisamment stable pour servir de
programme de test connu. Le bootloader reste un livrable officiel de PunkOS.

## Au-delà

Selon nos intérêts : réseau, USB, audio, interface graphique, multiprocesseur,
permissions, compatibilité POSIX partielle ou portage vers une autre architecture.
Ces sujets ne font pas encore partie des engagements du projet.
