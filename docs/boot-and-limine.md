# Démarrage et protocole Limine

## Responsabilités

Le linker, le bootloader et le processeur interviennent à des moments différents :

```text
LLD construit et organise kernel.elf
              ↓
Limine charge ses segments en mémoire
              ↓
Limine traite les requêtes du noyau
              ↓
Limine transfère le contrôle au point d’entrée
              ↓
Le processeur exécute kernel_main
```

Le linker ne charge rien dans la machine cible. Il prépare un fichier décrivant
les octets, les adresses virtuelles, les segments, leurs permissions et le point
d’entrée. Limine réalise le chargement au démarrage.

## Moitié haute

Le noyau est lié à partir de l’adresse virtuelle `0xffffffff80000000`, au début
de la région supérieure utilisée par la cible x86-64 de Limine. Une adresse
virtuelle n’est pas directement une position physique en RAM : les tables de
pagination préparées au démarrage établissent la correspondance nécessaire.

La section de code commence actuellement à `0xffffffff80001000`, car la page
précédente contient les requêtes Limine.

## Sections et segments

Les sections organisent les catégories produites par le compilateur et le linker.
Les segments décrivent au chargeur ce qui doit être placé en mémoire.

| Section | Segment | Permissions prévues |
| --- | --- | --- |
| `.limine_requests` | `limine_requests` | lecture, écriture |
| `.text` | `text` | lecture, exécution |
| `.rodata` | `rodata` | lecture |
| `.data`, `.bss` | `data` | lecture, écriture |

Le script utilise `KEEP` pour conserver les informations Limine même si une future
liaison supprime les objets apparemment inutilisés.

## Délimitation des requêtes

Le noyau contient :

```text
LIMINE_REQUESTS_START_MARKER
    LIMINE_BASE_REVISION(6)
    LIMINE_FRAMEBUFFER_REQUEST_ID
LIMINE_REQUESTS_END_MARKER
```

Les attributs Clang placent chaque objet dans la section attendue :

- `used` interdit au compilateur de le supprimer ;
- `section(...)` sélectionne sa section ELF ;
- `static` garde son nom privé à l’unité de compilation ;
- `volatile` exprime que le bootloader peut modifier les données.

La révision de base 6 décrit le comportement général demandé au protocole. La
révision `0` de la requête framebuffer est distincte : elle décrit la version de
cette fonctionnalité particulière.

## Modèle requête–réponse

Avant le chargement, le membre `framebuffer_request.response` vaut `NULL`. Limine
cherche l’identifiant de la requête dans l’image chargée et, s’il peut la traiter,
remplace ce membre par un pointeur vers sa réponse.

`kernel_main` vérifie impérativement :

1. que la révision de base est acceptée ;
2. que le pointeur de réponse n’est pas nul ;
3. qu’au moins un framebuffer est présent ;
4. que le premier framebuffer utilise le modèle RGB ;
5. que chaque pixel occupe 32 bits.

L’opérateur logique `||` évalue de gauche à droite et s’arrête dès que le résultat
est connu. La vérification du pointeur nul doit donc précéder son déréférencement.

## Écriture dans le framebuffer

Le champ `address` pointe vers la mémoire vidéo. Il est interprété comme un
tableau `volatile uint32_t` pour effectuer des écritures de 32 bits qui ne doivent
pas être supprimées par le compilateur.

Le framebuffer fournit :

- `width` et `height`, exprimés en pixels ;
- `pitch`, exprimé en octets par ligne ;
- `bpp`, exprimé en bits par pixel.

Pour un framebuffer de 32 bits, une ligne contient `pitch / 4` éléments de type
`uint32_t`. L’indice d’un pixel est donc :

```text
y * (pitch / 4) + x
```

Utiliser seulement `y * width + x` serait incorrect si le matériel ajoute du
remplissage à la fin de chaque ligne.

Le noyau écrit actuellement `0x00ff00ff` sur chaque pixel. Le magenta est choisi
comme preuve visuelle simple. Ce comportement ne sera considéré comme validé que
lorsque l’image aura réellement démarré dans QEMU.
