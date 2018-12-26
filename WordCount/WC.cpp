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

	bool IN_WORD = false, EXIT = false, SLASH_BEFORE = false, BSLASH_BEFORE = false,
		STAR_BEFORE = false, IN_STAR_COMMENT = false, COMMENT_LINE = false, EMPTY = true;
	int non_comment_char_count = 0;
	for (;;) {
		int size = fread(buf, 1, MAX_BUFFER_SIZE, file);
		if (size != MAX_BUFFER_SIZE) {
			if (ferror(file)) {
				throw;
			}
			if (feof(file)) {
				EXIT = true;
			}
		}
		for (int i = 0; i < size; i++) {
			char c = buf[i];
			if (a_flag) {
				if (c == '\\') {
					if (SLASH_BEFORE && !COMMENT_LINE) {
						COMMENT_LINE = true;
						non_comment_char_count--;
					}
					SLASH_BEFORE = true;
				}
				else {
					SLASH_BEFORE = false;
				}
				bool STAR = false, BSLASH = false;
				if (c == '/') {
					if (STAR_BEFORE && IN_STAR_COMMENT) {
						IN_STAR_COMMENT = false;
					}
					else {
						BSLASH = true;
					}
				}
				if (c == '*') {
					if (BSLASH_BEFORE && !IN_STAR_COMMENT) {
						IN_STAR_COMMENT = true;
						COMMENT_LINE = true;
						non_comment_char_count--;
					}
					else {
						STAR = true;
					}
				}
				BSLASH_BEFORE = BSLASH;
				STAR_BEFORE = STAR;
			}

			if (isspace(c)) {
				if (w_flag && IN_WORD) {
					word_count++;
					IN_WORD = false;
				}
				if (c == '\n') {
					if (l_flag) line_count++;
					if (a_flag) {
						if (non_comment_char_count > 1) {
							code_count++;
						}
						else if (COMMENT_LINE && !EMPTY) {
							comment_count++;
						}
						else {
							empty_count++;
						}
						non_comment_char_count = 0;
						COMMENT_LINE = IN_STAR_COMMENT;
					}
				}
			}
			else {
				if (a_flag) {
					EMPTY = false;
					if (!COMMENT_LINE && !IN_STAR_COMMENT) {
						non_comment_char_count++;
					}
				}
				if (w_flag) {
					IN_WORD = true;
				}
			}
		}
		if (EXIT) {
			break;
		}
	}
	result.line_count = line_count;
	result.word_count = word_count;
	result.code_count = code_count;
	result.comment_count = comment_count;
	result.empty_count = empty_count;
	return result;
}

WC::~WC()
{
}
