#ifndef _LIMIT_HPP_
#define _LIMIT_HPP_

struct compare{
	char port1[500] = "data_out";
	char component1[500] = "sender1";
};

void limit_output(const char *input, const char *output,...);

#endif // _LIMIT_HPP_
