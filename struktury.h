#pragma once

struct msgbuf
{
	long type;
	int subtype;
	int sender;
	char shortmsg[16];
	char longmsg[256];
};
struct pacjent
{
	struct pacjent *next;
	char pesel[12];
	char imie[16];
	char nazwisko[16];
};
struct lekarz
{
	struct lekarz *next;
	char imie[16];
	char nazwisko[16];
};
struct termin
{
	char godzina;
	char dzien;
	char miesiac;
	short int rok;
};
struct wizyta
{
	struct wizyta *next;
	struct termin *termin;
	struct pacjent *pacjent;
	struct lekarz *lekarz;
	char potwierdzenie;
};