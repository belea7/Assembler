# Assembler

**Assembler** is my final project for the "C programming lab course" course in my BSc.

## Summary

The program simulates an assembler, which translates code written in Assembly to machine language.
The program accepts a list of text files, processes the instructions they contain, verifies there is no syntax errors, and outputs the files containing the translated program to machine language.
If any syntax error is found, the program will output an informative error message for the user.

Example of input file:
```
; file ps.as

MAIN:       mov     S1.1, STR
            add     r2, STR
LOOP:       prn     #-5
            sub     r1,r4
            inc     K

            mov     S1.2,r3
END:        stop
STR:        .string "abcdef"
LENGTH:     .data   6,-9,15
K:          .data   22
S1:         .struct 8, "ab"
```

## Hardware
- CPU: 8 general registers (r1,r2,...,r8) and PSW (program status word), 10 bits each.
- RAM (including a stack) in a size of 256 words.
- A word's size is 10 bits.
- Signed 2's complement arithmetic integers (no support for real numbers).

## Incsruction structure
| 9 8 7 6 | 5 4 | 3 2 | 1 0 |
| --- | --- | --- | --- |
| Opcode | Source operand addressing method | Destination operand addressing method | E, R, A|

E, R, A - indicte the coding type: **A**bsolute (00) , **E**xternal (01) or **R**elocatable (10).

## Instructions
| Opcode | Instruction | Description | Example |
| --- | --- | --- | --- |
| 0 | mov | Copies content oof source operand to destination operand. | mov r2, r1 |
| 1 | cmp | Compares the content of two operands. | cmp r1, r2 | Compares the contents of the registers. |
| 2 | add | Sums contents of both operands and stores the result in the destination operand. | add r2 r1 |
| 3 | sub | Substracts the content of the first operand from the second one and stores the result in the second operand. | sub r1 r2|
| 4 | not | Flips the content of the operand. | not r2 |
| 5 | clr | Clears the content of the operand. | clr r2 |
| 6 | lea | Load affective address. Loads the address stored in the source operand to the destination operand. | lea HELLO, r1 |
| 7 | inc | Increases value of the operand by 1. | inc r1 |
| 8 | dec | Decreases value of the operand by 1. | dec r1 |
| 9 | jmp | Jump to the instruction whose address is stored in the operand (the PC will be updated to this address). | jmp LINE |
| 10 | bne | | | |
| 11 | red | Reds a character from STDIN and store it in the operand. | red r1 |
| 12 | prn | Prints to STDOUT the character from the operand | prn r1 |
| 13 | jsr | Pushes the content of the current PC counter to stack and loads the content of the operand. | jsr FUNC |
| 14 | rts | Pops the value from the top of the stack and stores in PC (no operands). | rts |
| 15 | stop | Exits the program (no operands).  | stop |

## Instruction line syntax
- Instructions are separated by new lines '\n'.
- Every line/instruction is limited to 80 characters (without '\n').
- Blank lines are allowed.
- Comments start with ';'.
- A comma has to separate between parameters (only one, blnaks and tabs are allowed).
- Every line can start with a label. A label has to start with a letter that can be followed by numbers or more letters, and has to terminate with ':'. For example:
```
MAIN:       mov     S1.1, STR
````
- A label will contain the line's  instruction counter.
