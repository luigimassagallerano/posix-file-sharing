#ifndef __INTERPETE_H__
#define __INTERPETE_H__

#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

/**
*Enumerazione di tutti i comandi che viaggiano sui Socket e che l'interprete deve riconoscere
*/
enum messaggi
{ 
	UNKNOWN_COMMAND	        = -1,	
	GET_FILE 		= 0 , 
	FILE_FOUND		= 1 ,
	FILE_NOT_FOUND          = 2 ,
	TRANSFER_COMPLETE       = 3 ,
	REGISTER_FILE           = 4 ,
	UNREGISTER_FILE		= 5 ,
	BYE			= 6 ,	
	PRENOTATION		= 7 ,
 
};

/**
*Si attende un messaggio così strutturato: COMANDO:NOMEFILE:IP:PORTA-
*@param x il messaggio da decodificare.
*@return ritorna l'IP della Locazione.
*/
string ricavaIPLocazione(string x);

/**
*Si attende un messaggio così strutturato: COMANDO:NOMEFILE:IP:PORTA-
*@param x il messaggio da decodificare.
*@return ritorna la PORTA della Locazione.
*/
string ricavaPortaLocazione(string x);

/**
*Si attende un messaggio così strutturato: COMANDO:IP:PORTA-
*@param x il messaggio da decodificare.
*@return ritorna l'IP della Locazione.
*/
string ricavaIP(string x);

/**
*Si attende un messaggio così strutturato: COMANDO:IP:PORTA-
*@param x il messaggio da decodificare.
*@return ritorna la PORTA della Locazione.
*/
string ricavaPorta(string x);

/**
*Ricava il comando dalla stringa in ingresso e ritorna il valore dell'enumerazione corrispondente.
*@param x il messaggio da decodificare.
*@return ritorna il Comando.
*/
int ricavaComando(string x);

/**
*Si attende un messaggio così strutturato: COMANDO:NOMEFILE:IP:PORTA-
*@param x il messaggio da decodificare.
*@return ritorna il nome del fil richiesto.
*/
string ricavaNomeFile(string x);

/**
*Si attende un messaggio così strutturato: COMANDO:NOMEFILE:IP:PORTA:MAXCLIENT-
*@param x il messaggio da decodificare.
*@return ritorna il numero maxClient.
*/
int ricavaMaxClient(string x);


#endif

