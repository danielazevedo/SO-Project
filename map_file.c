#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "auxiliar.h"
#include <signal.h>
/*Daniel Azevedo nº 2014200607
Nuno Afonso Santos nº 2014226541*/

char* map_localdns(){


	struct stat st; 
	size_t size;
	int fd;
	char *p;

	
	
   fd=open("localdns.txt",O_RDONLY);


    if(fd==-1){
    	perror("open\n");
    	return (char*)1;
    }


    if(fstat (fd,&st)==-1){
    	perror("fstat");
    	return (char*)1;
    }
 
    size=st.st_size;


    p = mmap (0, size,PROT_READ,MAP_SHARED,fd, 0);
    if (p == MAP_FAILED) {
        perror ("mmap");
        return (char*)1;
    }



    if(close(fd)==-1){
    	perror("close");
    	return (char*)1;

    }


	return p;
	


}

