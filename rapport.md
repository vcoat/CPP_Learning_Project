# Rapport tower_control
## COAT Valentin


## Réponse aux questions

### Task 0
### A - Execution

1) Allez dans le fichier tower_sim.cpp et recherchez la fonction responsable de gérer les inputs du programme. Sur quelle touche faut-il appuyer pour ajouter un avion ?

Il faut appuyer sur la touche 'c' pour ajouter un avion.


2) Comment faire pour quitter le programme ?

Il faut appuyer sur les touches 'x' ou 'q'.


3) A quoi sert la touche 'F' ?

Elle sert à basculer en mode plein écran.


4) Ajoutez un avion à la simulation et attendez. Que est le comportement de l'avion ?

L'avion plane un moment au-dessus de l'aéroport avant de se poser sur la piste d'atterissage. Il se dirige ensuite vers le terminal via la piste et attends quelques secondes avant de redécoller. L'action se répète en boucle.


5) Quelles informations s'affichent dans la console ?

Voici les informations sont affichées pour chaque avion (ex BA2433):
<nom_avion> is now landing...\
now servicing <nom_avion>\
done servicing <nom_avion>\
BA2433 <nom_avion>


6) Ajoutez maintenant quatre avions d'un coup dans la simulation. Que fait chacun des avions ?

Les avions occupent tous les terminaux disponible. Dès qu'un terminal se libère, un autre avion peut atterrir.

### B - Analyse du code

1) Listez les classes du programme à la racine du dossier src/. Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

Aircraft&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;=> Représente un avion (numéro, type, position, vitesse actuelle...)           
AircraftType&nbsp;&nbsp;&nbsp;=> Représente un type d'avion (vitesses max et texture associée)       
Airport&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; => Représente un aéroport ( défini par un type d'aéroport, une liste de terminaux...)         
AirportType     &nbsp;&nbsp;    => Représente un type d'aeroport (liste de pistes, et positions de ses élements clés)\
Point2D &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;=> Représente un point en 2D (coordonnées x et y)\
Point3D&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;=> Représente un point en 3D (coordonnées x,y,z)\
Runway&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; => Represénte une piste d'atterissage, avec deux Point3D\
Terminal &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;           => Représente un terminal (position et éventuel avion en service)  
Tower&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; => Représente une tour de contrôle (aeroport et liste les avions associés à un terminal)\
TowerSimulation  &nbsp;   => Représente la simulation complète (contient l'aéroport)\
Waypoint &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;           => Représente un point pour guider les avions, hérite de Point3D

2) Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ? Quel conteneur de la librairie standard a été choisi pour représenter le chemin ? Expliquez les intérêts de ce choix.

La première classe impliquée est Waypoint, qui représente le point de guidage de base. Un Waypoint peut-être de 3 types : air, ground ou terminal. La classe définie une WaypointQueue comme étant une deque de Waypoint, conteneur qui peut par exemple servir à insérer des Waypoint en tête et les retirer en queue en temps constant. Le conteneur est donc adapté pour retirer des points au fur et à mesure du parcours de l'avion. La classe Tower est également impliquée, puisqu'elle possède la fonction-membre get_instructions, qui permet à un avion de connaître le chemin a emprunter. Si l'avion n'est pas a un terminal, la tour regarde si un terminal est disponible, et si il n'y en a aucun la tour ordonne a l'avion de faire un cercle en attendant avec la fonction Tower::get_circle(). La classe Airport s'occupe de renvoyer un chemin pour l'avion qui lui permettra de passer des airs au terminal reservé, via la fonction reserve_terminal(), ou bien inversement, d'un terminal aux airs, via la fonction start_path(). Ces deux fonctions appellent respectivement les méthodes terminal_to_air() et air_to_terminal(), membres de la classe AirportType.

### C - Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion. Le Concorde est censé pouvoir voler plus vite que les autres avions. Modifiez le programme pour tenir compte de cela.

Les vitesses maximales sont définies dans le fichier aircraft_types.hpp (fonction init_aircraft_types())


2) Identifiez quelle variable contrôle le framerate de la simulation. Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur. Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.

La variable qui contrôle le framerate est GL::ticks_per_sec contenue dans le fichier config.hpp
Si l'on tente de mettre en pause le programme via le framerate, on a une exception en point flottant, car on tente de diviser par zero dans la fonction timer() de opengl_interface.cpp


3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.

La variable qui contrôle le temps de débarquement est SERVICE_CYCLES dans config.hpp


4) Lorsqu'un avion a décollé, il réattérit peu de temps après. Faites en sorte qu'à la place, il soit retiré du programme.
Indices :
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ? A quel endroit de la callstack pourriez-vous le faire à la place ?
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?

L'avion doit etre supprimé des move et display queue. On ne peut pas faire depuis la fonction move de aircraft a cause de la boucle for each ,qui empeche tout modification pendant son parcours. On doit renvoyer un booléen a la fonction timer indiquant que l'objet que l'on move peut etre supprimé.

5) Lorsqu'un objet de type Displayable est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher. Il faut également penser à le supprimer de cette liste avant de le détruire. Faites en sorte que l'ajout et la suppression de display_queue soit "automatiquement gérée" lorsqu'un Displayable est créé ou détruit. Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour DynamicObject ?

On ne retire pas directement le DynamicObject de la liste car il peut-être utilisé ailleurs dans la programme.


### D - Théorie

1) Comment a-t-on fait pour que seule la classe Tower puisse réserver un terminal de l'aéroport ?

Seule la classe Tower connaît l'association entre un avion et un numéro de terminal, puisqu'elle la stocke dans un conteneur. La réservation ne peut donc se faire que via une fonction membre de la classe Tower.


2) En regardant le contenu de la fonction void Aircraft::turn(Point3D direction), pourquoi selon-vous ne sommes-nous pas passer par une réference ? Pensez-vous qu'il soit possible d'éviter la copie du Point3D passé en paramètre ?

On ne passe pas par référence ici puisque la valeur de Point3D direction ne sera pas modifiée.


### Task 1
### Analyse de la gestion des avions

1) Si à un moment quelconque du programme, vous souhaitiez accéder à l'avion ayant le numéro de vol "AF1250", que devriez-vous faire ?

Il faudrait maitenir une liste des numéros de vols en plus des display_queue et move_queue.

### B - Déterminer le propriétaire de chaque avion
1) Qui est responsable de détruire les avions du programme ? (si vous ne trouvez pas, faites/continuez la question 4 dans TASK_0)

Le response est la fonction timer contenue dans opengl_interface.cpp


2) Quelles autres structures contiennent une référence sur un avion au moment où il doit être détruit ?

Hormis la move_queue, les autres structures qui contiennent une référence sur l'avion sont la display queue ainsi que le terminal associé si celui-ci était en service lorsqu'il est détruit.

3) Comment fait-on pour supprimer la référence sur un avion qui va être détruit dans ces structures ?

Il faut chercher la référence correspondante dans la structure avec std::find par exemple, puis erase la position indiquée par l'itérateur renvoyé par std::find.

4) Pourquoi n'est-il pas très judicieux d'essayer d'appliquer la même chose pour votre AircraftManager ?

Il n'est pas judicieux de rechercher la référence si on doit supprimer l'aicraft puisqu'on itère déjà sur tous les avions lorsqu'on les update.



Modifiez Aircraft::add_waypoint afin que l'évaluation du flag ait lieu à la compilation et non à l'exécution. Que devez-vous changer dans l'appel de la fonction pour que le programme compile ?

Il faut templater la fonction Aircraft::fonction add_waypoint avec la valeur du flag, donc il faut placer entre chevrons le flag avant l'appel de la fonction.


## Commentaires sur le projet

Je dois avouer que j'ai eu du mal avec ce projet au début car la base de code fournie était très importante, et comprendre comment toutes les classes intéragissent entre elles m'a pris pas mal de temps. Il a cependant été très enrichissant ! Dans la vraie vie réelle, en entreprise, on débute rarement un projet de zéro et on doit très souvent passer du temps à lire et comprendre le code des autres développeurs. Du coup ce projet était un très bon entraînement. J'ai l'impression que malgré le fait que le C++ soit un language très exigeant, il n'est pas si compliqué si l'on est rigoureux et que l'on a bien relu son cours 🙂 (bon la syntaxe fait quand même mal à la tête, mention spécial aux références universelles).\

J'ai quand même eu du mal sur certains points. Le premier concerne le fuel_stock, et comment les terminaux peuvent y accéder. Je n'ai pas réussi a faire autrement que de passer par un pointeur classique, comme current_aircraft, pour que chaque terminal puisse consulter le stock disponible. Peut être aurait-il fallu placer une référence de l'aéroport dans chaque terminal ?\
Un autre point concerne la Task 4, je n'ai pas réussi à faire marcher mon constructeur avec template variadique sans utiliser SFINAE, car même en définissant tous les constructeurs de copie possible et imaginable, le compilateur choissisait toujours l'overload avec template variadique, ce qui entrainait des erreurs à la compilation. Méchant GCC 😠 !
Enfin, je ne suis toujours pas vraiment à l'aise avec l'ownership, qui doit posséder la ressource, quand utiliser un shared_ptr ou un weak_ptr, et toutes ces choses là. J'imagine que ça va prendre un peu de temps encore avant que ce soit naturel. 

Le projet m'a fait rendre compte à quel point la STL est bien construite, avec les itérateurs et les lambdas, tout est très simple d'utilisation, et les fonctions adaptées se trouvent rapidement sur cppreference. J'ai également trouvé que le projet était très bien construit, les questions bien amenés et en rapport avec les chapitres que l'on a vu dans le cours. On est jamais perdus, vu que les fonctions a modifier sont indiquées, et la difficulté ou la longueur ne sont jamais trop importantes.


