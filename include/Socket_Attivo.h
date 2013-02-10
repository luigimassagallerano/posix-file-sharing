#ifndef __SOCKET_ATTIVO_H__
#define __SOCKET_ATTIVO_H__

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

/**
*Classe che modella l'oggetto Socket sul quale viaggeranno le comunicazioni.
*/
class Socket_Attivo
{
	int sd, s_port;
	struct sockaddr_in s_addr;
	struct hostent *server;
	
	public:
		/**
		*Costruttore tramite ip e porta.
		*Inizializza tutte le strutture tramite l'ip e la porta passati come parametri
		*@param ip Ip a cui connettere il Socket.
		*@param porta Porta a cui connettere il Socket.
		*/
		Socket_Attivo(const string& ip, int porta);

		/**
		*Costruttore tramite descrittore.
		*Setta l'intero sd dell'oggetto a quello passato per parametro.
		*Permette di creare un Socket_Attivo da dentro la classe Socket_Passivo.
		*@param sd descrittore di socket
		*/
		Socket_Attivo(int sd);

		/**
		*Fa la connect sul socket
		*/
		void connetti();

		/**
		*Fa la write di un messaggio sul socket.
		*@param mgs messaggio da scrivere sul socket.
		*/
		void invia(const string& mgs);

		/**
		*Fa la read sul socket.
		*/		
		string ricevi();

		/**
		*Riceve un file dal socket.
		*Crea il file.
		*Tramite un ciclo legge da socket e scrive su file.
		*Chiude il descrittore del file.
		*@param savepath percorso dove salvare il file.
		*/
		void ricevi_file(const string& savepath);

		/**
		*Invia un file sul socket.
		*Apre il File e tramite un ciclo ne fa la write sul socket.
		*@param path percorso del file da aprire.
		*/
		void invia_file(const string& path);

		/**
		*Fa la close di sd
		*/
		void disconnetti();
		

};

#endif
