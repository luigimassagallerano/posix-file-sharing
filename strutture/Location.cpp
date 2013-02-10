#include "../include/Location.h"

Location::Location(const string& ip, const string& porta, int mc)
{
	ip_FS = ip;
	porta_FS = porta;
	if(sem_init(&maxClient, 0, mc) == -1) sys_err("Inizializzazione maxClient Fallita!\n");
}

Location::Location(const string& ip, const string& porta)
{
	ip_FS = ip;
	porta_FS = porta;
}


string Location::getLocation()
{
	//prima di poter ottenere la Location bisogna acquisire maxClient
	sem_wait(&maxClient);
	//solo se ci sono slot liberi il thread otterrà la Location
	return ip_FS+":"+porta_FS;
}

bool Location::equals(Location& l)
{
	if( (ip_FS+":"+porta_FS).compare(l.getLocation()) == 0) return true;
	return false; 
}

void Location::liberaSlot()
{
	sem_post(&maxClient); //sarà il DFR a risvegliare i thread client che aspettano uno slot
}

bool Location::haveSlot()
{
	int value;
	sem_getvalue(&maxClient, &value);
	if(value > 0) return true;
	return false;
}

string Location::stampa()
{
	return ip_FS+":"+porta_FS;
}
