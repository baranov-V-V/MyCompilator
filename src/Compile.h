#pragma once

#define DEFAULT_OUTPUT
const char* standart_output = "asm_code.asm";

typedef struct Function {
    char* func_name;
    char** var_names;
    int arg_count;
    int var_count;
		int is_void;
} Function;

typedef struct NamesTable {
    Function* functions;
    int func_count;
} NamesTable;

typedef struct CompileInfo {
    NamesTable* table;
    FILE* fp;
    int labels_count;
    int func_no;
} CompileInfo;

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

const char* in_function = 
	"GetNumber:													 \n"
	"        xor r10, r10								 \n"
	"        mov rax, 0						  		 \n"
	"	       mov rdi, 1									 \n"
	"	       mov rsi, Buffer						 \n"
	"	       mov rdx, buf_size					 \n"
	"	       syscall	           				 \n"
	"	       													   \n"
	"	       xor r14, r14 							 \n"
	"	       xor rax, rax								 \n"
	"        														 \n"
	"	       mov byte al, [Buffer + r14] \n"
	"	       cmp al, '-'								 \n"
	"	       jne no_neg									 \n"
	"	       inc r14								     \n"
	"	       mov r10, 1							     \n"
	"																		 \n"
	"    no_neg:												 \n"
	"				 xor rdi, rdi								 \n"
	"        														 \n"
	"        mov byte al, [Buffer + r14] \n"
	"        inc r14										 \n"
	"																		 \n"
	"    loop_number_get:								 \n"
	"																     \n"
	"	       sub al, '0'							   \n"
	"	       														 \n"
	"	       add rdi, rax								 \n"
	"	       														 \n"
	"	       mov byte al, [Buffer + r14] \n"
	"	       inc r14										 \n"
	"              											 \n"
	"	       cmp al, 0xA								 \n"
	"	       je end_get_number					 \n"
	"	       														 \n"
	"	       mov bl, al								   \n"
	"        														 \n"
	"	       mov rax, rdi								 \n"
	"	       mov rdi, 10								 \n"
	"	       														 \n"
	"	       mul rdi										 \n"
	"	       mov rdi, rax								 \n"
	"              											 \n"
	"	       xor rax, rax								 \n"
	"	       mov al, bl									 \n"
	"	       jmp loop_number_get				 \n"
	"																		 \n"
	"    end_get_number:								 \n"
	"																		 \n"
	"	       mov rax, rdi								 \n"
	"              											 \n"
	"	       cmp r10, 1									 \n"
	"	       jne end_function						 \n"
	"        														 \n"
	"	       neg rax										 \n"
	"																		 \n"
	"    end_function:			 						 \n"
	" 																	 \n"
	"        ret												 \n";


//PrintSign needs translation number in rdi
const char* out_function =
	"PrintSign:													 \n"
	"				 xor r14, r14							   \n"
	"																	   \n"
  "        mov rax, mask        		   \n"
	"																	   \n"
  "        and rax, rdi							   \n"
  "        or rax, rax							   \n"
  "        jz print_num           	   \n"
  "     														   \n"
  "        neg rdi									   \n"
  "																	   \n"
  "        mov al, '-'							   \n"
  "        mov [Buffer + r14], al		   \n"
  "        inc r14       						   \n"
  "																	   \n"
	"    print_num:									     \n"
  "        mov rsi, 10						     \n"
  "        call PrintNum					     \n"
	"																     \n"
  "        ret										     \n"
	";END PrintSign          			  	   \n"
	"																	   \n"
	"PrintNum:         								   \n"
	"        mov rax, rdi;						   \n"
	"																	   \n"
  "        xor rdi, rdi							   \n"
  "        xor rcx, rcx							   \n"
  "																	   \n"
  "loop_number_transl:							   \n"
  "        xor rdx, rdx							   \n"
  "																	   \n"
  "        div rsi									   \n"
  "																	   \n"
  "        mov rbx, rdx							   \n"
  "        add rbx, Numbers					   \n"
  "        mov cl, byte [rbx]				   \n"
  "        push rcx									   \n"
  "																	   \n"
  "        inc rdi									   \n"
  "        or rax, rax							   \n"
  "        jnz loop_number_transl		   \n"
  "																	   \n"
  "loop_number_print:								   \n"
  "        pop rax									   \n"
  "																	   \n"
  "        mov byte [Buffer + r14], al \n"
  "        inc r14									   \n"
  "																	   \n"
  "        dec rdi									   \n"
  "        or rdi, rdi							   \n"
  "        jne loop_number_print		   \n"
  "																	   \n"
  "        call PrintBuff						   \n"
  "        ret											   \n"
  ";END PrintDec										   \n"
  "																	   \n"
	"PrintBuff:												   \n"
  "        or r14, r14					       \n"
  "        jz end_print_buff		       \n"
  "																		 \n"
	"				 mov al, 0xA								 \n"
	"				 mov byte [Buffer + r14], al \n"
	"				 inc r14				  		       \n"
  "																	   \n"
	"        mov rax, 1						       \n"
  "        mov rsi, Buffer             \n"
  "        mov rdx, r14                \n"
  "        mov rdi, 1						       \n"
  "        syscall							       \n"
  "                                    \n"
  "        xor r14, r14					       \n"
  "                                    \n"
  "end_print_buff:							       \n"
  "        ret									       \n"
  ";END PrintBuff								       \n";

const char* section_data =
	"section    .data								     \n"
	"Numbers:		db \"0123456789ABCDEF\"  \n"
	"buf_size   equ 64								   \n"
	"mask       equ 8000000000000000h    \n"
	"Buffer:    times buf_size db 0	     \n";

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
void DumpTable(NamesTable* table);
void ConstructCompile(CompileInfo* compile, NamesTable* table);
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
