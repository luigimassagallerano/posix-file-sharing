#include "../include/File_Condition.h"

File_Condition::File_Condition(const string& nf)
{
	nome_file = nf;
	if(sem_init(&cond, 0, 0) == -1) sys_err("Inizializzazione Condition Fallita!\n");

}

string File_Condition::getFileName()
{ 
	return nome_file;
}

void File_Condition::sospendi()
{
	sem_wait(&cond);
}

void File_Condition::sveglia()
{
	int value;
	while(true){
		value = sem_post(&cond);
		if(value == -1) break;
	}
}


