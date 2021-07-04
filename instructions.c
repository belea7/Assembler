/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo
 *                      Lea Ben Zvi
 *
 *      File Name:      instructions.c
 *      Description:    Includes all functions relevant for instruction processing.
 */
#include "header.h"

extern int ic;                                     /* Number of instruction coding lines */
extern int label_index;                            /* Index of current label */
extern int instructions_index;                     /* Index of current instruction */

extern label labels [];                            /* Contains all labels */
extern instruction_line instructions [];           /* Contains all instructions */

/* Array of all operations in the assembler */
static op op_arr[] = {
        {"mov",0,"0123","123"},
        {"cmp",1,"0123","0123"},
        {"add",2,"0123","123"},
        {"sub",3,"0123","123"},
        {"not",4,"","123"},
        {"clr",5,"","123"},
        {"lea",6,"12","123"},
        {"inc",7,"","123"},
        {"dec",8,"","123"},
        {"jmp",9,"","123"},
        {"bne",10,"","123"},
        {"red",11,"","123"},
        {"prn",12,"","0123"},
        {"jsr",13,"","123"},
        {"rts",14,"",""},
        {"stop",15,"",""},
        {"",0,"",""}
};

/******************************** Prototype ***********************************/

/* Description: (errors.c) Gets a string representing an error and prints the full error message
 * Input:       A short description representing the error (char *).
 * Output:      None.    */
void print_error (char *);

/* Description: (first_scan.c) Checks if any extraneous text at the end of the line.
 * Input:       Rest of the line.
 * Output:      None    */
void check_extraneous_text (char *);

/* Description: (first_scan.c) Updates a char pointer to point after a word
 * Input:       Pointer to update (char *), word to point after (char *).
 * Output:      The new pointer (char *).   */
char * update_ptr (char *, char *);

/* Description: (data.c )Checks if atoi() returns zero because number is zero or string not integer.
 * Input:       String of the number (char *).
 * Output:      YES (1) / NO (0)        */
int is_zero (char *);


/* Description: Checks if word is a valid operation and returns a pointer to op struct.
 * Input:       Name of a potential operation (char *).
 * Output:      Pointer of op * or NULL (if not an operation    */
op *get_operation (char * name);

/* Description: Finds what's the addressing type of an operand.
 * Input:       Name of an operand (char *).
 * Output:      Number of addressing type (unsigned int)    */
unsigned addressing_type (char * name);

/* Description: Adds argument to documentation.
 * Input:       Pointer to an op, type of operand (source/destination), operand name (char *).
 * Output:      None    */
void add_arg (op *, char *, char *);

/* Description: Checks if addressing type of an operand is valid for the op.
 * Input:       op pointer, type of operand (source/destination), operand name (char *).
 * Output:      None.   */
void validate_address_type (op *, char *, unsigned);
/* Description: Calculates number of lines an instruction line takes and checks syntax.
 * Input:       Operation name (char *), rest of the line (char *).
 * Output:      None.   */
void update_ic (char *, char *);

/******************************************************************************/

void update_ic (char * cmd, char * line) {
    op * command;
    char word[MAX_LINE], temp[MAX_LINE], *ptr;
    int rc;

    /* If op name wasn't found */
    if (!(command = get_operation(cmd))) {
        print_error("invalid_command");
        return;
    }
    else instructions[instructions_index].opcode = command->opcode;

    instructions[instructions_index].cur_ic = ic + START_LINE;

    /* Update ic */
    ic += 1;

    /* If no commas in the line */
    if(!strchr(line, ',')) {

        /* If 2 arguments passed without a comma */
        if ((rc = sscanf(line, " %s %s ", word, temp)) == 2)
            print_error("missing_comma");

        /* If no arguments passed */
        else if (rc == EOF) {

            /* If operation requires arguments - error */
            if (strlen(command->src_address_types) || strlen(command->dest_address_types)) {
                print_error("no_args");
                return;
            }
        }

        /* If one argument passed */
        else {
            /* Check if only one argument is supposed to be passed */
            rc = (int) strlen(command->src_address_types) + (int) strlen(command->dest_address_types);

            /* If only one source argument should be passed */
            if (rc == strlen(command->src_address_types))
                add_arg(command, "src", word);

            /* If only one destination argument should be passed*/
            else if (rc == strlen(command->dest_address_types))
                add_arg(command, "dest", word);

            /* Both destination and source args are supposed to be passed */
            else
                print_error("too_few_operands");
        }
    }
    /* If there's comma */
    else {
        ptr = strtok(line, ",");
        if (sscanf(ptr, " %s ", word) == EOF)
            print_error("empty_operand");
        if (word[0] == ',')
            print_error("two_commas");
        add_arg(command, "src", word);
        ptr = strtok(NULL, ",");

        if (sscanf(ptr, " %s ", word) == EOF)
            print_error("empty_operand");
        if (word[0] == ',')
            print_error("two_commas");
        add_arg(command, "dest", word);

        if(addressing_type(instructions[instructions_index].src_operand_name) == REG &&
                           addressing_type(instructions[instructions_index].dest_operand_name) == REG)
            ic -= 1;
    }

    line = update_ptr(line, word);
    /*check_extraneous_text(line);*/
}


op *get_operation (char * name) {
    int i = 0;
    /* While didn't reach the end of the op list (empty op name) */
    while (strlen(op_arr[i].op_name)) {
        /* If found an op with this name  - return a pointer to it */
        if (!strcmp(op_arr[i].op_name,name))
            return &(op_arr[i]);
        i++;
    }
    return NULL;
}


void add_arg (op * cmd, char * type /* src / dest */, char * name) {

    /* Get the addressing type of argument */
    unsigned address_type = addressing_type(name);

    /* Update ic */
    if (address_type == STRUCT)
        ic += 2;
    else
        ic += 1;

    if (!strcmp(type, "src")) {
        validate_address_type (cmd, "source", address_type);
        strcpy(instructions[instructions_index].src_operand_name, name);
        return;
    }

    validate_address_type (cmd, "destination", address_type);
    strcpy(instructions[instructions_index].dest_operand_name, name);
}


unsigned addressing_type (char * name) {
    int i = 0;
    /* Array of all register names */
    char *reg_arr[] = {
            "r0", "r1", "r2","r3", "r4", "r5", "r6", "r7", "PSW", "" };
    /* If string starts with # and followed by integer */
    if (name[0] == '#' && (atoi(name+1) || (!atoi(name+1) && is_zero(name+1))))
        return IMMEDIATE;
    if (strchr(name, '.'))
        return STRUCT;
    if (name[0] == 'r' || !strcmp(name, "PSW")) {
        /* Go through all register names and check if name is valid */
        while (strlen(reg_arr[i])) {
            /* If found the register name */
            if (!strcmp(reg_arr[i], name))
                break;
            i++;
        }
        /* If didn't find the register name  - assume it's a direct argument*/
        if (!strlen(reg_arr[i]))/*here we will change to macro/function to handle the errors*/ {
            print_error("not_reg");
            return DIRECT;
        }
        return REG;
    }
    return DIRECT;
}


void validate_address_type (op * cmd, char * operand_type /* source / dest */, unsigned address_type) {
    char *valid_types;
	
    if (!isdigit(address_type))
		return;
		
    /* If needs to add source argument */
    if (!strcmp(operand_type, "src"))
        valid_types = cmd->src_address_types;
    else valid_types = cmd->dest_address_types;

    if (!strlen(valid_types))
        print_error("too_many_operands");

    else if (!strchr(valid_types, address_type)) {
        print_error("invalid_address_type");
        printf(". Can only be one of these types:");
        if (strchr(cmd->src_address_types, IMMEDIATE + '0'))
            printf(" Immediate");
        if (strchr(cmd->src_address_types, DIRECT + '0'))
            printf(" Direct");
        if (strchr(cmd->src_address_types, STRUCT + '0'))
            printf(" Struct");
        if (strchr(cmd->src_address_types, REG + '0'))
            printf(" Register");
    }
}
