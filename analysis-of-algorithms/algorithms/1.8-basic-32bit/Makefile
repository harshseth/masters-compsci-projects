OPT= -O2

all: main32

main32: main32.o scaffold32.o myutil.o
	gcc $(OPT) -g -o main32 main32.o scaffold32.o myutil.o -lgmp -lrt

main32.o: main32.c scaffold32.h
	gcc $(OPT) -g -c main32.c

scaffold32.o: scaffold32.c scaffold32.h
	gcc $(OPT) -g -c scaffold32.c

myutil.o: myutil.c
	gcc $(OPT) -c myutil.c

clean: 
	rm *.o main32

# note! doesn't include your report file, if any
turnin: 
	echo Making `whoami`_`date '+%y-%m-%d'`.tar.gz 
	tar czvf `whoami`_`date '+%Y-%m-%d'`.tar.gz *.c *.h Makefile 
	
tar.gz: 
	tar czvf project_scaffold.tar.gz *.c *.h Makefile Readme
#	tar czvf project_scaffold_`date '+%Y-%m-%d'`.tar.gz *.c *.h Makefile Readme
	chmod 644 project_scaffold.tar.gz
	
