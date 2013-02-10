#include "../../../include/FS.h"

int main(){	
	FS* fs = new FS(8383);

	string maxClient = "2";
	string file1 = "friends.avi"; //FILE IN COMUNE
	string file2 = "foto4.jpg";
	string file3 = "foto5.jpg"; 

	fs -> collega_al_server("0.0.0.0", 7474);
	fs -> register_file(file1, maxClient);
	fs -> collega_al_server("0.0.0.0", 7474);
	fs -> register_file(file2, maxClient);
	fs -> collega_al_server("0.0.0.0", 7474);
	fs -> register_file(file3, maxClient);
	

	fs -> aspettaContattiClient();	

}
