#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "elf.h"

void read_section(FILE *f, Elf32_Shdr section_tab[],uint32_t offset,uint32_t nb_sect){
    
    int i = 0;

    fseek(f,offset,SEEK_SET); //Placement de l'indicateur de position au niveau de l'entête de la table des sections

    for(i=0;i<nb_sect;i++){
        fread(&section_tab[i].sh_name,sizeof(Elf32_Word),1,f);    //Lecture du name
        fread(&section_tab[i].sh_type,sizeof(Elf32_Word),1,f);    //Lecture du type
        fread(&section_tab[i].sh_flags,sizeof(Elf32_Word),1,f);   //Lecture des flags
        fread(&section_tab[i].sh_addr,sizeof(Elf32_Addr),1,f);    //Lecture de l'adresse
        fread(&section_tab[i].sh_offset,sizeof(Elf32_Off),1,f);  //Lecture de l'offset
        fread(&section_tab[i].sh_size,sizeof(Elf32_Word),1,f);     //Lecture de la taille
        fread(&section_tab[i].sh_link,sizeof(Elf32_Word),1,f);     //Lecture du lien
        fread(&section_tab[i].sh_info,sizeof(Elf32_Word),1,f);    //Lecture de l'info
        fread(&section_tab[i].sh_addralign,sizeof(Elf32_Word),1,f);  //Lecutre de l'addralign
        fread(&section_tab[i].sh_entsize,sizeof(Elf32_Word),1,f);   //Lecture de l'entsize

    }

}


void afficheTableSections (FILE *f,Elf32_Ehdr ehdr, Elf32_Shdr *section) {
  Elf32_Shdr tableName = section[ehdr.e_shstrndx];
  char *tmp = malloc(tableName.sh_size);
  if (tmp == NULL){
    printf("Impossible d'afficher la table des symboles \n");
    exit(0);
  }
  if(fseek(f, tableName.sh_offset, SEEK_SET)){
    printf("Valeur de l'offset de cette section trop grande\n");
    exit(0);
  }
  if (!fread(tmp, 1, tableName.sh_size, f)){
    printf("Taille de cette section trop grande pour ce fichier, impossible de lire les valeurs\n");
    exit(0);
  }
  printf("There are %u section headers, starting at offset 0x%x\n\n", ehdr.e_shnum, ehdr.e_shoff);
  printf("Section Headers:\n");
  printf("  [Nr]  Name\t\t      Type\tAddr\t    Off\t      Size\tES\tFLG\tLK\tINF\tAL\n");
// On affiche toutes les sections de 0 à shnum 
    for (uint16_t i = 0; i < ehdr.e_shnum; i ++) {
    //int j =section[i].sh_name;
    printf("  [%2d]  ", i);

    printf(" %s",  tmp + section[i].sh_name);
    for (int k = strlen(tmp + section[i].sh_name); k < 21; k ++) {
      printf(" ");
    }
    switch (section[i].sh_type) {
    case 0 :
      printf("NULL\t");
      break;
    case 1 :
      printf("PROGBITS\t");
      break;
    case 2 :
      printf("SYMTAB\t");
      break;
    case 3 :
      printf("STRTAB\t");
      break;
    case 4 :
      printf("RELA\t   ");
      break;
    case 5 :
      printf("HASH\t   ");
      break;
    case 6 :
      printf("DYNAMIC\t    ");
      break;
    case 7 :
      printf("NOTE\t    ");
      break;
    case 8 :
      printf("NOBITS\t");
      break;
    case 9 :
      printf("REL\t");
      break;
    case 10 :
      printf("SHLIB\t   ");
      break;
    case 11 :
      printf("DYNSYM\t   ");
      break;
    case 0x70000000 :
      printf("LOPROC\t   ");
      break;
    case 0x7fffffff :
      printf("HIPROC\t   ");
      break;
    case 0x80000000 :
      printf("LOUSER\t   ");
      break;
    case 0xffffffff :
      printf("HIUSER\t   ");
      break;
    default :
      printf("  \t");
    }
    printf("%08x    ", section[i].sh_addr);
    printf("%06x    ", section[i].sh_offset);
    printf("%06x    ", section[i].sh_size);
    printf("%02x      ", section[i].sh_entsize);
    if (section[i].sh_flags & SHF_WRITE) printf("W");

    if (section[i].sh_flags  & SHF_ALLOC) printf("A");
    
    if (section[i].sh_flags  & SHF_EXECINSTR) printf("X");
    
    if (section[i].sh_flags  & SHF_MERGE) printf("M");
    
    if (section[i].sh_flags  & SHF_STRINGS) printf("S");
    
    if (section[i].sh_flags  & SHF_INFO_LINK) printf("I");
    
    if (section[i].sh_flags  & SHF_LINK_ORDER) printf("L");
    
    if (section[i].sh_flags  & SHF_OS_NONCONFORMING) printf("x");
    
    if (section[i].sh_flags  & SHF_GROUP) printf("G");
    
    if (section[i].sh_flags  & SHF_TLS) printf("T");
    
    if (section[i].sh_flags  & SHF_MASKOS) printf("o");
    
    if (section[i].sh_flags  & SHF_MASKPROC) printf("p");
    
    if (section[i].sh_flags  & SHF_ORDERED) printf("O");
    
    if (section[i].sh_flags  & SHF_EXCLUDE) printf("E");
    
    if(section[i].sh_flags){
        for (int k = 0; k < 8; k ++) {
      printf(" ");
        }}
    else{
        for (int k = 0; k < 10; k ++) {
      printf(" ");
    }
    
    }
    
    printf("%d\t", section[i].sh_link);
    printf("%d\t", section[i].sh_info);
    printf("%u\n", section[i].sh_addralign);
    
    }
    free(tmp);
}

int main(int argc, char *argv[])
{
  FILE *f= fopen(argv[1],"rb");
  Elf32_Ehdr header;
  fread(&header, sizeof(header), 1, f);
  Elf32_Shdr *s = malloc(header.e_shnum * header.e_shentsize);
  read_section(f,s,header.e_shoff,header.e_shnum);
  afficheTableSections(f,header, s);
  printf("\nKeyto flas :\n  W (write), A (alloc), X (execute), M (merge), S (strings), I (info), L (link order), O (extra OS processing required), G (group), T (TLS),\n C (compressed), x (unknow),  o (OS specific), E (exclude), D (mbind), y (purecode), P (processor specific), \n");
  free(s);
  fclose(f);
  return 0;
}
