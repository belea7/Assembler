/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo
 *                      Lea Ben Zvi
 *
 *      File Name:      main_prog.c
 *      Description:    Main program file.
 */
#include "header.h"

/* Description: (errors.c) Gets a string representing an error and prints the full error message.
 * Input:       A short description representing the error (char *).
 * Output:      None.    */
void print_error (char *);

/* Description: (first_scan.c) Performs first scan.
 * Input:       Pointer to the file (FILE *),
 * Output:      None    */
void first_scan (FILE *);

/* Description: (create_ob_file.c) Creates .obj file.
 * Input:       Input file name (char *).
 * Output:      None    */
void create_obj_file (char * file_name);

/* Description: (create_ext_ent_files.c) Creates .ext file.
 * Input:       Input file name (char *).
 * Output:      None    */
void create_externals_file (char *);

/* Description: (create_ext_ent_files.c) Creates .ent file.
 * Input:       Input file name (char *).
 * Output:      None    */
void create_entries_file (char *);

/******************************** Counters ************************************/

int error;                             /* Indicator if any syntax error found in code*/

int main(int argc, char * argv[]) {
    FILE * fp;
    int i;

	for (i=1; i < argc; i++) {
        error = NO;
        fp = fopen(argv[i], "r");
        if (!fp) {
            printf("\nFile does not exist\n");
            exit(0);
        }
        first_scan(fp);
        fclose(fp);
        if (!error){
            create_obj_file(argv[i]);
		}
        if (!error) {
            create_externals_file(argv[i]);
            create_entries_file(argv[i]);
        }
        putchar('\n');
    }
    return 0;
}
