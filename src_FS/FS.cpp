#include "../include/FS.h"

FS::FS()
{
	int porta_client = 0;
	cout << "Inserisci la porta su cui ascoltare Client:\n";
	cin >> porta_client;

	sp = new Socket_Passivo(porta_client);
	sa = NULL;

	args[0] = (int)sp;
	args[1] = (int)this;

	int i = pthread_create(&clth, NULL, listen_client, (void *)&args);
	if(i != 0) sys_err("Errore nella creazione del Thread Client_Listener!\n");

	

}

FS::FS(int porta)
{
	sp = new Socket_Passivo(porta);
	sa = NULL;

	args[0] = (int)sp;
	args[1] = (int)this;

	int i = pthread_create(&clth, NULL, listen_client, (void *)&args);
	if(i != 0) sys_err("Errore nella creazione del Thread Client_Listener!\n");	

}

FS::~FS()
{
	delete sp;
	delete sa;
}

void FS::collega_al_server()
{
	string ip_DFR = "";
	int porta_DFR = 0;
	cout << "Inserisci l'indirizzo ip del DFR: \n";
	cin >> ip_DFR;
	cout << "Inserisci la porta del DFR: \n";
	cin >> porta_DFR;
	sa = new Socket_Attivo(ip_DFR, porta_DFR);
	sa -> connetti();
}

void FS::collega_al_server(const string& ip, int porta)
{
	sa = new Socket_Attivo(ip, porta);
	sa -> connetti();
}

void FS::register_file(const string& fn, const string& maxClient)
{
	string ip = sp -> getIP();
	string porta = sp -> getPorta();
	sa -> invia("REGISTER_FILE:"+fn+":"+ip+":"+porta+":"+maxClient+"-");
	string risultato = sa -> ricevi();
	int i = ricavaComando(risultato);
	if(i == BYE){
		cout << "File "+fn+" registrato con successo su DFR\n";
	}else{
		cout << "Errore nella registrazione del File "+fn+"\n";
	}
}

void FS::unregister_file(const string& fn)
{
	string ip = sp -> getIP();
	string porta = sp -> getPorta();
	sa -> invia("UNREGISTER_FILE:"+fn+":"+ip+":"+porta+"-");
	string risultato = sa -> ricevi();
	int i = ricavaComando(risultato);
	if(i == BYE){
		cout << "File "+fn+" deregistrato con successo su DFR\n\n";
	}else{
		cout << "Errore nella deregistrazione del File "+fn+"\n\n";
	}
}

void *FS::listen_client( void *ptr )
{
	int *args= (int *)ptr;
	Socket_Passivo *sock= (Socket_Passivo *)args[0];
	pthread_t tid;

	while(true){
		cout << "Attendo un contatto Client...\n";
		Socket_Attivo* sockA = sock -> accetta();
		args[2] = (int)sockA;
		cout << "Ricevuto contatto Client creo un Thread per lui\n";
		pthread_create(&tid, NULL, servi_client, args);
		cout << "Creato Thread pronto a servire Client\n\n";
	}

	pthread_detach(pthread_self());
}

void *FS::servi_client( void *ptr )
{
	int *args = (int *)ptr;
	Socket_Attivo *sock = (Socket_Attivo *)args[2];
	FS *th = (FS *)args[1];

	string comando = sock -> ricevi();
	int i = ricavaComando(comando);

	if(i == GET_FILE){
		string nomefile = ricavaNomeFile(comando);
		cout << nomefile+"\n";
		th -> start_file_transfer(nomefile, sock);				
	}

	cout << "Chiudo Thread-Client\n";
	pthread_detach(pthread_self());
}

void FS::start_file_transfer(const string& file_name, Socket_Attivo* sock)
{
	FILE *in;
	
	if(((in=fopen(("./"+file_name).c_str(), "r"))==NULL)){
		cout << file_name << "NON TROVATO\n";
	}else{
		cout << "Invio "+file_name+" al Client...\n";
		sock -> invia_file("./"+file_name);
		sock -> disconnetti();
		cout << file_name+" inviato al Client\n";
	}
		
	
}

void FS::aspettaContattiClient()
{
	pthread_join(clth, 0);
}

void FS::chiudi()
{
	if(sp != NULL){
		cout << "Chiusura Connessione con DFR...\n";	
		sp -> disconnetti();
	}
}

