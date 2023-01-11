
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>

#include "elf_reader.h"


// Lit la table de chaînes de noms de section à partir du fichier donné et la stocke dans le tableau donné
char *read_section_name_table(FILE *f, Elf32_Shdr *shstrtab)
{
    char *section_name_table = malloc(shstrtab->sh_size);
    fseek(f, shstrtab->sh_offset, SEEK_SET);
    fread(section_name_table, 1, shstrtab->sh_size, f);
    return section_name_table;
}

// Renvoie un pointeur vers la section avec le nom ou le numéro donné,
// ou NULL si aucune section de ce type n'existe
Elf32_Shdr *find_section_by_name_or_number(FILE *f, Elf32_Shdr *section_tab, char *section_name_table, int num_sections, char *section_name_or_number)
{
    // D'abord, vérifie si l'argument donné est un nombre
    char *endptr;
    long section_number = strtol(section_name_or_number, &endptr, 10);
    if (*endptr == '\0')
    {
        // L'argument est un nombre. Vérifiez s'il est dans la plage valide.
        if (section_number >= 0 && section_number < num_sections)
        {
            return &section_tab[section_number];
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        // L'argument n'est pas un nombre. Rechercher une section avec le nom donné.
        for (int i = 0; i < num_sections; i++)
        {
            if (strcmp(section_name_table + section_tab[i].sh_name, section_name_or_number) == 0)
            {
                return &section_tab[i];
            }
        }
        return NULL;
    }
}

// Imprime le contenu de la section donnée sur la sortie standard
void print_section(FILE *f, Elf32_Shdr *section, char *section_name_table)
{
    fseek(f, section->sh_offset, SEEK_SET);
    // initialisez l'adresse à l'adresse de début de la section
    int address = section->sh_addr;
    // Print the name of the section
    printf("Vidange hexadécimale de la section « '%s' » :\n", section_name_table + section->sh_name);

    for (int i = 0; i < section->sh_size; i++)
    {
        // Affichez l'adresse avant chaque ligne
        if (i % 16 == 0)
        {
            printf("0x%08x ", address);
            address += 16; // mettez à jour l'adresse pour la ligne suivante
        }
        printf("%02x", fgetc(f));
        if (i % 4 == 3)
        {
            printf(" ");
        }

        if (i % 16 == 15 || i == section->sh_size - 1)
        {
            printf(" ");

            // Affichez la chaîne de caractères correspondante
            fseek(f, section->sh_offset + i - (i % 16), SEEK_SET);
            for (int i = 0; i < 16; i++)
            {
                putchar(fgetc(f));
            }

            printf("\n");
        }
    }
}
