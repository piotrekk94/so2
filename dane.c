#include "dane.h"

struct pacjent *pacjenci[PACJENCI];
struct lekarz *lekarze[LEKARZE];
struct urlop *urlopy[LEKARZE];
struct wizyta *wizyty[PACJENCI];

char obciazenieLekarzy[12][30][LEKARZE];
int liczbaLekarzy;
int liczbaPacjentow;
char nieudaneLogowania[PACJENCI];
char zalogowaniPacjenci[PACJENCI];

int odstep(int m1,int d1,int m2,int d2)
{
	if (m1==m2)return d2-d1;
	else return (m2-m1)*30+d2-d1;
}

int znajdzPacjentaPotwierdzenie()
{
	for (int i=0;i<liczbaPacjentow;i++)
		if (wizyty[i]!=NULL)
			if (wizyty[i]->potwierdzenie==1)return i;
	return -1;
}

void usunWizytyDzis(int m,int d)
{
	for (int i=0;i<liczbaPacjentow;i++)
		if (wizyty[i]!=NULL)
			if (wizyty[i]->dzien==d&&wizyty[i]->miesiac==m){
				free(wizyty[i]);
				wizyty[i]=NULL;
			}
}

int lekarzNaId(char *imie,char *nazwisko)
{
	for (int i=0;i<liczbaLekarzy;i++)
		if(lekarze[i]!=NULL)
			if(strcmp(imie,lekarze[i]->imie)==0&&strcmp(nazwisko,lekarze[i]->nazwisko)==0)
				return i;
	return -1;
}

int peselNaId(char pesel[12])
{
	for (int i=0;i<PACJENCI;i++){
		if (pacjenci[i]!=NULL)
			if (strcmp(pesel,pacjenci[i]->pesel)==0)
				return i;
	}
	return -1;
}

struct pacjent *wezPacjenta(int idp)
{
	if (idp>=0||idp<PACJENCI)
		return pacjenci[idp];
	else return NULL;
}

struct wizyta *znajdzWizyte(int idp)
{
	if (idp!=-1)return wizyty[idp];
	else return NULL;
}

int dodajPacjenta(char *pesel,char *imie,char *nazwisko,char *haslo)
{
	struct pacjent *temp=malloc(sizeof(struct pacjent));
	strcpy(temp->pesel,pesel);
	strcpy(temp->imie,imie);
	strcpy(temp->nazwisko,nazwisko);
	strcpy(temp->haslo,haslo);
	if (liczbaPacjentow==PACJENCI)
		return -1;
	if (peselNaId(pesel)!=-1)
		return -2;
	else {
		pacjenci[liczbaPacjentow]=temp;
		liczbaPacjentow++;
		return liczbaPacjentow-1;
	}
}

int dodajLekarza(char *imie,char *nazwisko)
{
	struct lekarz *temp=malloc(sizeof(struct lekarz));
	strcpy(temp->imie,imie);
	strcpy(temp->nazwisko,nazwisko);
	if (liczbaLekarzy==LEKARZE)
		return -1;
	else if(lekarzNaId(imie,nazwisko)!=-1)return -2;
	else {
		lekarze[liczbaLekarzy]=temp;
		liczbaLekarzy++;
		return liczbaLekarzy-1;
	}
}

int dodajWizyte(int miesiac,int dzien,int numer,int idp,int potwierdzenie)
{
	int idl=0;
	char l[LEKARZE];
	for (int i=0;i<liczbaLekarzy;i++)
		l[i]=1;
	for (int i=0;i<liczbaPacjentow;i++)
		if (wizyty[i]!=NULL)
			if (wizyty[i]->miesiac==miesiac&&wizyty[i]->dzien==dzien&&wizyty[i]->numer==numer)
				l[wizyty[i]->idlekarza]=0;
	for (int i=0;i<liczbaLekarzy;i++)
		if (l[i]==1)
			if (obciazenieLekarzy[miesiac-1][dzien-1][idl]>obciazenieLekarzy[miesiac-1][dzien-1][i])
				idl=i;
	if (l[idl]==0||obciazenieLekarzy[miesiac-1][dzien-1][idl]==NUMERY)return -1;
	/*for (int i=0;i<liczbaLekarzy;i++)
		if (obciazenieLekarzy[miesiac][dzien][idl]>obciazenieLekarzy[miesiac][dzien][i])
			idl=i;
	for (int i=0;i<liczbaPacjentow;i++)
		if (wizyty[i]!=NULL)
			if (wizyty[i]->idlekarza==idl&&wizyty[i]->miesiac==miesiac&&wizyty[i]->dzien==dzien&&wizyty[i]->numer==numer)
				return -1;*/
	if (wizyty[idp]!=NULL){
		usunWizyteP(idp);
	}
	struct wizyta *temp=malloc(sizeof(struct wizyta));
	temp->miesiac=miesiac;
	temp->dzien=dzien;
	temp->numer=numer;
	temp->potwierdzenie=potwierdzenie;
	temp->idlekarza=idl;
	wizyty[idp]=temp;
	return 0;
}

int usunWizyteP(int idp)
{
	if (wizyty[idp]!=NULL){
		free(wizyty[idp]);
		wizyty[idp]=NULL;
		return 0;
	}
	else return -1;
}

void usunWizyteL(int miesiac,int dzien,int idl)
{
	for (int i=0;i<liczbaPacjentow;i++)
		if (wizyty[i]!=NULL)
			if (wizyty[i]->idlekarza==idl&&wizyty[i]->miesiac==miesiac&&wizyty[i]->dzien==dzien){
				free(wizyty[i]);
				wizyty[i]=NULL;
			}
}

int zalogujPacjenta(char *pesel,char *haslo)
{
	int idp=peselNaId(pesel);
	if (zalogowaniPacjenci[idp]==1)return -4;
	if (idp==-1)return -3;
	if (nieudaneLogowania[idp]>PROG_LOGOWAN)
		return -2;
	else if (strcmp(haslo,pacjenci[idp]->haslo)!=0){
		nieudaneLogowania[idp]++;
		return -1;
	}
	else {
		zalogowaniPacjenci[idp]=1;
		nieudaneLogowania[idp]=0;
		return idp;
	}
}
int wylogujPacjenta(int idp)
{
	if (zalogowaniPacjenci[idp]==0)return -2;
	zalogowaniPacjenci[idp]=0;
	return idp;
}
