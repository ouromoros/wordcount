#pragma once
#include <string>
#include <stdio.h>

const size_t MAX_BUFFER_SIZE = 1 << 12;
struct Result {
	int line_count, word_count, char_count, comment_count, code_count, empty_count;
};

class WC
{
public:
	WC(bool c_flag = false, bool w_flag = false, bool l_flag = false, bool a_flag = false);
	~WC();
	Result Tell(FILE *f);
private:
	bool c_flag, w_flag, l_flag, a_flag;
	char buf[MAX_BUFFER_SIZE];
};

