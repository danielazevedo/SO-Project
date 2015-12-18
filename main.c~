#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "auxiliar.h"
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>

#define BUFF 1024
/*Daniel Azevedo nº 2014200607
Nuno Afonso Santos nº 2014226541*/


void create_fifo(char* nome){
	signal(SIGALRM, SIG_IGN);
	signal(SIGUSR1,SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM,SIG_IGN);
	
	if ((mkfifo(nome, O_CREAT|O_EXCL|0600)<0) && (errno!= EEXIST)){
			perror("Cannot create pipe: ");
			exit(1);
	}
	//0666 permite a leitura e escrita para todos
	

}



Data sistema(){
	signal(SIGALRM, func);
	signal(SIGUSR1,manutencao);
	signal(SIGINT, cleanup);
	signal(SIGTERM,SIG_IGN);
	Data data;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    data.dia=tm.tm_mday;
    data.mes=tm.tm_mon +1;
    data.ano=tm.tm_year+1900;
    data.hora=tm.tm_hour;
    data.minuto=tm.tm_min;
    data.segundo=tm.tm_sec;
    return data;
	
}

void read_pipe(Server_info *d){
	signal(SIGALRM,SIG_IGN);
	signal(SIGUSR1,SIG_IGN);
	signal(SIGINT,cleanup);
	signal(SIGTERM,SIG_IGN);
	char buffer[BUFF];
	char *aux=d->NamedPipe;
	int s=open(aux,O_RDONLY);
	read(s,buffer,BUFF);
	printf("%s\n",buffer);
	close(s);
}

void write_pipe(Server_info *d, Info_Pipe *info){
	char buf[BUFF];
	int s;
	char *aux=d->NamedPipe;
	s= open(aux, O_WRONLY);
	sprintf(buf,"%.2d:%.2d\n%d\n%d\n%d\n%d\n%d/%d/%d %.2d:%.2d",info->inicial.hora,info->inicial.minuto,info->pedidos_total,info->pedidos_recusados,info->d_local_res,info->d_externo_res,info->atual.dia,info->atual.mes,info->atual.ano,info->atual.hora,info->atual.minuto);

	write(s,buf,sizeof(buf));
	close(s);

}

void exit_processes(int sig){
	signal(SIGTERM,exit_processes);
	signal(SIGINT,SIG_IGN);
	signal(SIGALRM,SIG_IGN);
    signal(SIGUSR1,SIG_IGN);


    exit(0);
}

void manutencao(){
	signal(SIGUSR1,manutencao);
	signal(SIGALRM,func);
	if(d->flag[0]==FALSE){
		d->flag[0]=TRUE;
		printf("Em modo de manutencao\n");
	}
	else{
		d->flag[0]=FALSE;
		d->flag[1]=TRUE;
		printf("Modo de manutencao terminou\n");
	}
}

void cleanup(int sig) {
	signal(SIGINT,cleanup);
	signal(SIGTERM,SIG_IGN);
	signal(SIGALRM,SIG_IGN);
    signal(SIGUSR1,SIG_IGN);
    kill(ge_pid,SIGTERM);
    kill(gc_pid,SIGTERM);
	int i;
	
    printf("\n");
	for(i=0; i < d->threads; i++){
		pthread_kill(p_threads[i], SIGUSR2);
	}
   for(i=0;i<d->threads;i++){
    	printf("Thread [%d] joined!\n",i);
		pthread_join(p_threads[i],NULL);
	}
	printf("\n");
     while(wait(NULL) != -1){
		printf("Child process terminated\n");
    }
    sem_close(mutex);
    unlink("mutex");
    unlink(d->NamedPipe);

    munmap(local,size);


    shmdt(d);
    shmctl(id_shm, IPC_RMID,NULL);
   
    printf("\nServer shutting down...\n\n");
    exit(0);

}
void exit_threads(int sig){
   	pthread_exit(NULL);
}



Info_Pipe *ini_info(){
	Info_Pipe *info=malloc(sizeof(Info_Pipe));
	info->inicial=sistema();
	info->atual=sistema();
	info->pedidos_total=0;
 	info->pedidos_recusados=0;
 	info->d_local_res=0;
 	info->d_externo_res=0;
 	return info;
}




int main( int argc , char *argv[]){
	
	clock_t inicial=clock()/ CLOCKS_PER_SEC; 
	info=ini_info();
	signal(SIGALRM, SIG_IGN);
	signal(SIGUSR1,SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM,SIG_IGN);


	local=malloc(sizeof(char));
	printf("A criar memoria partilhada...\n\n");

	#ifdef DEBUG
    printf("Criar a memoria partilhada\n");
    #endif

	if ((id_shm=shmget(IPC_PRIVATE,sizeof(Server_info), IPC_CREAT|0777))==-1){
		    perror("Error creating shared memory");
        	exit(1);
        }
     printf("A mapear memoria partilhada...\n\n");
	//Mapear memoria partilhada

	#ifdef DEBUG
    	printf("Mapear memoria partilhada\n");
    #endif

    d=shmat(id_shm,NULL,0);
    if (d==(Server_info*)-1){
            perror("Error Attaching memory\n");
        }
   	
    //Criar mutex
	sem_unlink("MUTEX");
	mutex=sem_open("MUTEX",O_CREAT|O_EXCL, 0700, 0);



	//Inicializar as flags
	d->flag[0]=FALSE;
	d->flag[1]=TRUE;
	d->flag[2]=TRUE;

	#ifdef DEBUG
    	printf("Mapear em memoria o ficheiro localdns.txt\n");
    #endif

	printf("A mapear em memoria o ficheiro de nomes/enderecos ip...\n\n");
    local=map_localdns();//gp
    printf("%s\n",local);
    printf("A criar processos...\n");
	
    
	if(fork()==0){
			//GESTOR DE CONFIGURACAO
			gc_pid=getpid();
			gestor_configuracao();
			exit(1);
		
	}


	else{
		//GESTOR DE PEDIDOS
		gp_pid=getpid();
		printf("PID do processo pai: %d\n\n",gp_pid);
		sleep(2);
		signal(SIGALRM,func);
		signal(SIGUSR1,manutencao);
		signal(SIGINT,cleanup);
		#ifdef DEBUG
    		printf("Criar named pipe\n");
	    #endif

		printf("A criar o named pipe...\n\n");
			create_fifo(d->NamedPipe);
		
		if(fork()!=0){
			alarm(30);
			pool_threads(d->threads);
			gestor_pedidos(argc,argv,local,info);
			
		
		}

	

		else{
			//GESTOR DE ESTATISTICAS
			ge_pid=getpid();
			sleep(2);
			gestor_estatistica(inicial,d);
			exit(1);

		}
		
	}


	return 0;
	
}

