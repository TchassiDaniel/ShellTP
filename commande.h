/*
    Ce 
*/

#ifndef COMMANDE_H
#define COMMANDE_H

#include "processus.c"
#include <stdio.h>
#include <stdlib.h>

//Une structure de pile pour gerer les variables d'environnement
typedef struct Environ{
    char *nomVariable;
    char *valeur;

    struct Environ * prev;
} Environ;

char *jumpSpace(char *msg);

int echo(char *str);

int cat(char *str);

int ps(char *str, PileProcess *pile);

void quit(PileProcess *pile);

Environ *export(Environ *varEnv, char *str);

void env(Environ *varEnv);

/*
    Cette fonction prend en paramètre un tableau de 2 chaine de caratère, le premier argument est le 
    pid du processus auquel on veut envoyer un signal(Le signal par défaut est SIGTERM) 
    tandis que le second s'il exite le signal qu'on veut envoyer
*/
void signalKill(PileProcess *pile, char *arg[2]);

#endif