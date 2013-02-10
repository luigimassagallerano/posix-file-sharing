#ifndef __DFR_H__
#define __DFR_H__

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "msg_err.h"
#include "Repository.h"
#include "Socket_Passivo.h"
#include "interprete.h"
#include "Lista_Prenotazioni.h"

/**
*Classe che modella l'oggetto DFR. 
*Per lanciare il DFR un main() non dovrà fare altro che creare un oggetto DFR.
*Il DFR oltre al costruttore e al distruttore non presenta metodi pubblici perché è completamente automatizzato.
*/
class DFR{

	Repository *r; /**< Puntatore all'oggetto Repository. su r il DFR potrà memorizzare, cancellare e ricercare File e loro Locazione. */
	Lista_Prenotazioni *lp; /**< Puntatore a Lista_Prenotazioni. Serve per mantenere traccia delle Prenotazioni e per Sospendere/Svegliare i Thread Client */

	
	Socket_Passivo* ss_CL; /**< Puntatore a Socket_Passivo, per ascoltare i Client */
	Socket_Passivo* ss_FS; /**< Puntatore a Socket_Passivo, per ascoltare i FS */

	int args1[3]; /**< array di puntatori, puntatori da passare ai metodi statici dei Thread Client*/
	int args2[3]; /**< array di puntatori, puntatori da passare ai metodi statici dei Thread FS*/

	pthread_t clth; /**< Thread in ascolto di Client, esegue la funzione listen_client()*/
	pthread_t fsth;	/**< Thread in ascolto di FS, esegue la funzione listen_fs()*/

	/**
	*Funzione passata al thread clth per ricevere le richieste da parte dei Client.
	*Crea un Socket_Attivo e un Thread per ogni richiesta arrivata.
	*La funzione esegue un ciclo infinito while(true)
	*/
	static void *listen_client(void *ss);

	/**
	*Funzione passata al thread fsth per ricevere le richieste da parte dei FS.
	*Crea un Socket_Attivo e un Thread per ogni richiesta arrivata.
	*La funzione esegue un ciclo infinito while(true)
	*/
	static void *listen_fs(void *ss);

	/**
	*Funzione passata ad ogni nuovo Thread creato per la connessione di un Client.
	*Riceve un comando dal socket, lo analizza tramite l'interprete e invoca start_file_transfer()
	*/
	static void *servi_client(void *ss);

	/**
	*Funzione passata ad ogni nuovo Thread creato per la connessione di un FS.
	*Riceve un comando dal socket, lo analizza e decide cosa fare.
	*Può registrare o deregistrare un file.
	*Invia in risposta un messaggio di chiusura così strutturato: BYE-
	*/
	static void *servi_fs(void *ss);
	
	/**
	*Dopo aver acquisito il mutex richiama il metodo salva() sul Repository
	*Stampa a video il Repository aggiornato.
	*Controlla se il nuovo file arrivato aveva prenotazioni e in caso le serve lanciando il metodo serviPrenotazione().
	*@param file_name nome del file da registrare.
	*@param ip Indirizzo ip del FS su cui si trova il file.
	*@param porta Porta del FS su cui si trova il file.
	*@param maxClient Numero massimo di Client che possono scaricare insieme il file.
	*/
	void register_file(const string& file_name, const string& ip, const string& porta, int maxClient);

	/**
	*Dopo aver acquisito il mutex richiama il metodo cancella() sul Repository.
	*Stampa a vide il Repository aggiornato.
	*@param file_name Nome del file da cancellare.
	*@param ip Ip del FS su cui si trovava il file.
	*@param porta Porta del FS su cui si trovava il file.
	*/
	void unregister_file(const string& file_name, const string& ip, const string& porta);

	/**
	*Richiede sul Repository la Locazione del File.
	*In caso il file è già in download da maxClient il thread si sospende.
	*Richiama le funzioni private file_found o file_not_found in base al tipo di risultato ottenuto dal Repository.
	*@param file_name Nome del file ricercato.
	*@param ss Puntatore al Socket verso il Client.
	*/
	void start_file_transfer(const string& file_name, Socket_Attivo* ss);

	/**
	*Invia la Location da cui scaricare il file al Client.
	*Aspetta la segnalazione della fine del download e libera un slot (maxClient) dalla Location in modo che un altro Client possa scaricare.
	*@param sock Socket verso il Client.
	*@param msg Messaggio dal REpository contenente la Locazione.
	*@nf Nome del File da scaricare.
	*/	
	void fileFound(Socket_Attivo* sock, const string& msg, const string& nf);

	/**
	*Segnala al Client che il File non è stato trovato tramite un msg: FILE_NOT_FOUND-
	*Si mette in attesa di una possibile prenotazione del file.
	*@param sock Socket verso il Client.
	*@param nf Nome del File non trovato.
	*/
	void fileNotFound(Socket_Attivo* sock, const string& nf);

	/**
	*Invoca il metodo bloccante aggiungi_prenotazione() sulla lista delle prenotazioni.
	*Il thread si sospende fin quando il DFR non lo segnalerà dal metodo register_file
	*Una volta svegliato il thread richiede il file sul REpository,
	*se non si blocca su maxClient invocherà file_found() o file_not_found().
	*@param sock il Socket verso il Client.
	*@param nf File per il quale si chiede la prenotazione
	*/
	void prenotation(Socket_Attivo* sock, const string& nf);
	
	public:
		/**
		*Costruttore del DFR, richiede tramite Console le porte su cui ascoltare Client e FS.
		*Inizializza tutte le strutture
		*Lancia i due thread in ascolto di Client e FS
		*/
		DFR();

		/**
		*Distruttore: cancella i puntatori al Repository e alla Lista_Prenotazioni
		*/
		~DFR();
		


};

#endif
