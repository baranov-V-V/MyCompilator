#include "Language.h"
#include "Compile.h"

#define BUF compile->buffer + compile->buf_ofs
#define OFS compile->buf_ofs

//#define PRINT_BUF_ARGS(format, arguments) compile->buf_ofs += sprintf(compile->buffer + compile->buf_ofs, format, arguments)
//#define PRINT_BUF_NO_ARGS(format) compile->buf_ofs += sprintf(compile->buffer + compile->buf_ofs, format)

#define FUNCTION compile->table->functions[compile->func_no]

void MakeNamesTable(Tree* tree, NamesTable* table) {
    assert(tree);

    int func_count = GetFuncCount(tree);
    NamesTableConstruct(table);

    table->functions = (Function*) calloc(func_count, sizeof(Function));
    table->func_count = func_count;

    Node* node = tree->root;

    for (int i = 0; i < table->func_count; ++i) {
        GetFunction(node->right, table->functions + i);
        node = node->left;
    }
}

int GetFuncCount(Tree* tree) {
    assert(tree);

    int count = 0;
    Node* node = tree->root;

    while (node != nullptr) {
        node = node->left;
        count++;
    }

    return count;
}

void NamesTableConstruct(NamesTable* table) {
    assert(table);

    table->func_count = 0;
    table->functions = nullptr;
}

void NamesTableDestruct(NamesTable* table) {
    assert(table);

    table->func_count = -1;
    free(table->functions);
}

void GetFunction(Node* node, Function* func) {
    assert(node);
    assert(func);

    func->func_name = node->data.id;

    func->arg_count = GetArgCount(node->right);
    func->var_count = GetVarCount(node->left->right);
    func->is_void   = VoidCheck(node->left->right);
    printf("func [%s], void count [%d]\n", func->func_name, func->is_void);

    func->var_names = (char**) calloc(func->arg_count + func->var_count, sizeof(char*));

    GetVarNames(node, func->var_names);
}

int GetArgCount(Node* node) {
    int count = 0;

    while (node != nullptr) {
        node = node->right;
        ++count;
    }

    return count;
}

int GetVarCount(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    if (node->type == TYPE_DECL) {
        return 1 + GetVarCount(node->left) + GetVarCount(node->right);
    }

    return GetVarCount(node->left) + GetVarCount(node->right);
}

int VoidCheck(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    if (node->type == TYPE_RETURN && node->right != nullptr) {
        return 1 + VoidCheck(node->left) + VoidCheck(node->right);
    }

    return VoidCheck(node->left) + VoidCheck(node->right);
}

void GetVarNames(Node* node, char** var_names) {
    assert(var_names);
    assert(node);

    Node* node_tmp = node->right;
    int ofs = 0;

    while (node_tmp != nullptr) {
        var_names[ofs] = node_tmp->data.id;
        node_tmp = node_tmp->right;
        ++ofs;
    }

    GetVarDecl(node->left->right, var_names, &ofs);
}

void GetVarDecl(Node* node, char** var_names, int* ofs) {
    assert(var_names);
    assert(ofs);

    if (node == nullptr) {
        return;
    }

    GetVarDecl(node->left, var_names, ofs);

    if (node->type == TYPE_DECL) {
        var_names[*ofs] = node->left->data.id;
        (*ofs)++;
        return;
    }

    GetVarDecl(node->right, var_names, ofs);
}

int GetFunctionId(NamesTable* table, char* func_name) {
    assert(table);
    assert(func_name);

    for (int i = 0; i < table->func_count; ++i) {
        if (strcmp(table->functions[i].func_name, func_name) == 0) {
            return i;
        }
    }

    return -1;
}

void DumpTable(NamesTable* table) {
    assert(table);

    printf("\nFunctions count: [%d]\n", table->func_count);

    for (int i = 0; i < table->func_count; ++i) {
        printf("Name: [%s]\n", table->functions[i].func_name);
        if (table->functions[i].is_void == 0) {
            printf("It is void [true]\n");
        } else {
            printf("It is non void [false]\n");
        }
        printf("Variables count: [%d]\nArguments count: [%d]\n", table->functions[i].var_count, table->functions[i].arg_count);

        printf("Variables and arguments:\n");
        for (int j = 0; j < table->functions[i].arg_count + table->functions[i].var_count; ++j) {
            printf("    [%s]\n", table->functions[i].var_names[j]);
        }
        printf("\n");
    }
}

void ConstructCompile(CompileInfo* compile, NamesTable* table) {
    assert(compile);
    assert(table);

    compile->table = table;
    compile->func_no = 0;
    compile->labels_count = 0;
    compile->buf_ofs = 0;
    compile->buffer = (char*) calloc(buf_size, sizeof(char));
    assert(compile->buffer);

    #ifdef DEFAULT_OUTPUT
    compile->fp = fopen(standart_output, "w");
    #else
    fprintf(stderr, "Enter file name\n");
    char* file_name = (char*) calloc(30, sizeof(char));
    scanf("%s", file_name);

    compile->fp = fopen(file_name, "w");
    if (compile->fp == nullptr) {
        fprintf(stderr, "Can not create file\n");
        return;
    }
    #endif
}

void DestructCompile(CompileInfo* compile) {
    assert(compile);

    NamesTableDestruct(compile->table);
    compile->func_no = -1;
    compile->labels_count = -1;

    free(compile->buffer);

    fclose(compile->fp);
}

void MakeCompilation(Tree* tree) {
    assert(tree);

    NamesTable table = {};
    NamesTableConstruct(&table);
    MakeNamesTable(tree, &table);

    CompileInfo compile = {};
    ConstructCompile(&compile, &table);

    DumpTable(&table);

    CompileFunctions(&compile, tree->root);

    fprintf(compile.fp, "%s", compile.buffer);

    DestructCompile(&compile);

    fprintf(stderr, "Done compilation!\n");
}

void CompileFunctions(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    OFS += sprintf(BUF, "section .text\n\n"
                        "global _start\n\n"
                        "_start:        \n"
                        "call main      \n"
                        "mov rax, 0x3C  \n"
                        "xor rdi, rdi   \n"
                        "syscall        \n");

    while (node != nullptr) {
        CompileFuction(compile, node->right);
        node = node->left;
        compile->func_no++;
    }

    AddStandartFuncs(compile);

    printf("Done functions compilation\n");
}

void CompileFuction(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    OFS += sprintf(BUF, "             \n\n"
                        "%s:            \n"
                        "push rbp       \n"
                        "mov rbp, rsp   \n"
                        "sub rsp, 8 * %d\n", node->data.id, FUNCTION.var_count + FUNCTION.arg_count);

    if (node->right != nullptr) {
        CompileArguments(compile, node->right);
    }

    CompileCompound(compile, node->left);
}

void CompileArguments(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    for (int i = 0; i < FUNCTION.arg_count; ++i) {
        OFS += sprintf(BUF, "mov rax, [rbp + 8 * %d + 8]   \n", GetVarIndex(&(FUNCTION), node->data.id));
        OFS += sprintf(BUF, "mov [rbp - 8 * %d], rax; fetching arg [%s] into local stack frame\n", GetVarIndex(&(FUNCTION), node->data.id), node->data.id);
        node = node->right;
    }
}

void CompileCompound(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    while (node->right != nullptr) {
        node = node->right;
        if (node->left != nullptr) {
            CompileStatement(compile, node->left);
        }
    }
}

void CompileStatement(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    switch (node->type) {
        case TYPE_DECL:
            CompileDeclaration(compile, node);
            break;

        case TYPE_ASSGN:
            CompileAssignment(compile, node);
            break;

        case TYPE_RETURN:
            CompileReturn(compile, node);
            break;

        case TYPE_IF:
            CompileCondition(compile, node);
            break;

        case TYPE_WHILE:
            CompileLoop(compile, node);
            break;

        case TYPE_CALL:
            CompileCall(compile, node);
            break;

        default :
            printf("Unknown node type %d", node->type);
            break;

    }
}

void CompileDeclaration(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    CompileExpression(compile, node->right);
    OFS += sprintf(BUF, "pop rax   \n");
    OFS += sprintf(BUF, "mov [rbp - 8 * %d], rax ; assembling declaration var [%s]\n", GetVarIndex(&(FUNCTION), node->left->data.id), node->left->data.id);
}

void CompileAssignment(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    CompileExpression(compile, node->right);
    OFS += sprintf(BUF, "pop rax   \n");
    OFS += sprintf(BUF, "mov [rbp - 8 * %d], rax ;assembling assignment var [%s]\n", GetVarIndex(&(FUNCTION), node->left->data.id), node->left->data.id);
}

void CompileReturn(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    if (node->right != nullptr) {
        CompileExpression(compile, node->right);
        OFS += sprintf(BUF, "pop rax ; getting return value\n");
    }

   OFS += sprintf(BUF, "add rsp, 8 * %d \n"
                        "pop rbp         \n"
                        "ret 8 * %d  ; returning from function [%s]\n", FUNCTION.var_count + FUNCTION.arg_count, FUNCTION.arg_count, FUNCTION.func_name);

}

void CompileCondition(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    CompileExpression(compile, node->left);
    int current_label = compile->labels_count++;

    OFS += sprintf(BUF, "\n;begin assembling <if> with label [%d]\n", current_label);
    OFS += sprintf(BUF, "pop rax      \n");
    OFS += sprintf(BUF, "cmp rax, 1   \n"
                        "je  true_%d  \n"
                        "jmp false_%d \n\n"
                        "true_%d:     \n", current_label, current_label, current_label);
    CompileCompound(compile, node->right->left);

    OFS += sprintf(BUF, "jmp end_cond_%d  \n\n"
                        "false_%d:        \n", current_label, current_label);

    if (node->right->right != nullptr) {
        CompileCompound(compile, node->right->right);
    }

    OFS += sprintf(BUF, "end_cond_%d:     \n", current_label);
    OFS += sprintf(BUF, "\n;end assembling <if> with label [%d]\n", current_label);
}

void CompileLoop(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    int current_label = compile->labels_count++;
    OFS += sprintf(BUF, "\n;begin assembling loop with label [%d]\n", current_label);
    OFS += sprintf(BUF, "while_%d:  \n", current_label);


    CompileExpression(compile, node->left);

    OFS += sprintf(BUF, "pop rax         \n");
    OFS += sprintf(BUF, "cmp rax, 0      \n"
                        "je end_while_%d \n\n", current_label);


    CompileCompound(compile, node->right);

    OFS += sprintf(BUF, "jmp while_%d   \n"
                        "end_while_%d:  \n", current_label, current_label);
    OFS += sprintf(BUF, "\n;end assembling <while> with label [%d]\n", current_label);
};

void CompileCall(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);


    Node* tmp_node = node;
    while (tmp_node->right != nullptr) {
        tmp_node = tmp_node->right;
    }

    int standart_index = GetStandartIndex(node->left->data.id);
    if (standart_index != -1) {
        CompileStandartFunc(compile, node, standart_index);
        return;
    }

    int func_id = GetFunctionId(compile->table, node->left->data.id);

    if (func_id == -1) {
        //fprintf(stderr, "No such function found <%s>\n", node->left->data.id);
        return;
    }

    for (int i = 0; i < compile->table->functions[func_id].arg_count; ++i) {
        CompileExpression(compile, tmp_node->left);
        tmp_node = tmp_node->parent;
    }

    OFS += sprintf(BUF, "call %s\n", compile->table->functions[func_id].func_name);
    if (compile->table->functions[func_id].is_void != 0) {
        OFS += sprintf(BUF, "push rax ; return value of called function\n");
    }

}

void CompileExpression(CompileInfo* compile, Node* node) {
    assert(node);
    assert(compile);

    if (node->type == TYPE_OP && (node->data.op >= OP_EQUAL && node->data.op <= OP_GREATER_EQUAL)) {
        CompileSimpExpression(compile, node->left);
        CompileSimpExpression(compile, node->right);
        MakeCompare(compile, node);
    }
    else {
        CompileSimpExpression(compile, node);
    }
}

void MakeCompare(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    OFS += sprintf(BUF, "\n;begin assembling <compare> with label [%d]\n", compile->labels_count);
    OFS += sprintf(BUF, "pop rbx      \n"
                        "pop rax      \n"
                        "cmp rax, rbx \n");

    switch (node->data.op) {
        case OP_EQUAL:
            OFS += sprintf(BUF, "je ");
            break;

        case OP_NOT_EQUAL:
            OFS += sprintf(BUF, "jne ");
            break;

        case OP_GREATER:
            OFS += sprintf(BUF, "jg ");
            break;

        case OP_LESS:
            OFS += sprintf(BUF, "jl ");
            break;

        case OP_GREATER_EQUAL:
            OFS += sprintf(BUF, "jge ");
            break;

        case OP_LESS_EQUAL:
            OFS += sprintf(BUF, "jle ");
            break;

        default:
            fprintf(stderr, "Unknown op type: %d", node->data.op);
            break;
    }

    int current_label = compile->labels_count++;
    OFS += sprintf(BUF, "true_%d           \n"
                        "push 0            \n"
                        "jmp end_comp_%d   \n"
                        "true_%d:          \n"
                        "push 1            \n"
                        "jmp end_comp_%d   \n"
                        "end_comp_%d:      \n", current_label, current_label, current_label, current_label, current_label);
}

void CompileSimpExpression(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    if (node->type == TYPE_OP) {
            CompileSimpExpression(compile, node->left);
            CompileSimpExpression(compile, node->right);

        OFS += sprintf(BUF, "\n");
        OFS += sprintf(BUF, "pop rbx  ;begin assembling math op [%s]\n"
                            "pop rax  \n", reference_op[node->data.op]);

        switch (node->data.op) {
            case OP_ADD:
                OFS += sprintf(BUF, "add rax, rbx\n");
                break;

            case OP_SUB:
                OFS += sprintf(BUF, "sub rax, rbx\n");
                break;

            case OP_MUL:
                OFS += sprintf(BUF, "cqo           \n");
                OFS += sprintf(BUF, "imul rbx      \n");
                break;

            case OP_DIV:
                OFS += sprintf(BUF, "cqo           \n");
                OFS += sprintf(BUF, "idiv rbx      \n");
                break;

            default:
                printf("Unknown math type %d\n", node->data.op);
        }
        OFS += sprintf(BUF, "push rax  ;end assembling math op\n");

    }
    else {
        CompilePrimExpression(compile, node);
    }
}

void CompilePrimExpression(CompileInfo* compile, Node* node) {
    assert(compile);
    assert(node);

    switch (node->type) {
        case TYPE_CONST:
            OFS += sprintf(BUF, "push %d\n", (int)node->data.num);
            break;

        case TYPE_VAR:
            OFS += sprintf(BUF, "mov rax, [rbp - 8 * %d] ; var [%s]\n", GetVarIndex(&(FUNCTION), node->data.id), node->data.id);
            OFS += sprintf(BUF, "push rax  \n"); // additional 8 because of ret in stack
            break;

        case TYPE_CALL:
            CompileCall(compile, node);
            break;

        default:
            printf("Unknown node type %d", node->type);
    }
}

void CompileStandartFunc(CompileInfo* compile, Node* node, int standart_index) {
    assert(compile);

    if (node->right != nullptr && standart_index != IDX_IN) {
        CompileExpression(compile, node->right->left);
    }

    switch (standart_index) {

        case IDX_SQRT:
            OFS += sprintf(BUF, "pop rax ; assembling sqrt    \n"
                                "cvtsi2sd xmm0, rax           \n"
                                "sqrtsd xmm0, xmm0            \n"
                                "cvttsd2si rax, xmm0          \n"
                                "push rax                     \n");
            break;

        case IDX_OUT:
            OFS += sprintf(BUF, "pop rdi        \n");
            OFS += sprintf(BUF, "call PrintSign \n");
            break;

        case IDX_IN:
            OFS += sprintf(BUF, "call GetNumber          \n");
            OFS += sprintf(BUF, "mov [rbp - 8 * %d], rax \n", GetVarIndex(&(FUNCTION), node->right->left->data.id));
            break;

        default:
            printf("No such standart function with IDX [%d] implemented yet\n", standart_index);

    }
}

int GetStandartIndex(char* func_name) {
    assert(func_name);

    for (int i = 0; i < IDX_NUM; ++i) {
        if (strcmp(standart_func_comp[i], func_name) == 0) {
            return i;
        }
    }

    return -1;
}

int GetVarIndex(Function* func, char* id) {
    assert(func);
    assert(id);

    for (int i = 0; i < func->arg_count + func->var_count; ++i) {
        if (strcmp(func->var_names[i], id) == 0) {
            return i + 1;
        }
    }

    fprintf(stderr, "Couldn't find var with name <%s>", id);
    return -1;
}

void AddStandartFuncs(CompileInfo* compile) {
    assert(compile);

    OFS += sprintf(BUF, "%s", standart_func_msg);
    OFS += sprintf(BUF, "%s\n", out_function);
    OFS += sprintf(BUF, "%s\n", in_function);
    OFS += sprintf(BUF, "%s", section_data);
}
