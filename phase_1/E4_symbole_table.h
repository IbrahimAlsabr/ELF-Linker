#ifndef SYMBOLE_TABLE   
#define SYMBOLE_TABLE


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>

#include "readelf.h"

void affichesymbol(Elf32_Sym s, int i, char *strtab); 

void read_table_symbol(FILE *f, Elf32_Ehdr header); 


#endif 