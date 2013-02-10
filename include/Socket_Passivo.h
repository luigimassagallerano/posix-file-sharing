#ifndef __SOCKET_PASSIVO_H__
#define __SOCKET_PASSIVO_H__

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <netdb.h>
#include <fcntl.h>
#include <sstream>

#include "Socket_Attivo.h"

using namespace std;

/**
*Classe che modella l'oggetto Socket_Passivo che servirà per mettersi in ascolto di Client.
*/
class Socket_Passivo
{
	struct sockaddr_in c_add;
	struct sockaddr_in my_addr;
	int base_sd, curr_sd;
	int addrlen;
	int port;
	
	
	public:
		/**
		*Costruttore: Crea un socket sulla porta specificata.
		*/
		Socket_Passivo(int porta);

		/**
		*Inizializza tutte le strutture.
		*Fa la bind e la listen e ritorna il descrittore al socket
		*/
		int init_sd(int port);

		/**
		*Metodo che ritorna un Socket_Attivo.
		*Fa l'accept() su curr_sd e crea con il risultato un Socket_Attivo.
		*@return Socket_Attivo sul quale comunicare.
		*/
		Socket_Attivo* accetta();

		/**
		*Per conoscere il proprio IP.
		*@return proprio IP.
		*/			
		string getIP();

		/**
		*Per conoscere la propria Porta.
		*@return propria Porta.
		*/	
		string getPorta();

		/**
		*Fa la close di base_sd
		*/
		void disconnetti();
		

};

#endif
