/**
 * \brief This code takes the  the output file of part D and limit
 *  the output in to new file.
 *
 *  This code has a variable function, which has output file of previous task
 *  as input and output file to store new output, number of arguments passed
 *   and a structure as input.This function is called in
 *  the main cpp of test source codes and top model source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdarg.h>

#include "../include/limit.hpp"


/**
 * This function takes the input file and output file and a variable number of
 * inputs here we takes a structure as input and iterate through file and
 * filter the output accordingly.This function uses concept of ellipsis.
 * @param input
 * @param output
 * @param k
 */

void limit_output(const char *input, const char *output,int k,...){

	/**<opening file pointers to do file operations*/
	FILE *fp;
	FILE *op;

	int i,j;

	/**<creating variable list*/
	va_list c1;

	/**<associate new variable with parameter list*/
	va_start(c1,k);
	const auto c = va_arg(c1,compare);

	/**<variables used to compare the strings*/
	int result1,result2,result;

	/**<variables to store file content, ie port and component*/
	char port[500],component[500];

	/**<to store the file line by line data*/
	char data[500];

	/**<opening the file in read and write mode*/

	fp = fopen(input,"r");
	op = fopen(output,"w");

	if(op == NULL){
		printf("ERROR in opening file");
	}
	char first_line[500] = "TIME\t\t\t\tP_VALUE\t\t\t\tPORT\t\t\t\tCOMPONENT\n";
	fputs(first_line, op);
	if(fp == NULL){
		printf("ERROR in opening file");
	}

	/**
	 * This while loop reads the file line by line and do the operations.
	 */
	while(fgets(data,500,fp)!= NULL){

		/**
		 * this if loop check whether line read has "00:" string, then only the
		 * following statements execute.
		 */
		if((strstr(data,"00:"))!= NULL){

			/**
			 * reading he value for component from the file
			 */
		    if(strstr(data,"generator_ack")){
				strcpy(component,"generator_ack");
			}
			else if(strstr(data,"receiver1")){
				strcpy(component,"receiver1");
			}
			else if(strstr(data,"sender1")){
				strcpy(component,"sender1");
			}
			else if(strstr(data,"subnet1")){
				strcpy(component,"subnet1");
			}
			else if(strstr(data,"subnet2")){
				strcpy(component,"subnet2");
			}
			else if (strstr(data,"generator_con")){
				strcpy(component,"generator_con");
			}
			else if (strstr(data,"generator")){
				strcpy(component,"generator");
			}

			/**
			 * reading the value for port from the file.
			 */
			if(strstr(data,"output")){
				strcpy(port,"output");
			}
			else if(strstr(data,"data_")){
				strcpy(port,"data_out");
			}
			else if(strstr(data,"pack")){
				strcpy(port,"packet_sent_out");
			}
			else if(strstr(data,"ack")){
				strcpy(port,"ack_received_out");
			}
			else if(strstr(data,"out")){
				strcpy(port,"out");
			}

			/**
			 * comparing the port and component with the values sored in structure
			 */
			result1 = strcmp(c.port1,port);
			result2 = strcmp(c.component1,component);
			result = result1+result2;

			/**
			 * checks whether the values are equal with structure elements and write to file.
			 */
			if(result == 0){

				fprintf(op,data);
			}
		}
	}

	/**<closing the va_list*/
	va_end(c1);
	/**< closing the file*/
	fclose(fp);
	fclose(op);
}
