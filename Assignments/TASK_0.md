
# Familiarisation avec le code du projet


## Exécution

Compilez et lancez le programme.

Allez dans le fichier tower_sim.cpp et rechercher la fonction responsable des inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?
Comment faire pour quitter le programme ?
A quoi sert la touche 'F' ?

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
Quelles informations s'affichent dans la console ?

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?

### Default Keyboard Shortcuts
|key | action |
|----|--------|
|x,q | exit   |
|f   | toggle fullscreen |
|+/- | zoom in/out|
|c   | create random aircraft |

deux avions atterissent (3?) et le reste vole autour le temps d'avoir de la place pour atterrir. une fois atterri il se gare et repars


## Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

Les classes : Aircraft, Airport, AirportType, Terminal, Tower, TowerSimulation, Waypoint

Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma entre ces difféntes classes pour illustrer comment elles intéragissent ensemble. 

Aircraft:
```cpp
const std::string& get_flight_num() const;
float distance_to(const Point3D& p) const;
void display() const override;
void move() override;
```

- get_flight_num() renvoie l'identifiant du vol
- distance_to(p) renvoie la distance au point p
- display() permet de dessiner l'avion
- move() permet le déplacement de l'avion

Tower:
```cpp
WaypointQueue get_instructions(Aircraft& aircraft);
void arrived_at_terminal(const Aircraft& aircraft);
```

- défini le fonctionnement d'un avion. s'il n'est pas dans un terminal, s'il y a de la
place dans un terminal : lui assigner un, faire un cercle autour de l'aéroport sinon.
s'il est dans un terminal, lui assigner un chemin pour repartir.
- arrived_at_terminal() vérifie que l'avion est bien arrivé au terminal (qu'il l'occupe)

Airport:
```cpp
Tower& get_tower();
void display() const override;
void move() override;
```

Fonctions de manipulation d'un aéroport : récupérer une tour, l'afficher, déplacer les terminaux (move).

Terminal:

```cpp
bool in_use() const;
bool is_servicing() const;
void assign_craft(const Aircraft& aircraft);

void start_service(const Aircraft& aircraft);

void finish_service();


void move() override
```

- in_use() : est-ce que le terminal est utilisé
- is_servicing() : est-ce que le terminal est utilisé par un avion
- assign_craft() : assigne un avion au terminal et le rend occupé.

- start_service(aircraft) : commence l'entretien d'un avion
- finish_service() : libère la place du terminal
- move(): actualisation du etrminal.

Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez pourquoi ce choix a été fait.

Le conteneur utilisé est std::deque, complexité constante pour toutes les opérations recherchées ici.
Les classes impliquées sont `Aircraft` (pour l'avion), `Tower` (pour le guider) et `Waypoint` (son chemin). Toute la résolution est effectuée dans `Tower::get_instructions(Aircraft);`


## Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.

- `aircraft_types.hpp` : init_aircraft_types();
Ici on peut modifier, pour chaque type d'avion, sa vitesse au sol, en l'air et son accélération maximale.

2) Identifiez quelle variable contrôle le framerate de la simulation.
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ? Fixez le problème.

- c'est la variable 
```cpp 
constexpr unsigned int DEFAULT_TICKS_PER_SEC = 16u;
```
aussi recupérée dans GL, `GL::`


3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.

La variable `service_progress` utilisée avec la constante `SERVICE_CYCLES`

4) Lorsqu'un avion décolle, celui-ci n'est pas retiré du programme.
Faites en sorte qu'il le soit.



5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Que pourriez-vous faire afin que l'ajout et la suppression de la liste soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit ?
Faites de même pour `DynamicObject`.

6) Le Tower a besoin de stocker pour tout `Aircraft` qui est actuellement affecté à un `Terminal` l'index de ce `Terminal` afin de pouvoir le "libérer" dès que son avion décollera. Cette affection prend actuellement la forme d'un `std::vector<std::pair<const Aircraft*, size_t>>` mais la recherche d'un avion dans ce vector prend temps linéaire (en nombre de `Terminal`s existant). C'est pas grave tant que ce nombre est petit, mais pour être préparé pour l'avenir, on aimerait bien remplacer le vector par une affectation qui reste efficace même en cas de beaucoup de `Terminal`s. Changez le code afin qu'il utilise un conteneur STL plus adapté (notamment, la fonction `find_craft_and_terminal(const Aicraft&)` ne sera plus nécessaire).


## Theorie

1) Comment a-t-on fait pour que person autre que le `Tower` peut reserver un terminal de l'aeroport?
2) En régardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`,
pouvez-vous dire pourquoi on a choisi de ne pas passer une réference?
Arrivez-vous à eviter copier le `Point3D` donné?


## Bonus

Actuellement, chaque execution de la fonction `move()` represente un interval de temps fix.
Si on augmente le nombre de ces executions par seconde, on accelere du coup le temps dans la simulation.
Faites en sorte que c'est plus le cas, i. e. le changement du framerate n'aura aucune influence sur la vitesse des avions, temps de débarquement, etc.
Vu qu'on va plus pouvoir controler le passage de temps dans la simulation avec le framerate,
ajoutez deux nouveaux inputs au programme permettant d'accelerer ou ralentir la simulation (controle du temps "simulé" qui se passe entre deux executions de `move()`).
