#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "elf.h"

#include "E3_content_of_section.h"
#include "elf_reader.h"

void read_section(FILE *f, Elf32_Shdr section_tab[], uint32_t offset, uint32_t nb_sect)
{

    int i = 0;

    fseek(f, offset, SEEK_SET); // Placement de l'indicateur de position au niveau de l'entête de la table des sections

    for (i = 0; i < nb_sect; i++)
    {
        fread(&section_tab[i].sh_name, sizeof(Elf32_Word), 1, f);      // Lecture du name
        fread(&section_tab[i].sh_type, sizeof(Elf32_Word), 1, f);      // Lecture du type
        fread(&section_tab[i].sh_flags, sizeof(Elf32_Word), 1, f);     // Lecture des flags
        fread(&section_tab[i].sh_addr, sizeof(Elf32_Addr), 1, f);      // Lecture de l'adresse
        fread(&section_tab[i].sh_offset, sizeof(Elf32_Off), 1, f);     // Lecture de l'offset
        fread(&section_tab[i].sh_size, sizeof(Elf32_Word), 1, f);      // Lecture de la taille
        fread(&section_tab[i].sh_link, sizeof(Elf32_Word), 1, f);      // Lecture du lien
        fread(&section_tab[i].sh_info, sizeof(Elf32_Word), 1, f);      // Lecture de l'info
        fread(&section_tab[i].sh_addralign, sizeof(Elf32_Word), 1, f); // Lecutre de l'addralign
        fread(&section_tab[i].sh_entsize, sizeof(Elf32_Word), 1, f);   // Lecture de l'entsize
    }
}

// Lit l'en-tête (header) ELF du fichier donné et le stocke dans la variable donnée
void read_elf_header(FILE *f, Elf32_Ehdr *ehdr)
{
    if (!fread(ehdr, sizeof(Elf32_Ehdr), 1, f))
    {
        fprintf(stderr, "cannot read ELF header from file");
        exit(3);
    }
}
