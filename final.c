#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <signal.h>

#define CHECK(sts,msg) if ((sts) == -1) { perror(msg); exit(-1); }

pid_t pid;

void entrerMdp(){
	int i =1;
	char str[15];	
	
	printf("Entrez votre mot de passe : \n");
	while(1){
		
		scanf("%s", str);
		if (strcmp(str, "mdp") == 0)
		{
			kill(getppid(),SIGUSR2);
			exit(0);
		}
		i++;
		if (i > 3)
		{
			//printf("Trop de tentatives\n");
			kill(getppid(),SIGUSR1);
			exit(1);
		}

	}
		
}

void deroute(int sig) 
{ 
    switch(sig){
    	case SIGUSR1 : 
    		printf("Trop de tentatives\n");
			break;
	case SIGUSR2 : 
    		printf("Mdp correct\n");
			break;
	case SIGALRM : 
    		printf("Temps écouler\n");
    		if(pid != 0){
    			kill(pid,SIGKILL);
    		}
			break;
	case SIGINT : 
    		printf("CTRL+C interdit\n");
			break;
    } 
} 

int main(){

	pid_t pidfils;
	int wstatus;
	
	
	struct sigaction newact;
	
	
	newact.sa_handler = deroute;
	newact.sa_flags = SA_RESTART;
	
	CHECK(sigemptyset(&newact.sa_mask), "problème sigemptyset"); 
	
	
	CHECK(sigaction(SIGINT, &newact, NULL), "probleme avec sigaction");
	CHECK(sigaction(SIGALRM, &newact, NULL), "problème avec sigaction");
	CHECK(sigaction(SIGUSR1, &newact, NULL), "problème avec sigaction");
	CHECK(sigaction(SIGUSR2, &newact, NULL), "problème avec sigaction");
	
	
	
	CHECK(pid = fork(), "Erreur FORK");
	if (pid == 0)
	{
		entrerMdp();
		exit(0);
	}

	alarm(60);
	
	CHECK(pidfils = wait(&wstatus), "ERREUR wait");
	




	exit(wstatus);
}

