/**
 * \brief This program converts the output files to meaningful files
 *
 * This program has a function output_filter which takes input and output
 * files as arguments, it then read the input file line by line
 * and converts it in to tabular formats. this function is called in the
 * main cpp file of receiver,sender,subnet and top_model.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/filter.hpp"

/**
 * This function is used to trim the tailing space in the input
 * @param ip of char
 * @return ip
 */

char* trim(char* ip){
	int c;
	int index;
	index = -1;
	c = 0;
	while(ip[c] != '\0'){
	    if(ip[c] != ' ' && ip[c] != '\t' && ip[c] != '\n'){
		    index = c;
		}
		c++;
	}
	ip[index+1] = '\0';
	return ip;

}

/**
 * This function is for arranging the unrecognizable output
 * in to a correct readable format.
 * @param input[]
 * @param output[]
 */

void output_filter(const char *input,const char *output){

	/**<This variables are used in various for loops for iteration*/
	int i,j,k,l;

	/**<This character array is used to store line by line data from input file*/
	char data[300];

	/**<This character arrays are used to store various results to write in output file*/
	char component[300], port[300], time[300], port_value[300];

	/**
	 * Opening file pointers in read and write mode
	 */

	FILE *op = fopen(output,"w");
	FILE *fp = fopen(input,"r");


	/**
	 * The below pointer to character is used to do various
	 * operations on the file to get a pattern
	 */
	char *p_generated_by;
	char *p_part1;
	char *p_for_port;
	char *p_value;


	
	/**
	 * checking if there is any error in opening the out put file
	 */
	if(op == NULL){
		printf("ERROR in opening file");
	}

	char first_line[500] = "TIME\t\t\t\tVALUE\t\t\t\tPORT\t\t\t\tCOMPONENT\n";
	/**
	 * writing the output headings to the output file
	 */
	fputs(first_line, op);
	if(fp == NULL){
		printf("ERROR in opening file");
	}
	/**
	 * This while loop is for reading line by line from the input file
	 */
	while(fgets(data,300,fp) != NULL){

		/**
		 * checking for the pattern in input line if the pattern is found
		 * then we store it in time.
		 */
		if((strstr(data,"00:"))!= NULL){
			trim(data);
			strcpy (time,data);

		}

		/**
		 * checking whether the line read not starts with []
		 */
		if(data[0] == '[' && data[1] != ']'){

			/**<The input line is divided in to two part based on deliminator and store in two variables*/
			p_part1 = strtok(data,"]");
			p_generated_by = strtok(NULL,"]");

			/**
			 * iterate through the generated_by part and find the component
			 * and store it in a variable
			 */

			for(i=19,j=0;p_generated_by[i]!='\0' && p_generated_by[i]!='\n';i++,j++){
				component[j] = p_generated_by[i];
			}
			/**<this is to set the last element in array NULL character*/
			component[j] = '\0';

			/**<splits the part1 of the input line to find the port name*/
			p_for_port = strtok(p_part1,",");

			/**
			 * while loop check whether the variable is not null and uses
			 * strstr, which finds the first occurrence of substring in a string
			 * to assign port value
			 */
			while(p_for_port != NULL){
				if(strstr(p_for_port,"output")){
					strcpy(port,"output");
				}
				else if(strstr(p_for_port,"data_")){
					strcpy(port,"data_out");
				}
				else if(strstr(p_for_port,"packet_")){
					strcpy(port,"packet_sent_out");
				}
				else if(strstr(p_for_port,"ack_")){
					strcpy(port,"ack_received_out");
				}
				else if(strstr(p_for_port,"out")){
					strcpy(port,"out");
				}
				/**<finds the occurrence of { in the line to find the port value*/
				p_value = strstr(p_for_port,"{");

				/**
				 * if the length of the value is greater than 2 then we use for
				 * loop to iterate through the value till } and assigns port value.
				 */
				if(strlen(p_value)>2){
					for(k=1,l=0;p_value[k]!='}';k++,l++){
						port_value[l] = p_value[k];
					}
					port_value[l] = '\0';

					/**
					 * writes the output to the new file
					 */
					fprintf(op,"%-18s %-18s %-18s %-18s \n",time,port_value,port,component);

				}

				/**
				 * This will again splits the line from the last pointer position.
				 */
				p_for_port = strtok(NULL,",");
			}
		}
	}
	/**
	 * closing the file after the operations
	 */
	fclose(fp);
	fclose(op);
}
