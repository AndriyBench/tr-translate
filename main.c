/* Author: Ruben Ramirez
   Specs: This program simulates the tr unix command. Translates characters
   from standard input. 

   Standard form: ./translate [Option] Argument1 [Argument2]

   Program provides ability to delete characters with the -d option. The -t 
   option truncates Argument1 to the length of Argument2.

   Returns 0 if no errors are encountered, -1 otherwise.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int delete(int, char *); 
int translate(char *, char *);
char opt(char *);
char * getbounds(int, char *);

int main(int argc, char *argv[]) {
    int len;

    /* Not enough arguments provided */
    if(argc == 1) {
        printf("Translate: missing operand\n");
	    return -1;
    }
    if(opt(argv[1]) == 'd') 
        delete(argc, argv[2]);
    else if(argc == 3 && argv[1][0] != '-')
        translate(argv[1], argv[2]);             
    else if(opt(argv[1]) == 't') { 
        len = strlen(argv[3]);
        if(strlen(argv[2]) > len) 
            argv[2][len] = '\0';
        translate(argv[2], argv[3]);
    }
    /* Too many arguments provided */
    else {
        printf("Translate: extra operand '%s'\n", argv[3]);
	    return -1;
    }
    return 0;
}

/**
 * Translates characters from standard input.
 * @param char *arg_one, characters to be replaced.
 * @param char *arg_two, replacement characters.
 */
int translate(char *arg_one, char *arg_two) {
    int iochar, i, flag;
    int lone = strlen(arg_one);
    int ltwo = strlen(arg_two);
    char *tmp;

    if(*arg_one == '-') {
        printf("Translate: missing operand after '%s'\n", arg_two);
        return -1;
    }
    /* Check for special range entries */
    if((tmp = getbounds(1, arg_one)) != NULL) {
        arg_one = tmp;
        lone = strlen(arg_one);
        if((tmp = getbounds(2, arg_two)) != NULL) {
            arg_two = tmp;
            ltwo = strlen(arg_two);
        }
    }
    /* Perform translations */
    while((iochar = getchar()) != EOF) {
        flag = 0;
        for(i = 0; i < lone; i++) {
            if(iochar == *(arg_one + i) && i < ltwo) {
                putchar(*(arg_two + i));
                flag++;
                break;
            }
            else if(iochar == arg_one[i]) {
                putchar(*(arg_two + ltwo - 1));
                flag++;
                break;
            }
        }
        if(flag == 0)
            putchar(iochar);
    }
    free(arg_one);
    free(arg_two);
    return 0;
}

/** 
 * Deletes characters from the input stream.
 * @param int arg_count, number of command line arguments entered.
 * @param char *del, string containing list of characters to delete.
 * @return int, return 0 if characters were deleted properly, -1 if an 
 * error is encountered. 
 */
int delete(int arg_count, char *del) {
    int iochar, i, flag;
    int lone = strlen(del);
    char *tmp;

    /* Too many command line arguments entered */
    if(arg_count > 3) {
        printf("Translate: invalid number of operands provided\n");
        printf("Only one string must be provided when using delete mode \n");
        return -1;
    }
    if((tmp = getbounds(1, del)) != NULL) {
        del = tmp;
        lone = strlen(del);
    }

    /* Process stream of input, delete appropriate characters */
    while((iochar = getchar()) != EOF) {
        flag = 0;
        /* Compare each input character to each character in the delete string */
        for(i = 0; i < lone; i++) {
            if(iochar == *(del + i)) {
                putchar('\0');
                flag++;
                break;
            }
        }
        if(flag == 0)
            putchar(iochar);
    }
    free(del);
}

/**
 * Checks an argument to see if it's a range entry. Generates a string containing 
 * the set of characters in the given range.
 * @param int val, value representing whether the given string is Argument1 or 
 * Argument2.
 * @param char *string, string value to be scanned.
 * @return char *, new;y allocated string containing the set of characters in 
 * the given range.
 *
 * [:digit:] -- all numeric values
 * [:alpha:] -- all alphabetical letters
 * [:upper:] -- all uppercase letters
 * [:lower:] -- all lowercase letters
 */ 
char * getbounds(int val, char *string) {

    char *newstring = (char *) malloc(sizeof(char) * 60);
    int i, index = 0;

    /* Numeric values */
    if(strcmp(string, "[:digit:]") == 0) {
        if(val == 2) {
            printf("Translate: the only character class that can appear in string2\
                    are [:upper:] and [:lower:]\n");
            exit(-1);
        }
        for(i = 48; i <= 57; i++)
            *(newstring + index++) = i;
       *(newstring + index) = '\0';
        return newstring;
    }
    /* Alphabetic characters */
    else if(strcmp(string, "[:alpha:]") == 0) {
        if(val == 2) {
            printf("Translate: the only character class that can appear in string2\
                    are [:upper:] and [:lower:]\n");
            exit(-1);
        }
        for(i = 65; i <= 122; i++) {
            if(i < 91 || i > 96)
                *(newstring + index++) = i;
        }
        *(newstring + index) = '\0';       
        return newstring;
    }
    /* Uppercase letters */
    else if(strcmp(string, "[:upper:]") == 0) {
        for(i = 65; i <= 90; i++)
            *(newstring + index++) = i;
        *(newstring + index) = '\0';
        return newstring;
    }
    /* Lowercase letters */
    else if(strcmp(string, "[:lower:]") == 0) {
        for(i = 97; i <= 122; i++)
            *(newstring + index++) = i;
        *(newstring + index) = '\0';
        return newstring;
    }
    free(newstring);
    return NULL;
}

/**
 * Checks the validity of the option argument. 
 * @param char *, string containing the option argument.
 * @return char, character that specifies the option to be used.
 *  'd' -- delete mode character
 *  't' -- truncate character
 *  '\0' -- No option character
 */
char opt(char *s) { 
    int len = strlen(s);
    int i, del, trim;

    /* Argument is doesn't include the option flag */
    if(*s != '-')
        return '\0';
    if(strcmp(s, "-d") == 0)
        return 'd';
    if(strcmp(s, "-t") == 0)
        return 't';

    /* Scans entire string to validate every character */
    for(i = 1; i < len; i++) {
        if(*(s + i) != 'd' && *(s + i) != 't') {
            printf("Translate: invalid option '%c'\n", *(s+ i));
            exit(-1);
        }
        else if(*(s + i) == 'd')
            del++; 
        else if(*(s + i) == 't')
            trim++;
    }
    if(del != 0)
        return 'd';
    else if(trim != 0)
        return 't';
    return '\0';
}
