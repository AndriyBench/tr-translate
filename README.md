# tr-translate

Specs: This C program simulates the tr unix command. Translates characters
from standard input. 

Standard form: ./translate [Option] Argument1 [Argument2]

Program provides ability to delete characters with the -d option. 
The -t option truncates Argument1 to the length of Argument2.

Range entries: 
 * [:digit:] -- represents all numeric values
 * [:alpha:] -- represents all alphabetical letters
 * [:upper:] -- represents all uppercase letters
 * [:lower:] -- represents all lowercase letters

Returns 0 if no errors are encountered, -1 otherwise.
