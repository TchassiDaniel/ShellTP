#include "processus.h"

//Fonction pour creer un nouveau processus
void createPileProcess(PileProcess *pile)
{
    pile->process = NULL;
}

void addProcess(PileProcess *pile, Etat etat, int pid, bool withShell)
{
    Processus *proc = malloc(sizeof(Processus));
    proc->etat = etat;
    proc->pid = pid;
    proc->precedent = pile->process;
    proc->withShell = withShell;

    pile->process = proc;

}

void removeProcess(PileProcess *pile, int pid)
{
    Processus *proc = pile->process;
    Processus *procSuivant;
    int trouve = 0;//Booleen qui va nous indiquer si on a trouvé le dit processus dans la pile

    while(trouve == 0)
    {
        if (proc->pid == pid)
        {
            break;
        }
        procSuivant = proc;
        proc = proc->precedent;
    }

    if(proc == pile->process)
    {
        proc->etat = TERMINE;
        proc->pid = 0;
        pile->process = proc->precedent;
        proc->precedent = NULL;
    }
    else
    {
        procSuivant->precedent = proc->precedent;
        proc->etat = TERMINE;
        proc->pid = 0;
        proc->precedent = NULL;
    }
}

void getPile(PileProcess *pile)
{
    Processus *proc = pile->process;

    printf(" PID \t\tETAT\n");

    while (proc != NULL)
    {
        if(proc->etat == EN_COURS_EXECUTION)
            printf("%d \t\tEN COURS D'EXECUTION\n",proc->pid);
        else
            printf("%d \t\tEN COURS D'EXECUTION\n",proc->pid);
        proc = proc->precedent;
    }
    
}

Processus *getProcess(PileProcess *pile, int pid)
{
    Processus *proc = pile->process;
    int trouve = 0;

    while(trouve == 0)
    {
        if (proc->pid == pid)
        {
            break;
        }
        proc = proc->precedent;
    }

    return proc;
}

void killProcess(PileProcess *pile, int pid, Etat etat)
{
    int success = 1;//permettra de savoir si le signal a été envoyé avec succès

    if(etat == EN_COURS_EXECUTION)
    {
        if((success = kill(pid, SIGCONT))==0)
            (getProcess(pile, pid))->etat = EN_COURS_EXECUTION;
    }
    else if(etat == ARRETE)
    {
        if((success = kill(pid, SIGSTOP)) == 0)
            (getProcess(pile, pid))->etat = ARRETE;
    }
    else if(etat == TERMINE)
    {
        if((success = kill(pid, SIGTERM)) == 0)
            removeProcess(pile, pid);
    }

    if(success != 0)
        printf("Erreur lors de l'envoi du signal au processus\n");
}

void killAllProcessShell(PileProcess *pile)
{
    Processus *proc = pile->process;

    while (proc != NULL)
    {
        if(proc->withShell == true)
            killProcess(pile, proc->pid, TERMINE);
        proc = proc->precedent;
    }
}