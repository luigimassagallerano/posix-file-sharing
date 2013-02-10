#ifndef __REPOSITORY_H__
#define __REPOSITORY_H__

#include <list>
#include <string>
#include <iostream>

#include "File_Location.h"
#include "msg_err.h"

using namespace std;

/**
*Classe che modella l'archivio di file con rispettive Location mantenuto dal DFR
*/
class Repository{
	
	list<File_Location> archivio; /**< Lista di File_Location che a sua volta è una lista di Location */
	sem_t mutex; /**< Semaforo mutex. Serve per accedere in mutua esclusione al Repository */

	public:

		/**
		*Costruttore: *Inizializza il semaforo a uno.
		*/
		Repository();
	
		/**
		*Controlla se esiste il file in archivio.
		*Se esiste controlla se la Location esiste già in quel file, se non esiste l'aggiunge.
		*Se il file non esiste in archivio lo crea e ci aggiunge la Location.
		*@param fn Nome del file da aggiungere.
		*@param ip IP della Location dove si trova il file.
		*@param porta Porta della Location dove si trova il file.
		*@param maxClient numero massimo di download simultanei del file da questo Location.
		*/
		void salva(const string& fn, const string& ip, const string& porta, int maxClient);

		/**
		*Cerca il file e se esiste cerca la Location e se esiste la cancella.
		*Se era l'ultima Location per quel file cancella il file dall'archivio.
		*@param fn Nome del file da cancellare.
		*@param ip IP della Location dove si trova il file.
		*@param porta Porta della Location dove si trova il file.
		*/
		void cancella(const string& fn, const string& ip, const string& porta);

		/**
		*Cerca il file in archivio. Se lo trova chiama il metodo (in caso bloccante) get_Location.
		*@param fn Nome del file ricercato.
		*@return FILE_FOUND:ip:porta in caso il file è stato trovato, FILE_NOT_FOUND altrimenti.
		*/
		string get_File_Location(const string& fn);

		/**
		*@return per ogni file: File <lista di location>
		*/
		string stampa();

		/**
		*Metodo richiamto dal DFR per segnalare la fine di un Download da quella Location e liberare così uno Slot
		*Svegliando un Thread sospeso su maxClient (semaforo dentro Location)
		*@param fn nome del file.
		*@param ip indirizzo ip della Location da cui è finito il download.
		*@param porta porta della Location da cui è finito il download.
		*/
		void liberaSlot(const string& fn, const string& ip, const string& porta);



};

#endif
