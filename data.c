/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo  2004545346
 *                      Lea Ben Zvi     313222333
 *
 *      File Name:      data.c
 *      Description:    Includes all functions relevant for data lines.
 */
#include "header.h"

extern int ic;                                     /* Number of instruction coding lines */
extern int dc;                                     /* Number of data declaration lines */

extern int label_index;                            /* Index of current label */
extern int data_index;                             /* Index of current data line */

extern label labels [];                            /* Contains all labels */
extern data_line data_lines [];                    /* Contains all data declarations */

/******************************** Prototype ***********************************/

/* Description: (errors.c) Gets a string representing an error and prints the full error message
 * Input:       A short description representing the error (char *).
 * Output:      None.    */
void print_error (char *);

/* Description: (first_scan.c) Checks if any extraneous text at the end of the line.
 * Input:       Rest of the line.
 * Output:      None    */
void check_extraneous_text (char *);

/* Description: Updates a char pointer to point after a word
 * Input:       Pointer to update (char *), word to point after (char *).
 * Output:      The new pointer (char *).   */
char * update_ptr (char *, char *);



/* Description: Checks if atoi() returns zero because number is zero or string not integer.
 * Input:       String of the number (char *).
 * Output:      YES (1) / NO (0)        */
int is_zero (char *);

/* Description: Gets a list of number (.data declarations).
 * Input:       Line from file (char *).
 * Output:      Pointer to a  linked list of integers */
void get_data(char *);

/* Description: Gets a string and validates it (.string declarations).
 * Input:       The string (char *).
 * Output:      Fixed string (char *).  */
char * get_string(char *);

/* Description: Gets a s and validates it (.struct declarations).
 * Input:       The line in the file where struct declared.
 * Output:      Pointer to s (a struct representing struct object in assembly). */
strct *get_struct(char *);

/* Description: Validates entry (.entry declarations).
 * Input:       Entry name (char *).
 * Output:      None.                   */
void is_entry_valid (char *);

/* Description: Validates entry (.extern declarations).
 * Input:       Extern name (char *).
 * Output:      None.                   */
void is_extern_valid (char *);

/* Description: Processes extern/entry declaration.
 * Input:       Entry/extern declaration in line (char *).
 * Output:      None.                   */
void get_entry_extern (char *, char *);


/* Description: (data.c) Adds ic to all data addresses (since they're located after all the instructions).
 * Input:       None
 * Output:      None    */
void update_data_addresses();

/* Description: Processes data declaration line (f.e: .data) and saves relevant data
 * Input:       data type (char *), rest of the line (char *).
 * Output:      None    */
void data_instruction(char *, char *);

/******************************************************************************/

void data_instruction(char * data_type, char * line) {
    char word[100];
    strct * temp;

    strcpy(data_lines[data_index].type, data_type);
    data_lines[data_index].address = dc;

    /* Omit the . from the data type */
    data_type++;
    /* If .data declaration */
    if (!strcmp(data_type, "data")) {
        strcpy(data_lines[data_index].name, labels[label_index].label_name);
        get_data(line);
    }

    else if (!strcmp(data_type, "string")) {
        strcpy(data_lines[data_index].name, labels[label_index].label_name);
        /* If no string was declared */
        if (sscanf(line, "%s ", word) == EOF) {
            print_error("no_string_declared");
            return;
        }
        /* Update line pointer */
        line = update_ptr(line, word);

        /* Validate string and omit apostrophes */
        strcpy(data_lines[data_index].value.string, get_string(word));

        /* Update dc */
        data_lines[data_index].address = dc;
        dc += strlen(data_lines[data_index].value.string) + 1;

        /* Verify no extraneous text */
        check_extraneous_text(line);
    }

    /* if .struct declaration */
    else if (!strcmp(data_type, "struct")) {
        strcpy(data_lines[data_index].name, labels[label_index].label_name);
        if (!(temp = get_struct(line)))
            return;
        data_lines[data_index].value.s = *temp;
    }
    /* If .entry declaration */
    else
        get_entry_extern(line, data_type);
}

void get_data(char * line) {
    char * temp_str1, * temp_str2 = (char *) malloc (MAX_LINE), * ptr = (char *) malloc (sizeof(MAX_LINE));
    int rc, i, number;

    /* If no comma in line */
    if (!(strchr(line, ','))) {
        temp_str1 = (char *) malloc (MAX_LINE);
        /* If two string in line, but no comma */
        if ((rc = sscanf(line, " %s %s", temp_str1, temp_str2)) == 2) {
            print_error("missing_comma");
            return;
        }

            /* If no data passed at all */
        else if (rc == EOF) {
            print_error("empty_data");
            return;
        }

            /* If user passed one number*/
        else {

            /* If number started with + - omit it */
            if (temp_str1[0] == '+')
                temp_str1 += 1;

            /* If 'atoi' returned 0 and string is not "0" - not integer */
            if (!(number = atoi(temp_str1)) && !is_zero(temp_str1)) {
                print_error("data_not_integer");
                return;
            }

            data_lines[data_index].value.numbers.arr[0] = number;
            data_lines[data_index].value.numbers.length = 1;

            dc++;
            free(temp_str1);
            free(temp_str2);
            return;
        }
    }
    strcpy(ptr, line);
    if (strstr(ptr, ",,"))
        print_error("invalid_comma");
    /* If comma exists */
    temp_str1 = strtok(ptr, ",");
    for (i=0; i < MAX_ARR_LEN; i++) {
        rc = sscanf(temp_str1, " %s ", temp_str2);
        /* If numbers starts with + omit it */
        if (temp_str2[0] == '+')
            temp_str2[0] = '0';

        if (rc == EOF) {
            print_error("invalid_comma");
        }

        /* If not a valid number */
        else if (!(number = atoi(temp_str2)) && !is_zero(temp_str2))
            print_error("data_not_integer");

        data_lines[data_index].value.numbers.arr[i] = number;
        data_lines[data_index].value.numbers.length = i+1;
        dc++;

        /* If end of line not reached */
        if (!(temp_str1 = strtok(NULL, ","))) {
            line = update_ptr(line, temp_str2);
            if ((rc = sscanf(line, " %s ", temp_str2)) != EOF)
                print_error("extraneous_text");
            break;
        }
    }
    free(temp_str1);
    free(temp_str2);
    free(ptr);
    return;
}

int is_zero (char * num) {
    if (num[0] == '0')
        return YES;
    return NO;
}

char * get_string(char * string) {
    /* If string doesn't start with " */
    if (string[0] != '"')
        print_error("no_apos_in_string_beginning");
    else string++;  /* Get string after " */

    /* If string doesn't end with " */
    if (string[strlen(string) - 1] != '"')
        print_error("no_apos_in_string_end");
    else string[strlen(string)-1] = '\0';   /* Omit the " from the end */

    /* Return a pointer to the new string */
    return string;
}

strct *get_struct(char * line) {
    strct * temp = (strct *) malloc (sizeof(strct));
    int rc;
    char * str1, * str2 = (char *) malloc (MAX_STRING);
    /* If no comma in line */
    if (!strchr(line, ',')) {
        str1 = (char *) malloc (MAX_STRING);
        rc = sscanf(line, " %s %s ", str1,str2);
        free(str1);
        free(str2);

        /* If no struct declared */
        if (rc == EOF) {
            print_error("no_struct_declared");
            return NULL;
        }

        /* If 2 arguments passed but no comma */
        else if (rc == 2) {
            print_error("missing_comma");
            return NULL;
        }

        /* If only one argument passed */
        else {
            print_error("one_struct_field");
            return NULL;
        }
    }

    /* If there's comma in line */
    str1 = strtok(line, ",");
    if((rc = sscanf(str1, " %s ", str2)) == EOF) {
        print_error("invalid_comma");
        return NULL;
    }

    /* If first field is not a valid integer */
    if (!(temp->num = atoi(str2)) && !is_zero(str2))
        print_error("not_int_struct");

    str1 = strtok(NULL, ",");
    sscanf(str1, " %s ", str2);
    strcpy(temp->string, str2);
    strcpy(temp->string, get_string(temp->string));
	dc += strlen(temp->string)+2;

    free(str2);
    return temp;
}

void get_entry_extern (char * line, char * type) {
    int rc;
    char *temp = (char *) malloc(MAX_STRING);

    if ((rc = sscanf(line, " %s %s ", data_lines[data_index].name, temp) == 2)) {
        print_error("extraneous_text");
        return;
    }
    if (!strcmp(type, "extern")) {
        if (rc == EOF) {
            print_error("empty_extern");
            return;
        }
        is_extern_valid(data_lines[data_index].name);
        strcpy(labels[label_index].label_name, data_lines[data_index].name);
        label_index++;
    }
    else {
        if (rc == EOF) {
            print_error("empty_entry");
            return;
        }
        is_entry_valid(data_lines[data_index].name);
    }
    free(temp);
}

void is_entry_valid (char * entry) {
    int i;
    for (i=0; i < data_index; i++) {
        if (!strcmp(data_lines[i].name, entry)) {
            if (!strcmp(data_lines[i].type, ".entry")) {
                print_error("entry_declared_twice");
                return;
            }
            if (!strcmp(data_lines[i].type, ".extern")) {
                print_error("entry_declared_extern");
                return;
            }
        }
    }
}

void is_extern_valid (char * name) {
    int i;
    for (i=0; i < label_index; i++) {
        if (!strcmp(labels[i].label_name, name)) { ;
            if (labels[i].is_extern)
                print_error("extern_declared_twice");
            else
                print_error("label_used_twice");
        }
    }
}

void update_data_addresses() {
    int i;
    /* Go through all data lines and add to their adress the address of the last instruction line */
    for(i=0; i < data_index; i++)
        data_lines[i].address += ic + START_LINE;

    for(i=0; i < label_index; i++)
        if (labels[i].type == 'd')
            labels[i].dec_address += ic + START_LINE;
}

