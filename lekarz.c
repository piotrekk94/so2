#define _SVID_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "struktury.h"

char imie[16];
char nazwisko[16];
int id;

void urlop()
{
	struct msgbuf snd;
	char poczatekm[3],poczatekd[3];
	snd.type=1;
	snd.subtype=4;
	snd.sender=getpid();
	printf("Podaj miesiac urlopu :");
	scanf("%s",poczatekm);
	printf("Podaj dzien urlopu :");
	scanf("%s",poczatekd);
	strcpy(snd.msg,imie);
	strcat(snd.msg,";");
	strcat(snd.msg,nazwisko);
	strcat(snd.msg,";");
	strcat(snd.msg,poczatekm);
	strcat(snd.msg,";");
	strcat(snd.msg,poczatekd);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
}

void rejestruj()
{
	struct msgbuf snd,rcv;
	int idl;
	snd.type=1;
	snd.subtype=3;
	snd.sender=getpid();
	strcpy(snd.msg,imie);
	strcat(snd.msg,";");
	strcat(snd.msg,nazwisko);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	sscanf(rcv.msg,"%d",&idl);
	if (idl==-1)printf("Brak miejsc w bazie\n");
	else if (idl==-2)printf("Juz istnieje lekarz o tych danych");
	else printf("Zarejestrowany\nID lekarza :%d\n",idl);
	if (idl<0) exit(0);
}

void init()
{
	printf("Podaj imie :");
	scanf("%s",imie);
	printf("Podaj nazwisko :");
	scanf("%s",nazwisko);
}

int main()
{
	id=msgget(1,0644|IPC_CREAT);
	init();
	while (1) {
		int wybor;
		printf("1.Zarejestruj sie w bazie\n2.Wez urlop\n");
		scanf("%d",&wybor);
		if (wybor==1)
			rejestruj();
		else if (wybor==2)
			urlop();
	}
	return 0;
}
