#include "../include/DFR.h"

DFR::DFR()
{
	int i;

	r = new Repository();
	lp = new Lista_Prenotazioni();
	
	int porta_client = 0;
	int porta_fs = 0;

	cout << "Inserisci la porta su cui ascoltare Client:\n";
	cin >> porta_client;

	cout << "Inserisci la porta su cui ascoltare FS:\n";
	cin >> porta_fs;	
	
	ss_CL = new Socket_Passivo(porta_client);
	ss_FS = new Socket_Passivo(porta_fs);
	
	//array di puntatori per i thread client
	args1[0] = (int)ss_CL;
	args1[1] = (int)this;
	
	i = pthread_create(&clth, NULL, listen_client, (void *)&args1);
	if(i != 0) sys_err("Errore nella creazione del Thread Client_Listener!\n");
	
	//array di puntatori per i thread fs
	args2[0] = (int)ss_FS;
	args2[1] = (int)this;

	i = pthread_create(&fsth, NULL, listen_fs, (void *)&args2);
	if(i != 0) sys_err("Errore nella creazione del Thread FS_Listener!\n");

	pthread_join(clth, 0);
	pthread_join(fsth, 0);

}

DFR::~DFR()
{
	delete r;
	delete lp;
}

void *DFR::listen_client( void *ptr )
{
	int *args= (int *)ptr;
	Socket_Passivo *ss= (Socket_Passivo *)args[0];
	pthread_t tid;

	while(true){	
		cout << "Attendo un contatto Client...\n";
		Socket_Attivo *sa = ss -> accetta();
		args[2] = (int)sa;
		cout << "Ricevuto contatto Client creo un Thread per lui\n";
		pthread_create(&tid, NULL, servi_client, args);
	}

	ss -> disconnetti();
	pthread_detach(pthread_self());
	
}

void *DFR::listen_fs( void *ptr )
{
	int *args= (int *)ptr;
	Socket_Passivo *sp= (Socket_Passivo *)args[0];
	pthread_t tid;

	while(true){	
		cout << "Attendo un contatto FS...\n";
		Socket_Attivo *sa = sp -> accetta();
		args[2] = (int)sa;
		cout << "Ricevuto contatto FS creo un Thread per lui\n";
		pthread_create(&tid, NULL, servi_fs, args);
	}
	
	sp -> disconnetti();
	pthread_detach(pthread_self());	
	
}

void *DFR::servi_client( void *ptr )
{
	int *args = (int *)ptr;
	DFR *th = (DFR *)args[1];
	Socket_Attivo *sa = (Socket_Attivo *)args[2];	
	
	string comando = sa -> ricevi();
	int i = ricavaComando(comando);

	if(i == GET_FILE){
		string nomefile = ricavaNomeFile(comando);
		th -> start_file_transfer(nomefile, sa);				
	}

	pthread_detach(pthread_self());
}

void *DFR::servi_fs( void *ptr )
{	
	int *args= (int *)ptr;
	DFR *th = (DFR *)args[1];	
	Socket_Attivo *sa = (Socket_Attivo *)args[2];
	

	string comando = sa -> ricevi();
	int i = ricavaComando(comando);
	
	if(i == REGISTER_FILE){
		string fn = ricavaNomeFile(comando);
		string ip = ricavaIPLocazione(comando);
		string porta = ricavaPortaLocazione(comando);
		int maxClient = ricavaMaxClient(comando);
		cout << "maxClient da interprete: " << maxClient << "\n";
		th -> register_file(fn, ip, porta, maxClient);
		sa -> invia("BYE-");
	}
	if(i == UNREGISTER_FILE){ 
		string fn = ricavaNomeFile(comando);
		string ip = ricavaIPLocazione(comando);
		string porta = ricavaPortaLocazione(comando);
		th -> unregister_file(fn, ip, porta);
		sa -> invia("BYE-");
	}
	
	pthread_detach(pthread_self());	
}

void DFR::start_file_transfer(const string& nomefile, Socket_Attivo* sa)
{
	
	string risultato = r -> get_File_Location(nomefile); 

	int i = ricavaComando(risultato);

	if(i == FILE_FOUND){
		fileFound(sa, risultato, nomefile);
	}	
	
	if(i == FILE_NOT_FOUND){
		fileNotFound(sa, nomefile);	
	}			
}

void DFR::register_file(const string& file_name, const string& ip, const string& porta, int maxClient)
{
	r -> salva(file_name, ip, porta, maxClient);
	cout << "\n\n***Repository Aggiornato***\n" << r -> stampa() << "\n";
	
	cout << "***************************\n\n";

	//Controllo se c'erano prenotazioni per questo file e in caso le servo
	lp -> serviPrenotazione(file_name);
}

void DFR::unregister_file(const string& file_name, const string& ip, const string& porta)
{
	
	r -> cancella(file_name, ip, porta);
	cout << "\n\n***Repository Aggiornato***\n" << r -> stampa() << "\n";
	
	cout << "***************************\n\nF";
}

void DFR::fileFound(Socket_Attivo* sock, const string& msg, const string& nf)
{
	cout << nf+": File trovato in archivio...\n";

	string ip = ricavaIP(msg+"-");
	string porta = ricavaPorta(msg+"-");

	sock -> invia(msg+"-");
	string risultato = sock -> ricevi();
	int i = ricavaComando(risultato);
		
	if(i == TRANSFER_COMPLETE){
		cout << "Il Client ha completato il trasferimento di: " << nf << ". Chiudo Thread!\n";
		//SVEGLIARE UN THREAD IN ATTESA SU SEMAFORO MAXCLIENT
				
		r -> liberaSlot(nf, ip, porta);
		
	}else{
		cout << "Il Client non è riuscito a completare il trasferimento di: " << nf << ". Chiudo Thread!\n";
		//SVEGLIARE UN THREAD IN ATTESA SU SEMAFORO MAXCLIENT
		
		r -> liberaSlot(nf, ip, porta);
		
	}	
}

void DFR::fileNotFound(Socket_Attivo* sock, const string& nf)
{
	cout << nf+": File non in archivio!!\n";
	sock -> invia("FILE_NOT_FOUND-");
	string risultato = sock -> ricevi();
	int i = ricavaComando(risultato);

	if(i == PRENOTATION){
		prenotation(sock, nf);
	}
}

void DFR::prenotation(Socket_Attivo* sock, const string& nf)
{
	cout << "Il Client ha richiesto la prenotazione di: "+nf+"\n";
	lp -> aggiungiPrenotazione(nf); //il thread si sospende
	
	
	string risultato = r -> get_File_Location(nf); //se viene svegliato significa che il file è arrivato
	

	int i = ricavaComando(risultato);

	if(i == FILE_FOUND){
		fileFound(sock, risultato, nf);
	}	
	
	if(i == FILE_NOT_FOUND){
		fileNotFound(sock, nf);	
	}
	
}



