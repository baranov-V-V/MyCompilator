#pragma once

#include <stdio.h>
#include <ctype.h>

/*
struct CompileInfo {
    char* buffer;
    int buf_ofs;
};
*/

const int EI_NIDENT = 16;
const int start_virt_adress = 0x400000;

struct ElfHeader {
  	//instead of standart "e_ident[EI_NIDENT]"
  	//i will make it manually, size is 16 bytes

    //!begin of ELF "magic number"
  	const uint8_t initial_byte = 0x7F;
  	const uint8_t elf_name[3] = {'E', 'L', 'F'};

  	const uint8_t elf_class = 2;
    const uint8_t file_endian = 1; //little endian for x86_64 processors

    const uint8_t header_version = 1;
    const uint8_t os_abi = 0;

    const uint8_t free_mem[8] = {'B', 'V', 'V', 0x0, 0x0, 0x0, 0x0, 0x0};
    //const uint64_t free_mem = 0x0;	//ELF64 header just needs it
    //!end of ELF "magic number"

    const uint16_t file_type          = 2;	                    //executive type
  	const uint16_t instruction_type   = 0x3E;	                  //x86_64 processors
    const uint32_t elf_version        = 1;
  	const uint64_t entry              = 0x401000;               // Entry point virtual address
  	const uint64_t prog_header_offset = sizeof(ElfHeader);		  // Program header table file offset, goes after elf header
  	const uint64_t sect_header_offset = 0;                      //section header is optional for exec files so we ommit it in our file
  	const uint32_t flags              = 0;                      //no flags for elf
  	const uint16_t elf_header_size    = sizeof(ElfHeader);
  	const uint16_t prog_header_size   = 56;                     //program header size
  	const uint16_t prog_header_num    = 3;	                    //only one program header (needed in exec file)
  	const uint16_t sect_header_size   = 0;                      //we dont' have section header (optional for exec)
  	const uint16_t sect_header_num    = 0;	                    //we dont' have section header (optional for exec)
  	const uint16_t sect_header_index  = 0;                      //we dont' have section header (optional for exec)
};

struct ProgramHeader {
    const uint32_t type           = 1;
    uint32_t flags                = 0;
    uint64_t program_offset       = 0;		                    /* Segment file offset */
    uint64_t p_vaddr              = 0;	                        /* Segment virtual address */
    const uint64_t p_paddr        = 0;		                    /* Segment physical address */
    uint64_t file_size            = 0;		                    /* Segment size in file */
    uint64_t mem_size             = 0;		                    /* Segment size in memory */
    const uint64_t program_align  = 0x1000;		                /* Segment alignment, file & memory */
};

/*
struct SectionHeader {
    const uint32_t name = 0x21;  //not useful???
    const uint32_t type = 1;      //SHT_PROGBITS
    const uint64_t flags = 3;     //alloc (2) + write (1)
    const uint64_t adress = 0x402000; //
    const uint64_t offset = 0x2000; //offset
    const uint64_t size = 80;    //size
    const uint32_t link = 0;
    const uint32_t info = 0;
    const uint64_t alignment = 0x1000;
    const uint64_t some_size = 0;
};
*/

enum Registers {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
    R8  = 8,
    R9  = 9,
    R10 = 10,
    R11 = 11,
    R12 = 12,
    R13 = 13,
    R14 = 14,
    R15 = 15
};

enum Opcodes {
    SYSCALL = 0x05,
    CALL_OP = 0xE8,
    RET_OP  = 0xC3,

    CMP_REG   = 0x39,
    CMP_CONST = 0x81,

    JMP  = 0xE9,
    JE   = 0x84,
    JNE  = 0x85,
    JG   = 0x8F,
    JGE  = 0x8D,
    JL   = 0x8C,
    JLE  = 0x8E,

    PUSH_REG   = 0x50,
    PUSH_CONST = 0x68,
    POP_REG    = 0x58,

    MOV_RC  = 0xB8,
    MOV_RR  = 0x89,
    MOV_RM  = 0x8B,
    MOV_MR  = 0x89,

    ADD_REG    = 0x01,
    ADD_CONST  = 0x81,
    SUB_REG    = 0x29,
    SUB_CONST  = 0x81,

    IMUL = 0xF7,
    IDIV = 0xF7,

    XOR  = 0x31,
    AND  = 0x21,
    OR   = 0x9,

    INC  = 0xC0,
    CQO  = 0x99,

    PREFIX = 0x0F
};

enum REX_INFO {
    REX   = 0x40,
    REX_W = 0x48,
    W     = 0x8, // 64-bit operand size is used
    R     = 0x4, // extension to the MODRM.reg field
    X     = 0x2, // extension to the SIB.index field
    B     = 0x1  // extension to the MODRM.rm field or the SIB.base field
};

enum MODRM {
  MOD_RR = 11,
  MOD_RM = 10,
  MOD_MR = 10
};

/*!
Basic view of REX byte:
+---+---+---+---+---+---+---+---+
| 0   1   0   0 | W | R | X | B |
+---+---+---+---+---+---+---+---+
It is used in long mode with some operations(registers rax - r15 and so on)
!*/

/*!
Basic view of MODRM byte:
+---+---+---+---+---+---+---+---+
|  mod  |    reg    |     rm    |
+---+---+---+---+---+---+---+---+
mod is 11 when we have reg/reg operands

!*/

/*!
Basic view if SIB byte:
+---+---+---+---+---+---+---+---+
| scale |   index   |    base   |
+---+---+---+---+---+---+---+---+
in memory adressing we have:
[base + index * scale + disp]
scale can be 1, 2, 4, 8 (coded in 2 bits in SIB)
!*/

inline void WriteByte(CompileInfo* compile, char byte);
inline void WriteWord(CompileInfo* compile, void* source);
inline void WriteDword(CompileInfo* compile, void* source);
inline void WriteQword(CompileInfo* compile, void* source);
inline void WriteBuf(CompileInfo* compile, void* source, int byte_count);

void WriteMov(CompileInfo* compile, int reg_dest, int reg_source);
void WriteMov(CompileInfo* compile, int reg_dest, int64_t constant);

/*!
mode can be:
  'rm' mov r64, [r64 + disp32]
  'mr' mov [r64 + disp32], r64
!*/
void WriteMov(CompileInfo* compile, int reg, int base, int disp, char* mode);

void WriteRet(CompileInfo* compile);

int WriteJmp(CompileInfo* compile, int jmp_type);

void WriteLogic(CompileInfo* compile, int reg_dest, int reg_source, int op);

/*!
type can be:
  'r' add r64, r64
  'c' add r64, imm32
!*/
void WriteAdd(CompileInfo* compile, int reg_dest, int operand, char type);

/*!
type can be:
  'r' sub r64, r64
  'c' sub r64, imm32
!*/
void WriteSub(CompileInfo* compile, int reg_dest, int operand, char type);

void WriteImul(CompileInfo* compile, int reg);
void WriteIdiv(CompileInfo* compile, int reg);

void WriteCall(CompileInfo* compile, char* func_name);
void WriteSyscall(CompileInfo* compile);

/*!
type can be:
  'r' cmp r64, r64
  'c' add r64, imm32
!*/
void WriteCmp(CompileInfo* compile, int reg, int operand, char type);

/*!
type can be:
  'r' push r64
  'c' push imm32
!*/
void WritePush(CompileInfo* compile, int operand, char type);

void WritePop(CompileInfo* compile, int reg);

void FillJmp(CompileInfo* compile, int jmp_pos, int jmp_disp);
void FillCall(CompileInfo* compile, int call_pos, int call_disp);

void FillProgramHeader(ProgramHeader* header, uint32_t flags, uint64_t ofs, uint64_t virt_adress, uint64_t file_size, uint64_t mem_size);

