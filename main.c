#include "stdio.h"
#include "stdlib.h"
#include "dane.h"

int main()
{
	struct pacjent *ozjasz=dodajPacjenta("1234567890","Ozjasz","Goldberg");
	struct lekarz *krul=dodajLekarza("Krul","Kucy");
	struct termin t={.godzina=8,.dzien=20,.miesiac=12,.rok=2015};
	struct wizyta *wiz=dodajWizyte(&t,ozjasz,krul,0);
	return 0;
}