# Architecture technique

## Préambule
L'escape room est remplie de machines à codes diverses et variées qu'il faut assembler, comprendre et utiliser pour avancer dans le scénario. Certains de ces dispositifs doivent être pilotables à distance par le maître de jeu (game master), donc connectés, d'autres seront low tech.

Ce document décrit une architecture technique simple permettant aux dispositifs connectés d'être contrôlés à distance et de communiquer entre eux.

## Contraintes

### Réseau des collèges
Le réseau informatique des collèges est sont de topologie compliquée. Seule une personne dédiée à cette charge peut l'opérer. Il ne faut donc pas être dépendant de cette complexité, d'autant que les lieux potentiels d'installation d'une escape room peuvent ne pas être raccordés en filaire ou hors converture wifi.
Il faut donc déployer un réseau spécifique pour l'escape game. 
_Le choix se portera sur un routeur wifi, capable de distribuer des adresses IP, d'une plage interne (192.168.xxx.xxx)_.

### Interconnexion des dispositifs
Certains dispositifs nécessiteront peut-être une inter-connexion afin de se transmettre des actions. Ces interconnexions se feront à travers le réseau wifi spécifique, via http, de façon à normaliser les échanges avec une API REST ou équivalent. L'objectif étant aussi de rester dans une architecture simple et compréhensible.
_Le choix du http / Api REST permet aussi une ouverture pour construire un back-office centralisé d'inter-action avec les dispitifs de l'Escape Room, à destination du maître de jeu_

### Tout doit être ré-initialisable, à tout moment
Il faut effet donner la possibilité au maître de jeu d'intervenir sur tout les dispositifs connectés afin de les changer d'état, selon l'aide qu'il veut fournir à l'équipe.

### Adressage des dispositifs
Voir s'il faut qu'ils aient une IP fixe, ou une sorte de résolution de nom du type "machin.local"...

## API
- "/" : La racine du serveur web, donne
	- Le descriptif technique du dispositif (IP, Mac, Nom, uptime, ...), 
	- La liste des api auxquelles il peut répondre (/open, /close, /up, /down, /on, /off, /play, /stop, ...)
	- L'état des capteurs et actionneurs reliés

- "/state/#pin-number" 
	- L'état de la pin (HIGH/LOW, 0/1, valeur analogique, Position en °, ...)

- "/open, /on, /play, ..."	: Enclencher une action d'ouverture, d'allumage, de lancement d'un média
- "/pause"					: Mettre en pause une action commencée
- "/close, /off, /stop, ..."	: Enclencher une action de fermeture, d'extinction, d'arrêt de média

- "/shutdown" : éteindre ou mettre en veille le dispositif
- "/startup"  : Allumer ou réveiller le dispositif
- "/reset" : Faire un reset de bas niveau sur le dispositif (enchînement de shutdown -> startup).

## Fonctionnalités de communication des dispositifs
- Répondre à des API Web
- Envoyer et recevoir des messages ou bundles OSC
- Envoyer et recevoir des siganux DMX
- Envoyer et recevoir des informetions par liaison série (utile pour le débug).

