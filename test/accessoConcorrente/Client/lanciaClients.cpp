#include "../../../include/Client.h"

int main()
{
	Client* c = new Client();

	string fn = "friends.avi";	
	
	while(true){
		sleep(20);
		c -> collega_al_server("0.0.0.0", 7575);
		int i = c -> richiedi_file(fn);
		c -> chiudi();
		cout << "\n\n";
	}
		

	return 1;

}
