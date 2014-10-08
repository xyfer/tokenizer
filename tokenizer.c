/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



 int x = 0;
 int y = 0;
 int z = 0;            /* arbitary loop indexes */
 int q = 0;
 int i = 0;
 int a = 0;
 int b = 0;

char* hex;
char* hexresponse;
char hexresponsearray[4];
int escapedelim;
int delimindex = 0;
int doublequotedelim = 0;
int specialcasefound = 0;
int delimflag = 0;


struct TokenizerT_ {

char tokens[20][20];
int numtokens;
char* delims;
char* stream;
int numdelims;



};

typedef struct TokenizerT_ TokenizerT;

char *getHex (char c) {               /* returns a hexcode based on the post-backslash character it recieves */

	if (c == 'n')
	{
		hex = "[0x0a]";
	}

	if (c == 't')
	{
		hex = "[0x09]";
	}

	if (c == 'v')
	{
		hex = "[0x0b]";
	}

	if (c == 'b')
	{
		hex = "[0x08]";
	}

	if (c == 'r')
	{
		hex = "[0x0d]";
	}

	if (c == 'f')
	{
		hex = "[0x0c]";	
	}

	if (c == 'a')
	{
		hex = "[0x07]";	
	}

	if (c == '\\')
	{
		hex = "[0x5c]";
	}

	if (c == '\"')
	{
		hex = "[0x22]";
	}

	return hex;

}

char getEscape(char c){

if (c == 'n')                  /* takes one of the valid characters after a backslash and returns the corresponding escape char */
{
	return '\n';
}

if (c == 't')
{
	return '\t';
}

if (c == 'v')
{
	return '\v';
}

if (c == 'b')
{
	return '\b';
}

if (c == 'r')
{
	return '\r';
}

if (c == 'f')
{
	return '\f';
}

if (c == 'a')
{
	return '\a';
}

if (c == '\"')
{
	return '\"';
}

if (c == '\\')
{
	return '\\';
}

return 'z';

}

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {

 int delimfound = 0;
 int tokenindex = 0;
 int charindex = 0;
 int chaindelim = 0;


TokenizerT *Tokenizer = malloc(sizeof (TokenizerT));
Tokenizer->delims = malloc(sizeof(strlen(separators) +1));
Tokenizer->stream = malloc(sizeof(strlen(ts) +1));
Tokenizer->numtokens = 1;

for (a=0; ts[a] != '\0'; a++)
{
	Tokenizer->stream[a] = ts[a];
}


printf("tokenstream:  %s \n", (Tokenizer->stream));
//printf("separators:  %s \n", (separators));



for (i=0; separators[i]!= '\0'; i++)
{
	if (separators[i] == '\\')  /*checks if the user is trying to delimit with a single escape char */
	{                          
		if (separators[i+1] == 'n'){
			Tokenizer->delims[delimindex] = '\n';
			delimflag = 1;
		}
		else if (separators[i+1] == 'v'){
			Tokenizer->delims[delimindex] = '\v';
			delimflag = 1;
		}
		else if (separators[i+1] == 'b'){
			Tokenizer->delims[delimindex] = '\b';
			delimflag = 1;
		}
		else if (separators[i+1] == 'a'){
			Tokenizer->delims[delimindex] = '\a';
			delimflag = 1;
		}
		else if (separators[i+1] == 't'){
			Tokenizer->delims[delimindex] = '\t';
			delimflag = 1;
		}
		else if (separators[i+1] == 'f'){
			Tokenizer->delims[delimindex] = '\f';
			delimflag = 1;
		}
		else if (separators[i+1] == 'r'){
			Tokenizer->delims[delimindex] = '\r';
			delimflag = 1;
		}
		else if (separators[i+1] == '\\'){
			Tokenizer->delims[delimindex] = '\\';
			delimflag = 1;
		}
		else{
		Tokenizer->delims[delimindex] = separators[i];
		delimflag =0;
		}
		
	}	
	else{
		Tokenizer->delims[delimindex] = separators[i];
		delimflag =0;

			if (separators[i] == '\"'){
				doublequotedelim = 1;
			}
	}

	printf("delims:  %c \n", (Tokenizer->delims[delimindex]) );
	delimindex++;

	if (delimflag == 1){
		i= i+1;
	}

Tokenizer->numdelims++;	

}



printf("numdelims:  %d \n", Tokenizer->numdelims );

for (x=0; Tokenizer->stream[x] != '\0'; x++)
{
	//printf("current char %c \n", Tokenizer->stream[x]);

	delimfound = 0;
	escapedelim = 0;

	if (Tokenizer->stream[x] == '\\')
	{
		char escaperesponse = getEscape(Tokenizer->stream[x+1]);      /* checks for the case a delimiter is an escape char */

		if (escaperesponse != 'z')   /* a return of 'z' denotes we dont have a valid escape char */
		{	

			for(b=0; Tokenizer->delims[b]!= '\0'; b++)
			{
				if (escaperesponse == Tokenizer->delims[b])   /* we found an escape char in our tokenstream and it's a delimiter! very annoying/ugly case */
				{
					if (chaindelim == 0){
					Tokenizer->tokens[tokenindex][charindex] = '\0';
					charindex =0;
					delimfound = 1;
					tokenindex++;
					Tokenizer->numtokens++;
					chaindelim = 1;
					x=x+1;
					escapedelim = 1;
					}
					else{
					delimfound = 1;
					x=x+1;
					}
				}
			}	
		}

		if (escaperesponse == 'z'){             
			goto end;
		}


		if (Tokenizer->stream[x+1] == 'n' || Tokenizer->stream[x+1] == 't' || Tokenizer->stream[x+1] == 'v' || Tokenizer->stream[x+1] == 'b' || Tokenizer->stream[x+1] == 'r' || Tokenizer->stream[x+1] == 'f' || Tokenizer->stream[x+1] == 'a' || Tokenizer->stream[x+1] == '\\' || Tokenizer->stream[x+1] == '\"')
		{
			if (escapedelim == 0)   /* we found an escape character and its not a delimiter, covert to hex code */
			{
				hexresponse = (char*) getHex(Tokenizer->stream[x+1]);
				//printf("hexresponse: %s \n", hexresponse);
				Tokenizer->tokens[tokenindex][charindex] = hexresponse[0];
				Tokenizer->tokens[tokenindex][charindex+1] = hexresponse[1];
				Tokenizer->tokens[tokenindex][charindex+2] = hexresponse[2];
				Tokenizer->tokens[tokenindex][charindex+3] = hexresponse[3];
				Tokenizer->tokens[tokenindex][charindex+4] = hexresponse[4];
				Tokenizer->tokens[tokenindex][charindex+5] = hexresponse[5];
				charindex = charindex+6;
				x = x+1;
				chaindelim= 0;
				continue;
			}
		}
	}

	if (Tokenizer->stream[x] == '\"' && doublequotedelim == 0)   
	{
			hexresponse = "[0x22]";
			Tokenizer->tokens[tokenindex][charindex] = hexresponse[0];
			Tokenizer->tokens[tokenindex][charindex+1] = hexresponse[1];
			Tokenizer->tokens[tokenindex][charindex+2] = hexresponse[2];
			Tokenizer->tokens[tokenindex][charindex+3] = hexresponse[3];
			Tokenizer->tokens[tokenindex][charindex+4] = hexresponse[4];
			Tokenizer->tokens[tokenindex][charindex+5] = hexresponse[5];
			charindex = charindex+6;
			chaindelim =0;
			continue;
	}  

	end:

	for(z=0; z<Tokenizer->numdelims; z++)                           /* iterate through array of delimiters and compare current character to them*/
	{
		if (Tokenizer->stream[x] == Tokenizer->delims[z])           /* ordinary case for normal characters, check each against the array of delmiters */
		{
			if (chaindelim == 0){
			Tokenizer->tokens[tokenindex][charindex] = '\0';
			charindex =0;
			delimfound = 1;
			tokenindex++;
			Tokenizer->numtokens++;
			chaindelim = 1;             /*we dont want to make a new token if two chars in a row are delimiters, chaindelim is a flag to prevent this */
			}

			else{
			delimfound = 1;   
			}
		}
	}

	if (delimfound == 0)
	{	
		if (Tokenizer->stream[x] == '\\')
		{
			/* do nothing, ignore slash */
		}
		else
		{
			Tokenizer->tokens[tokenindex][charindex] = Tokenizer->stream[x];
			//printf("current char : %c \n", Tokenizer->tokens[tokenindex][charindex]);
			charindex++;
			chaindelim = 0;
		}
	}

}
 

return Tokenizer;

}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {

    free(tk);
    return;

}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk, int index) {

  return tk->tokens[index];
}


int  NullTokenChecker(TokenizerT *t){      /* checks if all the tokens are empty, if so, we should just print one empty one */

int f;

for (f=0; f< t->numtokens; f++ )
{
	if(t->tokens[f][0] != '\0')
	{
		return -1;
	}
}
	return 1;
}
/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */




int main(int argc, char **argv) {

	if (argc != 3)
	{
		printf("Wrong number of arguments (2 required) \n ");
		exit(1);
	}

	TokenizerT *tok = TKCreate(argv[1], argv[2]);
	char* word;
	int nullresponse = NullTokenChecker(tok);

	if (nullresponse == 1)
	{
		printf("token:  %s\n", tok->tokens[0] );
	}

	else

		for(q=0; q<tok->numtokens; q++)
		{

		word =TKGetNextToken(tok, q);

			if (q == 0 && tok->tokens[0][0] == '\0' && tok->numtokens > 2)   /* if the only token is blank, might asd well print it */
			{
				/* first token is empty, ignore */
			}
		

			else if (q == tok->numtokens-1 && tok->tokens[tok->numtokens-1][0] == '\0' && tok->numtokens > 2 )
			{
				/* last token is empty, ignore */	
			}

			else

			printf("token:  %s\n", word);
		}


  TKDestroy(tok);
  return 0;
}