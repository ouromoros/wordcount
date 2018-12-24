#include "pch.h"
#include "WC.h"
#include <ctype.h>


WC::WC(bool c_flag, bool w_flag, bool l_flag, bool a_flag)
	: c_flag(c_flag), w_flag(w_flag), l_flag(l_flag), a_flag(a_flag) {};

Result WC::Tell(FILE *file) {
	int char_count, word_count = 0, line_count = 0, code_count = 0,
		empty_count = 0, comment_count = 0;
	Result result;

	fseek(file, 0L, SEEK_END);
	char_count = ftell(file);
	result.char_count = char_count;
	if (c_flag && !l_flag && !a_flag && !w_flag) {
		// 若只需要字符数则直接返回，不需要遍历文件
		return result;
	}
	// 将文件位置退回到开始`
	rewind(file);
	bool IN_WORD = false;
	bool EXIT = false;
	// bool SLASH_BEFORE = false;
	// bool IN_COMMENT = false;
	// int non_comment_char_count = 0;
	for (;;) {
		int size;
		size = fread(buf, 1, MAX_BUFFER_SIZE, file);
		if (size != MAX_BUFFER_SIZE) {
			if (ferror(file)) {
				throw;
			}
			if (feof(file) && buf[size - 1] != '\n') {
				buf[size++] = '\n';
				EXIT = true;
			}
		}
		for (int i = 0; i < size; i++) {
			char c = buf[i];
			//if (c == '\\') {
			//	if (SLASH_BEFORE) {
			//		IN_COMMENT = true;
			//	}
			//	SLASH_BEFORE = true;
			//}
			//else {
			//	SLASH_BEFORE = false;
			//}
			if (isspace(c)) {
				if (IN_WORD) {
					word_count++;
					IN_WORD = false;
				}
				if (c == '\n') {
					if (l_flag) line_count++;
					//if (a_flag) {
					//	if (!non_comment_char_count) {

					//	}
					//}
					//non_comment_char_count = 0;
				}
			}
			else {
				IN_WORD = true;
			}
		}
		if (EXIT) {
			break;
		}
	}
	result.line_count = line_count;
	result.word_count = word_count;
	return result;
}

WC::~WC()
{
}
