#ifndef PROCESSUS_H_
#define PROCESSUS_H_

#include "stdio.h"
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>

//Pour definir les etats des processus
typedef enum Etat
{
    EN_COURS_EXECUTION,
    ARRETE,
    TERMINE
}Etat;

//on definit les booleen
typedef enum bool
{
    false,
    true
}bool;
//Structure definissant un processus, un  élément de la pile
typedef struct Processus
{
    Etat etat;
    int pid;
    bool withShell;//Est associé au shel ou pas?
    struct Processus *precedent;
}Processus;

//Structure qui va soutenir la pile
typedef struct PileProcess
{
    Processus *process;
}PileProcess;

//Fonction pour creer un nouveau processus
void createPileProcess(PileProcess *pile);
//Fonction pour ajouter  un processus a la pile
void addProcess(PileProcess *pile, Etat etat, int pid, bool withShell);
//Fonction pour supprimer un processus de la pile a partir de son pid
void removeProcess(PileProcess *pile, int pid);
//Fonction permettant d'afficher toute la pile
void getPile(PileProcess *Pile);
//Fontion qui retoune un pointeur vers un element de la pile qu'on retrouve avec son pid
Processus *getProcess(PileProcess *pile, int pid);
//Fonction qui va permettre d'exécuter un signal envoyer à un processus crée par le shell 
//Et mettre à jour la liste de processus
void killProcess(PileProcess *pile, int pid, Etat etat);
//Fonction qui tue tout les processus associé au terminal
void killAllProcessShell(PileProcess *PileProcess);
#endif