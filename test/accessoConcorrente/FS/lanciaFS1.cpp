#include "../../../include/FS.h"

int main(){	
	FS* fs = new FS(8282);

	string maxClient = "2";
	string file1 = "foto1.jpg";
	string file2 = "foto2.jpg";
	string file3 = "friends.avi"; //FILE IN COMUNE

	fs -> collega_al_server("0.0.0.0", 7474);
	fs -> register_file(file1, maxClient);
	fs -> collega_al_server("0.0.0.0", 7474);
	fs -> register_file(file2, maxClient);
	fs -> collega_al_server("0.0.0.0", 7474);
	fs -> register_file(file3, maxClient);
	
	fs -> aspettaContattiClient();	

}
