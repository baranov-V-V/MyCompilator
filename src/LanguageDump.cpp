#include "Language.h"
#include "Compile.h"

void TreeVertexPrint(Node* node, FILE* fp) {
    assert(fp != nullptr);

    if (node != nullptr) {
        TreeVertexPrint(node->left, fp);
        switch (node->type) {
            case TYPE_FICT:
                fprintf(fp, "node%p [label =\"<first> Fict\", color=\"grey14\", fillcolor=\"gray38\"]\n",
                        node);
                break;

            case TYPE_DECL:
                fprintf(fp, "node%p [label =\"<first> Decl |<second> %s\", color=\"grey14\", fillcolor=\"plum1\"]\n",
                        node, node->data.id);
                break;

            case TYPE_VAR:
                fprintf(fp, "node%p [label =\"<first> Var |<second> %s\", color=\"grey14\", fillcolor=\"aquamarine2\"]\n",
                        node, node->data.id);
                break;

            case TYPE_ARG:
                fprintf(fp, "node%p [label =\"<first> Arg\", color=\"grey14\", fillcolor=\"lightblue\"]\n",
                        node);
                break;

            case TYPE_BRACE:
                fprintf(fp, "node%p [label =\"<first> Brace |<second> \\%s\", color=\"grey14\", fillcolor=\"snow3\"]\n",
                        node, node->data.id);
                break;

            case TYPE_STAT:
                fprintf(fp, "node%p [label =\"<second> %s\", color=\"grey14\", fillcolor=\"snow3\"]\n",
                        node, node->data.id);
                break;

            case TYPE_IF:
                fprintf(fp, "node%p [label =\"<first> If\", color=\"grey14\", fillcolor=\"mediumturquoise\"]\n",
                        node);
                break;

            case TYPE_IF_ELSE:
                fprintf(fp, "node%p [label =\"<first> If-Else\", color=\"grey14\", fillcolor=\"mediumturquoise\"]\n",
                        node);
                break;

            case TYPE_WHILE:
                fprintf(fp, "node%p [label =\"<first> While\", color=\"grey14\", fillcolor=\"mediumturquoise\"]\n",
                        node);
                break;

            case TYPE_ASSGN:
                fprintf(fp, "node%p [label =\"<first> Assign |<second> %s\", color=\"grey14\", fillcolor=\"palegreen\"]\n",
                        node, node->data.id);
                break;

            case TYPE_CALL:
                fprintf(fp, "node%p [label =\"<first> Call\", color=\"grey14\", fillcolor=\"plum1\"]\n",
                        node);
                break;

            case TYPE_RETURN:
                fprintf(fp, "node%p [label =\"<second> %s\", color=\"grey14\", fillcolor=\"plum1\"]\n",
                        node, node->data.id);
                break;

            case TYPE_OP:
                fprintf(fp, "node%p [label =\"<first> Op |<second> \\%s\", color=\"grey14\", fillcolor=\"palegreen\"]\n",
                        node, reference_math_op[node->data.op]);
                break;

            case TYPE_CONST:
                fprintf(fp, "node%p [label =\"<first> Const |<second> %lg\", color=\"grey14\", fillcolor=\"aquamarine\"]\n",
                        node, node->data.num);
                break;

            default:
                fprintf(stderr, "Unknown node type %d", node->type);
                break;

        }
        TreeVertexPrint(node->right, fp);
    }
}

void TreeEdgesPrint(Node* node, FILE* fp) {
    assert(fp != nullptr);

    if (node != nullptr) {
        TreeEdgesPrint(node->left, fp);
        if (node->left != nullptr) {
            fprintf(fp, "    node%p:sw -> node%p [color=\"navy\"];\n", node, node->left);
        }
        if (node->right != nullptr) {
            fprintf(fp, "    node%p:se -> node%p [color=\"crimson\"];\n", node, node->right);
        }
        TreeEdgesPrint(node->right, fp);
    }
}

void TreeVisualDump(Node* root, const char* file_name) {
    assert(root != NULL);
    assert(file_name != NULL);

    char* command_dot = (char*) calloc(100, sizeof(char));
    char* graph_file  = (char*) calloc(strlen(file_name) + 10, sizeof(char));

    strcpy(graph_file, file_name);
    strcat(graph_file, ".dot");
    FILE* fp = fopen(graph_file, "w");
    assert(fp != NULL);

    strcat(command_dot, "dot -Tpng ");   //graph_test1.dot -o Demo3.png"
    strcat(command_dot, graph_file);
    //strcat(command_dot, " -Gcharset = latin1");
    strcat(command_dot, " -o ");
    strcat(command_dot, file_name);
    strcat(command_dot, "_graph.png");

    fprintf(fp, "digraph structs {\n        node [shape=\"record\", style=\"filled\"];");  //rankdir = LR\n

    TreeVertexPrint(root, fp);
    TreeEdgesPrint(root, fp);

    fprintf(fp, "}");

    fclose(fp);

    fprintf(stderr, "graphing done with code: %d\n", system(command_dot));
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

void DumpFuncCalls(CompileInfo* compile) {
    assert(compile);

    printf("Function calls dump\n\n");
    for (int i = 0; i < compile->call_count; ++i) {
        printf("    [%d] called \"%s\" at pos [0x%x]\n", i, compile->calls[i].name, compile->calls[i].pos);
    }
    printf("End Function calls dump\n\n");
}

void DumpFuncEntries(CompileInfo* compile) {
    assert(compile);

    printf("Function entries dump\n\n");
    for (int i = 0; i < compile->table->func_count; ++i) {
        printf("    [%d] entry of \"%s\" at pos [0x%x]\n", i, compile->table->functions[i].func_name, compile->table->functions[i].entry);
    }
    printf("End Function entries dump\n\n");
}
