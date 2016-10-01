#Logger library	

.PHONY: clean 

liblogger.so :  logmngr.o read_config.o zlog.o 
	gcc -shared  -g -Wl,-soname,liblogger.so -o liblogger.so logmngr.o read_config.o zlog.o -L./lib -lds -lm

logmngr.o : src/logmngr.c inc/logmngr.h inc/zlog.h inc/read_config.h inc/ADTDefs.h
	gcc -c -g src/logmngr.c -fPIC

read_config.o : inc/read_config.h inc/HashMap.h inc/ADTDefs.h
	gcc -c -g src/read_config.c -fPIC

zlog.o : src/zlog.c inc/zlog.h inc/HashMap.h inc/read_config.h
	gcc -c -g src/zlog.c -fPIC

clean:
	rm -f *.o
