#define _SVID_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "struktury.h"
#include "dane.h"

int id;

extern char obciazenieLekarzy[12][30][LEKARZE];
extern struct lekarz *lekarze[LEKARZE];
extern struct wizyta *wizyty[PACJENCI];
extern int liczbaPacjentow;
extern int liczbaLekarzy;

int miesiac=1,dzien=1;
int nastepnyDzien=0;

void funkcja()
{
	int wybor;
	printf("Co chcesz zrobic?\n1.Przejsc o X dni do przodu\n2.Wyswietlic wszytskie wizyty\n");
	scanf("%d",&wybor);
	if (wybor==1){
		scanf("%d",&nastepnyDzien );
	}
	else if (wybor==2){
		for (int i=0;i<liczbaPacjentow;i++)
			if (wizyty[i]==NULL)printf("Pacjent o ID :%d brak wizyty\n",i);
			else printf("Pacjent o ID :%d wizyta %d.%d:%d u %d , %d\n",i,wizyty[i]->dzien,wizyty[i]->miesiac,wizyty[i]->numer,wizyty[i]->idlekarza,wizyty[i]->potwierdzenie);
	}
	signal(SIGINT,funkcja);
}

void zakonczDzien()
{
	usunWizytyDzis(miesiac,dzien);
	int idp=znajdzPacjentaPotwierdzenie();
	while (idp!=-1){
		struct wizyta *w=znajdzWizyte(idp);
		if (w!=NULL&&odstep(w->miesiac,w->dzien,miesiac,dzien)<14){
			usunWizyteP(idp);
		}
		idp=znajdzPacjentaPotwierdzenie();
	}
	dzien++;
	if (dzien==31){
		miesiac++;
		dzien=1;
	}
	if (miesiac==12){
		printf("Koniec\n");
		exit(0);
	}
}

void potwierdzWizyte(struct msgbuf *rcv)
{
	struct msgbuf snd;
	int idp;
	idp=peselNaId(rcv->msg);
	if (wizyty[idp]!=NULL&&wizyty[idp]->potwierdzenie==1){
		wizyty[idp]->potwierdzenie=0;
		snd.subtype=0;
	}
	else (snd.subtype=1);
	snd.type=rcv->sender;
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
}

void informacjeOWizycie(struct msgbuf *rcv)
{
	struct msgbuf snd;
	int idp=peselNaId(rcv->msg);
	snd.type=rcv->sender;
	if (idp!=-1){
		if(wizyty[idp]!=NULL){
			snd.subtype=0;
			sprintf(snd.msg,"%2d.%2d:%d u %s %s",wizyty[idp]->dzien,wizyty[idp]->miesiac,wizyty[idp]->numer,lekarze[wizyty[idp]->idlekarza]->imie,lekarze[wizyty[idp]->idlekarza]->nazwisko);
		}
		else snd.subtype=1;
	}
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
}

void odwolajWizyte(struct msgbuf *rcv)
{
	struct msgbuf snd;
	snd.type=rcv->sender;
	snd.msg[0]='\0';
	int r=usunWizyteP(peselNaId(rcv->msg));
	if (r==0)snd.subtype=0;
	else snd.subtype=1;
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	printf("%s odwolal swoja wizyte\n",rcv->msg);
}

void przesunWizyte(struct msgbuf *rcv)
{
	struct msgbuf snd;
	int idp=peselNaId(rcv->msg);
	snd.type=rcv->sender;
	snd.subtype=1;
	int potw=wizyty[idp]->potwierdzenie;
	for (int i=wizyty[idp]->miesiac;i<13;i++)
		for (int j=1;j<31;j++){
			if (i==wizyty[idp]->miesiac)j=wizyty[idp]->dzien;
			for (int k=0;k<NUMERY;i++){
				if (j==wizyty[idp]->dzien)
					k=wizyty[idp]->numer+1;
					if (dodajWizyte(i,j,k,idp,potw)==0){
						sprintf(snd.msg,"%d %d:%d",i,j,k);
						snd.subtype=0;
						i=13;
						j=31;
						break;
					}
			}
		}
		msgsnd(id,&snd,sizeof(struct msgbuf),0);
		if (snd.subtype==0)printf("%s przesunal wizyte na %s",rcv->msg,snd.msg);
		else printf("%s probowal przesunac wizyte\n",rcv->msg);
}

void rejestrujWizyte(struct msgbuf *rcv)
{
	char *msc,*dz,*numer,*pesel;
	struct msgbuf snd;
	int m,d,n,idp,potwierdzenie=0;
	snd.type=rcv->sender;
	pesel=strtok(rcv->msg,";");
	msc=strtok(NULL,";");
	dz=strtok(NULL,";");
	numer=strtok(NULL,";");
	sscanf(msc,"%d",&m);
	sscanf(dz,"%d",&d);
	sscanf(numer,"%d",&n);
	idp=peselNaId(pesel);
	if (odstep(miesiac,dzien,m,d)>60)
		potwierdzenie=1;
	printf("%d\n",odstep(miesiac,dzien,m,d));
	if (idp!=-1&&m>=miesiac&&m<12&&d>=0&&d<30&&n>=0&&n<NUMERY&&(miesiac!=m||dzien<d)){
		if (dodajWizyte(m,d,n,idp,potwierdzenie)==0)
			snd.subtype=0;
		else snd.subtype=1;
	}
	else snd.subtype=1;
	snd.msg[0]='\0';
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	if (snd.subtype==0)printf("Zarejestrowano wizyte %s o %d.%d:%d\n",pesel,m,d,n);
	else printf("%s probowal zarejestrowac wizyte\n",pesel);
}

void terminyWDniu(struct msgbuf *rcv)
{
	struct msgbuf snd;
	char *miesiac,*dzien;
	int m,d;
	miesiac=strtok(rcv->msg,";");
	dzien=strtok(NULL,";");
	sscanf(miesiac,"%d",&m);
	sscanf(dzien,"%d",&d);
	char wolneTerminy[NUMERY];
	snd.msg[0]='\0';
	snd.type=rcv->sender;
	snd.subtype=0;
	for (int i=0;i<NUMERY;i++)
		wolneTerminy[i]=0;
	for (int i=0;i<liczbaPacjentow;i++)
		if (wizyty[i]!=NULL)
			if (wizyty[i]->dzien==d&&wizyty[i]->miesiac==m)
				wolneTerminy[wizyty[i]->numer]++;
	for (int i=0;i<LEKARZE;i++)
		if (obciazenieLekarzy[m-1][d-1][i]==NUMERY)
			for (int j=0;j<NUMERY;j++)
				wolneTerminy[j]++;
	for (int i=0;i<NUMERY;i++)
		if (wolneTerminy[i]<liczbaLekarzy){
			char temp[4];
			sprintf(temp,"%d,",i);
			strcat(snd.msg,temp);
		}

	msgsnd(id,&snd,sizeof(struct msgbuf),0);
}

void terminyLekarza(struct msgbuf *rcv)
{
	char *imie,*nazwisko;
	struct msgbuf snd;
	int idl;
	imie=strtok(rcv->msg,";");
	nazwisko=strtok(NULL,";");
	idl=lekarzNaId(imie,nazwisko);
	snd.msg[0]='\0';
	snd.type=rcv->sender;
	snd.subtype=0;
	if (idl==-1){
		snd.subtype=1;
	}
	else {
		for (int i=0;i<liczbaPacjentow;i++)
			if (wizyty[i]!=NULL)
				if (wizyty[i]->idlekarza==idl){
					char temp[10];
					sprintf(temp,"%2d.%2d:%d;",wizyty[i]->miesiac,wizyty[i]->dzien,wizyty[i]->numer);
					strcat(snd.msg,temp);
				}
	}
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
}

void urlop(struct msgbuf *rcv)
{
	char *m,*d,*imie,*nazwisko;
	int msc,dz,idl;
	imie=strtok(rcv->msg,";");
	nazwisko=strtok(NULL,";");
	m=strtok(NULL,";");
	d=strtok(NULL,";");
	idl=lekarzNaId(imie,nazwisko);
	sscanf(m,"%d",&msc);
	sscanf(d,"%d",&dz);
	if (msc>=1&&msc<=12&&dz>=0&&dz<=30&&odstep(miesiac,dzien,msc,dz)>0&&idl!=-1){
		usunWizytyDzis(msc,dz);
		obciazenieLekarzy[msc-1][dz-1][idl]=NUMERY;
	}
	printf("%s %s ma urlop %d.%2d \n",imie,nazwisko,dz,msc);
}

void lekarzeWTerminie(struct msgbuf *rcv)
{
	struct msgbuf snd;
	char *miesiac,*dzien;
	int m,d;
	miesiac=strtok(rcv->msg,";");
	dzien=strtok(NULL,";");
	sscanf(miesiac,"%d",&m);
	sscanf(dzien,"%d",&d);
	snd.msg[0]='\0';
	snd.type=rcv->sender;
	if (m>0&&m<=12&&d>0&&d<=30){
		for (int i=0;i<LEKARZE;i++)
			if (obciazenieLekarzy[m-1][d-1][i]<NUMERY){
				strcat(snd.msg,lekarze[i]->imie);
				strcat(snd.msg," ");
				strcat(snd.msg,lekarze[i]->nazwisko);
				strcat(snd.msg,"\n");
			}
		snd.subtype=0;
	}
	else snd.subtype=1;
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
}

void zalogujP(struct msgbuf *rcv)
{
	int idp;
	char *pesel,*haslo;
	pesel=strtok(rcv->msg,";");
	haslo=strtok(NULL,";");
	struct msgbuf snd;
	idp=zalogujPacjenta(pesel,haslo);
	sprintf(snd.msg,"%d;",idp);
	if (wizyty[idp]!=NULL&&wizyty[idp]->potwierdzenie==1)strcat(snd.msg,"Wizyta wymaga potwierdzenia;");
	snd.type=rcv->sender;
	snd.subtype=0;
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	printf("%s zalogowal sie\n",pesel);
}

void rejestrujP(struct msgbuf *rcv)
{
	int idp;
	char *imie,*nazwisko,*pesel,*haslo,temp[3];
	struct msgbuf snd;
	imie=strtok(rcv->msg,";");
	nazwisko=strtok(NULL,";");
	pesel=strtok(NULL,";");
	haslo=strtok(NULL,";");
	idp=dodajPacjenta(pesel,imie,nazwisko,haslo);
	sprintf(temp,"%d",idp);
	snd.type=rcv->sender;
	snd.subtype=0;
	strcpy(snd.msg,temp);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	printf("%s %s zarejestrowal sie\n",imie,nazwisko );
}

void rejestrujL(struct msgbuf *rcv)
{
	int idl;
	char *imie,*nazwisko,temp[3];
	struct msgbuf snd;
	imie=strtok(rcv->msg,";");
	nazwisko=strtok(NULL,";");
	idl=dodajLekarza(imie,nazwisko);
	sprintf(temp,"%d",idl);
	snd.type=rcv->sender;
	snd.subtype=0;
	strcpy(snd.msg,temp);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	printf("%s %s zarejestrowal sie\n",imie,nazwisko );
}

void wylogujP(struct msgbuf *rcv)
{
	struct msgbuf snd;
	char temp[3];
	int idp=peselNaId(rcv->msg);
	if (idp>=0)idp=wylogujPacjenta(idp);
	sprintf(temp,"%d",idp);
	snd.type=rcv->sender;
	snd.subtype=0;
	strcpy(snd.msg,temp);
	msgsnd(id,&snd,sizeof(struct msgbuf),0);
	printf("%s wylogowal sie\n",rcv->msg);
}

int main()
{
	id=msgget(1,0644|IPC_CREAT);
	printf("Dzisiaj jest %2d.%2d\n",dzien,miesiac);
	signal(SIGINT,funkcja);
	while(1)
	{
		struct msgbuf rcv;
		if (msgrcv(id,&rcv,sizeof(struct msgbuf),-9,IPC_NOWAIT)!=-1){
			if (rcv.type==1){
				if (rcv.subtype==0)
					rejestrujP(&rcv);
				else if (rcv.subtype==1)
					zalogujP(&rcv);
				else if (rcv.subtype==2)
					wylogujP(&rcv);
				else if (rcv.subtype==3)
					rejestrujL(&rcv);
				else if (rcv.subtype==4)
					urlop(&rcv);
			}
			else if (rcv.type==2){
				if (rcv.subtype==0)
					lekarzeWTerminie(&rcv);
				else if (rcv.subtype==1)
					terminyLekarza(&rcv);
				else if (rcv.subtype==2)
					terminyWDniu(&rcv);
				else if (rcv.subtype==3)
					rejestrujWizyte(&rcv);
				else if (rcv.subtype==4)
					odwolajWizyte(&rcv);
				else if (rcv.subtype==5)
					informacjeOWizycie(&rcv);
				else if (rcv.subtype==6)
					potwierdzWizyte(&rcv);
				else if (rcv.subtype==7)
					przesunWizyte(&rcv);
			}
		}
		if (nastepnyDzien>0){
			zakonczDzien();
			nastepnyDzien--;
			printf("Dzisiaj jest %2d.%2d\n",dzien,miesiac);
		}
	}
}
