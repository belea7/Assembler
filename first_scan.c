/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo
 *                      Lea Ben Zvi
 *
 *      File Name:      first_scan.c
 *      Description:    Includes all functions relevant for first scan.
 */

#include "header.h"

/******************************** Prototype ***********************************/

/* Description: (errors.c) Gets a string representing an error and prints the full error message
 * Input:       A short description representing the error (char *).
 * Output:      None.    */
void print_error (char *);

/* Description: (data.c) Processes data declaration line (f.e: .data) and saves relevant data
 * Input:       data type (char *), rest of the line (char *).
 * Output:      None    */
void data_instruction(char *, char *);

/* Description: (data.c) Adds ic to all data addresses (since they're located after all the instructions).
 * Input:       None
 * Output:      None    */
void update_data_addresses();

/* Description: (instructions.c) Calculates number of lines an instruction line takes and checks syntax.
 * Input:       Operation name (char *), rest of the line (char *).
 * Output:      None.   */
void update_ic (char *, char *);


/* Description: Verifies that a declared label is distinct.
 * Input:       label name (char *).
 * Output:      None    */
void is_label_distinct(char *);

/* Description: Validates label name (length, starts with a letter etc.)
 * Input:       label name (char *).
 * Output:      None    */
void is_label_valid (char *);

/* Description: Updates a char pointer to point after a word
 * Input:       Pointer to update (char *), word to point after (char *).
 * Output:      The new pointer (char *).   */
char * update_ptr (char *, char *);

/* Description: Documents a label and all relevent data
 * Input:       Label name (char *).
 * Output:      None    */
void add_label (char *);

/* Description: Check if sting is a data type (.data / .string / .struct / .entry / .extern)
 * Input:       String to check (char *).
 * Output:      Yes (1) / NO (0) (int)      */
int is_data_type(char *);

/* Description: Checks if any extraneous text at the end of the line.
 * Input:       Rest of the line.
 * Output:      None    */
void check_extraneous_text (char *);

/* Description: Validates no entry was declared without pointing to a line.
 * Input:       None
 * Output:      None    */
void no_spare_entries ();

/* Description: Performs first scan.
 * Input:       Pointer to the file (FILE *),
 * Output:      None    */
void first_scan (FILE *);

/******************************** Data structs ********************************/

/* Was any error found in the code file (1 - YES, 0 - NO) */
extern int error;

int ic;                                     /* Number of instruction coding lines */
int dc;                                     /* Number of data declaration lines */
int line_num;                               /* Current line number */

int label_index;                            /* Index of current label */
int data_index;                             /* Index of current data line */
int instructions_index;                     /* Index of current instruction */

label labels [MAX_DATA_STRUCT];                         /* Contains all labels */
data_line data_lines [MAX_DATA_STRUCT];                 /* Contains all data declarations */
instruction_line instructions [MAX_DATA_STRUCT];        /* Contains all instructions */

/******************************** Main Function ********************************/

void first_scan (FILE * fp) {
    char * line = (char *) malloc (MAX_LINE), * ptr;
    char word[MAX_STRING];
    int labeled;

    /* Initiate all counters */
    ic = 0;
    dc = 0;
    line_num = 0;
    instructions_index = 0;
    data_index = 0;
    label_index = 0;

    while (!feof(fp)) {
        line_num += 1;
        labeled = NO;

        fgets(line, MAX_LINE, fp);
        
        /* If empty row or a comment (start with ; ) */
        if (sscanf(line, "%s ", word) == EOF|| word[0] == ';')
            continue;
        ptr = update_ptr(line, word);

        /* Check if first word is a label */
        if (word[strlen(word) - 1] == ':') {
            labeled = YES;

            /* Add it to the list and check if it's distinct */
            add_label(word);
            is_label_distinct(word);


            /* If the line is empty but contains a lebl - print error*/
            if (sscanf(ptr, "%s ", word) == EOF) {
                print_error("label_on_empty_line");
                continue;
            }
            ptr = update_ptr(ptr, word);
        }
        
        /* If data declaration line */
        if (is_data_type(word)) {
            labels[label_index].type = 'd';
            labels[label_index].dec_address = dc;
            if (!strcmp(".extern", word))
                labels[label_index].is_extern = YES;
            data_instruction(word, ptr);
            data_index++;
        }

        else {
            update_ic(word, ptr);
            instructions_index++;
        }

        if (labeled) {
            label_index++;
            labeled = NO;
        }
    }
    free(line);

    /* Update all data addresses according to ic */
    update_data_addresses();
    no_spare_entries ();
}

/**************************** General Functions ********************************/

char * update_ptr (char * line, char * word) {
    char * ptr = strstr(line, word) + (int)strlen(word);
    return ptr;
}

int is_data_type(char * word) {
    char * data_types [] = { ".data", ".string", ".struct", ".entry", ".extern"};
    int i;
    if (word[0] != '.')
        return NO;
    for (i=0; i < 5; i++) {
        if (!strcmp(word, data_types[i])) {
            return YES;
        }
    }
    return NO;
}

void check_extraneous_text (char * line) {
    char * temp = (char *) malloc (MAX_LINE);
    if (sscanf(line, " %s ", temp) != EOF) {
        free(temp);
        print_error("extraneous_text");
    }
}

/******************************** Label Functions ******************************/

void is_label_valid (char * word) {
    int i = 0;
    /* Make sure Label name is shorter than 30 chars */
    if (strlen(word) > MAX_LABEL_NAME)
        print_error("long_label");

    /* Make sure Label starts with a letters */
    if (!isalpha(word[i]))
        print_error("int_in_label");

    /* Make sure all chars in the Label are numbers/letters */
    for (i=0; i < strlen(word); i++) {
        if (!isalnum(word[i])) {
            print_error("bad_char_in_label");
            break;
        }
    }
}

void add_label (char * name) {
    /* Omit the : from label */
    name[strlen(name) - 1] = '\0';
    is_label_valid(name);

    strcpy(labels[label_index].label_name, name);
    labels[label_index].dec_address = ic + START_LINE;
    labels[label_index].is_extern = NO;
    labels[label_index].type = 'i';
}

void is_label_distinct(char * tag) {
    int i;
    for(i=0; i < label_index; i++) {
        if (!strcmp(labels[i].label_name, tag)) {
            if (labels[i].is_extern)
                print_error("label_declared_extern");
            else
                print_error("label_used_twice");
            return;
        }
    }
}

void no_spare_entries () {
    int i, j;
    for (i=0; i < data_index; i++) {
        if (!strcmp(data_lines[i].type, ".entry")) {
            for (j=0; j < label_index; j++) {
                if (!strcmp(labels[j].label_name, data_lines[i].name)) {
                    data_lines[i].address = labels[j].dec_address;
                    break;
                }
            }
            if (j == label_index) {
                error = YES;
                printf("\nError:   Not all declared entries are initiated");
                break;
            }
        }
    }
}


