Objectif: manipulation des fonctions C liées à la création des processus. Un futur TP ira directement dans le noyau.

Faire un mini-shell offrant les services suivants:

-Boucle indéfiniment en présentant un prompt
-Implante des commandes internes:
  *exit: sort du shell en tuant tous les processus qui sont attachés au shell
  *ps: affiche les processus créés dans le shell (pid, ppdi, commande qui a crée le processus)
  *export: permet de définir une variables d'environnement
  *env: affiche la liste des variables d'environnement de son shell, ainsi que leur valeur
  *kill: pour envoyer un signal à un processus en cours d'exécution. Un processus que le shell avait crée.
    **implanter trois options de kill: arrêt définitif, suspension, continuation
  *bg: met le dernier processus précédemment suspendu avec ctr-z (voir ci-dessous) en arrière plan.
-Est capable d'exécuter des commandes externes ou des programmes externes. Pour cela, le shell prend en compte la variable d'environnement PATH interne à lui même. Pas celui de votre shell
-nohup: ne pas associer l'exécution d'une commande au shell. Ainsi, lorsqu'on sort du shell, les processus qui ne lui sont pas associés ne sont pas tués
-Exécute une commande en arrière plan lorsque la commande se termine par &
-Tout processus qui s'exécute en avant plan peut être arrêté avec ctr-C. Il peut aussi être suspendu avec ctr-z. Un processus précédemment suspendu qui est relancé (reçoit le signal de continuation), revient en avant plan.
