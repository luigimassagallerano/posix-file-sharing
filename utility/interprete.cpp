#include "../include/interprete.h"

#define SEP ":-"

int ricavaComando(string x)
{
	char buf[x.size()];
	char *saveptr = NULL;
	
	memcpy(buf, x.c_str(), strlen(x.c_str()));

	//string comando = strtok(buf,SEP); //COMANDO
	string comando = strtok_r(buf, SEP, &saveptr);

	if(comando.compare("GET_FILE") == 0) return GET_FILE;
	if(comando.compare("FILE_FOUND") == 0) return FILE_FOUND;
	if(comando.compare("FILE_NOT_FOUND") == 0) return FILE_NOT_FOUND;
	if(comando.compare("TRANSFER_COMPLETE") == 0) return TRANSFER_COMPLETE;
	if(comando.compare("REGISTER_FILE") == 0) return REGISTER_FILE;
	if(comando.compare("UNREGISTER_FILE") == 0) return UNREGISTER_FILE;
	if(comando.compare("BYE") == 0) return BYE;
	if(comando.compare("PRENOTATION") == 0) return PRENOTATION;
	return UNKNOWN_COMMAND;
}

string ricavaNomeFile(string x) // COMANDO:NOMEFILE:IP:PORTA-
{
	char buf[x.size()];
	char *saveptr = NULL;
	
	memcpy(buf, x.c_str(), strlen(x.c_str()));

	string comando = strtok_r(buf, SEP, &saveptr); //COMANDO
	string nf = strtok_r(NULL, SEP, &saveptr); //NOME FILE
	
	return nf;
}


string ricavaIP(string x) // COMANDO:IP:PORTA-
{
	char buf[x.size()];
	char *saveptr = NULL;
	
	memcpy(buf, x.c_str(), strlen(x.c_str()));

	string comando = strtok_r(buf, SEP, &saveptr); //COMANDO
	string ip = strtok_r(NULL, SEP, &saveptr); //IP
	
	return ip;
}

string ricavaPorta(string x) // COMANDO:IP:PORTA-
{
	char buf[x.size()];
	char *saveptr = NULL;
	
	memcpy(buf, x.c_str(), strlen(x.c_str()));
	
	strtok_r(buf, SEP, &saveptr); //COMANDO
	strtok_r(NULL, SEP, &saveptr); //IP
	string porta = strtok_r(NULL, SEP, &saveptr); //PORTA

	return porta;
}

string ricavaIPLocazione(string x)  // COMANDO:NOMEFILE:IP:PORTA-
{
	char buf[x.size()];
	char *saveptr = NULL;
	
	memcpy(buf, x.c_str(), strlen(x.c_str()));
	
	strtok_r(buf, SEP, &saveptr); //COMANDO
	strtok_r(NULL, SEP, &saveptr); //NOME FILE
	string ip = strtok_r(NULL, SEP, &saveptr); //IP
	
	return ip;
}

string ricavaPortaLocazione(string x) // COMANDO:NOMEFILE:IP:PORTA-
{
	char buf[x.size()];
	char *saveptr = NULL;
	
	memcpy(buf, x.c_str(), strlen(x.c_str()));
	
	strtok_r(buf, SEP, &saveptr); //COMANDO
	strtok_r(NULL, SEP, &saveptr); //NOME FILE
	strtok_r(NULL, SEP, &saveptr); //IP
	string porta = strtok_r(NULL, SEP, &saveptr); //PORTA
	return porta;
}

int ricavaMaxClient(string x) // COMANDO:NOMEFILE:IP:PORTA:MAXCLIENT-
{
	char buf[x.size()];
	char *saveptr = NULL;
	
	memcpy(buf, x.c_str(), strlen(x.c_str()));
	
	strtok_r(buf, SEP, &saveptr); //COMANDO
	strtok_r(NULL, SEP, &saveptr); //NOME FILE
	strtok_r(NULL, SEP, &saveptr); //IP
	strtok_r(NULL, SEP, &saveptr); //PORTA
	int maxClient = atoi(strtok_r(NULL, SEP, &saveptr)); //MAXCLIENT
	return maxClient;
}



