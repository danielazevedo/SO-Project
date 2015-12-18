main: main.o map_file.o gestor_estatisticas.o gestor_pedidos.o pool_threads.o gestor_configuracao.o
	gcc -o main -pthread -D_REENTRANT main.o map_file.o gestor_estatisticas.o gestor_pedidos.o pool_threads.o gestor_configuracao.o
main.o: main.c auxiliar.h
	gcc -c main.c -Wall
map_file.o: map_file.c auxiliar.h
	gcc -c map_file.c -Wall
ler_config.o:ler_config.c auxiliar.h
	gcc -c ler_config.c -Wall
gestor_estatisticas.o: gestor_estatisticas.c auxiliar.h
	gcc -c gestor_estatisticas.c -Wall
gestor_pedidos.o: gestor_pedidos.c auxiliar.h
	gcc -c gestor_pedidos.c -Wall
pool_threads.o: pool_threads.c auxiliar.h
	gcc -c pool_threads.c -Wall
gestor_configuracao.o: gestor_configuracao.c auxiliar.h
	gcc -c gestor_configuracao.c -Wall
