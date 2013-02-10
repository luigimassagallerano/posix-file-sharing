#ifndef __LISTA_PRENOTAZIONI_H__
#define __LISTA_PRENOTAZIONI_H__

#include <list>

#include "File_Condition.h"

/**
*Una lista di File_Condition.
*Serve al DFR per creare una struttura per memorizzare e bloccare i thread su determinati file.
*/
class Lista_Prenotazioni{
	
	list<File_Condition> prenotazioni; /**< Lista di File_Condition */
	sem_t mutex;	/**< Mutex per proteggere la Lista Prenotazioni */
	
	public:	
		/**
		*Costruttore: *Inizializza il semaforo a uno.
		*/
		Lista_Prenotazioni();
		
		/**
		*Cerca una condition per il File. 
		*Se esiste già una condition per il File sospende il thread su essa.
		*Se non esiste crea una condition per il file e sospende il thread su essa.
		*@param nome_file nome del file per il quale è richiesta la prenotazione.
		*/
		void aggiungiPrenotazione(const string& nome_file);

		/**
		*Cerca le prenotazioni sul file.
		*Se esitono le serve tutte svegliando tutti i thread.
		*Cancella la condition dopo aver svegliato tutti.
		*@param nome_file nome del file per il quale devo svegliare i thread.
		*/
		void serviPrenotazione(const string& nome_file);
};

#endif
