#include "dane.h"

struct pacjent *pacjenci=NULL;
struct lekarz *lekarze=NULL;
struct wizyta *wizyty=NULL;

struct pacjent *dodajPacjenta(char *pesel,char *imie,char *nazwisko)
{
	struct pacjent *temp=malloc(sizeof(struct pacjent));
	strcpy(temp->pesel,pesel);
	strcpy(temp->imie,imie);
	strcpy(temp->nazwisko,nazwisko);
	temp->next=NULL;
	if (pacjenci==NULL)
		pacjenci=temp;
	else
	{
		struct pacjent *ptr=pacjenci;
		while(ptr->next!=NULL)
			ptr=ptr->next;
		ptr->next=temp;
	}
	return temp;
}

struct lekarz *dodajLekarza(char *imie,char *nazwisko)
{
	struct lekarz *temp=malloc(sizeof(struct lekarz));
	strcpy(temp->imie,imie);
	strcpy(temp->nazwisko,nazwisko);
	temp->next=NULL;
	if (lekarze==NULL)
		lekarze=temp;
	else
	{
		struct lekarz *ptr=lekarze;
		while(ptr->next!=NULL)
			ptr=ptr->next;
		ptr->next=temp;
	}
	return temp;
}

struct wizyta *dodajWizyte(struct termin *t,struct pacjent *p,struct lekarz *l,char potwierdzenie)
{
	struct wizyta *temp=malloc(sizeof(struct wizyta));
	temp->termin=t;
	temp->pacjent=p;
	temp->lekarz=l;
	temp->potwierdzenie=potwierdzenie;
	temp->next=NULL;
	if (wizyty==NULL)
		wizyty=temp;
	else
	{
		struct wizyta *ptr=wizyty;
		while(ptr->next!=NULL)
			ptr=ptr->next;
		ptr->next=temp;
	}
	return temp;
}
struct pacjent *znajdzPacjenta(char *pesel)
{
	struct pacjent *ptr=pacjenci;
	while (ptr!=NULL)
		if (!strcmp(ptr->pesel,pesel))return ptr;
		else ptr=ptr->next;
	return NULL;
}
struct lekarz *znajdzLekarza(char *imie,char *nazwisko)
{
	struct lekarz *ptr=lekarze;
	while(ptr!=NULL)
	{
		if ((!strcmp(nazwisko,ptr->nazwisko))&&(!strcmp(imie,ptr->imie)))return ptr;
		else ptr=ptr->next;
	}
	return NULL;
}
struct wizyta *znajdzWizyte(struct termin *t,struct pacjent *p,struct lekarz *l)
{
	struct wizyta *ptr=wizyty;
	if (t)
	{
		while (ptr!=NULL)
			if(ptr->termin->godzina==t->godzina&&ptr->termin->dzien==t->dzien&&ptr->termin->miesiac==t->miesiac&&ptr->termin->rok==t->rok)return ptr;
			else ptr=ptr->next;
		return NULL;
	}
	else if (p)
	{
		while (ptr!=NULL)
			if (!strcmp(ptr->pacjent->pesel,p->pesel))return ptr;
			else ptr=ptr->next;
		return NULL;
	}
	else if (l)
	{
		while (ptr!=NULL)
			if((!strcmp(l->imie,ptr->lekarz->imie))&&(!strcmp(l->nazwisko,ptr->lekarz->nazwisko)))return ptr;
			else ptr=ptr->next;
		return NULL;
	}
	else return NULL;
}
void usunPacjenta(char *pesel)
{
	struct pacjent *ptr=pacjenci;
	if (ptr==NULL)return;
	else if (!strcmp(ptr->pesel,pesel))
	{
		pacjenci=ptr->next;
		free(ptr);
	}
	else while (ptr->next!=NULL)
		if (!strcmp(ptr->next->pesel,pesel))
		{
			struct pacjent *temp=ptr->next;
			ptr->next=temp->next;
			free (temp);
			return;
		}
		else ptr=ptr->next;
}
void usunLekarza(char *imie,char *nazwisko)
{
	struct lekarz *ptr=lekarze;
	if (ptr==NULL)return;
	else if ((!strcmp(ptr->nazwisko,nazwisko))&&(!strcmp(ptr->imie,imie)))
	{
		lekarze=ptr->next;
		free(ptr);
	}
	else while (ptr->next!=NULL)
		if ((!strcmp(ptr->nazwisko,nazwisko))&&(!strcmp(ptr->imie,imie)))
		{
			struct lekarz *temp=ptr->next;
			ptr->next=temp->next;
			free (temp);
			return;
		}
		else ptr=ptr->next;
}