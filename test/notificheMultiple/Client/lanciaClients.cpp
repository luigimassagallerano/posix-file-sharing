#include "../../../include/Client.h"

int main()
{
	Client* c = new Client();

	string fn = "foto.jpg";	
	
	sleep(5);
	c -> collega_al_server("0.0.0.0", 7575);
	int i = c -> richiedi_file(fn);
	if( i == -1){
		c -> richiedi_prenotazione(fn);
	}
	c -> chiudi();
	cout << "\n\n";
	
	return 1;

}
