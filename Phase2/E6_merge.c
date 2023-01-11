#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>
#include "elf.h"
#include <string.h>
#include <math.h>
/*
int find_section_by_name_and_type(Elf32_Shdr *section_tab2, Elf32_Shdr *section_tab, char *section_name_table, int num_sections, char *section_name)
{
    // L'argument n'est pas un nombre. Rechercher une section avec le nom donné.
    for (int i = 0; i < num_sections; i++)
    {
        if (section_tab[i].sh_type == SHT_PROGBITS && strcmp(section_name_table + section_tab2[i].sh_name, section_name) == 0) {
            return i;
        }
    }
    return -1;
}*/
int find_section_by_name(Elf32_Shdr *section_tab, char *section_name_table, int num_sections, char *section_name)
{
    // L'argument n'est pas un nombre. Rechercher une section avec le nom donné.
    for (int i = 0; i < num_sections; i++)
    {
        if (strcmp(section_name_table + section_tab[i].sh_name, section_name) == 0) {
            return i;
        }
    }
    return -1;
}

char *read_section_name_table(FILE *f, Elf32_Shdr *shstrtab)
{
    char *section_name_table = malloc(shstrtab->sh_size);
    fseek(f, shstrtab->sh_offset, SEEK_SET);
    fread(section_name_table, 1, shstrtab->sh_size, f);
    return section_name_table;
}

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

int k=0;
void afficheTableSections(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *section,FILE *f2, Elf32_Ehdr ehdr2, Elf32_Shdr *section2)
{
    Elf32_Shdr tableName = section[ehdr.e_shstrndx];
    Elf32_Shdr tableName2 = section2[ehdr2.e_shstrndx];
    char *tmp = malloc(tableName.sh_size+tableName2.sh_size);
    if (tmp == NULL)
    {
        printf("Impossible d'afficher la table des sections \n");
        exit(0);
    }
    fseek(f, tableName.sh_offset, SEEK_SET);
    fread(tmp, 1, tableName.sh_size, f);
    /////////
    Elf32_Shdr *shstrtab = &section[ehdr.e_shstrndx];
    char *section_name_table = read_section_name_table(f, shstrtab);
    /////////
    if(k==0){
    printf("There are %u section headers, starting at offset 0x%x:\n\n", ehdr.e_shnum, ehdr.e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name\t\t   Type\t\t   Addr\t    Off\t   Size   ES Flg Lk Inf Al\n");}
    // On affiche toutes les sections de 0 à shnum
    for (uint16_t i = 0; i < ehdr.e_shnum; i++)
    {int l=0;
        // int j =section[i].sh_name;
        printf("  [%2d] ", k);k++;

    
        printf(" %s", tmp + section[i].sh_name);
        //boucle pour créer des espaces 
        for (int k = strlen(tmp + section[i].sh_name); k < 17; k++)
        {
            printf(" ");
        }
        
        switch (section[i].sh_type)
        {
        case 0:
            printf("NULL    ");
            break;
        case 1:
            printf("PROGBITS");
            break;
        case 2:
            printf("SYMTAB  ");
            break;
        case 3:
            printf("STRTAB  ");
            break;
        case 4:
            printf("RELA   ");
            break;
        case 5:
            printf("HASH   ");
            break;
        case 6:
            printf("DYNAMIC ");
            break;
        case 7:
            printf("NOTE    ");
            break;
        case 8:
            printf("NOBITS  ");
            break;
        case 9:
            printf("REL\t");
            break;
        case 10:
            printf("SHLIB   ");
            break;
        case 11:
            printf("DYNSYM  ");
            break;
        case 14:
   	    printf("INIT_ARRAY  ");
            break;
	    case 15:
            printf("FINI_ARRAY  ");
	        break;
	    case 16:
	    printf("PREINIT_ARRAY  ");
            break;
	    case 17:
            printf("GROUP  ");
	        break;
	    case 18:
	    printf("SYMTAB_SHNDX  ");
	        break;
        case 0x70000000:
            printf("LOPROC   ");
            break;
        case 0x7fffffff:
            printf("HIPROC   ");
            break;
        case 0x80000000:
            printf("LOUSER   ");
            break;
        case 0xffffffff:
            printf("HIUSER   ");
            break;
        case 0x70000003:
            printf("ARM_ATTRIBUTES");
            break;
        default:
            printf("         ");
        }
        printf("\t   ");
        printf("%08x ", section[i].sh_addr);
        Elf32_Shdr *shstrtab2 = &section2[ehdr2.e_shstrndx];
        char *section_name_table2 = read_section_name_table(f2, shstrtab2);

        if(find_section_by_name(section2, section_name_table2, ehdr.e_shnum,tmp + section[i].sh_name )!=-1){
            printf("n");
            l=find_section_by_name(section2, section_name_table2, ehdr.e_shnum,tmp + section[i].sh_name );
            printf("%06x ",(unsigned int)section[i].sh_offset^ section2[l].sh_offset);
            printf("%06x ", section[i].sh_size+section2[l].sh_size);
            printf("%02x  ", section[i].sh_entsize+section2[l].sh_entsize);
        /*}
        else if(find_section_by_name(section2, section_name_table2, ehdr.e_shnum,tmp + section[i].sh_name )!=-1){
            printf("%06x ", section[i].sh_offset);
            printf("%06x ", section[i].sh_size);
            printf("%02x  ", section[i].sh_entsize);*/
        }
        else{
            printf("%06x ", section[i].sh_offset);
            printf("%06x ", section[i].sh_size);
            printf("%02x  ", section[i].sh_entsize);
        }
        int cmpt=14;
        if (section[i].sh_flags & SHF_WRITE)
            printf("W");
        else cmpt--;
        if (section[i].sh_flags & SHF_ALLOC)
            printf("A");
        else cmpt--;
        if (section[i].sh_flags & SHF_EXECINSTR)
            printf("X");
        else cmpt--;
        if (section[i].sh_flags & SHF_MERGE)
            printf("M");
        else cmpt--;
        if (section[i].sh_flags & SHF_STRINGS)
            printf("S");
        else cmpt--;
        if (section[i].sh_flags & SHF_INFO_LINK)
            printf("I");
        else cmpt--;
        if (section[i].sh_flags & SHF_LINK_ORDER)
            printf("L");
        else cmpt--;
        if (section[i].sh_flags & SHF_OS_NONCONFORMING)
            printf("x");
        else cmpt--;
        if (section[i].sh_flags & SHF_GROUP)
            printf("G");
        else cmpt--;
        if (section[i].sh_flags & SHF_TLS)
            printf("T");
        else cmpt--;
        if (section[i].sh_flags & SHF_MASKOS)
            printf("o");
        else cmpt--;
        if (section[i].sh_flags & SHF_MASKPROC)
            printf("p");
        else cmpt--;
        if (section[i].sh_flags & SHF_ORDERED)
            printf("O");
        else cmpt--;
        if (section[i].sh_flags & SHF_EXCLUDE)
            printf("E");
        else cmpt--;
        int h= 4-cmpt;
        while (h--) printf(" ");

        printf("%d   ", section[i].sh_link);
        //printf("\t");
        printf("%d  ", section[i].sh_info);
        //printf("\t");
        printf("%u\n", section[i].sh_addralign);
       /* if(find_section_by_name_and_type(f, section, section_name_table, ehdr.e_shnum,tmp + section[i].sh_name )!=-1)
        printf("section1[%d].name_of_section==section2[%d].name_of_section + type == PROGBITS\n",i,l);*/
    }
    //2nd file
    free(tmp);

}void afficheLeResteDuTableSections2(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *section,FILE *f2, Elf32_Ehdr ehdr2, Elf32_Shdr *section2)
{
    Elf32_Shdr tableName = section[ehdr.e_shstrndx];
    Elf32_Shdr tableName2 = section2[ehdr2.e_shstrndx];
    char *tmp = malloc(tableName.sh_size);
    if (tmp == NULL)
    {
        printf("Impossible d'afficher la table des sections \n");
        exit(0);
    }
    fseek(f, tableName.sh_offset, SEEK_SET);
    fseek(f2, tableName2.sh_offset, SEEK_SET);
    fread(tmp, 1, tableName.sh_size, f);
    /////////
    Elf32_Shdr *shstrtab2 = &section2[ehdr2.e_shstrndx];
    char *section_name_table2 = read_section_name_table(f2, shstrtab2);
    
    /////////
    if(k==0){
    printf("There are %u section headers, starting at offset 0x%x:\n\n", ehdr.e_shnum, ehdr.e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name\t\t   Type\t\t   Addr\t    Off\t   Size   ES Flg Lk Inf Al\n");}
    // On affiche toutes les sections de 0 à shnum
    for (uint16_t i = 0; i < ehdr.e_shnum; i++){    
                int l=0;
                if(find_section_by_name(section2, section_name_table2, ehdr.e_shnum,tmp + section[i].sh_name )==-1){
        // int j =section[i].sh_name;
        printf("  [%2d] ", k);k++;

    
        printf(" %s", tmp + section[i].sh_name);
        //boucle pour créer des espaces 
        for (int k = strlen(tmp + section[i].sh_name); k < 17; k++)
        {
            printf(" ");
        }
        
        switch (section[i].sh_type)
        {
        case 0:
            printf("NULL    ");
            break;
        case 1:
            printf("PROGBITS");
            break;
        case 2:
            printf("SYMTAB  ");
            break;
        case 3:
            printf("STRTAB  ");
            break;
        case 4:
            printf("RELA   ");
            break;
        case 5:
            printf("HASH   ");
            break;
        case 6:
            printf("DYNAMIC ");
            break;
        case 7:
            printf("NOTE    ");
            break;
        case 8:
            printf("NOBITS  ");
            break;
        case 9:
            printf("REL\t");
            break;
        case 10:
            printf("SHLIB   ");
            break;
        case 11:
            printf("DYNSYM  ");
            break;
        case 14:
   	    printf("INIT_ARRAY  ");
            break;
	    case 15:
            printf("FINI_ARRAY  ");
	        break;
	    case 16:
	    printf("PREINIT_ARRAY  ");
            break;
	    case 17:
            printf("GROUP  ");
	        break;
	    case 18:
	    printf("SYMTAB_SHNDX  ");
	        break;
        case 0x70000000:
            printf("LOPROC   ");
            break;
        case 0x7fffffff:
            printf("HIPROC   ");
            break;
        case 0x80000000:
            printf("LOUSER   ");
            break;
        case 0xffffffff:
            printf("HIUSER   ");
            break;
        case 0x70000003:
            printf("ARM_ATTRIBUTES");
            break;
        default:
            printf("         ");
        }
        printf("\t   ");
        printf("%08x ", section[i].sh_addr);
        printf("%06x ", section[i].sh_offset);
        printf("%06x ", section[i].sh_size);
        printf("%02x  ", section[i].sh_entsize);
        
        int cmpt=14;
        if (section[i].sh_flags & SHF_WRITE)
            printf("W");
        else cmpt--;
        if (section[i].sh_flags & SHF_ALLOC)
            printf("A");
        else cmpt--;
        if (section[i].sh_flags & SHF_EXECINSTR)
            printf("X");
        else cmpt--;
        if (section[i].sh_flags & SHF_MERGE)
            printf("M");
        else cmpt--;
        if (section[i].sh_flags & SHF_STRINGS)
            printf("S");
        else cmpt--;
        if (section[i].sh_flags & SHF_INFO_LINK)
            printf("I");
        else cmpt--;
        if (section[i].sh_flags & SHF_LINK_ORDER)
            printf("L");
        else cmpt--;
        if (section[i].sh_flags & SHF_OS_NONCONFORMING)
            printf("x");
        else cmpt--;
        if (section[i].sh_flags & SHF_GROUP)
            printf("G");
        else cmpt--;
        if (section[i].sh_flags & SHF_TLS)
            printf("T");
        else cmpt--;
        if (section[i].sh_flags & SHF_MASKOS)
            printf("o");
        else cmpt--;
        if (section[i].sh_flags & SHF_MASKPROC)
            printf("p");
        else cmpt--;
        if (section[i].sh_flags & SHF_ORDERED)
            printf("O");
        else cmpt--;
        if (section[i].sh_flags & SHF_EXCLUDE)
            printf("E");
        else cmpt--;
        int h= 4-cmpt;
        while (h--) printf(" ");

        printf("%d   ", section[i].sh_link);
        //printf("\t");
        printf("%d  ", section[i].sh_info);
        //printf("\t");
        printf("%u\n", section[i].sh_addralign);
    }}
    //2nd file
    free(tmp);

}


int main(int argc, char **argv)
{
	FILE *fp,*fd;
	Elf32_Ehdr header;
	Elf32_Shdr section;
	int i;

	// Ouverture du fichier1 Elf32
	fp = fopen(argv[1], "rb");
	if (!fp)
		return 0;
	// Ouverture du fichier2 Elf32
	fd = fopen(argv[2], "rb");
	if (!fd)
		return 0;

	// Lecture des sections du 1er fichier
    Elf32_Ehdr ehdr1;
	fread(&ehdr1, sizeof(Elf32_Ehdr), 1, fp);
    Elf32_Shdr *s1 = malloc(ehdr1.e_shnum * ehdr1.e_shentsize);
    read_section(fp, s1, ehdr1.e_shoff, ehdr1.e_shnum);
    // Lecture des sections du 2eme fichier
    Elf32_Ehdr ehdr2;
	fread(&ehdr2, sizeof(Elf32_Ehdr), 1, fd);
    Elf32_Shdr *s2 = malloc(ehdr2.e_shnum * ehdr2.e_shentsize);
    read_section(fd, s2, ehdr2.e_shoff, ehdr2.e_shnum);
    //affichage des sections des deux fichiers
    afficheTableSections(fd, ehdr2, s2,fp, ehdr1, s1);
    afficheLeResteDuTableSections2(fp, ehdr1, s1,fd, ehdr2, s2);
    
	fclose(fp);
	fclose(fd);
	return 0;
}