/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo  2004545346
 *                      Lea Ben Zvi     313222333
 *
 *      File Name:      errors.c
 *      Description:    Includes all error messages and function to print them.
 */
#include <string.h>
#include <stdio.h>
enum {NO,YES};

extern int line_num;        /* Current line number */
extern int error;

/* Description: Gets a string representing an error and prints the full error message
 * Input:       A short description representing the error (char *).
 * Output:      None.    */
void print_error (char *);

typedef struct{
    char *ID;
    char *message;
}error_message;

static error_message errors_list[] =  {
        { "long_label", "Label cannot be longer than 30 characters" },
        { "int_in_label", "Label has to start with a letter" },
        { "bad_char_in_label", "Label can contain only numbers or letters" },
        { "label_on_empty_line", "Cannot label an empty line" },
        { "extraneous_text", "Extraneous text at the end of the line" },
        { "missing_comma", "Syntax error - missing a comma"},
        { "empty_data", "Cannot declare empty data"},
        { "data_not_integer", "data value can contain only valid numbers"},
        { "no_string_declared", "Cannot declare empty string"},
        { "no_apos_in_string_beginning", "String variable has to start with apostrophes"},
        { "no_apos_in_string_end", "String variable has to end with apostrophes"},
        { "no_struct_declared", "Cannot declare empty struct"},
        { "not_int_struct", "First struct field can only be integer"},
        { "one_struct_field", "Struct has to have 2 fields"},
        { "empty_extern", "Cannot declare empty extern label"},
        { "empty_entry", "Cannot declare empty entry"},
        { "entry_declared_extern", "Entry already declared as extern"},
        { "label_used_twice", "Label is already used"},
        { "extern_declared_twice", "Extern declared already"},
        { "label_declared_extern", "Label already declared external"},
        { "invalid_command", "Operation does not exist" },
        { "no_args", "No arguments were passed"},
        { "too_many_operands", "Too many operands passed"},
        { "invalid_address_type", "Invalid addressing type of operand"},
        { "too_few_operands", "Not enough operands were passed"},
        { "empty_operand", "no operand passed"},
        { "not_reg", "Operand is not a valid register"},
        { "entry_declared_twice", "Entry already declared"},
        { "invalid_comma", "Syntax error - invalid comma"},
        { "", ""}
};

void print_error (char * id) {
    int i=0;
    error = YES;
    while (strlen(errors_list[i].ID)) {
        if (!strcmp(errors_list[i].ID, id))
            break;
        i++;
    }
    printf("\nError in line %d:   %s", line_num, errors_list[i].message);
}