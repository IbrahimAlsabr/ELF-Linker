#ifndef SYMBOLE_TABLE   
#define SYMBOLE_TABLE


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>

#include "elf_reader.h"

void affichesymbol(Elf32_Sym s, int i); 

void read_table_symbol(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *shdr); 


#endif 