#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include "auxiliar.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
/*Daniel Azevedo nº 2014200607
Nuno Afonso Santos nº 2014226541*/


void insere_lpedidos(Lpedido lista,struct QUERY dominio,unsigned short id,int sockfd, struct sockaddr_in dest){
	Lpedido no;
    no = (Lpedido) malloc (sizeof (Lpedido_no));
    no->Query_name=malloc(sizeof(struct QUERY*));
	if (no != NULL){

        while(lista->next!=NULL)
            lista=lista->next;
         
        memcpy(no->Query_name,&dominio,sizeof(struct QUERY));
        no->dest=dest;
        no->id=id;
        no->sockfd=sockfd;
        no->next=NULL;
        lista->next=no;
    }
}





char* verifica_ip(char* query,char* linha){
	int i=0;
	int a=0;
	char *ip=malloc(sizeof(char));
	if(strstr(linha,query)!=NULL){
		while(linha[i]!=' '){
			i++;
		}
		i++;
		while(linha[i]!='\0'){
			ip[a]=linha[i];
			a++;
			i++;
		}
		ip[a-1]='\0';
		return ip;
	}
	return NULL;
}




char* resolve_ip(char* t, char* local){
	char linha[30];
	char x=local[0];
	int p=0;
	int aux=0;
	while(x!='\0'){
		
		while(x!='\n'){
			x=local[p];
			linha[p-aux]=local[p];
			//printf("%c",x);
			p++;
		}
		linha[p-aux]='\0';
		if(verifica_ip(t,linha)!=NULL){
			const char *v=malloc(sizeof(char));
			v=verifica_ip(t,linha);
			return (char*)v;
		}
		linha[0]='\0';
		aux=p;
		x=local[p];


	}
	return NULL;
}


int verifica_validade(char tabela[][30], char* dominio){
	int i=0;
	while(strcmp(tabela[i],"\0")!=0){

		if(strstr(dominio,tabela[i])!=NULL){
			return 1;

		}
		i++;
		
	}

	return 0;
	
}

void sendReply(unsigned short id, unsigned char* query, int ip_addr, int sockfd, struct sockaddr_in dest) {
		unsigned char bufReply[65536], *rname;
		char *rip;
		struct R_DATA *rinfo = NULL;
		
		//Set the DNS structure to reply (according to the RFC)
		struct DNS_HEADER *rdns = NULL;
		rdns = (struct DNS_HEADER *)&bufReply;
		rdns->id = id;
		rdns->qr = 1;
		rdns->opcode = 0;
		rdns->aa = 1;
		rdns->tc = 0;
		rdns->rd = 0;
		rdns->ra = 0;
		rdns->z = 0;
		rdns->ad = 0;
		rdns->cd = 0;
		rdns->rcode = 0;
		rdns->q_count = 0;
		rdns->ans_count = htons(1);
		rdns->auth_count = 0;
		rdns->add_count = 0;
		
		// Add the QUERY name (the same as the query received)
		rname = (unsigned char*)&bufReply[sizeof(struct DNS_HEADER)];
		convertName2RFC(rname , query);
		
		// Add the reply structure (according to the RFC)
		rinfo = (struct R_DATA*)&bufReply[sizeof(struct DNS_HEADER) + (strlen((const char*)rname)+1)];
		rinfo->type = htons(1);
		rinfo->_class = htons(1);
		rinfo->ttl = htonl(3600);
		rinfo->data_len = htons(sizeof(ip_addr)); // Size of the reply IP address

		// Add the reply IP address for the query name 
		rip = (char *)&bufReply[sizeof(struct DNS_HEADER) + (strlen((const char*)rname)+1) + sizeof(struct R_DATA)];
		memcpy(rip, (struct in_addr *) &ip_addr, sizeof(ip_addr));
		
		// Send DNS reply
		printf("\nSending Answer... ");
		if( sendto(sockfd, (char*)bufReply, sizeof(struct DNS_HEADER) + (strlen((const char*)rname) + 1) + sizeof(struct R_DATA) + sizeof(ip_addr),0,(struct sockaddr*)&dest,sizeof(dest)) < 0) {
			printf("FAILED!!\n");
		} else {
			printf("SENT!!!\n");
		}
}

/**
	convertRFC2Name: converts DNS RFC name to name
**/
u_char* convertRFC2Name(unsigned char* reader,unsigned char* buffer,int* count) {
    unsigned char *name;
    unsigned int p=0,jumped=0,offset;
    int i , j;
    *count = 1;
    name = (unsigned char*)malloc(256);
 
    name[0]='\0';
 
    while(*reader!=0) {
        if(*reader>=192) {
            offset = (*reader)*256 + *(reader+1) - 49152;
            reader = buffer + offset - 1;
            jumped = 1;
        } else {
            name[p++]=*reader;
        }
 
        reader = reader+1;
 
        if(jumped==0) {
            *count = *count + 1;
        }
    }
 
    name[p]='\0';
    if(jumped==1) {
        *count = *count + 1;
    }
 
    for(i=0;i<(int)strlen((const char*)name);i++) {
        p=name[i];
        for(j=0;j<(int)p;j++) {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
    name[i-1]='\0';
    return name;
}

/**
	convertName2RFC: converts name to DNS RFC name
**/
void convertName2RFC(unsigned char* dns,unsigned char* host) {
    int lock = 0 , i;
    strcat((char*)host,".");
     
    for(i = 0 ; i < strlen((char*)host) ; i++) {
        if(host[i]=='.') {
            *dns++ = i-lock;
            for(;lock<i;lock++) {
                *dns++=host[lock];
            }
            lock++;
        }
    }
    *dns++='\0';
}

void gestor_pedidos( int argc , char *argv[], char *local, Info_Pipe *info){
	
	signal(SIGINT, cleanup);
	signal(SIGALRM,SIG_IGN);
	signal(SIGTERM,exit_processes);
	signal(SIGUSR1,manutencao);
	char *nome;
	char* c;
	int y;
	
	
	printf("A enviar para o named pipe...\n\n");
		write_pipe(d,info);
	

	lprioritaria= (Lpedido)malloc(sizeof(Lpedido_no));
	lprioritaria->next=NULL;
	

	lnormal= (Lpedido)malloc(sizeof(Lpedido_no));
	lnormal->next=NULL;

	

	unsigned char buf[65536], *reader;
	int sockfd, stop;
	struct DNS_HEADER *dns = NULL;
	
	struct sockaddr_in servaddr,dest;
	socklen_t len;
	
	// Check arguments
	if(argc <= 1) {
		printf("Usage: dnsserver <port>\n");
		exit(1);
	}
	
	// Get server UDP port number
	int port = atoi(argv[1]);
	
	if(port <= 0) {
		printf("Usage: dnsserver <port>\n");
		exit(1);
	}




	// ****************************************
	// Create socket & bind
	// ****************************************
	
	// Create UDP socket
    sockfd = socket(AF_INET , SOCK_DGRAM , IPPROTO_UDP); //UDP packet for DNS queries
 
	if (sockfd < 0) {
         printf("ERROR opening socket.\n");
		 exit(1);
	}

	// Prepare UDP to bind port
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(port);
	
	// Bind application to UDP port
	int res = bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	
	if(res < 0) {
         printf("Error binding to port %d.\n", servaddr.sin_port);
		 
		 if(servaddr.sin_port <= 1024) {
			 printf("To use ports below 1024 you may need additional permitions. Try to use a port higher than 1024.\n");
		 } else {
			 printf("Please make sure this UDP port is not being used.\n");
		 }
		 exit(1);
	}
	
	// ****************************************
	// Receive questions
	// ****************************************
	
	while(1) {
		// Receive questions
		sleep(1);
		len = sizeof(dest);
		printf("\n\n-- Wating for DNS message --\n\n");
		if(recvfrom (sockfd,(char*)buf , 65536 , 0 , (struct sockaddr*)&dest , &len) < 0) {
			printf("Error while waiting for DNS message. Exiting...\n");
			exit(1);
		}
		
		printf("DNS message received\n");
	 
		// Process received message
		dns = (struct DNS_HEADER*) buf;
		//qname =(unsigned char*)&buf[sizeof(struct DNS_HEADER)];
		reader = &buf[sizeof(struct DNS_HEADER)];
	 
		printf("\nThe query %d contains: ", ntohs(dns->id));
		printf("\n %d Questions.",ntohs(dns->q_count));
		printf("\n %d Answers.",ntohs(dns->ans_count));
		printf("\n %d Authoritative Servers.",ntohs(dns->auth_count));
		printf("\n %d Additional records.\n\n",ntohs(dns->add_count));
		
		// We only need to process the questions
		// We only process DNS messages with one question
		// Get the query fields according to the RFC specification
		struct QUERY query;

		if(ntohs(dns->q_count) == 1) {
			// Get NAME
			query.name = convertRFC2Name(reader,buf,&stop);
			reader = reader + stop;
			
			// Get QUESTION structure
			query.ques = (struct QUESTION*)(reader);
			reader = reader + sizeof(struct QUESTION);
			
			// Check question type. We only need to process A records.
			if(ntohs(query.ques->qtype) == 1) {
				printf("A record request.\n\n");
			} else {
				printf("NOT A record request!! Ignoring DNS message!\n");
				continue;
			}
			
		} else {
			printf("\n\nDNS message must contain one question!! Ignoring DNS message!\n\n");
			continue;
		}
		
		// Received DNS message fulfills all requirements.
		
		
		// ****************************************
		// Print received DNS message QUERY
		// ****************************************
		printf(">> QUERY: %s\n", query.name);
		printf(">> Type (A): %d\n", ntohs(query.ques->qtype));
		printf(">> Class (IN): %d\n\n", ntohs(query.ques->qclass));
			
		// ****************************************
		// Example reply to the received QUERY
		// (Currently replying 10.0.0.2 to all QUERY names)
		// ****************************************
		
		
		#ifdef DEBUG
    		printf("Verificar se o domino e local\n");
   	 	#endif

		

		nome=(char *)query.name;
		c=strstr(nome,d->LocalDomain);
		if(c!=NULL){
			printf("%s e um dominio local\n",nome);
			insere_lpedidos(lprioritaria,query,dns->id,sockfd,dest);
			sem_post(mutex);

		}



		else{
			#ifdef DEBUG
			    printf("Verificar se o domino esta no config.txt ou nao\n");
		    #endif

			if(d->flag[0]==TRUE && d->flag[1]==FALSE){
				printf("Em modo de manutencao, opcao indisponivel\n");
				sendReply(dns->id, query.name, inet_addr("0.0.0.0"), sockfd, dest);
				info->pedidos_recusados++;
			}
			else{
				
				if((y=verifica_validade(d->Domains,nome))==0){
					printf("Dominio nao e valido\n");
					sendReply(dns->id, query.name, inet_addr("0.0.0.0"), sockfd, dest);
					info->atual=sistema();
					info->pedidos_recusados++;
				}
				else{
					printf("Dominio e externo\n");
					insere_lpedidos(lnormal,query,dns->id,sockfd,dest);
					sem_post(mutex);
					}
			
			}

			
		}
			info->pedidos_total++;


	


		

	}
}


	




	









