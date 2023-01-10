#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <elf.h>

#include "readelf.h"

void afficheTableSections(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *section)
{
    Elf32_Shdr tableName = section[ehdr.e_shstrndx];
    char *tmp = malloc(tableName.sh_size);
    if (tmp == NULL)
    {
        printf("Impossible d'afficher la table des sections \n");
        exit(0);
    }
    fseek(f, tableName.sh_offset, SEEK_SET);
    fread(tmp, 1, tableName.sh_size, f);
    printf("There are %u section headers, starting at offset 0x%x:\n\n", ehdr.e_shnum, ehdr.e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr] Name\t\t   Type\t\t   Addr\t    Off\t   Size   ES Flg Lk Inf Al\n");
    // On affiche toutes les sections de 0 à shnum
    for (uint16_t i = 0; i < ehdr.e_shnum; i++)
    {
        // int j =section[i].sh_name;
        printf("  [%2d] ", i);

    
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
    }
    free(tmp);
}


