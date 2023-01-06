#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "elf.h"

#include "readelf.h"

void afficher_type(int type)
{
    switch (type)
    {
    case 2:
        printf("  R_ARM_ABS32");
        break;
    case 5:
        printf("  R_ARM_ABS16");
        break;
    case 8:
        printf("  R_ARM_ABS8");
        break;
    case 0x1c:
        printf("  R_ARM_CALL");
        break;
    case 0x1d:
        printf("  R_ARM_JUMP24");
        break;
    default:
        printf("    ");
    }
}

void afficheTableReimplentation(FILE *f, Elf32_Ehdr ehdr, Elf32_Shdr *s)
{
    Elf32_Shdr tableName = s[ehdr.e_shstrndx];
    char *tmp = malloc(tableName.sh_size);
    if (tmp == NULL)
    {
        printf("Impossible d'afficher la table des symboles \n");
        exit(0);
    }
    if (fseek(f, tableName.sh_offset, SEEK_SET))
    {
        printf("Valeur de l'offset de cette section trop grande\n");
        exit(0);
    }
    if (!fread(tmp, 1, tableName.sh_size, f))
    {
        printf("Taille de cette section trop grande pour ce fichier, impossible de lire les valeurs\n");
        exit(0);
    }
    for (int i = 0; i < ehdr.e_shnum; i++)
    {
        if (s[i].sh_type == 4)
        {
            int nbElt = s[i].sh_size / sizeof(Elf32_Rela);
            printf("Relocation section ' %s ' at offset 0x%x contains %d entries :\n", tmp + s[i].sh_name, s[i].sh_offset, nbElt);
            printf("  Offset      Info           Type           Addenda\n");
            Elf32_Rela *relaTab = malloc(s[i].sh_size);
            if (relaTab == NULL)
            {
                printf("Pas assez de place mémoire pour réserver la table de réimplentation\n");
                exit(0);
            }
            if (fseek(f, s[i].sh_offset, SEEK_SET))
            {
                printf("Valeur de l'offset de cette section trop grande\n");
                exit(0);
            }
            if (!fread(relaTab, 1, s[i].sh_size, f))
            {
                printf("Taille de cette section trop grande pour ce fichier, impossible de lire les valeurs\n");
                exit(0);
            }
            for (int j = 0; j < nbElt; j++)
            {
                printf("%08x  ", relaTab[j].r_offset);
                printf("%08x  ", relaTab[j].r_info);
                afficher_type((unsigned char)relaTab[j].r_info);
                if (relaTab[j].r_addend >= 0)
                {
                    printf(" +");
                    printf("%x", relaTab[j].r_addend);
                }
                else
                {
                    printf(" %d", relaTab[j].r_addend);
                }
                printf("\n");
            }
            printf("\n");
        }

        if (s[i].sh_type == 9)
        {
            int nbElt = s[i].sh_size / sizeof(Elf32_Rel);
            printf("Relocation section ' %s ' at offset 0x%x contains %d entries :\n", tmp + s[i].sh_name, s[i].sh_offset, nbElt);
            printf(" Offset   Info        Type\n");
            Elf32_Rel *relTab = malloc(s[i].sh_size);
            if (relTab == NULL)
            {
                printf("Pas assez de place mémoire pour réserver la table de réimplentation\n");
                exit(0);
            }
            if (fseek(f, s[i].sh_offset, SEEK_SET))
            {
                printf("Valeur de l'offset de cette section trop grande\n");
                exit(0);
            }
            if (!fread(relTab, 1, s[i].sh_size, f))
            {
                printf("Taille de cette section trop grande pour ce fichier, impossible de lire les valeurs\n");
                exit(0);
            }
            for (int k = 0; k < nbElt; k++)
            {
                printf(" %08x  ", relTab[k].r_offset);
                printf(" %08x  ", relTab[k].r_info);
                afficher_type((unsigned char)relTab[k].r_info);
                printf("\n");
            }
            printf("\n");
        }
    }
    free(tmp);
}
