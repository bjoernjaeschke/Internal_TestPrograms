// RudisAufgabe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARSIZE 1000
#define LIMIT_TABLEFILE 10

const char PATH_INPUTFILE [100] = "input.txt";
const char PATH_OUTPUTFILE[100] = "output.txt";
const char PATH_TABLEFILE [100] = "tabelle.txt";

char *moveThisStringOneIndexForward(char line[], int index_nullTerminator);

int checkLengthOf_TableFile(FILE *tablefile);
int getNullTerminatorOfString(char c, char line[]);
int printReplacementStringToOutputFile(FILE *outputfile, int linelength, int linelength2, char rep_line[], char commandTerminator);
int searchStringInTableAndReplace(FILE *tablefile, FILE *outputfile, char line[], char commandTerminator);
int thisCharIsNotCommand(char c, int flag_CommandFound);
int thisCharIsPartOfA_Command(char c);

int main()
{
	FILE *inputfile, *outputfile, *tablefile;

	FILE *appendfile = fopen(PATH_INPUTFILE, "a");
	fprintf(appendfile, "\n");	// Last line of original file is unreadable if there is no line-break added in.
	fclose(appendfile);

	inputfile  = fopen(PATH_INPUTFILE,  "r");
	outputfile = fopen(PATH_OUTPUTFILE, "w");
	tablefile  = fopen(PATH_TABLEFILE,  "r");

	if(checkLengthOf_TableFile(tablefile) == 0) {
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
	fclose(inputfile);
	fclose(outputfile);
	fclose(tablefile);
	return 0;
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

int printReplacementStringToOutputFile(FILE *outputfile, int line_length, int line_length2, char rep_line[], char commandTerminator) {
	for (int k = 0; k < line_length - line_length2; k++) {
		if (rep_line[k + line_length2 + 1] != '\n') {
			fprintf(outputfile, "%c", rep_line[k + line_length2 + 1]);
		}
		else break;
	}
	fprintf(outputfile, "%c", commandTerminator);
}

int searchStringInTableAndReplace(FILE *tablefile, FILE *outputfile, char line[], char commandTerminator) {
	char ref_line[CHARSIZE], rep_line[CHARSIZE];
	rewind(tablefile);
	int found_match = 0;
	while (fgets(ref_line, CHARSIZE, tablefile) != NULL) {
		char *p = strchr(ref_line, ';');
		int line_length = strlen(ref_line);
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