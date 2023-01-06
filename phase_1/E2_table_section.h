#ifndef SECTION
#define SECTION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "elf.h"

#include "readelf.h"

// void read_section(FILE *f, Elf32_Shdr section_tab[], uint32_t offset, uint32_t nb_sect);

void afficheTableSections(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *section);

#endif