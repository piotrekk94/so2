#!/bin/bash

gcc -std=c11 -Wall -g rejestracja.c dane.c -o r
gcc -std=c11 -Wall -g pacjent.c -o p
gcc -std=c11 -Wall -g lekarz.c -o l
