#include <elf.h>
#include <stdio.h>
// #include <string.h>

#include "readelf.h"

void print_header(Elf32_Ehdr *ehdr)
{
    printf("ELF Header:\n");

    // Magic:
    printf("  %-8s", "Magic:");
    for (int i = 0; i < 16; i++)
    {
        printf(" %02x", ehdr->e_ident[i]);
    }
    printf("\n");

    // Class:
    printf("  %-35s", "Class:");
    switch (ehdr->e_ident[EI_CLASS])
    {
    case ELFCLASSNONE:
        printf("Invalid class\n");
        break;
    case ELFCLASS32:
        printf("ELF32\n");
        break;
    case ELFCLASS64:
        printf("ELF64\n");
        break;
    default:
        printf("\n");
    }

    // Data:
    printf("  %-35s", "Data:");
    switch (ehdr->e_ident[EI_DATA])
    {
    case ELFDATANONE:
        printf("Invalid data encoding\n");
        break;
    case ELFDATA2LSB:
        printf("2's complement, little endian\n");
        break;
    case ELFDATA2MSB:
        printf("2's complement, big endian\n");
        break;
    default:
        printf("\n");
    }

    // Version:
    printf("  %-35s%d\n", "Version:", ehdr->e_ident[EI_VERSION]);

    // OS/ABI: (switch from internet)
    printf("  %-35s", "OS/ABI:");
    switch (ehdr->e_ident[EI_OSABI])
    {
    case 0x00:
        printf("SYSTEM V\n");
        break;
    case 0x01:
        printf("HP-UX\n");
        break;
    case 0x02:
        printf("NET BSD\n");
        break;
    case 0x03:
        printf("Linux\n");
        break;
    case 0x04:
        printf("HP-UX\n");
        break;
    case 0x05:
        printf("GNU Hurd\n");
        break;
    case 0x06:
        printf("Solaris\n");
        break;
    case 0x07:
        printf("AIX\n");
        break;
    case 0x08:
        printf("IRIX\n");
        break;
    case 0x09:
        printf("FreeBSD\n");
        break;
    case 0x0A:
        printf("TRU64\n");
        break;
    case 0x0B:
        printf("Novell Modesto\n");
        break;
    case 0x0C:
        printf("OpenBSD\n");
        break;
    case 0x0D:
        printf("OpenVMS\n");
        break;
    case 0x0E:
        printf("NonStop Kernel\n");
        break;
    case 0x0F:
        printf("AROS\n");
        break;
    case 0x10:
        printf("Fenix OS\n");
        break;
    case 0x11:
        printf("CloudABI\n");
        break;
    case 0x12:
        printf("Stratus Technologies OpenVOS\n");
        break;
    default:
        printf("\n");
    }

    // ABI Version:
    printf("  %-35s%d\n", "ABI Version:", ehdr->e_ident[EI_ABIVERSION]);

    // Type:
    printf("  %-35s", "Type:");
    switch (ehdr->e_type)
    {
    case ET_NONE:
        printf("No file type\n");
        break;
    case ET_REL:
        printf("Relocatable file\n");
        break;
    case ET_EXEC:
        printf("Executable file\n");
        break;
    case ET_DYN:
        printf("Shared object file\n");
        break;
    case ET_CORE:
        printf("Core file\n");
        break;
    default:
        printf("\n");
    }

    // Machine: (switch from internet)
    printf("  %-35s", "Machine:");
    switch (ehdr->e_machine)
    {
    case 0x00:
        printf("No specific instruction set\n");
        break;
    case 0x01:
        printf("AT&T WE 32100\n");
        break;
    case 0x02:
        printf("SPARC\n");
        break;
    case 0x03:
        printf("x86\n");
        break;
    case 0x04:
        printf("Motorola 68000k (M68k)\n");
        break;
    case 0x05:
        printf("Motorola 88000 (M88k)\n");
        break;
    case 0x06:
        printf("Intel MCU\n");
        break;
    case 0x07:
        printf("Intel 80860\n");
        break;
    case 0x08:
        printf("MIPS\n");
        break;
    case 0x09:
        printf("IBM_System/370\n");
        break;
    case 0x0A:
        printf("MIPS RS3000 Little-endian\n");
        break;
    case 0x0B:
        printf("Reserved for future use\n");
        break;
    case 0x0C:
        printf("Reserved for future use\n");
        break;
    case 0x0D:
        printf("Reserved for future use\n");
        break;
    case 0x0E:
        printf("Hewlett-Packard PA-RISC\n");
        break;
    case 0x0F:
        printf("Reserved for future use\n");
        break;
    case 0x13:
        printf("Intel 80960\n");
        break;
    case 0x14:
        printf("PowerPC\n");
        break;
    case 0x15:
        printf("PowerPC(64-bit)\n");
        break;
    case 0x16:
        printf("S390, including S390x\n");
        break;
    case 0x28:
        printf("ARM (up to ARMv7/Aarch32)\n");
        break;
    case 0x2A:
        printf("SuperH\n");
        break;
    case 0x32:
        printf("IA-64\n");
        break;
    case 0x3E:
        printf("amd64\n");
        break;
    case 0x8C:
        printf("TMS320C6000 Family\n");
        break;
    case 0xB7:
        printf("ARM 64-bits (ARMv8/Aarch64)\n");
        break;
    case 0xF3:
        printf("RISC-V\n");
        break;
    case 0xF7:
        printf("Berkeley Packet Filter\n");
        break;
    case 0x101:
        printf("WDC 65C816\n");
        break;
    default:
        printf("\n");
    }

    // Version:
    printf("  %-35s0x%x\n", "Version:", ehdr->e_version);

    // Entry point address:
    printf("  %-35s0x%x\n", "Entry point address:", ehdr->e_entry);

    // Start of program headers:
    printf("  %-35s%d (bytes into file)\n", "Start of program headers:", ehdr->e_phoff);

    // Start of section headers:
    printf("  %-35s%d (bytes into file)\n", "Start of section headers:", ehdr->e_shoff);

    // Flags:
    printf("  %-35s0x%x\n", "Flags:", ehdr->e_flags);

    // Size of this header:
    printf("  %-35s%d (bytes)\n", "Size of this header:", ehdr->e_ehsize);

    // Size of program headers:
    printf("  %-35s%d (bytes)\n", "Size of program headers:", ehdr->e_phentsize);

    // Number of program headers:
    printf("  %-35s%d\n", "Number of program headers:", ehdr->e_phnum);

    // Size of section headers:
    printf("  %-35s%d (bytes)\n", "Size of section headers:", ehdr->e_shentsize);

    // Number of section headers:
    printf("  %-35s%d\n", "Number of section headers:", ehdr->e_shnum);

    // Section header string table index:
    printf("  %-35s%d\n", "Section header string table index:", ehdr->e_shstrndx);
}

