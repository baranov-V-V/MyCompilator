# My compilator
In this progect i will make my own programming language with front-end and back-end compilation in nasm or directly into x86_64 instruction set. Also I will develop my own syntax to this language simular to C syntax.
## Table of contents
1. **[Description of my language](#my-language)**
     - [1. Tmp Fill](#tmp-fill)
2. **[How compilations works](#how-compilation-wokrs)**
     - [1. Front end](#2-front-end)
     - [2. Middle end](#3-middle-end)
     - [3. Back end](#4-back-end)
       - [Into asm language](#into-asm-language)
       - [Into x86_64 instriction](#into-x86_64-instructions)
3. **[Instruction encoding](#instruction-encoding)**
4. **[Making Custom Elf](#making-custom-elf)**
5. **[Optimization](#optimization)**
6. **[Final result](#final-result)**

# 1. Description of my language

# 2. How compilation works
Lets assume we have a program written in some programming language. When we want to compile and run a program, we run for example GCC (Gnu Compiler Collection) and get an output file, which can be executed. But how do we get file that can be run

It's a very complicated and interesting way.

## 1. Front end
Now let's see what is done in first stage of compiling a program.
If we have macros, the automaticly opens in contents they have. After that, a special program called "Lexer" starts checking if all the data in input file is correct according to syntax of programming language. If so, the program starts parcing into an array of tokens called "lexems"

## 2. Middle end

## 3. Back end

### Into asm language

### Into x86_64 instructions

# 3. Instruction encoding
Now it's time to figure out how we can generate byte 

# 4. Making custom Elf

# 5. Optimization

# 6. Final result

