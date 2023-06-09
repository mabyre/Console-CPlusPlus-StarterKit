La PM se configure par le fichier : pmEnv.h 
On y trouve une constante pour chaque module de la PM
a inclure ou non dans l'application.

Ce fichier inclut l'interface de mapping des fonctions : pmInterface.h
Ce fichier peut etre commun pour tous les projets mais il peu etre 
pratique de le particulariser pour un projet donné.

Les trois projet suivants sont independant du point de vue compilation :

**** TPMlite
les tests les plus complets sur la PM

**** TReport
test des fonctions de trace ERROR TRACE DEBUG

**** TSatck
test des fonctions de debogue de la pile système

**** PMLiteC
La lib qui est dans le workspace permet de créer une librairie static à 
l'aide des modules de la PM.
Pour la particulariser elle utilise les fichiers : pmEnv.h et pmInterface.h
qui sont dans le répertoire \inc