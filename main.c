#include <elf.h>
#include <stdio.h>
//#include <string.h>
#include <unistd.h>

#include "readfile.h"
#include "printinfo.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Le programme necessite un et un seul argument.\n");
        return 1;
    }
    if (access(argv[1], F_OK) != 0) {
        printf("Le fichier n'existe pas.\n");
        return 2;
    }

    Elf32_Ehdr ehdr;

    FILE* file = fopen(argv[1], "rb");
    read_elf_header(file, &ehdr);
    fclose(file);
    
    print_header(&ehdr);

    return 0;
}