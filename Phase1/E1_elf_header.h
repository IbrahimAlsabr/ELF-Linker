#ifndef HEADER   
#define HEADER

#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "elf_reader.h"

void print_header(Elf32_Ehdr *ehdr);

// void read_elf_header(FILE *file, Elf32_Ehdr *ehdr);

// Lit les en-tetes (headers) de section du fichier donne et les stocke dans le tableau donne
void read_section_headers(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *section_tab);

#endif 
