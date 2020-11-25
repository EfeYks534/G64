#pragma once

#include <stdint.h>

#define TK_IDENT             0x0 // identifier
#define TK_INT               0x1 // integer
#define TK_FLOAT             0x2 // float
#define TK_STRING            0x3 // string
#define TK_CHAR              0x4 // char
#define TK_PLUS              0x5 // +
#define TK_MINUS             0x6 // -
#define TK_STAR              0x7 // *
#define TK_SLASH             0x8 // /
#define TK_EQUALS            0x9 // =
#define TK_DOT               0xA // .
#define TK_COLON             0xB // :
#define TK_DOLLAR            0xC // $
#define TK_SQUARE            0xD // #
#define TK_LEFT_PHAR         0xE // (
#define TK_RIGHT_PHAR        0xF // )
#define TK_LEFT_BRACK       0x10 // [
#define TK_RIGHT_BRACK      0x11 // ]
#define TK_LEFT_CURLY       0x12 // {
#define TK_RIGHT_CURLY      0x13 // }
#define TK_SEMICOLON        0x14 // ;
#define TK_OR               0x15 // |
#define TK_AND              0x16 // &
#define TK_XOR              0x17 // ^
#define TK_LOGICAL_NOT      0x18 // !
#define TK_NOT              0x19 // ~
#define TK_CONDITIONAL      0x1A // ?
#define TK_GREATER          0x1B // >
#define TK_LESSER           0x1C // <
#define TK_MODULUS          0x1D // %
#define TK_COMMA            0x1E // ,
#define TK_COUNT            0x1F // total token count


#define TK_EOF -1
#define TK_ERROR -2
#define TK_UNKNOWN -3

typedef struct LexState
{
	int64_t           token;
	int64_t         cur_int;
	double        cur_float;
	char           *cur_str;
	uint32_t        str_len;
	void            (*error)(struct LexState*, int64_t );
	char            *source;
	uint32_t            pos;
	uint32_t           line;
	char               *buf;
	struct LexState   *next;
	struct LexState   *last;
} LexState;

LexState *LexStateNew(); // Make new lex state

void LexStateDelete(LexState *lex); // Delete lex state

int64_t Lex(LexState *lex); // Lex

int64_t LexPush(LexState **lex); // Lex and set *lex to *lex->next

const char *GetTokenName(int64_t tok); // Get token name
