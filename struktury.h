#pragma once

struct msgbuf
{
	long type;
	int subtype;
	int sender;
	char msg[256];
};
struct pacjent
{
	char pesel[12];
	char imie[16];
	char nazwisko[16];
	char haslo[9];
};
struct lekarz
{
	char imie[16];
	char nazwisko[16];
};
struct wizyta
{
	int idlekarza;
	int potwierdzenie;
	int miesiac;
	int dzien;
	int numer;
};
struct urlop
{
	int miesiac[2];
	int dzien[2];
};
