#include "../include/Socket_Passivo.h"
#include "../include/msg_err.h"

#define BUFFERSIZE 2000
#define CAST_ADDR(x) (struct sockaddr *)(x)

Socket_Passivo::Socket_Passivo(int porta)
{
	port = porta;
	addrlen = 1000;
	base_sd = init_sd(porta);
}

int Socket_Passivo::init_sd(int porta)
{
	int sd;

	bzero(&my_addr, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(porta);
   	my_addr.sin_addr.s_addr = INADDR_ANY;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(bind(sd, CAST_ADDR(&my_addr), sizeof(my_addr)) < 0) sys_err("Bind Failed!\n");

	if(listen(sd, 5) < 0) sys_err("Listen Failed!\n");

	printf("Server listening on port %d\n", porta);
	
	return sd;

}

Socket_Attivo* Socket_Passivo::accetta()
{
	curr_sd = accept(base_sd, CAST_ADDR(&c_add), ((socklen_t*)&addrlen));
	if(curr_sd < 0) sys_err("Accept Failed!\n");
	return new Socket_Attivo(curr_sd);
}
	
string Socket_Passivo::getIP()
{
	char *z;
	z = inet_ntoa(*(struct in_addr *)&my_addr.sin_addr.s_addr);
	
	return z;
}

string Socket_Passivo::getPorta()
{
	ostringstream oss;
	oss << port;

	string res = oss.str();	
	
	return res;
}

void Socket_Passivo::disconnetti()
{
	close(base_sd);
	cout << "Socket Passivo Closed!\n";
}


