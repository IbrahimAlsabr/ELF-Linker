#include <elf.h>
#include <stdio.h>
//#include <string.h>
//#include <unistd.h>

void read_elf_header(FILE* file, Elf32_Ehdr* ehdr) {
    fread(ehdr, sizeof(Elf32_Ehdr), 1, file);
}

// Lit les en-tetes (headers) de section du fichier donne et les stocke dans le tableau donne
void read_section_headers(FILE* f, Elf32_Ehdr* ehdr, Elf32_Shdr* section_tab) {
    fseek(f, ehdr->e_shoff, SEEK_SET);
    for (int i = 0; i < ehdr->e_shnum; i++) {
        fread(&section_tab[i].sh_name, sizeof(Elf32_Word), 1, f);
        fread(&section_tab[i].sh_type, sizeof(Elf32_Word), 1, f);
        fread(&section_tab[i].sh_flags, sizeof(Elf32_Word), 1, f);
        fread(&section_tab[i].sh_addr, sizeof(Elf32_Addr), 1, f);
        fread(&section_tab[i].sh_offset, sizeof(Elf32_Off), 1, f);
        fread(&section_tab[i].sh_size, sizeof(Elf32_Word), 1, f);
        fread(&section_tab[i].sh_link, sizeof(Elf32_Word), 1, f);
        fread(&section_tab[i].sh_info, sizeof(Elf32_Word), 1, f);
        fread(&section_tab[i].sh_addralign, sizeof(Elf32_Word), 1, f);
        fread(&section_tab[i].sh_entsize, sizeof(Elf32_Word), 1, f);
    }
}
