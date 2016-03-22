#CC=gcc
all:server client
#rw.o:rw.c rw.h
#	$(CC) -c rw.c -o rw.o
#server.o:server.c rw.h
#	$(CC)  -c -g server.c rw.h 
#client.o:fileclient.c rw.h
#	$(CC)  -c -g client.c rw.h 
#
#server:server.o rw.o
#	$(CC)  -o server rw.o server.o -o  -lpthread
#client:client.o rw.o
#	$(CC)  -o client rw.o client.o
server:server.c login_cmd_message.c rwdir_file.c rwfile.c func.c rw.h
	gcc -g $^ -o $@
client:client.c login_cmd_message.c rwdir_file.c rwfile.c func.c rw.h
	gcc -g $^ -o $@
clean:
	rm server client 
