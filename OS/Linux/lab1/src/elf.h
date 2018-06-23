#ifndef __LIBS_ELF_H__
#define __LIBS_ELF_H__

#include <defs.h>

#define ELF_MAGIC    0x464C457FU            // "\x7FELF" in little endian

/*
elfhdr     ELF file header 
  硬盘中文件存储的elf格式，这了主要用来存储OS

proghdr    program section header
　 内存中，程序块的存储格式

*/


/* file header ELF格式的文件 的header 文件头*/
struct elfhdr {
    uint32_t e_magic;     // must equal ELF_MAGIC
// e_magic，是用来判断读出来的ELF格式的文件是否为正确的格式
    uint8_t e_elf[12];
    uint16_t e_type;      // 1=relocatable, 2=executable, 3=shared object, 4=core image
    uint16_t e_machine;   // 3=x86, 4=68K, etc.
    uint32_t e_version;   // file version, always 1
    uint32_t e_entry;     // e_entry，是程序入口所对应的虚拟地址,如果存储的是个os，则是内核入口地址
                          // entry point if executable
    uint32_t e_phoff;     // e_phoff，是program header表的位置偏移 
                          // file position of program header or 0
    uint32_t e_shoff;     // file position of section header or 0
    uint32_t e_flags;     // architecture-specific flags, usually 0
    uint16_t e_ehsize;    // size of this elf header
    uint16_t e_phentsize; // size of an entry in program header
    uint16_t e_phnum;     // e_phnum，是program header表中的入口数目 大小
                          // number of entries in program header or 0
    uint16_t e_shentsize; // size of an entry in section header
    uint16_t e_shnum;     // number of entries in section header or 0
    uint16_t e_shstrndx;  // section number that contains section name strings
};

/* 在内存中 进程(程序)块 的存储方式
program section header */
struct proghdr {
    uint32_t p_type;   // loadable code or data, dynamic linking info,etc.
    uint32_t p_offset; // p_offset，段相对于文件头的偏移 file offset of segment
    uint32_t p_va;     // p_va，一个对应当前段的虚拟地址 virtual address to map segment
    uint32_t p_pa;     // 物理地址　physical address, not used
    uint32_t p_filesz; // size of segment in file
    uint32_t p_memsz;  // p_memsz，当前段的内存大小
                       // size of segment in memory (bigger if contains bss）
    uint32_t p_flags;  // read/write/execute bits
    uint32_t p_align;  // required alignment, invariably hardware page size
};

#endif /* !__LIBS_ELF_H__ */
