#include "../include/Lista_Prenotazioni.h"

Lista_Prenotazioni::Lista_Prenotazioni()
{
	if(sem_init(&mutex, 0, 1) == -1) sys_err("Inizializzazione Condition Fallita!\n");
}


void Lista_Prenotazioni::aggiungiPrenotazione(const string& nome_file)
{
	list<File_Condition>::iterator it;

	sem_wait(&mutex); //MUTUA ESCLUSIONE
	for ( it=prenotazioni.begin() ; it != prenotazioni.end(); it++ ){
		if((it -> getFileName()).compare(nome_file) == 0){ //se esiste già una Condition per questo file sospendi su essa

			sem_post(&mutex); //FINE MUTUA ESCLUSIONE
			
			cout << nome_file << ": Client aggiunto a condition di prenotazione per questo file\n";			
			it -> sospendi();
			return;
		}
	}
	//se sono qui vuol dire che non esiste ancora una condition per questo file e la devo creare
	File_Condition fc(nome_file);
	prenotazioni.push_front( fc );
	it = prenotazioni.begin();	//punto alla prenotazione che ho appena aggiunto
	
	sem_post(&mutex); //FINE MUTUA ESCLUSIONE

	cout << nome_file << ": creata condition di prenotazione per questo file\n";
	it -> sospendi();			
}

void Lista_Prenotazioni::serviPrenotazione(const string& nome_file)
{	
	list<File_Condition>::iterator it;

	sem_wait(&mutex); //MUTUA ESCLUSIONE
	for ( it=prenotazioni.begin() ; it != prenotazioni.end(); it++ ){
		if((it -> getFileName()).compare(nome_file) == 0){ //se esistono prenotazioni su questo file le servo
			it -> sveglia();
			cout << nome_file << ": svegliati tutti i thread su condition di prenotazione per questo file\n";
			it = prenotazioni.erase(it);
			sem_post(&mutex); //FINE MUTUA ESCLUSIONE
			cout << nome_file << ": Cancellata condition di prenotazione per questo file\n";
			return;
		}
	}

	sem_post(&mutex); //FINE MUTUA ESCLUSIONE
}
