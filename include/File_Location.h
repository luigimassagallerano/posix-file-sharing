#ifndef __FILE_LOCATION_H__
#define __FILE_LOCATION_H__

#include <string>
#include <list>
#include <iostream>

#include "Location.h"
#include "msg_err.h"

/**
*Un File_Location associa ad un File una lista di Locazioni (ip:porta) dove trovare il File.
*/
class File_Location
{
	string nome_file; /**< Nome del File */
	list<Location> locations; /**< Lista di Location associate al File */

	public:
		/**
		*Costruttore: inizializza nome_file a nf
		*@param nf Nome del File
		*/
		File_Location(const string& nf);

		/**
		*Ricerca nella lista se la Location esiste già (tramite il metodo equals() di Location)
		*In caso non esiste l'aggiunge.
		*@param l Location da inserire.
		*/
		void add_Location(Location& l);

		/**
		*Ricerca nella lista la Location per cancellarla.
		*@param l Location da cancellare
		*/		
		void remove_Location(Location& l);

		/**
		*@return il nome del file
		*/
		string get_File();

		/**
		*Metodo che ricerca una Location con slot liberi (tramite il metodo haveSlot() di Location)
		*Ritorna la prima Location che permette il download immediato. 
		*Se non trova Location libere sospende il thread Client sulla prima Location della lista.
		*@return ip:porta della Location.
		*/
		string get_Location(sem_t* mutex);

		/**
		*Controlla se esiste già una Location con questo ip e questa porta.
		*@param ip Ip della Location da confrontare.
		*@param porta Porta della Location da confrontare.
		*@return true se esiste, false se non esiste.
		*/
		bool esisteLocation(const string& ip, const string& porta);
		
		/**
		*Metodo usato per stampare a video l'intera lista di Location associate al File.
		*@return lista ip:porta delle Location.		
		*/
		string stampa();
		
		/**
		*@return true se il file non ha più Location
		*/
		bool noLocations();

		/**
		*Metodo usato indirettamente dal DFR per liberare uno slot di download sulla Location.
		*@param l Location sul quale liberare Slot.
		*/
		void liberaSlot(Location& l);


};


#endif
