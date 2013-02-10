#ifndef __FS_H__
#define __FS_H__

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "msg_err.h"
#include "Socket_Passivo.h"
#include "Socket_Attivo.h"
#include "interprete.h"

/**
*Classe che modella l'oggetto File Server.
*L'oggetto FS mette a disposizione una serie di metodi per interagire con il DFR e con i Client
*/
class FS{
	
	Socket_Passivo* sp; /**< Puntatore ad un Socket_Passivo utilizzato per ascoltare i Client */
	Socket_Attivo* sa; /**< Puntatore ad un Socket_Attivo utilizzato per contattare il DFR */

	pthread_t clth; /**< Thread che si occuperà di gestire le richieste Client */

	int args[2]; /**< array di puntatori per passare le strutture al metodo statico dei thread */

	/**
	*Metodo che ciclicamente di mette in ascolto di Client e crea un Socket_Attivo e un Thread per ognuno di loro.
	*Al thread creato passa il puntatore a servi_client()
	*/
	static void *listen_client(void *sp);

	/**
	*Metodo che si occupa di servire le richieste client.
	*Attende il comando dai CLient, riconosce che file è stato richiesto e lo invia al Client.
	*/
	static void *servi_client(void *sa);

	/**
	*Metodo che controlla se il file richiesto esiste e in caso positivo lo invia al Client.
	*@param file_name nome del file richiesto.
	*@param sa puntatore al Socket sul quale inviare il file richiesto.
	*/
	void start_file_transfer(const string& file_name, Socket_Attivo* sa);

	public:
		/**
		*Costruttore senza parametri.
		*Richiede di inserire la porta su cui ascoltare i Client tramite Console.
		*Inizializza le strutture e crea il Thread che si mette in ascolto dei Client.
		*/
		FS();

		/**
		*Costruttore con parametro porta.
		*Crea un Socket_Passivo in ascolta dei Client sulla porta specificata.
		*Inizializza le strutture e crea il Thread che si mette in ascolto dei Client.
		*/
		FS(int porta);

		/**
		*Distruttore: cancella i puntatori sa e sp
		*/
		~FS();
		
		/**
		*Versione senza parametri.
		*Chiede di inserire IP e Porta del DFR tramite Console.
		*Instanzia il Socket_Attivo e si connette al DFR.
		*/
		void collega_al_server();

		/**
		*Versione con IP e Porta passati come parametri.
		*Instanzia il Socket_Attivo e si connette al DFR.
		*@param ip IP del DFR.
		*@param porta porta del DFR.
		*/	
		void collega_al_server(const string& ip, int porta);

		/**
		*Permette di registrare un File sul DFR.
		*Invia un messaggio così strutturato: REGISTER_FILE:filename:ip:porta:maxClient-
		*Estrae il proprio IP e la propria porta tramite i metodi getIp() e getPorta() di Socket_Passivo.
		*Attende l'esito dell'operazione dal DFR e lo stampa a video.
		*@param fn nome del file che si vuole registrare.
		*@param maxClient numero massimo di download simultanei per il file.
		*/
		void register_file(const string& fn, const string& maxClient);

		/**
		*Permette di deregistrare un file dal DFR.
		*Invia un messaggio così strutturato: REGISTER_FILE:filename:ip:porta-
		*Attende l'esito dell'operazione dal DFR e lo stampa a video.
		*Estrae il proprio IP e la propria porta tramite i metodi getIp() e getPorta() di Socket_Passivo.
		*@param fn nome del file che si vuole deregistrare.
		*/
		void unregister_file(const string& fn);

		/**
		*Chiude il Socket_Passivo se diverso da NULL
		*/
		void chiudi();

		/**
		*Permette di attendere i Thread che servono i Client
		*/
		void aspettaContattiClient();
		

};

#endif
