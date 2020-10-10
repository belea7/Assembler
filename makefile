assembler: first_scan.o main_prog.o errors.o data.o instructions.o create_ob_file.o create_ext_ent_files.o
	gcc -g -Wall -pedantic -ansi first_scan.o main_prog.o errors.o create_ob_file.o data.o instructions.o create_ext_ent_files.o -o assembler
	
first_scan.o: first_scan.c header.h
	gcc -c -Wall -pedantic -ansi first_scan.c -o first_scan.o

data.o: data.c header.h
	gcc -c -Wall -pedantic -ansi data.c -o data.o

instructions.o: instructions.c header.h
	gcc -c -Wall -pedantic -ansi instructions.c -o instructions.o
	
main_prog.o: main_prog.c header.h
	gcc -c -Wall -pedantic -ansi main_prog.c -o main_prog.o

errors.o: errors.c
	gcc -c -Wall -pedantic -ansi errors.c -o errors.o
	
create_ob_file.o: create_ob_file.c header.h
	gcc -c -Wall -pedantic -ansi create_ob_file.c -o create_ob_file.o

create_ext_ent_files.o: create_ext_ent_files.c header.h
	gcc -c -Wall -pedantic -ansi create_ext_ent_files.c -o create_ext_ent_files.o