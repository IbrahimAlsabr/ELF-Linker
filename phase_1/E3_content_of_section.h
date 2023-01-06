#ifndef CONTENT_OF_SECTION   
#define CONTENT_OF_SECTION

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>
#include "readelf.h"

void print_error_and_exit(char *message); 

// void read_elf_header_3(FILE *f, Elf32_Ehdr *ehdr);

void read_section_headers(FILE *f, Elf32_Ehdr *ehdr, Elf32_Shdr *section_tab);

char *read_section_name_table(FILE *f, Elf32_Shdr *shstrtab); 

void close_elf_file(FILE *f); 

Elf32_Shdr *find_section_by_name_or_number(FILE *f, Elf32_Shdr *section_tab, char *section_name_table, int num_sections, char *section_name_or_number);

void print_section(FILE *f, Elf32_Shdr *section, char *section_name_table); 

#endif 