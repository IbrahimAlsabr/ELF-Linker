#ifndef READELF
#define READELF

#include <stdint.h>
#include <elf.h>

void read_section(FILE *f, Elf32_Shdr section_tab[], uint32_t offset, uint32_t nb_sect);

// Lit l'en-tête (header) ELF du fichier donné et le stocke dans la variable donnée
void read_elf_header(FILE *f, Elf32_Ehdr *ehdr);

#endif