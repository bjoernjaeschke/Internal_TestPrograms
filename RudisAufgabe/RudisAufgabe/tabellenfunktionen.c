#include "tabellenfunktionen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int f_standardized(FILE *pOutputFile, char *pParameter) {
	int stringlength = 0, index = 0;
	while (pParameter[index] != '\0') {
		fprintf(pOutputFile, "%c", pParameter);
		stringlength++; index++;
	}
}

int f0(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f1(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f2(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f3(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f4(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f5(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f6(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f7(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f8(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
int f9(FILE *pOutputFile, char *pParameter) {
	return f_standardized(pOutputFile, pParameter);
}
