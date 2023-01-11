#ifndef TABLE_RE   
#define TABLE_RE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "elf.h"

#include "elf_reader.h"

// void read_section(FILE *f, Elf32_Shdr section_tab[], uint32_t offset, uint32_t nb_sect); 

void afficher_type(int type) ;

void afficheTableReimplentation(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *s); 

#endif 