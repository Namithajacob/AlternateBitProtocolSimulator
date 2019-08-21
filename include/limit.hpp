/**
 * \brief header file used for the function limit_output.
 * here one structure is declared with values, which is used
 * to limit the output by filtering out the values based on structure
 * members.
 */

#ifndef _LIMIT_HPP_
#define _LIMIT_HPP_

/**
 * this structure stores the values which is used to limit
 * the output file content by filtering based on the values
 * stored in the structure.
 */

struct compare{
	char port1[500] = "ack_received_out";
	char component1[500] = "sender1";
};

/**
 * this function is a variable function with variable number of parameters
 * which uses the concept of ellipsis
 * @param input
 * @param output
 * @param k of type in :-number of arguments
 */

void limit_output(const char *input, const char *output,int k...);

#endif // _LIMIT_HPP_
