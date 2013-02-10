#include "../../../include/FS.h"

int main(){	
	FS* fs = new FS(8282);

	string file = "foto.jpg"; //FILE IN COMUNE

	fs -> collega_al_server("0.0.0.0", 7474);
	fs -> register_file(file, "3");
		
	fs -> aspettaContattiClient();	

}
