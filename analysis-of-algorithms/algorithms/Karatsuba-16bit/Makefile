OPT= -O2

all: main16

main16: main16.o scaffold16.o myutil.o
	gcc $(OPT) -g -o main16 main16.o scaffold16.o myutil.o -lgmp -lrt

main16.o: main16.c scaffold16.h
	gcc $(OPT) -g -c main16.c

scaffold16.o: scaffold16.c scaffold16.h
	gcc $(OPT) -g -c scaffold16.c

myutil.o: myutil.c
	gcc $(OPT) -c myutil.c

clean: 
	rm *.o main16

# note! doesn't include your report file, if any
turnin: 
	echo Making `whoami`_`date '+%y-%m-%d'`.tar.gz 
	tar czvf `whoami`_`date '+%Y-%m-%d'`.tar.gz *.c *.h Makefile 
	
tar.gz: 
	tar czvf project_scaffold.tar.gz *.c *.h Makefile Readme
#	tar czvf project_scaffold_`date '+%Y-%m-%d'`.tar.gz *.c *.h Makefile Readme
	chmod 644 project_scaffold.tar.gz
	
