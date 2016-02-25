#define _SVID_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "struktury.h"

char imie[16]="Krul";
char nazwisko[16]="Korwin";
char pesel[12]="21362138";
char haslo[9]="xD";
int id,idpacjenta,zalogowany=0;
char oczekiwanieNaPotwierdzenie=0;

void funkcja()
{
	if (oczekiwanieNaPotwierdzenie==1)
		oczekiwanieNaPotwierdzenie=0;
	signal(SIGINT,funkcja);
}

void init()
{
	printf("Podaj imie :");
	scanf("%s",imie);
	printf("Podaj nazwisko :");
	scanf("%s",nazwisko);
	printf("Podaj pesel :");
	scanf("%s",pesel);
	printf("Podaj haslo :");
	scanf("%s",haslo);
}

void potwierdzWizyte()
{
	struct msgbuf snd,rcv;
	snd.sender=getpid();
	snd.type=2;
	snd.subtype=6;
	strcpy(snd.msg,pesel);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	if (rcv.subtype==0)
		printf("Potwierdzono\n");
	else printf("Blad\n");
}

void informacjeOWizycie()
{
	struct msgbuf snd,rcv;
	snd.sender=getpid();
	snd.type=2;
	snd.subtype=5;
	strcpy(snd.msg,pesel);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	if (rcv.subtype==0)
		printf("%s\n",rcv.msg);
	else printf("Brak wizyt\n");
}

void odwolajWizyte()
{
	struct msgbuf snd,rcv;
	snd.type=2;
	snd.subtype=4;
	snd.sender=getpid();
	strcpy(snd.msg,pesel);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	if (rcv.subtype==0)printf("Odwolano wizyte\n");
	else printf("Blad\n");
}

void przesunWizyte()
{
	struct msgbuf snd,rcv;
	snd.type=2;
	snd.subtype=7;
	snd.sender=getpid();
	strcpy(snd.msg,pesel);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	if (rcv.subtype==0){
		printf("Wizyte przeniesiono na %s\n",rcv.msg);
	}
	else printf("Blad\n");
}

void rejestrujWizyte()
{
	struct msgbuf snd,rcv;
	char miesiac[3],dzien[3],godzina[3];
	snd.type=2;
	snd.subtype=3;
	snd.sender=getpid();
	printf("Podaj miesiac\n");
	scanf("%2s",miesiac);
	printf("Podaj dzien\n");
	scanf("%2s",dzien);
	printf("Podaj numer\n");
	scanf("%2s",godzina);
	strcpy(snd.msg,pesel);
	strcat(snd.msg,";");
	strcat(snd.msg,miesiac);
	strcat(snd.msg,";");
	strcat(snd.msg,dzien);
	strcat(snd.msg,";");
	strcat(snd.msg,godzina);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	if (rcv.subtype==0)printf("Zarejestrowano\n");
	else if (rcv.subtype==1)printf("Zmieniono termin wizyty\n");
	else printf("Blad\n");
}

void terminyWDniu()
{
	struct msgbuf snd,rcv;
	char miesiac[3],dzien[3];
	printf("Podaj miesiac\n");
	scanf("%2s",miesiac);
	printf("Podaj dzien\n");
	scanf("%2s",dzien);
	snd.type=2;
	snd.subtype=2;
	snd.sender=getpid();
	strcat(snd.msg,miesiac);
	strcat(snd.msg,";");
	strcat(snd.msg,dzien);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	printf("Wolne terminy %s\n",rcv.msg );
}

void terminyLekarza()
{
	struct msgbuf snd,rcv;
	char imiel[16],nazwiskol[16];
	snd.type=2;
	snd.subtype=1;
	snd.sender=getpid();
	printf("Podaj imie lekarza\n");
	scanf("%s",imiel);
	printf("Podaj nazwisko lekarza\n");
	scanf("%s",nazwiskol);
	strcpy(snd.msg,imiel);
	strcat(snd.msg,";");
	strcat(snd.msg,nazwiskol);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	printf("Zajete terminy terminy :%s",rcv.msg);
}

void lekarzeWTerminie()
{
	struct msgbuf snd,rcv;
	char miesiac[3],dzien[3];
	snd.type=2;
	snd.subtype=0;
	snd.sender=getpid();
	printf("Podaj miesiac :");
	scanf("%2s",miesiac);
	printf("Podaj dzien :");
	scanf("%2s",dzien);
	strcpy(snd.msg,miesiac);
	strcat(snd.msg,";");
	strcat(snd.msg,dzien);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	if (rcv.subtype==0)printf("Dostepni lekarze :\n%s",rcv.msg);
	else printf("Zla data\n");
}

void rejestruj()
{
	struct msgbuf snd,rcv;
	int idp;
	snd.type=1;
	snd.subtype=0;
	snd.sender=getpid();
	strcpy(snd.msg,imie);
	strcat(snd.msg,";");
	strcat(snd.msg,nazwisko);
	strcat(snd.msg,";");
	strcat(snd.msg,pesel);
	strcat(snd.msg,";");
	strcat(snd.msg,haslo);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	sscanf(rcv.msg,"%d",&idp);
	if (idp==-2)printf("Użytkownik już zarejestrowany\n");
	else if (idp==-1)printf("Brak miejsc w bazie\n");
	else printf("Zarejestrowany\nID pacjenta :%d\n",idp);
}

void zaloguj()
{
	struct msgbuf snd,rcv;
	int idp;
	char *idpac,*wiadomosc;
	snd.type=1;
	snd.subtype=1;
	snd.sender=getpid();
	strcpy(snd.msg,pesel);
	strcat(snd.msg,";");
	strcat(snd.msg,haslo);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	//sscanf(rcv.msg,"%d",&idp);
	idpac=strtok(rcv.msg,";");
	wiadomosc=strtok(NULL,";");
	sscanf(idpac,"%d",&idp);
	if (idp==-4)printf("Użytkownik jest już zalogowany gdzieś indziej\n");
	else if (idp==-3)printf("Brak konta dla podanego peselu\n");
	else if (idp==-2)printf("Konto zablokowane\n");
	else if (idp==-1)printf("Zle haslo\n");
	else {
		printf("Zalogowany\nID pacjenta :%d\n",idp);
		if (wiadomosc!=NULL)
			printf("%s\n",wiadomosc);
		idpacjenta=idp;
		zalogowany=1;
	}
}

void wyloguj()
{
	struct msgbuf snd,rcv;
	int idp;
	snd.type=1;
	snd.subtype=2;
	snd.sender=getpid();
	strcpy(snd.msg,pesel);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	msgrcv(id,&rcv,sizeof(struct msgbuf),getpid(),0);
	sscanf(rcv.msg,"%d",&idp);
	if (idp==-2)printf("Nie można wylogowac\n");
	else if (idp==-1)printf("Nie ma pacjenta o podanym peselu\n");
	else {
		zalogowany=0;
		idpacjenta=-1;
		printf("Wylogowano\n");
	}
}

int main()
{
	id=msgget(1,0644|IPC_CREAT);
	init();
	signal(SIGINT,funkcja);
	while (1) {
		int wybor;
		printf("1.Zaloguj\n2.Rejestruj pacjenta\n3.Rejestruj wizyte\n4.Przesun wizyte\n5.Sprawdz wolne terminy w dniu\n6.Odwolaj wizyte\n7.Zmien haslo logowania\n8.Wyloguj\n9.Potwierdz wizyte\n10.Informacja o wizycie\n11.Wyjdz\n");
		scanf("%d",&wybor);
		switch (wybor) {
			case 1:
			if (zalogowany==0)
				zaloguj();
			else printf("Juz jestes zalogowany\n");
			break;
			case 2:
			rejestruj();
			break;
			case 3:
			if (zalogowany==1)
				rejestrujWizyte();
			else printf("Nie jestes zalogowany\n");
			break;
			case 4:
			if (zalogowany==1)
				przesunWizyte();
			else printf("Nie jestes zalogowany\n");
			break;
			case 5:
			terminyWDniu();
			break;
			case 6:
			if (zalogowany==1)
				odwolajWizyte();
			else printf("Nie jestes zalogowany\n");
			break;
			case 7:
			scanf("%s",haslo);
			break;
			case 8:
			if (zalogowany==1)
				wyloguj();
			else printf("Nie jestes zalogowany\n");
			break;
			case 9:
			if (zalogowany==1)
				potwierdzWizyte();
			else printf("Nie jestes zalogowany\n");
			break;
			case 10:
			if (zalogowany==1)
				informacjeOWizycie();
			else printf("Nie jestes zalogowany\n");
			break;
			case 11:
			if (zalogowany==1)
				wyloguj();
			return 0;
			break;
		}
	}
	return 0;
}
