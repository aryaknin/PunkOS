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

