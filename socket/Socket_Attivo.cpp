#include "../include/Socket_Attivo.h"
#include "../include/msg_err.h"

#define BUFFERSIZE 2000
#define CAST_ADDR(x) (struct sockaddr *)(x)

Socket_Attivo::Socket_Attivo(const string& ip, int porta)
{
	sd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&s_addr, sizeof(struct sockaddr_in));
	s_addr.sin_family = AF_INET;
	s_port = porta;
	s_addr.sin_port = htons(s_port);

	server = gethostbyname(ip.c_str());
	if(server == 0) sys_err("Server Not Found!\n");

	bcopy((char*)server->h_addr, (char*)&s_addr.sin_addr.s_addr, server->h_length);
}

Socket_Attivo::Socket_Attivo(int sock)
{
	sd = sock;
}

void Socket_Attivo::connetti()
{
	if(connect(sd, CAST_ADDR(&s_addr), sizeof(struct sockaddr_in)) < 0) sys_err("Connect Failed!\n");
	cout << "Connesso al Server\n";
}

void Socket_Attivo::invia(const string& msg)
{
	if(write(sd, msg.c_str(), BUFFERSIZE) < 0) sys_err("Write Failed!\n");

}

string Socket_Attivo::ricevi()
{
	char msg[BUFFERSIZE];

	int l = 1;	

	if(read(sd, msg, BUFFERSIZE) < 0) if(l < 0) sys_err("Read Failed!\n");
		
	return msg;
}

void Socket_Attivo::ricevi_file(const string& savepath)
{
	char buffer[BUFFERSIZE];
	int fd, l = 1;

	fd = open(savepath.c_str(), O_WRONLY | O_CREAT, 0666);

	while((l = read(sd, buffer, BUFFERSIZE)) > 0){
		if(l < 0) sys_err("Read Failed!\n");
		if(write(fd, buffer, l) < 0) sys_err("Write File Failed!\n");;
	}

	close(fd);
	

}

void Socket_Attivo::invia_file(const string& path)
{
	int fd, l=1;
	char buffer[BUFFERSIZE];
	
	fd = open(path.c_str(), O_RDONLY);
	
	while((l = read(fd, buffer, BUFFERSIZE)) > 0){
		if(l < 0) sys_err("Read File Failed!\n");
		if(write(sd, buffer, l) < 0) sys_err("Write Failed!\n");
	}
	
	close(fd);

}


void Socket_Attivo::disconnetti()
{
	close(sd);
	cout << "Socket Attivo Closed\n";
}




