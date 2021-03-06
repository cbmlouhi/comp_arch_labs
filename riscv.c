#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file
#include "string.h"

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}

int compare(char* str){
	
	if(*str == 'A'){
		str = str + 3; //if starts with A
		
		if(*str == NULL){
			return 1;  //add
		}
		else{             // |Returns value based on instuction|
			return 2; //addi
		}
	}
	if(*str == 'L'){
		return 3;  //LW
	}
	if(*str == 'S'){
		return 4;  //SW
	}
	return -1;

}



/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){
	char *itr = NULL;
	char** tokens = NULL;  //Store tokens in double pointer
	int size = 0;
	itr = instr;
	tokens = (char**) malloc(3 * sizeof(char));
	char* mem_file = "mem.txt";
	while(itr != NULL){
		
		tokens[size] = itr;
		itr = strtok(NULL, " ");  //feel token based on space delimeter
		size++;
	}
	itr = tokens[0];
	
	
	int i = compare(tokens[0]);  //determines which insruction
	
	if(i == -1){
		return false;
	}
	
	
	if (size == 4){
		if(i == 1){
		itr = tokens[1];
		itr ++;             //get register of every token
		int reg1 = atoi(itr);
		itr = tokens[2];
		itr++;                //ADD
		int reg2 = atoi(itr);
		itr = tokens[3];
		itr++;
		int reg3 = atoi(itr);
		reg[reg1] = reg[reg3] + reg[reg2];   //adds reg values
	
		}
		else{
			
		itr = tokens[1];
		itr ++;
		int reg1 = atoi(itr);
		itr = tokens[2];
		itr ++;
		int reg2 = atoi(itr);
		itr = tokens[3];		//ADDI
		int imm = atoi(itr);
		
		reg[reg1] = reg[reg2] + imm; //adds imm
		}
		
	
		

		return true;
	}
	else if(i != -1){
		itr = tokens[2];
		itr = strtok(itr,"(");   //splititng up the last token into two seperate
		tokens[2] = itr;     
		itr = strtok(NULL, ")");  
		itr++;
		tokens[3] = itr;
		itr = tokens[1];
		itr++;
		
		if(i == 3){
			
			int dest = atoi(itr); //dest equal to first reg
			int mem = atoi(tokens[2]) + reg[atoi(tokens[3])]; //adding and imm + reg 
			int32_t read = read_address(mem, mem_file); //reading data from mem
			printf("Read address %lu (0x%lX): %lu (0x%lX)\n", mem, mem, read, read);
			reg[dest] = read; //storing data in reg
			
		}
		else{
		
			int data = reg[atoi(itr)];  //destination address for memory
			int dest = atoi(tokens[2]) + reg[atoi(tokens[3])]; //data to store
			int32_t write = write_address(data, dest, mem_file);
			int32_t read = read_address(dest, mem_file);
			printf("Read address %lu (0x%lX): %lu (0x%lX)\n", dest, dest, read, read);
		}
		
		return true;

	}
	
}

/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %lu (0x%lX): %lu (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}
void print_regs(){
	int col_size = 10;
	for(int i = 0; i < 8; i++){
		printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
		printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
		printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
		printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
	}
}

int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	print_regs();

	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and space as a delimiter.\nEnter '^c'  to end program\n");

	char* instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	//fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	//write_read_demo();
	while(!is_null){
		instruction = strtok(instruction," ");	
		
		printf(interpret(instruction) ? "true" : "false"); //returns true or false after every instruction
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	
	return 0;
}
