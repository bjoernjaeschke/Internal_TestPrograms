// RudisAufgabe.c : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabellenfunktionen.h"

#define CHARSIZE 1000
#define LIMIT_TABLEFILE 10

const char PATH_INPUTFILE [] = "input.txt";
const char PATH_OUTPUTFILE[] = "output.txt";
const char PATH_TABLEFILE [] = "tabelle.txt";

char *getSubstringFromThisIndexOn(char *line, int index);
char *moveThisStringOneIndexForward(char line[], int index_nullTerminator);

int checkLengthOf_TableFile(FILE *tablefile);
int getNullTerminatorOfString(char c, char line[]);
int handleTableCommand(FILE *tablefile, FILE *outputfile, char *pointer);
int keepWindowOpen();
int printReplacementStringToOutputFile(FILE *outputfile, int linelength, int linelength2, char rep_line[], char commandTerminator);
int runTheWholeProgram();
int searchStringInTableAndReplace(FILE *tablefile, FILE *outputfile, char line[], char commandTerminator);
int thisCharIsNotCommand(char c, int flag_CommandFound);
int thisCharIsPartOfA_Command(char c);
int thisTableCharIsCommand(char c);

int errorMessage_unreachableCode();
int errorMessage_wrongFunctionCalled();

int main()
{
	runTheWholeProgram();
	keepWindowOpen();
	return 0;
}

char *getSubstringFromThisIndexOn(char *line, int index) {
	char outputline[100];
	int i = 0;
	while (line[index] != '\0') {
		outputline[i] = line[index];
		i++; index++;
		printf("char: %c\n", line[index]);
	}
	return outputline;

}

char *moveThisStringOneIndexForward(char line[], int index_nullTerminator) {
	for (int i = 1; i < index_nullTerminator; i++) {
		line[i - 1] = line[i];
	}
	line[index_nullTerminator - 1] = '\0';
	return line;
}

int checkLengthOf_TableFile(FILE *tablefile) {
	int entries = 0;
	char c;
	if (tablefile) {
		while ((c = getc(tablefile)) != EOF) {
			if (c == ';') {
				entries++;
			}
		}
		rewind(tablefile);
	}
	if (entries > LIMIT_TABLEFILE) {
		printf("Too many entries in table-file! 'Used/Allowed': %d/%d\nPlease adjust file and restart.", entries, LIMIT_TABLEFILE);
		getchar(); getchar();
		return 1;
	}
	else {
		return 0;
	}
}

int getNullTerminatorOfString(char c, char line[]) {
	int index_null = 0;
	while (line[index_null] != '\0') {
		index_null++;
	}
	return index_null;
}

int handleTableCommand(FILE *tablefile, FILE *outputfile, char *pointer) {
	if (pointer[1] == 'f') {
		switch (pointer[2]) {
		case '0':
			f0(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '1':
			f1(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '2':
			f2(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '3':
			f3(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '4':
			f4(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '5':
			f5(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '6':
			f6(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '7':
			f7(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '8':
			f8(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		case '9':
			f9(outputfile, getSubstringFromThisIndexOn(pointer, 3));
			break;
		default:
			errorMessage_wrongFunctionCalled();
			break;
		}
	}
	else if(pointer[1] != 'f') {
		printf("Incorrect command found!\n");
	}
	else {
		errorMessage_unreachableCode();
	}

	return 0;
}

int keepWindowOpen() {
	getchar(); getchar();
	return 0;
}

int printReplacementStringToOutputFile(FILE *outputfile, int line_length, int line_length2, char rep_line[], char commandTerminator) {
	for (int k = 0; k < line_length - line_length2; k++) {
		if (rep_line[k + line_length2 + 1] != '\n') {
			fprintf(outputfile, "%c", rep_line[k + line_length2 + 1]);
		}
		else break;
	}
	fprintf(outputfile, "%c", commandTerminator);

	return 0;
}

int runTheWholeProgram() {
	FILE *inputfile, *outputfile, *tablefile;

	FILE *appendfile = fopen(PATH_INPUTFILE, "a");
	fprintf(appendfile, "\n");	// Last line of original file is unreadable if there is no line-break added in.
	fclose(appendfile);

	inputfile = fopen(PATH_INPUTFILE, "r");
	outputfile = fopen(PATH_OUTPUTFILE, "w");
	tablefile = fopen(PATH_TABLEFILE, "r");

	if (checkLengthOf_TableFile(tablefile) == 0) {
		int index = 0, flag_CommandFound = 0;
		char line[CHARSIZE], commandTerminator = ' ';
		if (inputfile) {
			char c;
			while ((c = getc(inputfile)) != EOF) {
				if (thisCharIsNotCommand(c, flag_CommandFound) == 1) {
					fprintf(outputfile, "%c", c);
					continue;
				}
				if (thisCharIsPartOfA_Command(c) == 1) {
					if (flag_CommandFound == 0) {
						flag_CommandFound = 1;
					}
					line[index] = c;
					index++;
					continue;
				}
				commandTerminator = c;
				line[index] = '\0';
				int line_nullpos = getNullTerminatorOfString(c, line);
				strcpy(moveThisStringOneIndexForward(line, line_nullpos), line);

				if (searchStringInTableAndReplace(tablefile, outputfile, line, commandTerminator) == 0) {
					fprintf(outputfile, "???");
				}
				index = 0; flag_CommandFound = 0;
			}
		}
	}
	else if ((checkLengthOf_TableFile(tablefile) != 0) && (checkLengthOf_TableFile(tablefile) != 1)) {
		errorMessage_unreachableCode();
	}

	if (inputfile != NULL)
		fclose(inputfile);
	if (tablefile != NULL)
		fclose(tablefile);
	if (outputfile != NULL)
		fclose(outputfile);
	return 0;
}

int searchStringInTableAndReplace(FILE *tablefile, FILE *outputfile, char line[], char commandTerminator) {
	char ref_line[CHARSIZE], rep_line[CHARSIZE];
	rewind(tablefile);
	int found_match = 0;
	while (fgets(ref_line, CHARSIZE, tablefile) != NULL) {
		char *p = strchr(ref_line, ';');
		printf("Character: %c\n", p[1]);
		int line_length = strlen(ref_line);
		if (thisTableCharIsCommand(p[1]) == 0) {
			for (int k = 0; k < line_length; k++) {
				rep_line[k] = ref_line[k];
			}
			ref_line[p - ref_line] = '\0';
			if (strcmp(line, ref_line) == 0) {
				found_match = 1;
				int line_length2 = strlen(ref_line);
				printReplacementStringToOutputFile(outputfile, line_length, line_length2, rep_line, commandTerminator);
				break;
			}
		}
		else if(thisTableCharIsCommand(p[1]) == 1){
			printf("Found command!\n");
			//################TO-DO: Handle fx-function and following string######################
			handleTableCommand(tablefile, outputfile, ref_line);
		}
		else {
			errorMessage_unreachableCode();
		}

	}
	return found_match;
}

int thisCharIsNotCommand(char c, int flag_CommandFound) {
	if ((c != '$') && (flag_CommandFound == 0)) {
		return 1;
	}
	else return 0;
}

int thisCharIsPartOfA_Command(char c) {
	if ((c != '\n') && (c != ' ')) {
		return 1;
	}
	else return 0;
}

int thisTableCharIsCommand(char c) {
	if (c == '$') {
		return 1;
	}
	else return 0;
}

int errorMessage_unreachableCode() {
	printf("What?\nHow did you get into this part of the code?\nPlease contact me at bjoern.jaeschke@sis.gmbh if you know how you managed to get the program to\n");
	printf("display this message. I'm truly baffled.\nAnyway, try restarting the program to fix the issue. If the problem persists, send me a text.\nCheers.\n\n");
}

int errorMessage_wrongFunctionCalled() {
	printf("\n\nHi, I'm the guy who wrote this program.\n\n");
	printf("It seems that there's something wrong with the table-file.\n");
	printf("You see, normally, a command should start with a '$', be followed by an 'f', and then be followed by a one digit number.\n");
	printf("For some reason, that's not the case. Either the author of the table-file made a mistake, or you managed to\n");
	printf("majorly screw with the program. In any case. This command doesn't work.\n");
	printf("Instead of what should be there (in the table-author's opinion), I'm going to have the program write \"???\" to the file.\n");
	printf("Is that cool with you? Great. Good luck figuring this out.\nHave a nice day!\n\n");
	return 0;
}