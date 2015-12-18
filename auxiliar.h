#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <stdbool.h>
#include <signal.h>
#define TRUE true 
#define FALSE false
/*Daniel Azevedo nº 2014200607
Nuno Afonso Santos nº 2014226541*/

typedef struct no_pedido *Lpedido;
typedef struct no_pedido{

    struct sockaddr_in dest;
    struct QUERY *Query_name;
    int sockfd;
    unsigned short id;
    Lpedido next;
}Lpedido_no;




struct QUERY
{
    unsigned char *name;
    struct QUESTION *ques;
};




struct DNS_HEADER
{
    unsigned short id; // identification number
 
    unsigned char rd :1; // recursion desired
    unsigned char tc :1; // truncated message
    unsigned char aa :1; // authoritive answer
    unsigned char opcode :4; // purpose of message
    unsigned char qr :1; // query/response flag
 
    unsigned char rcode :4; // response code
    unsigned char cd :1; // checking disabled
    unsigned char ad :1; // authenticated data
    unsigned char z :1; // its z! reserved
    unsigned char ra :1; // recursion available
 
    unsigned short q_count; // number of question entries
    unsigned short ans_count; // number of answer entries
    unsigned short auth_count; // number of authority entries
    unsigned short add_count; // number of resource entries
};
 
//Constant sized fields of query structure
struct QUESTION
{
    unsigned short qtype;
    unsigned short qclass;
};
 
//Constant sized fields of the resource record structure
#pragma pack(push, 1)
struct R_DATA
{
    unsigned short type;
    unsigned short _class;
    unsigned int ttl;
    unsigned short data_len;
};
#pragma pack(pop)
 
//Pointers to resource record contents
struct RES_RECORD
{
    unsigned char *name;
    struct R_DATA *resource;
    unsigned char *rdata;
};
 

typedef struct 
{
	char LocalDomain[20];
	char NamedPipe[20];
	int threads;
    bool flag[3];//[0] start manutencao, [1] stop manutencao, [2] start program
	char Domains[][30];


	
}Server_info;


typedef struct
{
    int dia;
    int mes;
    int ano;
    int segundo;
    int minuto;
    int hora;
}Data;

typedef struct{
	Data inicial;
	int pedidos_total;
 	int pedidos_recusados;
 	int d_local_res;
 	int d_externo_res;
 	Data atual;
	
}Info_Pipe;


Server_info *init();
void config(Server_info *);
void gestor_estatistica(clock_t,Server_info*);
void create_fifo(char*);
void read_pipe(Server_info *);
void write_pipe(Server_info *, Info_Pipe *);
Data sistema();
void gestor_pedidos( int ,char *[], char *, Info_Pipe*);
char* map_localdns();
void pool_threads(int);
void sendReply(unsigned short , unsigned char* , int , int , struct sockaddr_in );
u_char* convertRFC2Name(unsigned char*,unsigned char* ,int* );
void convertName2RFC(unsigned char*,unsigned char*);
char* verifica_ip(char*,char*);
char*  resolve_ip(char*, char*);
void func();
void manutencao();
void gestor_configuracao();
void cleanup(int);
void exit_processes(int);
void exit_threads(int );

Server_info *d;
Lpedido lprioritaria;
Lpedido lnormal;
int id_shm;
char *local;
Info_Pipe *info;
pid_t gc_pid,ge_pid, gp_pid;
sem_t *mutex;
pthread_t *p_threads;
size_t size;
char *p;



