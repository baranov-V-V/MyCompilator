//#include Compile.h

#include <iostream>
#include <assert.h>
#include <string.h>

#include "Translator.h"

#define BUF compile->buffer + compile->buf_ofs
#define OFS compile->buf_ofs

inline void WriteBuf(CompileInfo* compile, void* source, int byte_count) {
    assert(compile);
    assert(source);
    assert(byte_count > 0);

    memcpy(BUF, source, byte_count);
    OFS += byte_count;
}
inline void WriteByte(CompileInfo* compile, char byte) {
    assert(compile);

    WriteBuf(compile, &byte, 1);
}
inline void WriteWord(CompileInfo* compile, void* source) {
    assert(compile);
    assert(source);

    WriteBuf(compile, source, 2);
}
inline void WriteDword(CompileInfo* compile, void* source) {
    assert(compile);
    assert(source);

    WriteBuf(compile, source, 4);
}
inline void WriteQword(CompileInfo* compile, void* source) {
    assert(compile);
    assert(source);

    WriteBuf(compile, source, 8);
}

inline void UpdateOptimizeData(CompileInfo* compile, int current_op, int operand) {
    assert(compile);
    compile->optimize->op = current_op;
    compile->optimize->operand = operand;
    compile->optimize->ofs = OFS;
}

/*!
Quick reference:
"x >> 3" stands for x / 8
"x &  7" stands for x % 8
!*/

void WriteMov(CompileInfo* compile, int reg_dest, int reg_source) {
    assert(compile);
    assert(reg_dest >= 0 && reg_dest <= 15);
    assert(reg_source >= 0 && reg_source <= 15);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, MOV_RR, -1); };

    char mov_opcode[] = {REX_W | ((reg_source >> 3) << 2) | (reg_dest >> 3), (char) MOV_RR,
                        (MOD_RR << 6) | ((reg_source & 7) << 3) | (reg_dest & 7)};

    WriteBuf(compile, mov_opcode, sizeof(mov_opcode));
};
void WriteMov(CompileInfo* compile, int reg_dest, int64_t constant) {
    assert(compile);
    assert(reg_dest >= 0 && reg_dest <= 15);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, MOV_RC, -1); };

    char mov_opcode[] = {REX_W | (reg_dest >> 3), (char) MOV_RC | (reg_dest & 7)};

    WriteWord(compile, mov_opcode);
    WriteQword(compile, &constant);     //writing imm64 constant after mov opcode
};
void WriteMov(CompileInfo* compile, int reg, int base, int disp, char* mode) {
    assert(compile);
    assert(reg >= 0 && reg <= 15);
    assert(base >= 0 && base <= 15);
    assert(mode);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, MOV_RM, -1); };

    char mov_opcode[] = {REX_W | ((reg >> 3) << 2) | (base >> 3), 0x0,                  //0x0 is a tmp fill which i will fill later
                        (MOD_MR << 6) | ((reg & 7) << 3) | (base & 7), 0x0};

    if (mode[0] == 'r' && mode[1] == 'm') {
        mov_opcode[1] = MOV_RM;
    } else if (mode[0] == 'm' && mode[1] == 'r') {
        mov_opcode[1] = MOV_MR;
    } else {
        fprintf(stderr, "No such mode [%s] in WriteMov", mode);
    }

    if (base == RSP || base == R12) {
        mov_opcode[3] = 0x24;
        WriteDword(compile, mov_opcode);
    } else {
        WriteBuf(compile, mov_opcode, 3);   // only 3 in length because we don't add 0x24
    }

    WriteDword(compile, &disp);             // copying disp32 into command
};

void WriteRet(CompileInfo* compile) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, RET_OP, -1); };
    WriteByte(compile, RET_OP);
};

int WriteJmp(CompileInfo* compile, int jmp_type) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, JMP, -1); };

    if (jmp_type != JMP) {
        WriteByte(compile, PREFIX);           // every jump except always-taken jmp needs this byte
    }

    int adress_fill = -1;
    WriteByte(compile, jmp_type);           // here we write jump instruction and fill relative adress of jump with temporary constant -1
    WriteDword(compile, &adress_fill);      // later i will put real adress of jump instead of this fill

    return OFS - 5;                         // ofs of byte that represents jmp_type.
};

void FillJmp(CompileInfo* compile, int jmp_pos, int jmp_disp) {
    assert(compile);

    jmp_disp -= 5;                          // -5 is because we count disp from end of the jmp instruction
    memcpy(compile->buffer + jmp_pos + 1, &(jmp_disp), sizeof(int));
}

void FillCall(CompileInfo* compile, int call_pos, int call_disp) {
    assert(compile);

    call_disp -= 5;                         // -5 is because we count disp from end of the call instruction
    memcpy(compile->buffer + call_pos + 1, &(call_disp), sizeof(int));
}

void WriteLogic(CompileInfo* compile, int reg_dest, int reg_source, int op) {
    assert(compile);
    assert(reg_dest >= 0 && reg_dest <= 15);
    assert(reg_source >= 0 && reg_source <= 15);
    assert(op == OR || op == XOR || op == AND);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, -1, -1); };

    char logic_opcode[] = {REX_W | ((reg_source >> 3) << 2) | (reg_dest >> 3), op,
                          (MOD_RR << 6) | ((reg_source & 7) << 3) | (reg_dest & 7)};

    WriteBuf(compile, logic_opcode, sizeof(logic_opcode));
};

void WriteAdd(CompileInfo* compile, int reg_dest, int operand, char type) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, -1, -1); };

    if (type == 'r') {              // this type stands for "add r64, r64"
        char add_opcode[] = {REX_W | ((operand >> 3) << 2) | (reg_dest >> 3), (char) ADD_REG,
                            (MOD_RR << 6) | ((operand & 7) << 3) | (reg_dest & 7)};

        WriteBuf(compile, add_opcode, sizeof(add_opcode));
    } else if (type == 'c') {       // this type stands for "add r64, imm32"
        char add_opcode[] = {REX_W | (reg_dest >> 3), (char) ADD_CONST,
                            (MOD_RR << 6) | (reg_dest & 7)};

        WriteBuf(compile, add_opcode, sizeof(add_opcode));
        WriteDword(compile, &operand);
    } else {
        fprintf(stderr, "No such type [%c] in WriteAdd", type);
    }
};

void WriteSub(CompileInfo* compile, int reg_dest, int operand, char type) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, SUB_REG, -1); };

    if (type == 'r') {              // this type stands for "sub r64, r64"
        char sub_opcode[] = {REX_W | ((operand >> 3) << 2) | (reg_dest >> 3), (char) SUB_REG,
                            (MOD_RR << 6) | ((operand & 7) << 3) | (reg_dest & 7)};

        WriteBuf(compile, sub_opcode, sizeof(sub_opcode));
    } else if (type == 'c') {       // this type stands for "sub r64, imm32"
        char sub_opcode[] = {REX_W | (reg_dest >> 3), (char) SUB_CONST,
                            (MOD_RR << 6) | (reg_dest & 7) | (5 << 0x3)};

        WriteBuf(compile, sub_opcode, sizeof(sub_opcode));
        WriteDword(compile, &operand);
    } else {
        fprintf(stderr, "No such type [%c] in WriteSub", type);
    }
};

void WriteImul(CompileInfo* compile, int reg) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, IMUL, -1); };

    char imul_opcode[] = {REX_W | (reg >> 3), (char) IMUL,
                         (MOD_RR << 6) | (5 << 3) | (reg & 7)};  // "5 << 3" for mul so we have modrm 11 101 ***

    WriteBuf(compile, imul_opcode, sizeof(imul_opcode));
};

void WriteIdiv(CompileInfo* compile, int reg) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, IDIV, -1); };

    char idiv_opcode[] = {REX_W | (reg >> 3), (char) IDIV,
                         (MOD_RR << 6) | (7 << 3) | (reg & 7)};  //"7 << 3" for div so we have modrm 11 111 ***

    WriteBuf(compile, idiv_opcode, sizeof(idiv_opcode));
};

void WriteCall(CompileInfo* compile, char* func_name) {
    assert(compile);
    assert(func_name);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, CALL_OP, -1); };

    int adress_fill = -1;
    WriteByte(compile, CALL_OP);                // here we write call instruction and fill relative adress of call with temporary constant -1
    WriteDword(compile, &adress_fill);          // later i will put real adress of call instead of this fill

    compile->calls[compile->call_count].name = func_name;   //adding function to call table of all functions to fill it's adress later
    compile->calls[compile->call_count].pos = OFS - 5;
    compile->call_count++;
};

void WriteSyscall(CompileInfo* compile) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, SYSCALL, -1); };

    WriteByte(compile, PREFIX);
    WriteByte(compile, SYSCALL);
};

void WriteCqo(CompileInfo* compile) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, CQO, -1); };

    WriteByte(compile, REX_W);
    WriteByte(compile, CQO);
}

void WriteCmp(CompileInfo* compile, int reg, int operand, char type) {
    assert(compile);

    if (compile->flags->optimize) { UpdateOptimizeData(compile, CMP_REG, -1); };

    if (type == 'r') {
        char cmp_opcode[] = {REX_W | ((operand >> 3) << 2) | (reg >> 3), (char) CMP_REG,
                            (MOD_RR << 6) | ((operand & 7) << 3) | (reg & 7)};

        WriteBuf(compile, cmp_opcode, sizeof(cmp_opcode));
    } else if (type == 'c') {
        char cmp_opcode[] = {REX_W | (reg >> 3), (char) CMP_CONST, (31 << 3) | (reg & 7)};   // 31 is 11111 in bits
                                            // so basically "cmp r64, disp32" has MODRM which is in bits: "11 111 ***"

        WriteBuf(compile, cmp_opcode, sizeof(cmp_opcode));
        WriteDword(compile, &operand);
    } else {
        fprintf(stderr, "No such type [%c] in WriteCmp", type);
    }

};

void WritePush(CompileInfo* compile, int operand, char type) {
    assert(compile);

    if (type == 'r') {
        if (compile->flags->optimize) {
            if (compile->optimize->op == POP_REG && compile->optimize->operand == operand) {
                OFS = compile->optimize->ofs;
                UpdateOptimizeData(compile, -1, -1);
                return;
            }
            UpdateOptimizeData(compile, PUSH_REG, operand);
        }
        if (operand <= 7) {
            WriteByte(compile, PUSH_REG + operand);
        } else {
            WriteByte(compile, REX | B);
            WriteByte(compile, PUSH_REG + operand & 7);
        }
    } else if (type == 'c') {
        if (compile->flags->optimize) { UpdateOptimizeData(compile, PUSH_CONST, operand); };

        WriteByte(compile, PUSH_CONST);
        WriteDword(compile, &operand);
    } else {
        fprintf(stderr, "No such type [%c] in WritePush", type);
    }
};

void WritePop(CompileInfo* compile, int reg) {
    assert(compile);
    assert(reg < 16);

    if (compile->flags->optimize) {
        if (compile->optimize->op == PUSH_REG && compile->optimize->operand == reg) {
            OFS = compile->optimize->ofs;
            UpdateOptimizeData(compile, -1, -1);
            return;
        }
        UpdateOptimizeData(compile, POP_REG, reg);
    }

    if (reg <= 7) {
        WriteByte(compile, POP_REG + reg);
    } else {
        WriteByte(compile, REX | B);
        WriteByte(compile, POP_REG + reg & 7);
    }
};


