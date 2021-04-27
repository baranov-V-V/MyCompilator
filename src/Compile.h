#pragma once

const char* standart_output = "a.out";
const char* standart_nasm_out = "nasm_out.asm";

const int buf_size = 512000;

struct Function {
    char* func_name;
    char** var_names;
    int arg_count;
    int var_count;
    int is_void;
    int entry;
};

struct NamesTable {
    Function* functions;
    int func_count;
};

struct FunctionCall {
    char* name;
    int pos;
};

struct BuffInfo {
    FILE* fp;
    char* buffer;
    int buf_ofs;
};

struct Flags {
    bool standart_out;
    bool optimize;
    bool dump_nasm;
    bool dump_tree;
    bool dump_tokens;
    bool dump_func;
};

struct OptimizeData {
    int ofs;
    int operand;
    int op;
};

struct CompileInfo {
    NamesTable* table;
    FunctionCall* calls;
    int call_count;

    FILE* fp;
    char* buffer;
    int buf_ofs;

    OptimizeData* optimize;
    Flags* flags;

    int labels_count;
    int func_no;
    int code_size;
    int data_ofs;
};

const char* standart_func_comp[] {
    "B.1.2",        //"printf
    "B.1.3",        //"scanf"
    "B.4_2",        //"cos"
    "B.4_1",        //"sin"
    "B.4_3",        //"sqrt"
};

const char* standart_func_asm[] {
    "out",
    "in",
    "cos",
    "sin",
    "sqrt"
};

enum STANDART_INDEX {
    IDX_OUT,
    IDX_IN,
    IDX_COS,
    IDX_SIN,
    IDX_SQRT,

    IDX_NUM
};

const int standart_func_count = 2;

const int get_buff_entry1 = 15;
const int get_buff_entry2 = 39;
const int get_buff_entry3 = 62;
const int get_buff_entry4 = 77;
const int get_number_length = 124;
const unsigned char GetNumber[] = {
0x4d, 0x31, 0xd2,                                           //xor r10, r10
0xb8, 0x00, 0x00, 0x00, 0x00,                               //mov rax, 0
0xbf, 0x01, 0x00, 0x00, 0x00,                               //mov rdi, 1
0x48, 0xbe, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rsi, Buffer
0xba, 0x40, 0x00, 0x00, 0x00,                               //syscall                       ;getting string with number in it
0x0f, 0x05,                                                 //xor r14, r14                  ;current pos in string
0x4d, 0x31, 0xf6,                                           //xor rax, rax
0x48, 0x31, 0xc0,                                           //mov byte al, [Buffer + r14]
0x41, 0x8a, 0x86, 0x10, 0x20, 0x40, 0x00,                   //cmp al, '-'                   ;checking if there is '-' in front of string
0x3c, 0x2d,                                                 //jne no_neg
0x75, 0x09,                                                 //inc r14
0x49, 0xff, 0xc6,                                           //mov r10, 1                    ;r10 stores if number is neg
0x41, 0xba, 0x01, 0x00, 0x00, 0x00,                         //no neg: xor rdi, rdi
0x48, 0x31, 0xff,                                           //mov byte al, [Buffer + r14]
0x41, 0x8a, 0x86, 0x10, 0x20, 0x40, 0x00,                   //inc r14
0x49, 0xff, 0xc6,                                           //get_num: sub al, '0'          ;getting one byte in each iteration of loop
0x2c, 0x30,                                                 // add rdi, rax
0x48, 0x01, 0xc7,                                           // mov byte al, [Buffer + r14]
0x41, 0x8a, 0x86, 0x10, 0x20, 0x40, 0x00,                   // inc r14
0x49, 0xff, 0xc6,                                           //cmp al, 0xA     ; '\n'        ;checks if we reached the end of string with number
0x3c, 0x0a,                                                 //je end_get_num
0x74, 0x17,                                                 //mov bl, al
0x88, 0xc3,                                                 //mov rax, rdi
0x48, 0x89, 0xf8,                                           //mov rax, rdi
0xbf, 0x0a, 0x00, 0x00, 0x00,                               //mov rdi, 10
0x48, 0xf7, 0xe7,                                           //mul rdi
0x48, 0x89, 0xc7,                                           //mov rdi, rax
0x48, 0x31, 0xc0,                                           //xor rax, rax
0x88, 0xd8,                                                 //mov al, bl
0xeb, 0xd6,                                                 //jmp get_num
0x48, 0x89, 0xf8,                                           //end_get_num: mov rax, rdi
0x49, 0x83, 0xfa, 0x01,                                     //cmp r10, 1                      ;checks if number is neg
0x75, 0x03,                                                 //jne end_func
0x48, 0xf7, 0xd8,                                           //neg rax                         ;invert if number has to be neg
0xc3                                                        //end_func: ret
};

const int print_buff_entry1 = 29;
const int print_buff_entry2 = 81;
const int print_buff_entry3 = 120;
const int print_numbers_entry1 = 62;
const int print_sign_length = 142;
const unsigned char PrintSign[] = {
0x4d, 0x31, 0xf6,                                           //xor r14, r14                      ;preparing buff len
0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, //mov rax, mask
0x48, 0x21, 0xf8,                                           //and rax, rdi                      ;checking if number is negative
0x48, 0x09, 0xc0,                                           //or rax, rax
0x74, 0x0f,                                                 //jz print_num
0x48, 0xf7, 0xdf,                                           //neg rdi                           ;int case of negative, make the number positive with same value
0xb0, 0x2d,                                                 //mov al, '-'                       ;put '-' in buffer (case of negative)
0x41, 0x88, 0x86, 0x10, 0x20, 0x40, 0x00,                   //mov [Buffer + r14], al
0x49, 0xff, 0xc6,                                           //inc r14
0xbe, 0x0a, 0x00, 0x00, 0x00,                               //print_num: mov rsi, 10            ;loop to print number
0x48, 0x89, 0xf8,                                           //mov rax, rdi;
0x48, 0x31, 0xff,                                           //xor rdi, rdi
0x48, 0x31, 0xc9,                                           //xor rcx, rcx
0x48, 0x31, 0xd2,                                           //loop_number_transl: xor rdx, rdx
0x48, 0xf7, 0xf6,                                           //div rsi                           ;getting number from 0 to 9
0x48, 0x89, 0xd3,                                           //mov rbx, rdx
0x48, 0x81, 0xc3, 0x00, 0x20, 0x40, 0x00,                   //add rbx, Numbers
0x8a, 0x0b,                                                 //mov cl, byte [rbx]                ;getting ascii code of number to print
0x51,                                                       //push rcx                          ;pushing number we got into stack
0x48, 0xff, 0xc7,                                           //inc rdi                           ;this is made because we get inverted number
0x48, 0x09, 0xc0,                                           //or rax, rax                       ;if we put directly into buffer
0x75, 0xe5,                                                 //jnz loop_number_transl
0x58,                                                       //loop_number_print: pop rax
0x41, 0x88, 0x86, 0x10, 0x20, 0x40, 0x00,                   //mov byte [Buffer + r14], al       ;popping numbers from stack and put into buffer
0x49, 0xff, 0xc6,                                           //inc r14
0x48, 0xff, 0xcf,                                           //dec rdi
0x48, 0x09, 0xff,                                           //or rdi, rdi
0x75, 0xed,                                                 //jne loop_number_print
0x4d, 0x09, 0xf6,                                           //or r14, r14
0x74, 0x28,                                                 //jz end_print_buff
0xb0, 0x0a,                                                 //mov al, 0xA                       ;putting '\n' in the end of buffer
0x41, 0x88, 0x86, 0x10, 0x20, 0x40, 0x00,                   //mov byte [Buffer + r14], al
0x49, 0xff, 0xc6,                                           //inc r14
0xb8, 0x01, 0x00, 0x00, 0x00,                               //mov rax, 1
0x48, 0xbe, 0x10, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, //mov rsi, Buffer                   ;preparing buff for s
0x4c, 0x89, 0xf2,                                           //mov rdx, r14
0xbf, 0x01, 0x00, 0x00, 0x00,                               //mov rdi, 1
0x0f, 0x05,                                                 //syscall                           ;making syscall to print buff
0x4d, 0x31, 0xf6,                                           //xor r14, r14
0xc3                                                        //ret
};

const int buffer_length = 64;
const int section_data_length = 16;
const int data_segment_size = 80;
const char SectionData[] = {
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46};

//nasm functions
const char* standart_func_msg =
	"                                                                       \n"
	";This is standart input output funcs                                   \n"
	"                                                                       \n";

const char* in_function =
	"GetNumber:													            \n"
	"        xor r10, r10                                                   \n"
	"        mov rax, 0                                                     \n"
	"        mov rdi, 1                                                     \n"
	"        mov rsi, Buffer                                                \n"
	"        mov rdx, buf_size                                              \n"
	"        syscall                                                        \n"
	"                                                                       \n"
	"        xor r14, r14                                                   \n"
	"        xor rax, rax                                                   \n"
	"                                                                       \n"
	"        mov byte al, [Buffer + r14]                                    \n"
	"        cmp al, '-'                                                    \n"
	"        jne no_neg	                                                    \n"
	"        inc r14                                                        \n"
	"        mov r10, 1                                                     \n"
	"                                                                       \n"
	"    no_neg:                                                            \n"
	"        xor rdi, rdi                                                   \n"
	"                                                                       \n"
	"        mov byte al, [Buffer + r14]                                    \n"
	"        inc r14                                                        \n"
	"                                                                       \n"
	"    loop_number_get:                                                   \n"
	"                                                                       \n"
	"        sub al, '0'                                                    \n"
	"                                                                       \n"
	"        add rdi, rax                                                   \n"
	"                                                                       \n"
	"        mov byte al, [Buffer + r14]                                    \n"
	"        inc r14                                                        \n"
	"                                                                       \n"
	"        cmp al, 0xA                                                    \n"
	"        je end_get_number                                              \n"
	"                                                                       \n"
	"        mov bl, al                                                     \n"
	"                                                                       \n"
	"        mov rax, rdi                                                   \n"
	"        mov rdi, 10                                                    \n"
	"                                                                       \n"
	"        mul rdi                                                        \n"
	"        mov rdi, rax                                                   \n"
	"                                                                       \n"
	"        xor rax, rax                                                   \n"
	"        mov al, bl                                                     \n"
    "        jmp loop_number_get                                            \n"
	"                                                                       \n"
	"    end_get_number:                                                    \n"
	"                                                                       \n"
	"        mov rax, rdi                                                   \n"
	"                                                                       \n"
	"        cmp r10, 1                                                     \n"
	"        jne end_function                                               \n"
    "                                                                       \n"
	"        neg rax                                                        \n"
	"                                                                       \n"
	"    end_function:                                                      \n"
	"                                                                       \n"
	"        ret                                                            \n"
    ";END GetNumber                                                         \n";

//PrintSign needs translation number in rdi
const char* out_function =
	"PrintSign:                                                             \n"
  "        xor r14, r14                                                     \n"
  "                                                                         \n"
  "        mov rax, mask                                                    \n"
  "                                                                         \n"
  "        and rax, rdi                                                     \n"
  "        or rax, rax                                                      \n"
  "        jz print_num                                                     \n"
  "                                                                         \n"
  "        neg rdi                                                          \n"
  "                                                                         \n"
  "        mov al, '-'                                                      \n"
  "        mov [Buffer + r14], al                                           \n"
  "        inc r14                                                          \n"
  "                                                                         \n"
  "        print_num:                                                       \n"
  "        mov rsi, 10                                                      \n"
  "                                                                         \n"
  ";begin of PrintNum                                                       \n"
  "                                                                         \n"
  "        mov rax, rdi;                                                    \n"
  "                                                                         \n"
  "        xor rdi, rdi                                                     \n"
  "        xor rcx, rcx                                                     \n"
  "                                                                         \n"
  "loop_number_transl:                                                      \n"
  "        xor rdx, rdx                                                     \n"
  "                                                                         \n"
  "        div rsi                                                          \n"
  "                                                                         \n"
  "        mov rbx, rdx                                                     \n"
  "        add rbx, Numbers                                                 \n"
  "        mov cl, byte [rbx]                                               \n"
  "        push rcx                                                         \n"
  "                                                                         \n"
  "        inc rdi                                                          \n"
  "        or rax, rax                                                      \n"
  "        jnz loop_number_transl                                           \n"
  "                                                                         \n"
  "loop_number_print:                                                       \n"
  "        pop rax                                                          \n"
  "                                                                         \n"
  "        mov byte [Buffer + r14], al                                      \n"
  "        inc r14                                                          \n"
  "                                                                         \n"
  "        dec rdi                                                          \n"
  "        or rdi, rdi                                                      \n"
  "        jne loop_number_print                                            \n"
  "                                                                         \n"
  ";begin of PrintBuff                                                      \n"
  "                                                                         \n"
  "        or r14, r14                                                      \n"
  "        jz end_print_buff                                                \n"
  "                                                                         \n"
  "        mov al, 0xA                                                      \n"
  "        mov byte [Buffer + r14], al                                      \n"
  "        inc r14                                                          \n"
  "                                                                         \n"
  "        mov rax, 1                                                       \n"
  "        mov rsi, Buffer                                                  \n"
  "        mov rdx, r14                                                     \n"
  "        mov rdi, 1                                                       \n"
  "        syscall                                                          \n"
  "                                                                         \n"
  "        xor r14, r14                                                     \n"
  "                                                                         \n"
  "end_print_buff:                                                          \n"
  "        ret                                                              \n"
  "                                                                         \n"
  ";END PrintSign                                                           \n";

const char* section_data =
	"section    .data                                       \n"
	"Numbers:   db \"0123456789ABCDEF\"                     \n"
	"buf_size   equ 64                                      \n"
	"mask       equ 8000000000000000h                       \n"
	"Buffer:    times buf_size db 0                         \n";

Tree* BuildTree(Tokens* tokens, FILE* fp, Flags* flags);
void Assemble(Tree* tree);
void WriteAsmCode(Tree* tree, NamesTable* table, FILE* file);

void MakeNamesTable(Tree* tree, NamesTable* table);
void NamesTableConstruct(NamesTable* table);
void GetFunction(Node* node, Function* func);
int GetFuncCount(Tree* tree);
int GetArgCount(Node* node);
int GetVarCount(Node* node);
int VoidCheck(Node* node);
void GetVarNames(Node* node, char** var_names);
void GetVarDecl(Node* node, char** var_names, int* ofs);

void DumpTable(NamesTable* table);

void NamesTableConstruct(NamesTable* table);
void FillNamesTable(Tree* tree, NamesTable* table);
void GetFunction(Node* node, Function* func);
int GetArgCount(Node* node);
int GetVarCount(Node* node);
void GetVarNames(Node* node, char** var_names);
void GetVarDecl(Node* node, char** var_names, int* ofs);
int GetFunctionId(NamesTable* table, char* func_name);
void InsertInOutFunctions(NamesTable* table);
void DumpTable(NamesTable* table);
void DumpFuncCalls(CompileInfo* compile);
void DumpFuncEntries(CompileInfo* compile);
void ConstructCompile(CompileInfo* compile, NamesTable* table, Flags* flags);
void DestructCompile(CompileInfo* compile);
void MakeCompilation(Tree* tree);
void CompileFunctions(CompileInfo* compile, Node* node);
void CompileFuction(CompileInfo* compile, Node* node);
void CompileArgumets(CompileInfo* compile, Node* node);
void CompileCompound(CompileInfo* compile, Node* node);
void CompileStatement(CompileInfo* compile, Node* node);
void CompileDeclaration(CompileInfo* compile, Node* node);
void CompileAssignment(CompileInfo* compile, Node* node);
void CompileReturn(CompileInfo* compile, Node* node);
void CompileCondition(CompileInfo* compile, Node* node);
void CompileLoop(CompileInfo* compile, Node* node);
void CompileCall(CompileInfo* compile, Node* node);
void CompileExpression(CompileInfo* compile, Node* node);
void MakeCompare(CompileInfo* compile, Node* node);
void CompileSimpExpression(CompileInfo* compile, Node* node);
void CompilePrimExpression(CompileInfo* compile, Node* node);
void CompileStandartFunc(CompileInfo* compile, Node* node, int standart_index);
void AddStandartFuncs(CompileInfo* info);
int GetStandartIndex(char* func_name);
void CompileArguments(CompileInfo* compile, Node* node);
int GetVarIndex(Function* func, char* id);

void AddStandartFuncs(CompileInfo* compile);
void FillStandartEntries(CompileInfo* compile, int numbers_ofs, int buff_ofs);
void MakeElfHeaders(CompileInfo* compile);
void FillFuncCalls(CompileInfo* compile);
