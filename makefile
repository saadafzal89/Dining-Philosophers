all:	philosopher host

philosopher: philosopher.c
	gcc -pthread -o philosopher philosopher.c smp.c
host: host.c
	gcc -pthread -o host host.c smp.c
clean:
	rm -rf philosopher host barrier global_lock counter sem* status*
