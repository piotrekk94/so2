#pragma once
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "struktury.h"
#define PACJENCI 20
#define LEKARZE 5
#define PROG_LOGOWAN 5
#define NUMERY 8

int odstep(int m1,int d1,int m2,int d2);

int znajdzPacjentaPotwierdzenie();

void usunWizytyDzis(int m,int d);

int lekarzNaId(char *imie,char *nazwisko);

int peselNaId(char pesel[12]);

struct pacjent *wezPacjenta(int idp);

struct wizyta *znajdzWizyte(int idp);

int dodajPacjenta(char *pesel,char *imie,char *nazwisko,char *haslo);

int dodajLekarza(char *imie,char *nazwisko);

int dodajWizyte(int miesiac,int dzien,int numer,int idp,int potwierdzenie);

int usunWizyteP(int idp);

void usunWizyteL(int miesiac,int dzien,int idl);

int zalogujPacjenta(char *pesel,char *haslo);

int wylogujPacjenta(int idp);
