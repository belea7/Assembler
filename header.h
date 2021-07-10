/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo
 *                      Lea Ben Zvi
 *
 *      File Name:      header.h
 *      Description:    Header file for all files in the project.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

#define MAX_LINE 81                     /* Maximum line length */
#define MAX_LABEL_NAME 31               /* Maximum label length */
#define MAX_OPERAND_NAME 20             /* Maximum argument name length */
#define MAX_STRING 30                   /* Maximum string length */
#define MAX_CMD 8                       /* Maximum operation name length */
#define START_LINE 100                  /* Address of first line in program */
#define MAX_ARR_LEN 50                  /* Maximum array length */
#define MAX_DATA_STRUCT 100             /* Maximum .data list length */

enum {IMMEDIATE,DIRECT,STRUCT,REG};
enum {NO,YES};

/****************** Data structs ****************/

/* Struct object (in the assembler) */
typedef struct {
    int num;                                    /* the first field contains a number  */
    char string [100];                          /* the second field contains a string */
} strct;


/* Label (Linked list) */
typedef struct Label {
    char label_name[MAX_LABEL_NAME];            /* Name of label */
    unsigned int dec_address;                   /* labels address in decimal */
    int is_extern;                              /* Is label external (1 - YES,  0 - NO) */
    char type;                                  /* What type of the label ('i' - instruction,  'd' - direction */
}label;

typedef struct {
    int arr [MAX_ARR_LEN];
    int length;
}data_list;

/* A union for all data types in the assembler */
typedef union {
    data_list numbers;                          /* .data */
    char string [MAX_STRING];                   /* .string / .entry / .extern*/
    strct s;                                	/* .struct */
} data;


/* Represents data declaration */
typedef struct Data_Line{
    char name [MAX_LABEL_NAME];                 /* pointer to its label */
    char type [8];                              /* string/data/struct/entry/extern */
    int address;                                /* the address in the data */
    data value;
}data_line;


/************* Instruction structs **********/

/* Represents an operation in assembly containing all relevant data about it */
typedef struct {
	char op_name[MAX_CMD];
	unsigned int opcode;                         /* Operation code: 16 different operations . */
	char * src_address_types;                    /* Valid addressing types for source operand (0,1,2,3)*/
	char * dest_address_types;                   /* Valid addressing types for destination operand (0,1,2,3)*/
}op;


/* Represents and instructions line */
typedef struct Instruction{
	unsigned opcode;                                /* The operation performed */
    char src_operand_name [MAX_OPERAND_NAME];           /* Source operand */
    char dest_operand_name [MAX_OPERAND_NAME];          /* Destination operand */
	int cur_ic;
} instruction_line;

/**************************** Bit fields for the instruction coding ****************************/

/* A struct representing an instruction decoding */
typedef struct {
	unsigned int AER:2;
	unsigned int dest_address_type:2;
	unsigned int src_address_type:2;
	unsigned int opcode:4;
} instruction;

/* A struct representing:
		1) Immediate addressing type.
		2) A s addressing type (two of these: one for the struct, second for the field).
		3) A data addressing type.
*/

/* Coding for struct.num , adress code, and immediate addressing */
typedef struct {
	unsigned int AER:2;
	unsigned int value:8;
} number_code;

/* A struct representing a register access decoding */
typedef struct {
	unsigned int AER:2;			/* AER */
	unsigned int dest_reg:4;		/* number of dest reg */
	unsigned int src_reg:4;			/* number of src  reg */
} reg_code;

/* A struct that helps converting number to 32 base */
typedef struct {
	unsigned int num:10;
} int_32print;

/* A struct that helps converting to 32 base */
typedef struct {
	unsigned int second_digit:5;    /* First digit of 32 code */
	unsigned int first_digit:5;     /* Second number of 32 code */
} literal32;

/* A union of all possible printed objects in 32 base */
typedef union { 
	instruction instruc;
	reg_code reg;
	number_code num;
	int_32print n32;
	literal32 lit32;
} union_literal;
