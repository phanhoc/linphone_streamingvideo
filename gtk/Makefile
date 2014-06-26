all:
	rm -rf *~ sendfile requestim server
	gcc -Wall -g -o sendfile sendfile.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
	gcc -Wall -g -o requestim requestim.c `pkg-config --cflags --libs gtk+-2.0` -export-dynamic
	gcc server.c -o server
clean:
	rm -rf *~ sendfile requestim server