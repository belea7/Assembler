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
| Opcode | Instruction | Description | Example | Example Explanation |
| --- | --- | --- | --- | --- |
| 0 | mov | Copies content oof source register to destination register. | mov r2, r1 | Copies contents of r2 to r1 |
| 1 | cmp | Compares the content of two operands. | cmp r1, r2 | Compares the contents of the registers. |
| 2 | add | Sums contents of both operands and stores the result in the source operand. | add r2 r1 | Sums r1 and r2 |
| 3 | sub | | | |
| 4 | not | | | |
| 5 | clr | | | |
| 6 | lea | | | |
| 7 | inc | | | |
| 8 | dec | | | |
| 9 | jmp | | | |
| 10 | bne | | | |
| 11 | red | | | |
| 12 | prn | | | |
| 13 | jsr | | | |
| 14 | rts | | | |
| 15 | stop | | | |

