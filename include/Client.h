#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <string>
#include <stdio.h>
#include "Socket_Attivo.h"
#include "msg_err.h"
#include "interprete.h"

using namespace std;

/**
*Classe che modella l'oggetto Client. 
*Questo oggetto è creato da un main() ed è usato per creare un Client su cui poter richiamare
*tutte le operazioni necessarie
*/
class Client{
	
	Socket_Attivo *sa_DFR; /**< Puntatore a Socket_Attivo. Un oggetto Socket_Attivo verso il DFR verrà instanziato in un secondo momento. */
	Socket_Attivo *sa_FS; /**< Puntatore a Socket_Attivo. Un oggetto Socket_Attivo verso il FS verrà instanziato in un secondo momento. */	
	
	/**
	*Metodo privato usato da richiedi_prenotazione e richiedi_file per creare un socket verso il FS.
	*@param ip indirizzo IP del FS
	*@param porta Porta del FS
	*/
	void crea_socket_fs(const string& ip, int porta);

	/**
	*Invia il comando: GET_FILE:nome_file- al FS e scarica il file
	*@param nf Nome del File da scaricare
	*/
	void inizia_trasferimento(const string& nf);

	/**
	*Invia un messaggio al DFR con il seguente formato: TRANSFER_COMPLETE:nome_file-.
	*Segnala la fine del download del file.
	*@param nf Nome del file per il quale il download è terminato
	*/
	void fine_trasferimento(const string& nf);

	public:
		/**
		*Il costruttore crea un oggetto Client settando a NULL i due puntatori a Socket sa_DFR e sa_FS.
		*/
		Client();
		
		/**
		*IL distruttore cancella i puntatori sa_DFR e sa_FS.
		*/
		~Client();

		/**
		*Versione senza paramentri di collega_al_server.
		*Richiede l'inserimento da Console dell'ip e della porta del DFR a cui collegarsi
		*dopo di che crea un Socket_Attivo e ci chiama sopra il metodo: connetti().
		*/
		void collega_al_server();

		/**
		*Versione con parametri di collega_al_server.
		*Invece di richiedere l'inserimento di ip e porta del DFR tramite Console, questo metodo
		*li accetta come parametro creando successicamente un Socket_Attivo su cui chiama connetti().
		*@param ip indirizzo ip del DFR a cui collegarsi.
		*@param porta porta del DFR a cui collegarsi.
		*/
		void collega_al_server(const string& ip, int porta);

		/**
		*Metodo che permette al Client di richiedere un file tramite il nome al DFR.
		*Invia un messaggio sul socket al DFR con il seguente formato: GET_FILE:nome_file-.
		*Si mette in attesa di una risposta del DFR. 
		*In caso di FILE_FOUND si collega all'FS estraendo IP e PORTA tramite l'interprete e scarica il file.
		*Se tutto va bene segnala la fine del download al DFR e ritorna 1.
		*In caso di FILE_NOT_FOUND ritorna -1.
		*@param nome_file il nome del file che si vuole scaricare.
		*@return successo: 1 insuccesso: -1
		*/
		int richiedi_file(const string& nome_file);

		/**
		*In caso di file non ancora registrato sul DFR un Client può richiederne la prenotazione passandone il nome a questo metodo.
		*Invia un messaggio al DFR con il seguente formato: PRENOTATION:nome_file- 
		*Si mette in attesa di una notifica non appenail file sarà disponibile.
		*A notifica arrivata tenta di scaricare il file.
		*@param nome_file nome del file per il quale si richiede la prenotazione
		*@return successo: 1 insuccesso: -1
		*/		
		int richiedi_prenotazione(const string& nome_file);

		/**
		*Invoca il metodo disconnetti sui due Socket sa_DFR e sa_FS se diversi da NULL
		*/		
		void chiudi();

};

#endif
