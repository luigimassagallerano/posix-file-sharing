#ifndef __FILE_CONDITION_H__
#define __FILE_CONDITION_H__


#include <string>
#include <iostream> 
#include <semaphore.h>

#include "msg_err.h"

using namespace std;

/**
*Oggetto che serve per sospendere un thread in caso richieda la prenotazione di un File.
*E' formato da una coppia: nome_file e semaforo
*/
class File_Condition{
	
	string nome_file; /**< Nome del file a cui è associato il semafoto */
	sem_t cond;	/**< Semaforo su cui i thread si sospendono quando richiedono una prenotazione */

	public:
		/**
		*Costruttore: costruisce un File_Condition di un file.
		*Inizializza il semaforo a zero.
		*@param nome_file nome del file su cui creare il semaforo.
		*/
		File_Condition(const string& nome_file);

		/**
		*Metodo per avere il nome del file.
		*@return Nome del File dell'oggetto.
		*/
		string getFileName();

		/**
		*Invoca la wait sul semaforo cond.
		*/
		void sospendi();

		/**
		*Fa il post di tutti i thread sospesi su cond.
		*/
		void sveglia();

};
	
#endif
