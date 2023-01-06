#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <getopt.h>

#include "E1_elf_header.h"
#include "E2_table_section.h"
#include "E3_content_of_section.h"
#include "E4_symbole_table.h"
#include "E5_table_of_reimplantation.h"

#include "readelf.h"

int main(int argc, char *argv[])
{
    int hflag, sflag, Sflag, rflag;
    char *xflag;
    int index;
    int c;

    if (argc < 3)
    {
        fprintf(stderr, "./main [nom fichier] [commande(-h, -S, -x, ...)]\n");
        exit(1);
    }

    opterr = 0;
    while ((c = getopt(argc, argv, "hsSrx:")) != -1)
    {
        switch (c)
        {
        case 'h':
            hflag = 1;
            break;
        case 's':
            sflag = 1;
            break;
        case 'S':
            Sflag = 1;
            break;
        case 'r':
            rflag = 1;
            break;
        case 'x':
            xflag = optarg;
            break;
        case '?':
            if (optopt == 'x')
                fprintf(stderr, "L'option -x necessite un argument (section name).\n");
            else if (isprint(optopt))
                fprintf(stderr, "Option inconnue '-%c'.\n", optopt);
            else
                fprintf(stderr, "Option caractere incounnue '\\x%x'.\n", optopt);
            break;
        default:
            abort();
        }
    }

    for (index = optind; index < argc; index++)
    {
        char file[20];
        strcpy(file, argv[index]);
        if (access(file, F_OK) != 0)
        {
            fprintf(stderr, "Erreur: Le fichier %s n'existe pas.\n\n", file);
        }
        else
        {
            /*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
            printf("File: %s\n", file);
            Elf32_Ehdr ehdr;

            FILE *f = fopen(file, "rb");
            if (f == NULL)
            {
                fprintf(stderr, "Erreur: Erreur lors de l'ouverture du fichier %s.\n\n", file);
            }
            else if (!fread(&ehdr, sizeof(Elf32_Ehdr), 1, f))
            {
                fprintf(stderr, "Impossible de lire le ELF Header du fichier %s\n\n", file);
            }
            // Vérifiez que le fichier est bien un fichier ELF
            else if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
                     ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
                     ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
                     ehdr.e_ident[EI_MAG3] != ELFMAG3)
            {
                fprintf(stderr, "%s n'est pas un ELF file\n\n", file);
            }

            // Vérifiez que le fichier est un fichier ELF 32 bits
            else if (ehdr.e_ident[EI_CLASS] != ELFCLASS32)
            {
                fprintf(stderr, "%s n'est pas un 32-bit ELF file\n\n", file);
            }
            else
            {

                /* Affichage de l’en-tete */
                if (hflag)
                {
                    {
                        print_header(&ehdr);
                        printf("\n\n");
                    }

                    /* Affichage de la table des sections */
                    if (Sflag)
                    {
                        Elf32_Shdr *s = malloc(ehdr.e_shnum * ehdr.e_shentsize);
                        read_section(f, s, ehdr.e_shoff, ehdr.e_shnum);
                        afficheTableSections(f, ehdr, s);
                        printf("\nKeyto flas :\n  W (write), A (alloc), X (execute), M (merge), S (strings), I (info), L (link order), O (extra OS processing required), G (group), T (TLS),\n C (compressed), x (unknow),  o (OS specific), E (exclude), D (mbind), y (purecode), P (processor specific), \n");
                        free(s);
                        printf("\n\n");

                    }

                    /*  Affichage de la table des symboles */
                    if (xflag)
                    {

                        // Lire l'en-tête ELF
                        fread(&ehdr, sizeof(Elf32_Ehdr), 1, f);

                        // Lire les en-têtes de section
                        Elf32_Shdr section_tab[ehdr.e_shnum];
                        read_section(f, section_tab, ehdr.e_shoff, ehdr.e_shnum);

                        // Lire la table de chaînes de noms de section
                        Elf32_Shdr *shstrtab = &section_tab[ehdr.e_shstrndx];
                        char *section_name_table = read_section_name_table(f, shstrtab);

                        // Trouver la section demandée
                        Elf32_Shdr *section = find_section_by_name_or_number(f, section_tab, section_name_table, ehdr.e_shnum, xflag);
                        if (!section)
                        {
                            fprintf(stderr, "section not found");
                        }
                        else
                        {
                            // Imprimer le contenu de la section
                            print_section(f, section, section_name_table);

                            // Nettoyage
                            free(section_name_table);
                            printf("\n\n");

                        }
                    }
                }

                /* Affichage de la table des symboles */
                if (sflag)
                {

                    fread(&ehdr, 1, sizeof(ehdr), f);

                    read_table_symbol(f, ehdr);
                    printf("\n\n");

                }

                /* Affichage des tables de réimplantation */
                if (rflag)
                {
                    Elf32_Shdr *s = malloc(ehdr.e_shnum * ehdr.e_shentsize);
                    read_section(f, s, ehdr.e_shoff, ehdr.e_shnum);
                    afficheTableReimplentation(f, ehdr, s);
                    free(s);
                    printf("\n\n");
                }
            }
            fclose(f);
        }
    }
}

