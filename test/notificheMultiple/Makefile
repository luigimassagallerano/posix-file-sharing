dfr: lanciaDFR.o 
	g++ lanciaDFR.cpp ../../socket/Socket_Passivo.o ../../socket/Socket_Attivo.o ../../utility/msg_err.o ../../utility/interprete.o ../../strutture/Repository.o ../../strutture/File_Condition.o ../../strutture/File_Location.o ../../strutture/Location.o ../../strutture/Lista_Prenotazioni.o ../../src_DFR/DFR.o -lpthread -o dfr 

clean :
	-@rm *.o dfr

