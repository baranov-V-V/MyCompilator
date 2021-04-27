#pragma once

const char* help_message =
"-------------------K&R compilator HELP---------------------\n"
"   Options:                                                \n"
"                                                           \n"
"       -o [output file]                                    \n"
"           make custom output file                         \n"
"                                                           \n"
"       --optimize                                          \n"
"           optimize program for speed                      \n"
"                                                           \n"
"       -s                                                  \n"
"           generate only nasm code                         \n"
"                                                           \n"
"       --dump_tree [output file name]                      \n"
"           dump abstract syntax tree of your program       \n"
"                                                           \n"
"       --dump_tokens                                       \n"
"           dump parsed tokens in console                   \n"
"                                                           \n"
"       --dump_functions                                    \n"
"           NOT AVAILABLE WITH -s FLAG                      \n"
"           dump calls, entries and general info of         \n"
"           each function in code                           \n"
"           dumps in console                                \n"
"                                                           \n"
"       --dump_all [output file name]                       \n"
"           NOT AVAILABLE WITH -s FLAG                      \n"
"           make all previous dumps                         \n"
"           needs file name for tree dump                   \n";

const char* file_output_str = "-o";
const char* nasm_generate_str = "-s";
const char* optimize_str = "--optimize";
const char* help_str = "--help";
const char* dump_tree_str = "--dump_tree";
const char* dump_tokens_str = "--dump_tokens";
const char* dump_functions_str = "--dump_functions";
const char* dump_all_str = "--dump_all";

const char* help_message2 =
"------------------------------K&R-compiler-HELP--------------------------------\n"
"   Options:                                                                   |\n"
"                                                                              |\n"
"       -o [output file]                                                       |\n"
"           make custom output file                                            |\n"
"                                                                              |\n"
"       --optimize                                                             |\n"
"           optimize program for speed                                         |\n"
"                                                                              |\n"
"       -s                                                                     |\n"
"           generate only nasm code                                            |\n"
"                                                                              |\n"
"       --dump_tree [output file name]                                         |\n"
"           dump abstract syntax tree of your program                          |\n"
"                                                                              |\n"
"       --dump_tokens                                                          |\n"
"           dump parsed tokens in console                                      |\n"
"                                                                              |\n"
"       --dump_functions                                                       |\n"
"           NOT AVAILABLE WITH -s FLAG                                         |\n"
"           dump calls, entries and general info of                            |\n"
"           each function in code                                              |\n"
"           dumps in console                                                   |\n"
"                                                                              |\n"
"       --dump_all [output file name]                                          |\n"
"           NOT AVAILABLE WITH -s FLAG                                         |\n"
"           make all previous dumps                                            |\n"
"           needs file name for tree dump                                      |\n";

