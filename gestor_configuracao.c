#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include "auxiliar.h"
#include <signal.h>
/*Daniel Azevedo nº 2014200607
Nuno Afonso Santos nº 2014226541*/


void gestor_configuracao(){
	signal(SIGALRM,SIG_IGN);
	signal(SIGINT, cleanup);
	signal(SIGTERM,exit_processes);
	signal(SIGUSR1,SIG_IGN);

	#ifdef DEBUG
	    printf("Ler o ficheiro config.txt para memoria partilhada\n");
    #endif

	printf("A ler o ficheiro config.txt para memoria partilhada...\n\n");
		while(1){
			if(d->flag[0]==FALSE && d->flag[1]==TRUE){
				config(d);//gc
				d->flag[1]=FALSE;
			}
		}

}



void config(Server_info *dominio){
	signal(SIGALRM,SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGUSR2,SIG_IGN);
	signal(SIGUSR1,manutencao);


	char *aux=malloc(sizeof(char));
	if(aux==NULL){
		printf("Erro na alocaçao da memoria\n");
		exit(1);
	}

	
	int k=0,j=0,c=0,a=0;
	char str1[20][20];
	FILE *f= fopen("config.txt","r");

	if(d->flag[2]==TRUE){
		fscanf(f,"Threads = %d\nDomains = ",&dominio->threads);
		fgets(aux,75,f);
		fscanf(f,"LocalDomain = %s\n",dominio->LocalDomain);
		fscanf(f,"NamedPipeEstatisticas = %s",dominio->NamedPipe);
		

		d->flag[2]=FALSE;
	   
	}
	else{
		//a thread_aux e o namedpipe_aux é descartavel
		int threads_aux;
		char *namedpipe_aux=NULL;
		fscanf(f,"Threads = %d\nDomains = ",&threads_aux);
		fgets(aux,75,f);
		fscanf(f,"LocalDomain = %s\n",dominio->LocalDomain);
		fscanf(f,"NamedPipeEstatisticas = %s",namedpipe_aux);
	}

		fclose(f);
		aux[strlen(aux)-1]='\0';
		while(k<strlen(aux)){
			if(aux[k]==';'){
				str1[c][j]='\0';
				strcpy(dominio->Domains[a],str1[c]);
				j=-2;
				a++;
				c++;
			}
			else
				str1[c][j]=aux[k];
			j++;
			k++;
		}
		str1[c][j]='\0';
		strcpy(dominio->Domains[a],str1[c]);
		strcpy(dominio->Domains[a+1],"\0");


}