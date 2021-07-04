/*      Assembler Project
 *
 *      Created By:     Daniel de-Mayo
 *                      Lea Ben Zvi
 *
 *      File Name:      create_ob_file.c
 *      Description:    Includes all error messages and function to print them.
 */
#include "header.h"

enum {A,E,R};/*Absolute, External, Relocatable*/


/******************************** Prototype ***********************************/


/* Description: (instructions.c) Finds what's the addressing type of an operand.
 * Input:       Name of an operand (char *).
 * Output:      Number of addressing type (unsigned int)    */
unsigned addressing_type (char * name);


/* Description: Finds data in the data array and return a pointer to it.
 * Input:       Data label (char *).
 * Output:      Pointer to a data line (data_line *)    */
data_line *find_data(char *);

/* Description: Finds label in the label array and return a pointer to it.
 * Input:       Label name (char *).
 * Output:      Pointer to a label (label *)    */
label *find_label(char *);

/* Description: Each iteration we walk on the instruction array,
 * 				We built an instruction in the convection of an memory word (literal).
 * Input:       Current instruction line (Instruction line).
 * Output:      Instruction coding (union_literal)    */
union_literal build_instruc_literal(instruction_line);

/* Description: Prints in 32 base.
 * Input:       None.
 * Output:      None    */
void print32();

/* Description: Prints in 32 if the addressing type is struct.
 * Input:       None.
 * Output:      None    */
void print32_struct(char *);

/* Description: Prints in 32 if the addressing type is direct.
 * Input:       None.
 * Output:      None    */
void print32_direct(char *);

/* Description: Prints in 32 if the addressing type is immediate.
 * Input:       None.
 * Output:      None    */
void print32_immediate(char *);

/* Description: Prints array of number from the assembly program.
 * Input:       data line to print (data_line).
 * Output:      None    */
void print_type_data(data_line);

/* Description: Prints array of characters (one in a line) from the assembly program.
 * Input:       data line to print (data_line).
 * Output:      None    */
void print_type_string(data_line);

/* Description: Maintains and prints the address of each literal.
 * Input:       None
 * Output:      None    */
void update_and_print_adres();

/* Description: Creates .obj file.
 * Input:       Input file name (char *).
 * Output:      None    */
void create_obj_file (char *);

/************** Counters (declared in first_scan.c) ***************/

extern int ic;                                     /* Number of instruction coding lines */
extern int dc;                                     /* Number of data declaration lines */
extern int line_num;                               /* Current line number */

extern int label_index;                            /* Index of current label */
extern int data_index;                             /* Index of current data line */
extern int instructions_index;                     /* Index of current instruction */

extern label labels [];                            	/* Contains all labels */
extern data_line data_lines [];                    	/* Contains all data declarations */
extern instruction_line instructions [];           	/* Contains all instructions */

extern int error;                                  	/* Indicator if any syntax error found in code*/

FILE *fp;
unsigned adres; 					/* Address of the instruction - will be printed */
int is_null_src=NO;					/* If source argument is empty */
int is_null_dest=NO;					/* If destination argument is empty */
union_literal literal;

/********************************************************************************************/

/* These are the digits for the 32 base */
unsigned int digit_arr[] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f',
	'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};


void create_obj_file (char * file_name){
	int i,k;
	
	/* Opening file for writing the .ob file:  */
	char *object_file_name = (char *)malloc(sizeof(file_name)+5);
	if (!object_file_name){
		printf("ERROR - cannot allocate");
		error = YES;
	}
    adres = START_LINE;
	sprintf(object_file_name,"%s.ob",file_name);
	fp = fopen(object_file_name,"w");
	
	/* Write the dc ic :*/
	literal.n32.num = (unsigned)ic;
	fprintf(fp, "\t%c%c\t", digit_arr[literal.lit32.first_digit], digit_arr[literal.lit32.second_digit]);
	literal.n32.num = (unsigned)dc;
	fprintf(fp, "%c%c\n", digit_arr[literal.lit32.first_digit], digit_arr[literal.lit32.second_digit]);
	
	/* Write the instructions */
	for(i=0; i<instructions_index; i++){
		update_and_print_adres();
		literal = build_instruc_literal(instructions[i]);
		print32();
		
		if(!is_null_src)
			switch (literal.instruc.src_address_type){

				case IMMEDIATE:
					print32_immediate(instructions[i].src_operand_name+1);
					break;		

				case DIRECT:
					print32_direct(instructions[i].src_operand_name);
					break;	

				case STRUCT:
					print32_struct(strtok(instructions[i].src_operand_name,"."));
					break;

				case REG:
					if(literal.instruc.dest_address_type == REG)
						break;/*we will handle the print on dest switch*/
					update_and_print_adres();
					literal.reg.src_reg = atoi(instructions[i].src_operand_name+1);
					literal.reg.dest_reg = 0;
					literal.reg.AER = 0;
					print32();
					break;

				default:
					error = YES;
					printf("ERROR - missing addressing type ");
			};
		
		literal = build_instruc_literal(instructions[i]);
		if(!is_null_dest)
			switch (literal.instruc.dest_address_type){

				case IMMEDIATE:
					print32_immediate(instructions[i].dest_operand_name+1);
					break;

				case DIRECT:
					print32_direct(instructions[i].dest_operand_name);
					break;

				case STRUCT:
					print32_struct(strtok(instructions[i].dest_operand_name,"."));
					break;	

				case REG:
					if(literal.instruc.src_address_type == REG){
						update_and_print_adres();
						literal.reg.src_reg = atoi(instructions[i].src_operand_name+1);
						literal.reg.dest_reg = atoi(instructions[i].dest_operand_name+1);
						literal.reg.AER = 0;						
						print32();
						break;
					}
					update_and_print_adres();
					literal.reg.src_reg = 0;
					literal.reg.dest_reg = atoi(instructions[i].dest_operand_name+1);
					literal.reg.AER = 0;						
					print32();
					break;
					
				default:
					error = YES;
					printf("ERROR - missing addressing type ");
			};
	}
	
	/* Write the data */
	for(i=0; i<data_index; i++){
		if(!strcmp(data_lines[i].type, ".data"))
			print_type_data(data_lines[i]);
		if(!strcmp(data_lines[i].type, ".string"))
			print_type_string(data_lines[i]);
		if(!strcmp(data_lines[i].type, ".struct")){
			update_and_print_adres();
			literal.n32.num = data_lines[i].value.s.num;
			print32();
			for(k=0; k < strlen(data_lines[i].value.s.string); k++){
				update_and_print_adres();	
				literal.n32.num = data_lines[i].value.s.string[k];
				fprintf(fp, "%c%c\n", digit_arr[literal.lit32.first_digit],digit_arr[literal.lit32.second_digit]);
			}
				update_and_print_adres();
				literal.n32.num = 0;
				fprintf(fp, "%c%c\n", digit_arr[literal.lit32.first_digit],digit_arr[literal.lit32.second_digit]);			
		}
	}
	
	fclose(fp);
	if (error)
		remove(object_file_name);
}


/********************************  more functions   *****************************************/

data_line* find_data(char *name){ 
	int i;
	for(i=0; i<data_index; i++)
		if (!strcmp(name,data_lines[i].name))
			return &data_lines[i];
	return NULL;
}


label* find_label(char *name){ 
	int i;
	for(i=0; i<label_index; i++)
		if (!strcmp(name,labels[i].label_name))
			return &labels[i];
	return NULL;
}


union_literal build_instruc_literal(instruction_line instruct){
	union_literal lit;
	lit.instruc.opcode = instruct.opcode;
	if (!strlen(instruct.src_operand_name)){
		lit.instruc.src_address_type = 0;
		is_null_src = YES;
	}
	else{
		lit.instruc.src_address_type = addressing_type(instruct.src_operand_name);
		is_null_src = NO;
	}
	
	if (!strlen(instruct.dest_operand_name)){
		lit.instruc.dest_address_type = 0;
		is_null_dest = YES;
	}
	else{
		lit.instruc.dest_address_type = addressing_type(instruct.dest_operand_name);
		is_null_dest = NO;
	}
	lit.instruc.AER = 0;	
	return lit;
}


void print32(){
	fprintf(fp, "%c%c\n", digit_arr[literal.lit32.first_digit],digit_arr[literal.lit32.second_digit]);
}


void update_and_print_adres(){
	literal.n32.num = adres;
	fprintf(fp, "%c%c\t\t\t", digit_arr[literal.lit32.first_digit],digit_arr[literal.lit32.second_digit]);
	adres++;
}


void print32_struct(char *name){
	data_line *data_ptr = find_data(name);
	label *label_ptr = find_label(name);
	
	if (data_ptr){
		update_and_print_adres();
		literal.num.value = data_ptr->address;
		literal.num.AER = R;
		print32();
		update_and_print_adres();
		literal.num.value = atoi(strtok(NULL,"."));
		literal.num.AER = 0;
		print32();
		return;
	}			
	else if (label_ptr){
		update_and_print_adres();
		literal.num.value = label_ptr->dec_address;
		literal.num.AER = R;
		print32();
		update_and_print_adres();
		literal.num.value = atoi(strtok(NULL,"."));
		literal.num.AER = 0;
		print32();
		return;
	}
		error = YES;
		printf("ERROR - operand's name wasn't found ");
}


void print32_direct(char *name){
	data_line *data_ptr = find_data(name);
	label *label_ptr = find_label(name);
	
	if (data_ptr){
		update_and_print_adres();
		if(!strcmp(data_ptr->type,".extern")){
			literal.num.AER = E;
			literal.num.value = 0;
		}
		else {
			literal.num.AER = R;
			literal.num.value = data_ptr->address;
		}
		print32();
		return;
	}
	else if(label_ptr){
		update_and_print_adres();
		if(label_ptr->is_extern){
			literal.num.AER = E;
			literal.num.value = 0;
		}
		else{
			literal.num.AER = R;
			literal.num.value = label_ptr->dec_address;
		}
		print32();
		return;
	}
		error = YES;
		printf("ERROR - label wasn't found ");
}


void print32_immediate(char *name){
	update_and_print_adres();
	literal.num.value = atoi(name);
	literal.num.AER = A;
	print32();
}


void print_type_data(data_line l){
	int k;
	for(k=0; k < l.value.numbers.length; k++){
		update_and_print_adres();
		literal.n32.num = l.value.numbers.arr[k];
		fprintf(fp, "%c%c\n", digit_arr[literal.lit32.first_digit],digit_arr[literal.lit32.second_digit]);
	}
}


void print_type_string(data_line data_line1){
	int k;
	for(k=0; k < strlen(data_line1.value.string); k++){
		update_and_print_adres();	
		literal.n32.num = data_line1.value.string[k];
		fprintf(fp, "%c%c\n", digit_arr[literal.lit32.first_digit],digit_arr[literal.lit32.second_digit]);
	}
	update_and_print_adres();
	literal.n32.num = 0;
	fprintf(fp, "%c%c\n", digit_arr[literal.lit32.first_digit],digit_arr[literal.lit32.second_digit]);
}
