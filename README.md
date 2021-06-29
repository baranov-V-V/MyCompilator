# My compilator
In this progect i will make my own programming language with front-end and back-end compilation in nasm or directly into x86_64 instruction set. Also I will develop my own syntax to this language simular to C syntax.
## Table of contents
1. **[Description of my language](#1-description-of-my-language)**
     - [1. Tmp Fill](#tmp-fill)
2. **[How compilations works](#2-how-compilation-works)**
     - [Front end](#front-end)
     - [Middle end](#middle-end)
     - [Back end](#back-end)
       - [Into asm language](#into-asm-language)
       - [Into x86_64 instriction](#into-x86_64-instructions)
3. **[Instruction encoding](#3-instruction-encoding)**
     * [Mov](#mov)
       - [Mov r64, r64](#mov-r64-r64)
       - [Mov r64, imm64](#mov-r64-imm64)
       - [Mov r64, m64 or m64, r64](#mov-r64-m64-or-m64-r64)
         - [Only base](#only-base)
         - [Base and index](#base-and-index)
     * [Add](#add)
       - [Add r64, r64](#add-r64-r64)
       - [Add r64, imm32](#add-r64-imm32)
     * [3. Sub](#sub)
       - [Sub r64, r64](#sub-r64-r64)
       - [Sub r64, imm32](#sub-r64-imm32)
     * [4. Imul](#imul)
     * [5. Idiv](#idiv)
     * [6. Push](#push)
       - [Push r64](#push-r64)
       - [Push imm32](#push-imm32)
     * [7. Pop](#pop)
     * [8. Cmp](#cmp)
       - [Cmp r64, r64](#cmp-r64-r64)
       - [Cmp r64, imm32](#cmp-r64-imm32)
     * [9. Jump](#jump)
     * [10. Call](#call)
     * [11. Logic](#logic)
4. **[Making Custom Elf](#4-making-custom-elf)**
5. **[Optimization](#5-optimization)**
6. **[Final result](#6-final-result)**

# 1. Description of my language

# 2. How compilation works
Lets assume we have a program written in some programming language. When we want to compile and run a program, we run for example GCC (Gnu Compiler Collection) and get an output file, which can be executed. But how do we get file that can be run

It's a very complicated and interesting way.

## Front end
Now let's see what is done in first stage of compiling a program.
If we have macros, the automaticly opens in contents they have. After that, a special program called "Lexer" starts checking if all the data in input file is correct according to syntax of programming language. If so, the program starts parcing into an array of tokens called "lexems"

## Middle end

## Back end

### Into asm language

### Into x86_64 instructions

# 3. Instruction encoding
Now it's time to figure out how we can generate _byte code_ of  

So operation code or opcode for short is a byte sequenct that the processor will read and execute. Opcode can be from 1 to 15 bytes long and is usually generated my assembler. However, in my progect i will make opcodes of several operation by myself.
First of all, we need to undestand basic view of a command and .

So if we want to make an opcode what do we need?
Firstly, we need some bytes that will tell general info of an instruction
Secondly, we need place to store operands: registers or immidiate constant


I will not describe every command in this chapter, but i will try to give you basic knowledge of x86_64 opcode.

Since we work in 64-bit mode or _long mode_ is some opcodes we have special byte called REX byte which is placed at the beginning of an opcode.

Basic view of REX byte:
```
+---+---+---+---+---+---+---+---+
| 0   1   0   0 | W | R | X | B |
+---+---+---+---+---+---+---+---+
```

Basic view of MODRM byte:
```
+---+---+---+---+---+---+---+---+
|  mod  |    reg    |     rm    |
+---+---+---+---+---+---+---+---+
```


Next is Scale Index Base byte or SIB byte. It is used in encoding [mov r64, m64](#mov-r64-m64) or [mov m64, r64](#mov-m64-r64) with both base and index registers.
Basic view of SIB byte:
```
+---+---+---+---+---+---+---+---+
| scale |   index   |    base   |
+---+---+---+---+---+---+---+---+
```
Since we can adress to memory using construction [base + index * scale + disp]
Scale is encoded with 2 bits and can be 1, 2, 4 or 8
Index is 3-bit field which has index register coded in 3 bits and if we use r8-15 registers we set REX.X bit as 1
Base is 3-bit field which has base register coded in 3 bits and if we use r8-r15 registers we set REX.B bit as 1

First of all 
Some commands have REX byte o

## Mov
### Mov r64, r64
### Mov r64, imm64
### Mov r64, m64 or m64, r64
##### Only base
##### Base and index

## Add
### Add r64, r64
### Add r64, imm32

## Sub
### Sub r64, r64
### Sub r64, imm32

## Imul

## Idiv

## Push
### Push r64
### Push imm32

## Pop

## Cmp
### Cmp r64, r64
### Cmp r64, imm32
  
## Jump

## Call

## Logic

# 4. Making custom Elf

# 5. Optimization

# 6. Final result

