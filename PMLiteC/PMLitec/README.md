## 05/07/2002 - Mabyre
Je viens de trouver le moyen de tracer des flottants en me servant de la fonction vsprintf.
Le Reporter possède maintenant 3 types de traces TRACE, ERROR et DEBUG.
Pour chacun de ces trois type il y a 4 niveaux de traces.
Les traces en flottants sont implementées au dessus des modules DEBUG et TRACE
Ces fonctions sont implemantées dans les modules pmTrace.c et pmDebug.c
	 
## 27/12/2000 - Mabyre
DEBUGx functions added to be compible with unix style debug
Elles ne fonctionnent pas bien dans le sens ou on ne peut pas les
supprimer !

## 03/04/2002 - Mabyre
Je regarde encore les traces et je trouve que cela ne va pas
c'est trop bordelique.
J'ai decide une campagne de renommage. Tout ce soft constitue en fait
la Machine Virtuelle (ou a porter) et pour ne pas confondre avec
la MV PlatForme j'appellerai tout cela Protocol Machinerie Signature : PM
comme cela fut sans doute le cas lors des premiers developpements

