#include "commande.h"

char *jumpSpace(char *msg)
{
    int i =0;
    if(i == 0 && msg[i] == ' ')
        while(msg[i] == ' ')
        {
            i++;
            continue;
        }
    return msg+i;
}
int echo(char *str)
{
    printf("%s\n", str);
    return 0;
}

int cat(char *str)
{
    FILE *file = fopen(str, "r");
    //Si on ouvre pas le fichier, on arrete tout
    if(file == NULL)
    {
        printf("%s\n", str);
        exit(-1);
    }

    char msg;

    while((msg = fgetc(file)) != EOF)
        printf("%c", msg);
    printf("\n");
}

int ps(char *str, PileProcess *pile)
{
    getPile(pile);
}

void quit(PileProcess *pile)
{
    killAllProcessShell(pile);
}

void signalKill(PileProcess *pile, char *arg[2])
{
    char* endptr;
    int pid = strtol(arg[0], &endptr, 10);

    //Si
    if(endptr == arg[0])
    {
        printf("Pid invalide\n");
    }
    else
    {
        endptr+=1;
        endptr[7]='\0';
        if(strcmp(endptr, "SIGCONT") == 0)
            killProcess(pile, pid, EN_COURS_EXECUTION);
        else if(strcmp(endptr, "SIGTERM") == 0)
            killProcess(pile, pid, TERMINE);
        else if(strcmp(endptr, "SIGSTOP") == 0)
            killProcess(pile, pid, ARRETE);
        else
            printf("Signal inconnu\n");
    }
}

//lmbench