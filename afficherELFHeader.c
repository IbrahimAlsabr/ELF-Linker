#include <elf.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//#if defined(__LP64__)
//#define ElfW(type) Elf64_ ## type
//#else
#define ElfW(type) Elf32_ ## type
//#endif

// Either Elf64_Ehdr or Elf32_Ehdr depending on architecture.
ElfW(Ehdr) header;

void read_elf_header(const char* elfFile) {
  FILE* file = fopen(elfFile, "rb");
  if(file) {
    // read the header
    fread(&header, sizeof(header), 1, file);
    // check so its really an elf file
    if (memcmp(header.e_ident, ELFMAG, SELFMAG) == 0) {
       // this is a valid elf file
    }
    // finally close the file
    fclose(file);
  }

}

void print_header() {
  printf("ELF Header:\n");
  
  // Magic:
  printf("  %-8s", "Magic:");
  for (int i = 0; i < 16; i++) {printf(" %02x", header.e_ident[i]);}
  printf("\n");

  // Class:
  printf("  %-35sELF", "Class:");
  if (header.e_ident[EI_CLASS] == ELFCLASS32) {printf("32\n");}
  else {printf("64\n");}

  // Data:
  printf("  %-35s2's complement, ", "Data:");
  if (header.e_ident[EI_DATA] == ELFDATA2LSB) {printf("little endian\n");}
  else {printf("big endian\n");}

  // Version:
  printf("  %-35s%d\n", "Version:", header.e_ident[EI_VERSION]);

  // OS/ABI: (switch from internet)
  printf("  %-35s", "OS/ABI:");
  switch (header.e_ident[EI_OSABI]) {
    case 0x00: printf("SYSTEM V"); break;
    case 0x01: printf("HP-UX"); break;
    case 0x02: printf("NET BSD"); break;
    case 0x03: printf("Linux"); break;
    case 0x04: printf("HP-UX"); break;
    case 0x05: printf("GNU Hurd"); break;
    case 0x06: printf("Solaris"); break;
    case 0x07: printf("AIX"); break;
    case 0x08: printf("IRIX"); break;
    case 0x09: printf("FreeBSD"); break;
    case 0x0A: printf("TRU64"); break;
    case 0x0B: printf("Novell Modesto"); break;
    case 0x0C: printf("OpenBSD"); break;
    case 0x0D: printf("OpenVMS"); break;
    case 0x0E: printf("NonStop Kernel"); break;
    case 0x0F: printf("AROS"); break;
    case 0x10: printf("Fenix OS"); break;
    case 0x11: printf("CloudABI"); break;
    case 0x12: printf("Stratus Technologies OpenVOS"); break;
  }
  printf("\n");

  // ABI Version:
  printf("  %-35s%d\n", "ABI Version:", header.e_ident[EI_OSABI]);

  // Type:
  printf("  %-35s", "Type:");
  if (header.e_type == ET_REL) {printf("Relocatable file\n");}
  else if (header.e_type == ET_EXEC) {printf("Executable file\n");}
  else if (header.e_type == ET_DYN) {printf("Shared object file\n");}
  else if (header.e_type == ET_CORE) {printf("Core file\n");}
  else {printf("Error\n");}

  // Machine: (switch from internet)
  printf("  %-35s", "Machine:");
  switch (header.e_machine) {
    case 0x00: printf("No specific instruction set"); break;
    case 0x01: printf("AT&T WE 32100"); break;
    case 0x02: printf("SPARC"); break;
    case 0x03: printf("x86"); break;
    case 0x04: printf("Motorola 68000k (M68k)"); break;
    case 0x05: printf("Motorola 88000 (M88k)"); break;
    case 0x06: printf("Intel MCU"); break;
    case 0x07: printf("Intel 80860"); break;
    case 0x08: printf("MIPS"); break;
    case 0x09: printf("IBM_System/370"); break;
    case 0x0A: printf("MIPS RS3000 Little-endian"); break;
    case 0x0B: printf("Reserved for future use"); break;
    case 0x0C: printf("Reserved for future use"); break;
    case 0x0D: printf("Reserved for future use"); break;
    case 0x0E: printf("Hewlett-Packard PA-RISC"); break;
    case 0x0F: printf("Reserved for future use"); break;
    case 0x13: printf("Intel 80960"); break;
    case 0x14: printf("PowerPC"); break;
    case 0x15: printf("PowerPC(64-bit)"); break;
    case 0x16: printf("S390, including S390x"); break;
    case 0x28: printf("ARM (up to ARMv7/Aarch32)"); break;
    case 0x2A: printf("SuperH"); break;
    case 0x32: printf("IA-64"); break;
    case 0x3E: printf("amd64"); break;
    case 0x8C: printf("TMS320C6000 Family"); break;
    case 0xB7: printf("ARM 64-bits (ARMv8/Aarch64)"); break;
    case 0xF3: printf("RISC-V"); break;
    case 0xF7: printf("Berkeley Packet Filter"); break;
    case 0x101: printf("WDC 65C816"); break;
    }
  printf("\n");

  // Version:
  printf("  %-35s0x%x\n", "Version:", header.e_version);

  // Entry point address:
  printf("  %-35s0x%x\n", "Entry point address:", header.e_entry);

  // Start of program headers:
  printf("  %-35s%d (bytes into file)\n", "Start of program headers:", header.e_phoff);

  // Start of section headers:
  printf("  %-35s%d (bytes into file)\n", "Start of section headers:", header.e_shoff);

  // Flags:
  printf("  %-35s0x%x\n", "Flags:", header.e_flags);

  // Size of this header:
  printf("  %-35s%d (bytes)\n", "Size of this header:", header.e_ehsize);

  // Size of program headers:
  printf("  %-35s%d (bytes)\n", "Size of program headers:", header.e_phentsize);

  // Number of program headers:
  printf("  %-35s%d\n", "Number of program headers:", header.e_phnum);

  // Size of section headers:
  printf("  %-35s%d (bytes)\n", "Size of section headers:", header.e_shentsize);

  // Number of section headers:
  printf("  %-35s%d\n", "Number of section headers:", header.e_shnum);

  // Section header string table index:
  printf("  %-35s%d\n", "Section header string table index:", header.e_shstrndx);

}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Le programme nécessite un et un seul argument.\n");
    return 1;
  }
  if (access(argv[1], F_OK) != 0) {
    printf("Le fichier n'existe pas.\n");
    return 2;
  }
  read_elf_header(argv[1]);
  print_header();
  return 0;
}