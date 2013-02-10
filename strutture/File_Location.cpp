#include "../include/File_Location.h"

File_Location::File_Location(const string& nf)
{
	nome_file = nf;
}

void File_Location::add_Location(Location& l)
{
	list<Location>::iterator it;

	for ( it=locations.begin() ; it != locations.end(); it++ ){
		if( it -> equals(l) ){
			cout << "Locazione già esistente!\n";
			return;
		}
	}

	locations.push_back (l);

}

void File_Location::remove_Location(Location& l)
{
	if( locations.empty() ){
		cout << "Locations Vuota\n";
		return;
	}
	
	list<Location>::iterator it;
	for ( it=locations.begin() ; it != locations.end(); it++ ){
		if( it -> equals(l) ){
			it = locations.erase (it);
		}
	}

}

string File_Location::get_File()
{
	return nome_file;
}

string File_Location::get_Location(sem_t* mutex) //CERCO UNA LOCATION CON SLOT LIBERI IN CASO TUTTO PIENO MI SOSPENDO SULL'INIZIALE
{
	list<Location>::iterator it;
	for ( it=locations.begin() ; it != locations.end(); it++ ){
		if( it -> haveSlot() ){
			sem_post(mutex);
			return it -> getLocation(); 
		}
	}
	
	it = locations.begin();
	sem_post(mutex);
	return it -> getLocation(); //bloccante
}

bool File_Location::esisteLocation(const string& ip, const string& porta)
{
	list<Location>::iterator it;
	for ( it=locations.begin() ; it != locations.end(); it++ ){
		if((it -> stampa()).compare(ip+":"+porta) == 0) return true; //esiste già una Location con quell'ip e quella porta
	}

	return false;
}			

string File_Location::stampa()
{
	string s = "";
	list<Location>::iterator it;
	for ( it=locations.begin() ; it != locations.end(); it++ ){
		s = s + it -> stampa() + " - ";
	}
	
	return s;

}

bool File_Location::noLocations()
{
	return locations.empty();
}

void File_Location::liberaSlot(Location& l)
{
	list<Location>::iterator it;
	for ( it=locations.begin() ; it != locations.end(); it++ ){
		if( it -> equals(l) ){
			it -> liberaSlot();
		}
	}
}





