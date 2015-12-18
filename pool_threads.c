#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <pthread.h>
#include "auxiliar.h"
#include <arpa/inet.h>
#include  <netinet/in.h>
#include <signal.h>
/*Daniel Azevedo nº 2014200607
Nuno Afonso Santos nº 2014226541*/

void elimina_first(Lpedido lista){
    Lpedido ant=lista;
    Lpedido actual=lista->next;
    if (actual != NULL) {
        ant->next = actual->next;
        free (actual);
    }
    else
    	printf("ERROR elimina lista\n");
}


void* worker(void *i){
	signal(SIGINT, cleanup);
	signal(SIGALRM, func);
    signal(SIGUSR1, manutencao);
    signal(SIGTERM, SIG_IGN);
    signal(SIGUSR2,exit_threads);
	char *r=malloc(sizeof(char));
	char cmd[50];
	char aux_ip[50];
		
		while(1){
			sem_wait(mutex);
			Lpedido l=lprioritaria->next;
			Lpedido l2=lnormal->next;
			
			
			if(l!=NULL){
					r=resolve_ip((char*)l->Query_name->name,local);
					//ver se o dominio local esta no ficheiro
					if(r!=NULL){
						sendReply(l->id,l->Query_name->name, inet_addr(r),l->sockfd,l->dest);
						
						info->d_local_res++;
						elimina_first(lprioritaria);


					}
					else{
						printf("O dominio nao se encontra no localdns.txt\n");
						sendReply(l->id,l->Query_name->name, inet_addr("0.0.0.0"),l->sockfd, l->dest);
						elimina_first(lprioritaria);
						info->pedidos_recusados++;

					}

			}
			else if(l2!=NULL && d->flag[0]==FALSE){

					char *aux1=(char*)l2->Query_name->name;
					sprintf(cmd,"dig +short %s",aux1);
						
					FILE *dig;
					dig = popen (cmd, "r");
					if (!dig){
				     	 fprintf (stderr,"incorrect parameters or too many files.\n");
				     	 exit(1);
				    }

				    if(fscanf(dig,"%[^\n]",aux_ip)!=1){
						sendReply(l2->id,l2->Query_name->name,(int)(intptr_t)"0.0.0.0",l2->sockfd, l2->dest);
						elimina_first(lnormal);
						info->d_externo_res++;
					}
					else{
						sendReply(l2->id,l2->Query_name->name, inet_addr(aux_ip),l2->sockfd, l2->dest);
						info->d_externo_res++;
						elimina_first(lnormal);
					}
				    if (pclose (dig) != 0){
				      fprintf (stderr,"Could not run more or other error.\n");
				    
					}

					}
			
			info->atual=sistema();

	}
}


void pool_threads(int n_threads) {
  signal(SIGALRM, SIG_IGN);
  signal(SIGUSR1,SIG_IGN);
  signal(SIGINT, SIG_IGN);
  signal(SIGTERM,SIG_IGN);
  int pool[n_threads];
  int i;
  pthread_t my_thread[n_threads];
  p_threads=my_thread;

  #ifdef DEBUG
    printf("Criar as threads\n");
  #endif

  printf("A criar pool de threads...\n");
  for(i=0;i<n_threads;i++){
  		pool[i]=i;
  		if(pthread_create(&my_thread[i],NULL,worker,&pool[i])!=0){
  			perror("Error creating thread!");
  			exit(0);
  		}
		printf("Thread %d created\n",i);


		
	}



}
		

			

