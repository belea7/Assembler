/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo
 *                      Lea Ben Zvi
 *
 *      File Name:      create_ext_ent_files.c
 *      Description:    Includes all error messages and function to print them.
 */
#include "header.h"


/******************************** Prototype ***********************************/

/* Description: (instructions.c) Finds what's the addressing type of an operand.
 * Input:       Name of an operand (char *).
 * Output:      Number of addressing type (unsigned int)    */
unsigned addressing_type (char * name);

/* Description: Goes through all data declarations and checks if there's a struct named this way.
 * Input:       Name of an operand (char *).
 * Output:      YES (1) or NO (0)    */
int is_struct (char *);

/* Description: Creates .ext file.
 * Input:       Input file name (char *).
 * Output:      None    */
void create_externals_file (char *);

/* Description: Creates .ent file.
 * Input:       Input file name (char *).
 * Output:      None    */
void create_entries_file (char *);

/************** Counters (declared in first_scan.c) ***************/

extern int ic;                                     /* Number of instruction coding lines */
extern int dc;                                     /* Number of data declaration lines */
extern int line_num;                               /* Current line number */

extern int label_index;                            /* Index of current label */
extern int data_index;                             /* Index of current data line */
extern int instructions_index;                     /* Index of current instruction */

extern label labels [];                            /* Contains all labels */
extern data_line data_lines [];                    /* Contains all data declarations */
extern instruction_line instructions [];           /* Contains all instructions */

extern int error;                                  /* Indicator if any syntax error found in code*/

extern unsigned int digit_arr[];

/********************************************************************************************/

void create_entries_file (char * file_name) {
    FILE * fp;
    char *entries_file_name = (char *)malloc(sizeof(file_name)+5);      /* Stores the name of the .ent file */
    char * address_32base = (char *) malloc (sizeof(3));                /* Stores the address in 32 base */
    int i, count = 0 /* Counter to indicate if no .entry declarations in file */;
    union_literal address;                                              /* Stores the address to print */

    /* Open the .ent file */
    sprintf(entries_file_name,"%s.ent",file_name);
    fp = fopen(entries_file_name,"w");
    /* If could not open entries file */
    if (!fp) {
        printf("\nERROR - Cannot create entries file\n");
        return;
    }

    /* Go through all data declarations */
    for (i=0; i < data_index; i++) {
        /* If found .entry declaration */
        if (!strcmp(data_lines[i].type, ".entry")) {
            count++;
            address.n32.num = (unsigned)data_lines[i].address;
            sprintf(address_32base, "%c%c", digit_arr[address.lit32.first_digit], digit_arr[address.lit32.second_digit]);
            fprintf(fp, "%s\t%s\n", data_lines[i].name, address_32base);
        }
    }
    free(address_32base);
    fclose(fp);
    /* If not entries found - remove empty entries file*/
    if (!count)
        remove(entries_file_name);
    free(entries_file_name);
}

void create_externals_file (char * file_name) {
    FILE * fp;
    char extern_name [MAX_LABEL_NAME];                                  /* Stores the name of the extern label */
    char * address_32base = (char *) malloc (sizeof(3));                /* Stores the address in 32 base */
    char * externals_file_name = (char *)malloc(sizeof(file_name)+5);   /* Stores the name of the .ext file */
    int i, j, count = 0 /* Counter to indicate if no .extern declarations in file */;
    union_literal address;                                              /* Stores the address to print */

    /* Open the .ext file */
    sprintf(externals_file_name,"%s.ext",file_name);
    fp = fopen(externals_file_name,"w");
    /* If could not open externals file */
    if (!fp) {
        printf("\nERROR - Cannot create externals file\n");
        return;
    }

    /* Go through all data declarations */
    for (i=0; i < data_index; i++) {
        /* If found .extern declaration */
        if (!strcmp(data_lines[i].type, ".extern")) {
            strcpy(extern_name, data_lines[i].name);
            /* Go through all instructions declarations */
            for (j=0; j < instructions_index; j++) {
                /* If source operand name is equal to external label name */
                if (!strcmp(instructions[j].src_operand_name, extern_name)) {
                    count++;
                    address.n32.num = (unsigned)(instructions[j].cur_ic + 1);
                    sprintf(address_32base, "%c%c", digit_arr[address.lit32.first_digit], digit_arr[address.lit32.second_digit]);
                    fprintf(fp, "%s\t%s\n", extern_name, address_32base);
                }
                /* If destination operand name is equal to external label name */
                if (!strcmp(instructions[j].dest_operand_name, extern_name)) {
                    count++;
                    if ((int)strlen(instructions[j].src_operand_name) == NO) {
                        address.n32.num = (unsigned) (instructions[j].cur_ic + 1);
                    }
                        /* If first addressing type is not empty and is struct*/
                    else if (is_struct(instructions[j].src_operand_name)) {
                        /* Source operand code is two lines after instruction line */
                        address.n32.num = (unsigned) (instructions[j].cur_ic + 3);
                    }
                        /* If addressing type is not struct  - 1 line after instruction line */
                    else  {
                        address.n32.num = (unsigned)(instructions[j].cur_ic + 2);
                    }
                    sprintf(address_32base, "%c%c", digit_arr[address.lit32.first_digit], digit_arr[address.lit32.second_digit]);
                    fprintf(fp, "%s\t%s\n", extern_name, address_32base);
                }
            }
        }
    }
    free(address_32base);
    fclose(fp);
    /* If not entries found - remove empty entries file*/
    if (!count)
        remove(externals_file_name);
    free(externals_file_name);
}

int is_struct (char * name) {
    int i;
    for (i=0; i < data_index; i++) {
        if (!strcmp(data_lines[i].name, name) && !strcmp(data_lines[i].type, ".struct"))
            return YES;
    }
    return NO;
}
