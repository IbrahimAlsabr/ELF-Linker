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
        printf("Impossible d'afficher la table des symboles \n");
        exit(0);
    }
    fseek(f, tableName.sh_offset, SEEK_SET);
    fread(tmp, 1, tableName.sh_size, f);
    
    printf("There are %u section headers, starting at offset 0x%x\n\n", ehdr.e_shnum, ehdr.e_shoff);
    printf("Section Headers:\n");
    printf("  [Nr]  Name\t\t      Type\t\tAddr\t Off\tSize   ES FLG\tLK\tINF\tAL\n");
    // On affiche toutes les sections de 0 à shnum
    for (uint16_t i = 0; i < ehdr.e_shnum; i++)
    {
        // int j =section[i].sh_name;
        printf("  [%2d]  ", i);

        printf(" %s", tmp + section[i].sh_name);
        //boucle pour créer des espaces 
        for (int k = strlen(tmp + section[i].sh_name); k < 21; k++)
        {
            printf(" ");
        }
        switch (section[i].sh_type)
        {
        case 0:
            printf("NULL\t");
            break;
        case 1:
            printf("PROGBITS\t");
            break;
        case 2:
            printf("SYMTAB\t");
            break;
        case 3:
            printf("STRTAB\t");
            break;
        case 4:
            printf("RELA\t   ");
            break;
        case 5:
            printf("HASH\t   ");
            break;
        case 6:
            printf("DYNAMIC\t    ");
            break;
        case 7:
            printf("NOTE\t    ");
            break;
        case 8:
            printf("NOBITS\t");
            break;
        case 9:
            printf("REL\t");
            break;
        case 10:
            printf("SHLIB\t   ");
            break;
        case 11:
            printf("DYNSYM\t   ");
            break;
        case 0x70000000:
            printf("LOPROC\t   ");
            break;
        case 0x7fffffff:
            printf("HIPROC\t   ");
            break;
        case 0x80000000:
            printf("LOUSER\t   ");
            break;
        case 0xffffffff:
            printf("HIUSER\t   ");
            break;
        default:
            printf("  \t");
        }
        printf("\t");
        printf("%08x ", section[i].sh_addr);
        printf("%06x ", section[i].sh_offset);
        printf("%06x ", section[i].sh_size);
        printf("%02x ", section[i].sh_entsize);
        if (section[i].sh_flags & SHF_WRITE)
            printf("W");

        if (section[i].sh_flags & SHF_ALLOC)
            printf("A");

        if (section[i].sh_flags & SHF_EXECINSTR)
            printf("X");

        if (section[i].sh_flags & SHF_MERGE)
            printf("M");

        if (section[i].sh_flags & SHF_STRINGS)
            printf("S");

        if (section[i].sh_flags & SHF_INFO_LINK)
            printf("I");

        if (section[i].sh_flags & SHF_LINK_ORDER)
            printf("L");

        if (section[i].sh_flags & SHF_OS_NONCONFORMING)
            printf("x");

        if (section[i].sh_flags & SHF_GROUP)
            printf("G");

        if (section[i].sh_flags & SHF_TLS)
            printf("T");

        if (section[i].sh_flags & SHF_MASKOS)
            printf("o");

        if (section[i].sh_flags & SHF_MASKPROC)
            printf("p");

        if (section[i].sh_flags & SHF_ORDERED)
            printf("O");

        if (section[i].sh_flags & SHF_EXCLUDE)
            printf("E");

        printf("\t");

        printf("%d  ", section[i].sh_link);
        printf("\t");
        printf("%d", section[i].sh_info);
        printf("\t");
        printf("%u\n", section[i].sh_addralign);
    }
    free(tmp);
}




