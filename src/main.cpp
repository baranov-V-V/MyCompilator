//#include "TXLib.h"

#include "Language.h"
#include "Compile.h"
#include "BuildTree.cpp"
#include "Translator.h"
#include "TranslateBin.cpp"
#include "LanguageFunctions.cpp"
#include "LanguageDump.cpp"
#include "Compile_old.cpp"
#include "Help.h"

int main(int argc, char* argv[]) {
    if (argc > 1 && strcmp(argv[1], help_str) == 0) {
        printf("%s", help_message2);
        return 0;
    }

    if (argc < 2) {
        fprintf(stderr, "No input file, abort compilation\nTry \"main.exe --help\"\n");
        return 2;
    }

    Flags* flags = ConstructFlags();

    char* output_file = nullptr;
    char* tree_file = nullptr;
    char* input_file = argv[1];
    FILE* fp_input = fopen(input_file, "r");
    if (fp_input == nullptr) {
        fprintf(stderr, "Could find file with name [%s] abort compilation\n", input_file);
        return 1;
    }

    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], file_output_str) == 0) {
            if (output_file != nullptr) {
                fprintf(stdout, "warning: several output files declaration, only last one will be used: [%s]\n", argv[i + 1]);
            }
            output_file = argv[++i];
            flags->standart_out = false;
        } else if (strcmp(argv[i], nasm_generate_str) == 0) {
            flags->dump_nasm = true;
        } else if (strcmp(argv[i], optimize_str) == 0) {
            flags->optimize = true;
        } else if (strcmp(argv[i], dump_tree_str) == 0) {
            if (tree_file != nullptr) {
                fprintf(stdout, "warning: several tree dump files declaration, only last one will be used: [%s]\n", argv[i + 1]);
            }
            tree_file = argv[++i];
            flags->dump_tree = true;
        } else if (strcmp(argv[i], dump_tokens_str) == 0) {
            flags->dump_tokens = true;
        } else if (strcmp(argv[i], dump_functions_str) == 0) {
            flags->dump_func = true;
        } else if (strcmp(argv[i], dump_all_str) == 0) {
            flags->dump_func   = true;
            flags->dump_tokens = true;
            flags->dump_tree   = true;
            if (tree_file != nullptr) {
                fprintf(stdout, "warning: several tree dump files declaration, only last one will be used: [%s]\n", argv[i + 1]);
            }
            tree_file = argv[++i];
        }
    }

    Tokens tokens;
    ConstructTokens(&tokens);

    Tree* tree = BuildTree(&tokens, fp_input, flags);

    if (flags->dump_tree) {
        TreeVisualDump(tree->root, tree_file);
    }

    MakeCompilation(tree, flags, output_file);

    DestructToxens(&tokens);
    TreeDestruct(tree);
}
