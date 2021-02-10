#include "LexLib.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static const char *token_name_table[] = {
	"<unknown>", "<error>", "<eof>", "<identifier>", "<integer>",
	"<float>", "<string>", "<char>", "'+'", "'-'", "'*'", "'/'",
	 "'='", "'.'", "':'", "'$'", "'#'", "'('", "')'", "'['", "']'",
	 "'{'", "'}'", "';'", "'|'", "'&'", "'^'", "'!'", "'~'", "'?'",
	 "'>'", "'<'", "'%'", "','"
};

const char *GetTokenName(int64_t tok)
{
	if(tok < -3 || tok >= TK_COUNT)
		return token_name_table[0];
	return token_name_table[tok+3];
}

LexState *LexStateNew()
{
	LexState *lex = calloc(sizeof(LexState), 1);
	return lex;
}

void LexStateDelete(LexState *lex)
{
	free(lex->cur_str);
	free(lex);
 
}
char LexGetch(char *source, unsigned int *posv)
{
	unsigned int pos = *posv;
	char ch = source[pos++];
	if(ch == '\\') {
		ch = source[pos++];
		switch(ch)
		{
			case 'x': {
				char i = 0;
				ch = source[pos++];
				for(int j = 0; j < 2; j++) {
					if(!isxdigit(ch)) { *posv = pos; return i; }
					i = i * 16 + (isdigit(ch) ? (ch - '0') : (10 + toupper(ch) - 'A'));
					ch = source[pos++];
				}
				*posv = pos;
				break;
			}
			case '0'...'9': *posv=pos; return ch-'0';
			case 't': *posv = pos; return '\t';
			case 'n': *posv = pos; return '\n';
			case 'r': *posv = pos; return '\r';
			case 'f': *posv = pos; return '\f';
			case 'a': *posv = pos; return '\a';
			case 'v': *posv = pos; return '\v';
			case 'b': *posv = pos; return '\b';
			case '\\': *posv = pos;return '\\';
			case '"': *posv = pos;   return -1;
			case '\'': *posv = pos;  return -2;
			default: *posv = pos; return ch;
		}
	}
	*posv = pos;
	return ch;
}

long int LexPush(LexState **lex)
{
	long int tok = Lex(*lex);
	*lex = (*lex)->next;
	return tok;
}

long int Lex(LexState *lex)
{
	char *source = lex->source;
	unsigned int pos = lex->pos;
	long int token;
	char *buf = lex->buf;
	int line = lex->line;
	while(1) {
		char ch = source[pos++];
		while(ch == '\n' || ch == ' ' || ch == '\t' || ch == '\v' || ch == '\a' || ch == '\r') {
			if(ch == '\n') line++;
			ch = source[pos++];
		}

		switch(ch)
		{
		case 0:
			token = TK_EOF;
			goto lex_end;
		case '+': token = TK_PLUS;			goto lex_end;
		case '-': token = TK_MINUS;			goto lex_end;
		case '*': token = TK_STAR;			goto lex_end;
		case '/': token = TK_SLASH;			goto lex_end;
		case '=': token = TK_EQUALS;		goto lex_end;
		case '.': token = TK_DOT;			goto lex_end;
		case ':': token = TK_COLON;			goto lex_end;
		case '$': token = TK_DOLLAR;		goto lex_end;
		case '#': token = TK_SQUARE;		goto lex_end;
		case '(': token = TK_LEFT_PHAR;		goto lex_end;
		case ')': token = TK_RIGHT_PHAR;	goto lex_end;
		case '[': token = TK_LEFT_BRACK;	goto lex_end;
		case ']': token = TK_RIGHT_BRACK;	goto lex_end;
		case '{': token = TK_LEFT_CURLY;	goto lex_end;
		case '}': token = TK_RIGHT_CURLY;	goto lex_end;
		case ';': token = TK_SEMICOLON;		goto lex_end;
		case '|': token = TK_OR;			goto lex_end;
		case '&': token = TK_AND;			goto lex_end;
		case '^': token = TK_XOR;			goto lex_end;
		case '!': token = TK_LOGICAL_NOT;	goto lex_end;
		case '~': token = TK_NOT;			goto lex_end;
		case '?': token = TK_CONDITIONAL;	goto lex_end;
		case '>': token = TK_GREATER;		goto lex_end;
		case '<': token = TK_LESSER;		goto lex_end;
		case '%': token = TK_MODULUS;		goto lex_end;
		case ',': token = TK_COMMA;			goto lex_end;
		case 'a'...'z':
		case 'A'...'Z':
		case '_': {
				token = TK_IDENT;
				int i = 0;
				while(isalnum(ch) || ch == '_') {
					buf[i++] = ch;
					ch = source[pos++];
				}

				lex->cur_str = memcpy(malloc(i+1), buf, i);
				lex->cur_str[i] = 0;
				lex->str_len = i + 1;
				memset(buf, 0, i);
				pos--;
				goto lex_end;
			}
		case '0'...'9': {
				token = TK_INT;
				int j = 0;
				long int i = 0;
				double f = 0;
				if(source[pos] == 'x') {
					pos++;
					ch = source[pos++];
					if(ch == 0) goto lex_error;
					while(1) {
						if(!isxdigit(ch) && isalpha(ch)) goto lex_error;	
						if(!isxdigit(ch)) goto lex_num_end;
						i = i * 16 + (isdigit(ch) ? (ch-'0') : (10 + toupper(ch)-'A'));
						ch = source[pos++];
					}
				}
				while(1) {
					if(ch == '.') goto lex_float;
					if(isalpha(ch)) goto lex_error;
					if(!isdigit(ch)) goto lex_num_end;
					i = i * 10 + ch - '0';
					ch = source[pos++];
				}
lex_float:
				token = TK_FLOAT;
				ch = source[pos++];
				f = i;
				while(1) {
					if(isalpha(ch)) goto lex_error;
					if(!isdigit(ch)) goto lex_num_end;
					f = f * 10 + ch - '0';
					j++;
					ch = source[pos++];
				}
lex_num_end:
				lex->cur_int = i;
				lex->cur_float = f * pow(10, -j);
				pos--;
				goto lex_end;
			}
		case '"': {
				token = TK_STRING;
				int i = 0;
				ch = LexGetch(source, &pos);
				while(ch != '"') {
					if(source[pos] == 0) goto lex_error;
					buf[i++] = (ch==-1 ? '"' : (ch==-2 ? '\'' : ch));
					ch = LexGetch(source, &pos);
				}
				if(i == 0) goto lex_error;
				lex->cur_str = memcpy(malloc(i + 1), buf, i);
				lex->cur_str[i] = 0;
				lex->str_len = i + 1;
				memset(buf, 0, i);
				goto lex_end;
			}
		case '\'': {
				token = TK_CHAR;
				ch = LexGetch(source, &pos);
				if(ch == '\'') goto lex_error;
				lex->cur_int = (ch==-1?'"':(ch==-2?'\'':ch));
				ch = source[pos++];
				if(ch != '\'') goto lex_error;
				goto lex_end;
			}
		default:
			token = TK_UNKNOWN;
			lex->cur_int = ch;
			goto lex_end;
		}
	}

lex_error:
	if(lex->error != NULL) lex->error(lex, token);
	token = TK_ERROR;
lex_end:
	lex->token = token;
	lex->next = LexStateNew();
	lex->next->last = lex;
	lex = lex->next;
	lex->pos = pos;
	lex->source = source;
	lex->error = lex->last->error;
	lex->buf = lex->last->buf;
	lex->line = line;
	return token;
}
