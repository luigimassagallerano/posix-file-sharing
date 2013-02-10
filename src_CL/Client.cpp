#include "../include/Client.h"


Client::Client()
{
	sa_DFR = NULL;
	sa_FS = NULL;
}

Client::~Client()
{
	delete sa_DFR;
	delete sa_FS;
}

void Client::collega_al_server()
{
	string ip_DFR = "";
	int porta_DFR = 0;
	cout << "Inserisci l'indirizzo ip del DFR: \n";
	cin >> ip_DFR;
	cout << "Inserisci la porta del DFR: \n";
	cin >> porta_DFR;
	sa_DFR = new Socket_Attivo(ip_DFR, porta_DFR);
	sa_DFR -> connetti();
}

void Client::collega_al_server(const string& ip, int porta)
{
	sa_DFR = new Socket_Attivo(ip, porta);
	sa_DFR -> connetti();
}

int Client::richiedi_file(const string& nf)
{
	sa_DFR -> invia("GET_FILE:"+nf+"-");
	
	string risultato = sa_DFR -> ricevi();
	int i = ricavaComando(risultato);

	if( i == FILE_FOUND){
		cout << nf+": file trovato! Mi connetto al FS...\n";
		string ip = ricavaIP(risultato);
		int porta = atoi(ricavaPorta(risultato).c_str());
		crea_socket_fs(ip, porta);
		inizia_trasferimento(nf);
		fine_trasferimento(nf);
		return 1; //TUTTO OK
	}
	
	cout << nf+" File non trovato!\n";
	return -1; //FILE NON TROVATO - DA GESTIRE IN SEGUITO
	
	
}

void Client::crea_socket_fs(const string& ip, int porta)
{
	sa_FS = new Socket_Attivo(ip, porta);
	sa_FS -> connetti();	
}

void Client::inizia_trasferimento(const string& nf)
{
	cout << nf+"\n";
	sa_FS -> invia("GET_FILE:"+nf+"-");
	sa_FS -> ricevi_file("./download/"+nf); //I file vengono salvati in download a partire dal path relativo	
	cout << "Download di: "+nf+" Completato!\n";
}

void Client::fine_trasferimento(const string& nf)
{
	sa_DFR -> invia("TRANSFER_COMPLETE:"+nf+"-");  //segnalo al DFR che ho finito di scaricare il file
}

int Client::richiedi_prenotazione(const string& nome_file)
{
	cout << "Invio richiesta di prenotazione per: "+nome_file+"...\n";
	sa_DFR -> invia("PRENOTATION:"+nome_file+"-");
	cout << "Richiesta di prenotazione per: "+nome_file+" inviata! Attendo notifica...\n";
	string risultato = sa_DFR -> ricevi();
	
	int i = ricavaComando(risultato);

	if( i == FILE_FOUND){
		cout << "Notifica per: "+nome_file+" ricevuta! Mi collego al FS...\n";
		string ip = ricavaIP(risultato);
		int porta = atoi(ricavaPorta(risultato).c_str());
		crea_socket_fs(ip, porta);
		inizia_trasferimento(nome_file);
		fine_trasferimento(nome_file);
		return 1; //TUTTO OK
	}
	
	cout << "Errore nel Download di: "+nome_file+" dopo la Prenotazione!\n";	
	return -1; 
	
}

void Client::chiudi()
{
	if(sa_DFR != NULL){
		cout << "Chiusura Connessione con DFR...\n";	
		sa_DFR -> disconnetti();
	}
	if(sa_FS != NULL){
		cout << "Chiusura Connessione con FS...\n";		
		sa_FS -> disconnetti();
	}
}

