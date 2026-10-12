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

Les commandes d’installation dépendront du système hôte et seront ajoutées après
inventaire. Nous enregistrerons également les versions testées.

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

## Prochain inventaire

Avant le premier fichier C, nous vérifierons ensemble :

- le système d’exploitation hôte ;
- les compilateurs disponibles ;
- les outils binaires disponibles ;
- la présence et la version de QEMU ;
- la présence et la version de GDB ;
- les méthodes possibles pour intégrer Limine de manière reproductible.

