**Assembler** is my final project for the "C programming lab course" course in my BSc.

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

Example of output files:
```
	!i	!f
$%			@%
$^			g&
$&			!%
$*			eq
$<			^k
$>			%!
$a			eq
$b			o!
$c			vc
$d			*s
$e			#g
$f			fk
$g			#!
$h			g#
$i			@c
$j			g&
$k			!<
$l			!c
$m			us
$n			g&
$o			!c
$p			$@
$q			$#
$r			$$
$s			$%
$t			$^
$u			$&
$v			!!
%!			!&
%@			vn
%#			!f
%$			!m
%%			!<
%^			$@
%&			$#
%*			!!
```
