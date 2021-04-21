#include "TXLib.h"

#include "Language.h"
#include "Compile.h"
#include "BuildTree.cpp"
#include "LanguageFunctions.cpp"
#include "LanguageDump.cpp"
#include "Compile_old.cpp"

int main() {
    Tokens tokens;
    ConstructTokens(&tokens);

    Tree* tree = BuildTree(&tokens);

    TreeVisualDump(tree->root, "new_syntax_tree8");

    MakeCompilation(tree);

    DestructToxens(&tokens);
    TreeDestruct(tree);
}
