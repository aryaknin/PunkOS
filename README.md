# PunkOS

PunkOS est un système d’exploitation éducatif développé depuis zéro, principalement
en C et avec une petite quantité d’assembleur x86_64.

Le projet poursuit deux objectifs indissociables : comprendre le fonctionnement
d’un ordinateur à bas niveau et apprendre le C système en construisant chaque
brique progressivement. Le code du noyau ne sera pas traité comme une boîte
noire : chaque module devra être compris, conçu, écrit et testé étape par étape.

> **État du projet : premier noyau ELF construit, amorçage pas encore testé.**
> PunkOS possède un point d’entrée C, un script de liaison, une interface C entre
> modules et une requête framebuffer conforme au protocole Limine. La prochaine
> étape consiste à créer l’image amorçable et à la démarrer dans QEMU.

## Vision

À terme, PunkOS devra pouvoir démarrer sur une machine x86_64, présenter une
interface en ligne de commande propre et exécuter les opérations fondamentales
d’un petit système d’exploitation : gérer la mémoire, les interruptions, le
clavier, les fichiers, les programmes et les tâches.

Limine sera utilisé comme chargeur d’amorçage pendant les premières phases. Un
chargeur d’amorçage propre à PunkOS sera ensuite développé pour remplacer Limine.
Il fait donc partie du périmètre final du projet, même s’il n’est pas notre point
de départ.

## Objectifs pédagogiques

- maîtriser les fondations du langage C sans bibliothèque standard disponible ;
- comprendre la compilation, l’édition de liens et le format d’un exécutable ;
- comprendre le démarrage d’une machine et le rôle d’un bootloader ;
- manipuler directement la mémoire et les périphériques ;
- découvrir les interruptions, la pagination et les niveaux de privilège ;
- concevoir des interfaces noyau robustes avant leur implémentation ;
- apprendre à diagnostiquer les pannes bas niveau avec des traces et un débogueur ;
- documenter les décisions, les expériences et les erreurs rencontrées.

## Périmètre cible

La cible à long terme comprend notamment :

- une image amorçable sur architecture x86_64 ;
- un noyau écrit majoritairement en C ;
- l’affichage texte et une console série de débogage ;
- la gestion des interruptions et du clavier ;
- une CLI interactive ;
- la gestion de la mémoire physique et virtuelle ;
- un ordonnanceur et des tâches ou processus ;
- un système de fichiers ;
- des appels système et, à terme, un espace utilisateur ;
- un shell propre à PunkOS ;
- un bootloader propre au projet.

Ce périmètre est volontairement ambitieux. Chaque élément sera découpé en petits
jalons utilisables et vérifiables.

## Technologies prévues

| Élément | Choix initial | Rôle |
| --- | --- | --- |
| Architecture | x86_64 | Plateforme matérielle ciblée en premier |
| Langage principal | C | Implémentation de la majorité du noyau |
| Bas niveau | Assembleur | Entrée noyau, interruptions et instructions spécifiques au processeur |
| Bootloader temporaire | Limine | Charger le noyau pendant son développement initial |
| Émulateur | QEMU | Exécuter et tester PunkOS sans redémarrer la machine hôte |
| Débogueur | GDB | Inspecter les registres, la mémoire et l’exécution du noyau |
| Construction | Make, initialement | Automatiser compilation, image et lancement |
| Contrôle de version | Git et GitHub | Conserver l’historique du code et de la documentation |

La chaîne de compilation exacte, le format du noyau et les versions prises en
charge seront fixés pendant le premier jalon technique.

## Principes du projet

1. **Comprendre avant de coder.** Chaque nouveau mécanisme commence par un modèle
   mental et un petit objectif observable.
2. **Écrire nous-mêmes les briques pédagogiques.** Les bibliothèques et outils
   externes ne doivent pas masquer ce que nous cherchons à apprendre.
3. **Avancer par jalons minuscules.** Un changement doit idéalement produire un
   résultat testable dans QEMU.
4. **Documenter les décisions.** Une décision importante doit être accompagnée
   de son contexte, de ses compromis et de la possibilité de la réviser.
5. **Ne jamais confondre prototype et acquis.** Le statut réel de chaque composant
   doit rester visible dans la documentation.

## Documentation

- [Feuille de route](docs/roadmap.md)
- [État exact du projet](docs/current-status.md)
- [Architecture initiale](docs/architecture.md)
- [De la source C au noyau ELF](docs/build-pipeline.md)
- [Démarrage et protocole Limine](docs/boot-and-limine.md)
- [Environnement et méthode de développement](docs/development.md)
- [Journal d’apprentissage](docs/learning-log.md)

La documentation évolue avec le code. Lorsqu’un comportement change, le document
correspondant doit être mis à jour dans le même commit.

## Construction et exécution

Le noyau est actuellement construit manuellement afin de rendre visibles les
étapes de compilation et de liaison. Il n’existe pas encore de commande unique
ni d’image amorçable. Les commandes vérifiées sont consignées dans le document
[De la source C au noyau ELF](docs/build-pipeline.md).

## Avertissement

PunkOS est un projet d’apprentissage. Il ne doit pas être utilisé pour stocker des
données importantes ni comme système de production. Durant son développement,
il pourra planter, corrompre ses disques virtuels ou adopter des interfaces
incompatibles entre deux versions.

## Licence

PunkOS est distribué sous la licence [GNU General Public License version 3](LICENSE).
Cette licence garantit que le code et ses versions dérivées restent libres et
que leurs sources demeurent accessibles selon les conditions de la GPLv3.
