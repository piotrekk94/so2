#pragma once
#include "stdlib.h"
#include "string.h"

#include "struktury.h"

struct pacjent *dodajPacjenta(char *pesel,char *imie,char *nazwisko);

struct lekarz *dodajLekarza(char *imie,char *nazwisko);

struct wizyta *dodajWizyte(struct termin *t,struct pacjent *p,struct lekarz *l,char potwierdzenie);

struct pacjent *znajdzPacjenta(char *pesel);

struct wizyta *znajdzWizyte(struct termin *t,struct pacjent *p,struct lekarz *l);

void usunPacjenta(char *pesel);