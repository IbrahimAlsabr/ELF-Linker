#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>

void affichesymbol(Elf32_Sym s, int i, char *strtab)
{
    
    
    printf("    %2d:\t", i);
    printf("%08x    ", s.st_value);
    printf("%3d\t  ", s.st_size);

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
        printf("%-8s", "LOCAL");
        break;
    case 1:
        printf("%-8s", "GLOBAL");
        break;
    case 2:
        printf("%-8s", "WEAK");
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

    switch (ELF32_ST_BIND(s.st_other))
    {

    case 0:
        printf("%-8s", "DEFAULT");
        break;
    case 1:
        printf("%-8s", "INTERNAL");
        break;
    case 2:
        printf("%-8s", "HIDDEN");
        break;
    case 3:
        printf("%-8s", "PROTECTED");
        break;
    default:
        break;
    }

    switch (s.st_shndx)
    {
    case SHN_ABS:
        printf("%-8s", "ABS");
        break;
    case SHN_COMMON:
        printf("%-8s", "COMMON");
        break;
    case SHN_UNDEF:
        printf("%-8s", "UND");
        break;
    default:
        printf("%-8d", s.st_shndx);
        break;
    }

    // printf("\n");
    // printf("%s\n", s.st_name);
    // printf("%s",  strtab + s.st_name);
    // printf("\n");
}

void read_table_symbol(FILE *f, Elf32_Ehdr header)
{

    Elf32_Shdr tableName;
    fseek(f, header.e_shoff + header.e_shstrndx * header.e_shentsize, SEEK_SET);
    fread(&tableName, 1, sizeof(tableName), f);
    
    char *strtab = malloc(tableName.sh_size);
    
    if (strtab == NULL)
    {
        printf("Impossible d'afficher la table des symboles \n");
        exit(0);
    }
    
    fseek(f, tableName.sh_offset, SEEK_SET);
    
    fread(strtab, 1, tableName.sh_size, f);

    uint32_t symbol_offset = 0;
    Elf32_Word symbol_size = 0;

    fseek(f, header.e_shoff, SEEK_SET);

    for (int i = 0; i < header.e_shnum; i++)
    {
        Elf32_Shdr shdr;
        fread(&shdr, 1, sizeof(shdr), f);

        if (shdr.sh_type == SHT_SYMTAB)
        {
            fseek(f, shdr.sh_offset, SEEK_SET);

            Elf32_Sym sym;
            symbol_size = shdr.sh_size / sizeof(sym);

            printf("Symbol table '.symtab' contains %d entries:\n", symbol_size);
            printf("   Num:\t   Value   Size\t  Type\t  Bind\t  Vis\t  Ndx\t  Name\n");

            for (int j = 0; j < shdr.sh_size / sizeof(sym); j++)
            {
                fread(&sym, 1, sizeof(sym), f);

                affichesymbol(sym, j, strtab);
                printf("%s\n", strtab + sym.st_name);
            }

            return;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <elf_file>\n", argv[0]);
        exit(1);
    }

    FILE *f = fopen(argv[1], "rb");
    if (f == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    Elf32_Ehdr ehdr;
    fread(&ehdr, 1, sizeof(ehdr), f);

    read_table_symbol(f, ehdr);

    fclose(f);
    return 0;
}
