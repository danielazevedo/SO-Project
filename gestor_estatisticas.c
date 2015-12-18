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
#include <signal.h>
/*Daniel Azevedo nº 2014200607
Nuno Afonso Santos nº 2014226541*/

void func(){
	
	write_pipe(d,info);
	alarm(30);
}



void gestor_estatistica(clock_t inicial,Server_info* d){
	signal(SIGALRM,SIG_IGN);
	signal(SIGINT, cleanup);
	signal(SIGTERM,exit_processes);
	signal(SIGUSR1,SIG_IGN);
	while(1){
		read_pipe(d);
	}

	


}