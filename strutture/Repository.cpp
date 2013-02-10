#include "../include/Repository.h"


Repository::Repository()
{
	if(sem_init(&mutex, 0, 1) == -1) sys_err("Inizializzazione Mutex Repository Fallita!\n");
}


void Repository::salva(const string& fn, const string& ip, const string& porta, int maxClient)
{
	list<File_Location>::iterator it;

	sem_wait(&mutex); //MUTUA ESCLUSIONE
	for ( it=archivio.begin() ; it != archivio.end(); it++ ){
		if((it -> get_File()).compare(fn) == 0){ //se esiste già il file in archivio controllo se esiste la Location
			if(it -> esisteLocation(ip, porta)){ //se esiste la Location non aggiungo niente
				sem_post(&mutex); //FINE MUTUA ESCLUSIONE
				cout << fn << ": Location: " << ip+":"+porta+" già esistente per file\n";
				return;
			}else{ //se il file esiste ma non ha questa Location l'aggiungo
				Location l(ip, porta, maxClient);
				it -> add_Location(l);
				sem_post(&mutex); //FINE MUTUA ESCLUSIONE
				cout << fn << ": file già in archivio: aggiunta nuova Location: " << ip+":"+porta+"\n";
				return;
			}
		}
	}	
	//se il file non esiste in archivio lo aggiungo con questa Location
	File_Location fl(fn);
	Location l(ip, porta, maxClient);
	fl.add_Location(l);
	archivio.push_front( fl );

	sem_post(&mutex); //FINE MUTUA ESCLUSIONE

	cout << fn << ": aggiunto file in archivio con Location: " << ip+":"+porta+"\n";
}

void Repository::cancella(const string& fn, const string& ip, const string& porta)
{
	list<File_Location>::iterator it;

	sem_wait(&mutex); //MUTUA ESCLUSIONE
	for ( it=archivio.begin() ; it != archivio.end(); it++ ){ //cerco file con stesso nome
		if((it -> get_File()).compare(fn) == 0){ //trovato file con stesso nome
			if(it -> esisteLocation(ip, porta)){ //se esiste la Location la devo cancellare
				Location l(ip, porta);				
				it -> remove_Location(l);
				cout << fn+ ": rimossa Location "+ip+":"+porta+"\n";				

				//se era l'ultima Location devo eliminare il file
				if( it -> noLocations() ){
					it = archivio.erase(it);
					cout << fn+ ": non aveva più Location ed è stato cancellato dall'archivio\n";
				}

				sem_post(&mutex); //FINE MUTUA ESCLUSIONE
				return;
			}
		}
	}
	sem_post(&mutex); //FINE MUTUA ESCLUSIONE
	//se non esiste Location o addirittura non esiste file non faccio nulla
	cout << fn+" su "+ip+":"+porta << " - File o Locazione non trovati\n";
}

string Repository::get_File_Location(const string& fn)
{
	list<File_Location>::iterator it;

	sem_wait(&mutex); //MUTUA ESCLUSIONE
	for ( it=archivio.begin() ; it != archivio.end(); it++ ){
		if((it -> get_File()).compare(fn) == 0){ //Trovato file

			//getLocation mi costringe ad acquisire il semaforo maxClient
			return "FILE_FOUND:"+it -> get_Location(&mutex); //POTREBBE ESSERE BLOCCANTE = RILASCO DEL MUTEX ALL'INTERNO
		}
	}
	sem_post(&mutex); //FINE MUTUA ESCLUSIONE
	return "FILE_NOT_FOUND-";
}

string Repository::stampa()
{
	string s="";

	sem_wait(&mutex); //MUTUA ESCLUSIONE
	list<File_Location>::iterator it;
	for ( it=archivio.begin() ; it != archivio.end(); it++ ){
		s = s + it -> get_File() + " " + it -> stampa() + "\n";
	}
	sem_post(&mutex); //FINE MUTUA ESCLUSIONE
	if(s.compare("") == 0) s = "Archivio Vuoto\n";	

	return s;
}

void Repository::liberaSlot(const string& fn, const string& ip, const string& porta)
{
	list<File_Location>::iterator it;
	
	sem_wait(&mutex); //MUTUA ESCLUSIONE
	for ( it=archivio.begin() ; it != archivio.end(); it++ ){
		if((it -> get_File()).compare(fn) == 0){ //trovato file 
			Location l(ip, porta);
			it -> liberaSlot(l);
			sem_post(&mutex); //FINE MUTUA ESCLUSIONE
			return;
		}
	}

	sem_post(&mutex); //FINE MUTUA ESCLUSIONE
}


//int main()
//{

	//Repository r;
	//r.salva("Libro.txt", "127.0.0.1", "6565");
	//r.salva("Libro.txt", "192.200.0.1", "8888");
	//r.salva("Pino.txt", "200.200.200.200", "3333");
	//cout << r.get_File_Location("Libro.txt") << "\n";
	//cout << r.get_File_Location("Pino.txt") << "\n";
	//cout << r.stampa()+"\n";
	//r.cancella("Libro.txt", "127.0.0.1", "6565");
	//cout << r.get_File_Location("Libro.txt") << "\n";
	//r.cancella("Libro.txt", "192.200.0.1", "8888");
	//cout << r.get_File_Location("Libro.txt") << "\n";
	//r.cancella("Pino.txt", "200.200.200.200", "3333");
	//cout << r.get_File_Location("Pino.txt") << "\n";
	//cout << r.stampa()+"\n";

	
//}
