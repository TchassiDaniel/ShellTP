//Importation des bibliothèque du C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <sys/types.h>
#include <wait.h>
#include  <unistd.h>
#include <signal.h>
#include "commande.c"
#include "processus.h"

#define MAX 255

//Elle va nous permettre de recuperer la commande de l'utilisateur dans la chaine entree
//Elle retourne la position de la fin de la commande
int recupererCommande(char *commande, char* entree);
//Elle va nous permettre de savoir si le processus actuellement en cours d'exécution a demandé au parent
//D'attendre qu'il de temine
bool inParent = false;


PileProcess pile;//Cette variable contiendra tout les processus en cours d'execution ouarreté
PileProcess pileStop;//Cette pile va contenir les processus qui ont été arreté dans l'odre

void monSignchild(int signal)
{

    int status = -7;
    int pid = wait(&status);
    //Si le pid est -1 On ne fait rien
    if(pid == -1);
    //On regarde si le programme s'est ARRETE

    else if(WIFSTOPPED(status))
        (getProcess(&pile, pid))->etat = ARRETE;
    //On regarde si le programme a été relancé
    else if(WIFCONTINUED(status))
        (getProcess(&pile, pid))->etat = EN_COURS_EXECUTION;
    //On regarde si le programme cet arreté
    else if(WIFEXITED(status))
        removeProcess(&pile, pid);
    else if(WIFSIGNALED(status))
		removeProcess(&pile, pid);
        
}
//Cette fonction définit le comportement du signal SIGINT(envoyé par CTRL-C)
void monSignInt(int signal){
    if(inParent == true){
        int pid = (pile.process)->pid;
        killProcess(&pile, pid, TERMINE);
    }    
}
//Cette fonction définit le comportement du signal SIGSTP(envoyé par CTRL-Z)
void monSignStp(int signal){
    if(inParent == true){
        int pid = (pile.process)->pid;
        killProcess(&pile, pid, ARRETE);
        addProcess(&pileStop, ARRETE, pid, true);
    }
}

int main()
{
    createPileProcess(&pile);
    createPileProcess(&pileStop);

    //On definit la fonction qui gerera ce qui arrivze a l'enfant
    signal(SIGCHLD, &monSignchild);
    signal(SIGTSTP, &monSignStp);
    signal(SIGINT, &monSignInt);
    do{
        char *entree = malloc(MAX * sizeof(char));//Cette variable va contenir la ligne validée par l'utilisateur
        char commande[MAX];//Cette chaine extraire le premier mot extrait de l'entree supposé etre la commande a executer

        int i = 0;

        bool nohup = false;//pour savoir si la commande nohup à été activé

        //On regarde si le processus sera traité en parallèle
        int paralell = 0;
        
        printf("daniel@ubuntu$ ");
        fgets(entree, MAX, stdin);
        *strchr(entree, '\n') = '\0';

        //On recupere la commande de l'utilisateur
        i = recupererCommande(commande, entree);

        if(strcmp(commande, "nohup") == 0)
        {
            nohup = true;
            paralell = 1;
            i = recupererCommande(commande, entree+i+1);
        }

        if(strcmp(commande, "echo") == 0)
        {
            //Si l'utilisateur à mis des espaces avant son argument on les elimine d'abord
            entree = jumpSpace(entree+i);
            echo(entree);
        }
        else if(strcmp(commande, "cat") == 0)
        {
            char msg[255];
            strcpy(msg, entree+i+1);
            cat(msg);
        }
        else if(strcmp(commande, "ps") == 0)
        {
            ps(NULL, &pile);
        }
        //Si la commande entrée est exit, on tue tout les processus attaché au shell et on termine le shell
        else if(strcmp(commande, "exit") == 0)
        {
            quit(&pile);
            return 0;
        }
        else if(strcmp(commande, "kill") == 0)
        {
            char *arg[2];//Tableau qui contiendra les argumments
            int k = 0;

            arg[k] = strtok((entree + i), " ");
            while(arg[k] != NULL)
            {
                k++;
                arg[k] = strtok(NULL, " ");
            }

            signalKill(&pile, arg);
        }
        else//S'il ne s'agit pas de commande interne
        {
            
            if(entree[strlen(entree)-1] == '&')
            {
                paralell = 1;
                *strchr(entree, '&') = '\0';
            }

            int pid_child = fork();
            //Si on arrive pas a crée un processus on passe au tour suivant(Le fork échoue)
            if(pid_child == -1)
            {
                printf("Impossible de lancer la tache");
                continue;
            }

            if(pid_child == 0)
            {
                //On récupère les arguments pour la commande demande par l'utilisateur
                char* argv[MAX];
                int i = 0;
                //On recupere les éléments entrée par l'utilisateur en  argument, et la commande
                argv[i] = strtok(entree, " ");
                while (argv[i] != NULL)
                {
                    i++;
                    argv[i] = strtok(NULL, " ");
                }
                //On est dans l'enfant
                execvp(commande, argv);
                //On arrive ici si et seuleument si le execvp c'est mal passé
                printf("Command not found");
            }
            else//On est dans le parent
            {
                int resultWait = 0;
                int status;
                //On ajoute le processus fils a la pile en tant que en cours d'execution
                if(nohup == false)
                    addProcess(&pile, EN_COURS_EXECUTION, pid_child, true);
                else
                {
                    addProcess(&pile, EN_COURS_EXECUTION, pid_child, false);
                    
                }
                //Si on est pas en parallèle on attend
                if(paralell == 0)
                {
                    inParent = true;//On signale que le parent attend
            
                    resultWait = wait(&status);

                    if(WIFSTOPPED(status)){
                        addProcess(&pileStop, ARRETE, pid_child, true);
                        (getProcess(&pile, pid_child))->etat = ARRETE;
                    }

                    inParent =false;//Le pareent reprend la main
                }
            }
            
        }

    }while(1);

    return 0;
}

int recupererCommande(char *commande, char* entree)
{
    int i = 0, j = 0;//variable avec laquelle on parcourera les tableaux
    int boolean = 1;
    
    while(boolean && entree[i] != '\0')
        {
            //Si l'utilisateur a mis des espaces avant sa commande on les traverse d'abord
            entree = jumpSpace(entree);

            commande[j] = entree[i];
            j++;
            i++;
            //On arrete la boucle dès qu'on trouve un espace
            if(entree[i] == ' ')
            {
                boolean = 0;
            }
        }
        //On ajoute le caractère de fin de chaine
        commande[i] = '\0';

        return i;
}