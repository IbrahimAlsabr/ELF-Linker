#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>
#include <string.h>

#include "elf_reader.h"

void affichesymbol(Elf32_Sym s, int i)
{

    printf("%6d: ", i);
    printf("%08x", s.st_value);
    printf("%6d ", s.st_size);

    switch (ELF32_ST_TYPE(s.st_info))
    {
    case 0:
        printf("%-8s", "NOTYPE");
        break;
    case 1:
        printf("%-8s", "OBJECT");
        break;
    case 2:
        printf("%-8s", "FUNC");
        break;
    case 3:
        printf("%-8s", "SECTION");
        break;
    case 4:
        printf("%-8s", "FILE");
        break;
    case 5:
        printf("%-8s", "COMMON");
        break;
    case 6:
        printf("%-8s", "TLS");
        break;
    case 10:
        printf("%-8s", "LOOS");
        break;
    case 12:
        printf("%-8s", "HIOS");
        break;
    case 13:
        printf("%-8s", "LOPROC");
        break;
    case 15:
        printf("%-8s", "HIPROC");
        break;

    default:
        break;
    }

    switch (ELF32_ST_BIND(s.st_info))
    {
    case 0:
        printf("%-7s", "LOCAL");
        break;
    case 1:
        printf("%-7s", "GLOBAL");
        break;
    case 2:
        printf("%-7s", "WEAK");
        break;
    case 10:
        printf("%-7s", "LOOS");
        break;
    case 12:
        printf("%-7s", "HIOS");
        break;
    case 13:
        printf("%-7s", "LOPROC");
        break;
    case 15:
        printf("%-7s", "HIPROC");
        break;
    default:
        break;
    }

    switch (ELF32_ST_BIND(s.st_other))
    {

    case 0:
        printf("%-9s", "DEFAULT");
        break;
    case 1:
        printf("%-9s", "INTERNAL");
        break;
    case 2:
        printf("%-9s", "HIDDEN");
        break;
    case 3:
        printf("%-9s", "PROTECTED");
        break;
    default:
        break;
    }

    switch (s.st_shndx)
    {
    case SHN_ABS:
        printf("%3s ", "ABS");
        break;
    case SHN_COMMON:
        printf("%3s ", "COMMON");
        break;
    case SHN_UNDEF:
        printf("%3s ", "UND");
        break;
    default:
        printf("%3d ", s.st_shndx);
        break;
    }
}

void read_table_symbol(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *shdr) {

    Elf32_Shdr tableName = shdr[ehdr.e_shstrndx];
    char *shstr = malloc(tableName.sh_size);
    fseek(f, tableName.sh_offset, SEEK_SET);
    fread(shstr, 1, tableName.sh_size, f);

    for (uint16_t i = 0; i < ehdr.e_shnum; i++) {
        if (shdr[i].sh_type == SHT_SYMTAB || shdr[i].sh_type == SHT_DYNSYM) {

            Elf32_Sym sym;
            Elf32_Word symbol_size = shdr[i].sh_size / sizeof(sym);

            char *str = malloc(shdr[i+1].sh_size);
            fseek(f, shdr[i+1].sh_offset, SEEK_SET);
            fread(str, 1, shdr[i+1].sh_size, f);


            fseek(f, shdr[i].sh_offset, SEEK_SET);

            printf("Symbol table '%s' contains %d entries:\n", shstr + shdr[i].sh_name, symbol_size);
            printf("%7s%9s%6s %-8s%-7s%-9s%3s Name\n", "Num:", "Value", "Size", "Type", "Bind", "Vis", "Ndx");

            for (int j = 0; j < shdr[i].sh_size / sizeof(sym); j++)
            {
                fread(&sym, 1, sizeof(sym), f);

                affichesymbol(sym, j);
                if (sym.st_name != 0) {
                    printf("%-40s", str + sym.st_name);
                }
                printf("\n");
            }

            free(str);
        }
    }
    free(shstr);
}