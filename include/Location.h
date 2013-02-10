#ifndef __LOCATION_H__
#define __LOCATION_H__

#include <string>
#include <semaphore.h>
#include <iostream>

#include "msg_err.h"

using namespace std;

/**
*Classe che modella l'oggeto Location formato dall'ip e dalla porta del FS e dal numero massimo di client che possono scaricare insieme.
*Questa Location sarà associata ad un File tramite l'oggetto File_Location.
*/
class Location
{
	string ip_FS; /**< Ip del FS */
	string porta_FS; /**< Porta del FS */
	sem_t maxClient; /**< Semaforo che controllerà che il numero di download contemporanei non superi il numero maxClient */

	public:
		/**
		*Costruttore: costruisce una Location con questo ip e porta e inizializza il semaforo al valore maxClient.
		*@param ip Ip della Location
		*@param porta Porta della Location.
		*@param maxClient valore iniziale del semaforo maxClient.
		*/
		Location(const string& ip, const string& porta, int maxClient);
		
		/**
		*Costruttore: costruisce una Location con questo ip e porta.
		*Usato per ricercare la Location da cancellare. maxClient inutile.
		*@param ip Ip della Location
		*@param porta Porta della Location.
		*/
		Location(const string& ip, const string& porta);

		/**
		*Acquisisce tramite la wait il semaforo maxClient in modo che più di maxClient non possano avere indietro la Location.
		*@return ip:porta della Location su cui si trova il File.
		*/
		string getLocation();

		/**
		*Usato per confrontare due Location. Due Location sono uguali se hanno stesso ip e porta.
		*@return true se sono uguali, false se sono diverse.
		*/
		bool equals(Location& l);
		
		/**
		*Metodo usato indirettamente dal DFR per risvegliare i thread CLient che si erano sospesi su maxClient.
		*/
		void liberaSlot();

		/**
		*Metodo usato per bilanciare la richiesta di download su più Location.
		*@return true se ci sono slot per scaricare liberi, false altrimenti.
		*/
		bool haveSlot();

		/**
		*Metodo usato per ritornare la Location (per stampe o simili) senza dover acquisire il semaforo maxClient.
		*@return ip:porta della Location.
		*/
		string stampa();

};

#endif
