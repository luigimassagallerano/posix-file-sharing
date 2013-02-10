
all: fs1 fs2

fs1: lanciaFS1.o 
	g++ lanciaFS1.cpp ../../../src_FS/FS.o ../../../socket/Socket_Passivo.o ../../../socket/Socket_Attivo.o ../../../utility/msg_err.o ../../../utility/interprete.o -lpthread -o fs1

fs2: lanciaFS2.o 
	g++ lanciaFS2.cpp ../../../src_FS/FS.o ../../../socket/Socket_Passivo.o ../../../socket/Socket_Attivo.o ../../../utility/msg_err.o ../../../utility/interprete.o -lpthread -o fs2

clean :
	-@rm *.o fs1 fs2

