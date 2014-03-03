all: client.c server.c lib.c thread.c
	gcc -o server server.c lib.c
	gcc -o client client.c lib.c
	gcc -o if if.c
	gcc -o myif my_if.c
	gcc -o thread thread.c -lpthread
	gcc -o condition condition.c -lpthread
	gcc -o cancel cancel_thread.c -lpthread
my_service: _service_ip.c lib.c
	gcc -o service _service_ip.c lib.c -lpthread
clean:
	rm -rf server client if myif thread condition cancel service *~
