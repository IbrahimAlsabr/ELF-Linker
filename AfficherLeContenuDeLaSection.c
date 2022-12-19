#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>

// Imprime un message d'erreur et quitte le programme
void print_error_and_exit(char* message) {
  fprintf(stderr, "Error: %s\n", message);
  exit(1);
}

// Lit l'en-tête (header) ELF du fichier donné et le stocke dans la variable donnée
void read_elf_header(FILE* f, Elf32_Ehdr* ehdr) {
  if (!fread(ehdr, sizeof(Elf32_Ehdr), 1, f)) {
    print_error_and_exit("cannot read ELF header from file");
  }
}

// Lit les en-têtes (headers) de section du fichier donné et les stocke dans le tableau donné
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

// Lit la table de chaînes de noms de section à partir du fichier donné et la stocke dans le tableau donné
char* read_section_name_table(FILE* f, Elf32_Shdr* shstrtab) {
  char* section_name_table = malloc(shstrtab->sh_size);
  fseek(f, shstrtab->sh_offset, SEEK_SET);
  fread(section_name_table, 1, shstrtab->sh_size, f);
  return section_name_table;
}

// Ouvre le fichier ELF avec le nom donné et renvoie un pointeur FILE* vers celui-ci
FILE* load_elf_file(char* file_name) {
  FILE* f = fopen(file_name, "rb");
  if (!f) {
    print_error_and_exit("cannot open file");
  }
  return f;
}

// Ferme le fichier ELF donné
void close_elf_file(FILE* f) {
  fclose(f);
}

// Renvoie un pointeur vers la section avec le nom ou le numéro donné,
// ou NULL si aucune section de ce type n'existe
Elf32_Shdr* find_section_by_name_or_number(FILE* f, Elf32_Shdr* section_tab, char* section_name_table, int num_sections, char* section_name_or_number) {
  // D'abord, vérifie si l'argument donné est un nombre
  char* endptr;
  long section_number = strtol(section_name_or_number, &endptr, 10);
  if (*endptr == '\0') {
    // L'argument est un nombre. Vérifiez s'il est dans la plage valide.
    if (section_number >= 0 && section_number < num_sections) {
      return &section_tab[section_number];
    } else {
      return NULL;
    }
  } else {
    // L'argument n'est pas un nombre. Rechercher une section avec le nom donné.
    for (int i = 0; i < num_sections; i++) {
      if (strcmp(section_name_table + section_tab[i].sh_name, section_name_or_number) == 0) {
        return &section_tab[i];
      }
    }
    return NULL;
  }
}

// Imprime le contenu de la section donnée sur la sortie standard
void print_section(FILE* f, Elf32_Shdr* section, char* section_name_table) {
    fseek(f, section->sh_offset, SEEK_SET);
  // initialisez l'adresse à l'adresse de début de la section
    int address = section->sh_addr;   
      // Print the name of the section
  printf("Vidange hexadécimale de la section « '%s' » :\n", section_name_table + section->sh_name);

    for(int i = 0; i < section->sh_size; i++) {
    // Affichez l'adresse avant chaque ligne
    if (i % 16 == 0) {
      printf("0x%08x ", address);
      address += 16; // mettez à jour l'adresse pour la ligne suivante
    }
    printf("%02x", fgetc(f));
    if (i % 4 == 3) {
        printf(" ");
    }
    
    if (i % 16 == 15 || i == section->sh_size - 1) {
            printf(" ");

        // Affichez la chaîne de caractères correspondante
    fseek(f, section->sh_offset + i - (i % 16), SEEK_SET);
        for (int i = 0; i < 16; i++) {
            putchar(fgetc(f));
        }

        printf("\n");
    }
    }
}

int main(int argc, char** argv) {
  // Vérifier que le nombre d'arguments est correct
  if (argc != 3) {
    fprintf(stderr, "Please use this form: %s file.elf section_name\n", argv[0]);
    return 1;
  }

  // Ouvrir le fichier ELF
  FILE* f = load_elf_file(argv[1]);

  // Lire l'en-tête ELF
  Elf32_Ehdr ehdr;
  fread(&ehdr, sizeof(Elf32_Ehdr), 1, f);

  // Vérifiez que le fichier est bien un fichier ELF
  if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
      ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
      ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
      ehdr.e_ident[EI_MAG3] != ELFMAG3) {
    print_error_and_exit("not an ELF file");
  }

  // Vérifiez que le fichier est un fichier ELF 32 bits
  if (ehdr.e_ident[EI_CLASS] != ELFCLASS32) {
    print_error_and_exit("not a 32-bit ELF file");
  }

  // Lire les en-têtes de section
  Elf32_Shdr section_tab[ehdr.e_shnum];
  read_section_headers(f, &ehdr, section_tab);

  // Lire la table de chaînes de noms de section
  Elf32_Shdr* shstrtab = &section_tab[ehdr.e_shstrndx];
  char* section_name_table = read_section_name_table(f, shstrtab);

  // Trouver la section demandée
  Elf32_Shdr* section = find_section_by_name_or_number(f, section_tab, section_name_table, ehdr.e_shnum, argv[2]);
  if (!section) {
    print_error_and_exit("section not found");
  }

  // Imprimer le contenu de la section
  print_section(f, section, section_name_table);

  // Nettoyage
  free(section_name_table);
  close_elf_file(f);

  return 0;
}
