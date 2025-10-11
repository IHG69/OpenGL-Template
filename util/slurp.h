#ifndef __SLURP_H__
#define __SLURP_H__

// Just prints a message when an error happens
// Only if the functions doesn't need to assert/abort
#ifdef ENABLE_ERROR_LOGGIN
#endif // ENABLE_ERROR_LOGGIN

#include <stdio.h> // FILE, stderr, fopen, fclose, fseek, ftell, fprintf
#include <stdlib.h> // malloc
#include <errno.h> // errno
#include <string.h> // strerror

#ifndef SLURPDEF // Defines if the functions should be public (none) or private (static)
#define SLURPDEF
#endif // SLURPDEF

typedef char* CString; // Alias for (const)char *

SLURPDEF CString slurp(const CString file);

#endif // __SLURP_H__

#ifdef SLURP_IMPLEMENTATION

SLURPDEF CString slurp(const CString file) {
	FILE *fp = fopen(file, "r");
	if (!fp) {
#ifdef ENABLE_ERROR_LOGGIN
		fprintf(stderr, "Couldn't open file: %s\n", strerror(errno));
#endif // ENABLE_ERROR_LOGGIN
		return NULL;
	}

	(void)fseek(fp, 0, SEEK_END);
	long fsz = ftell(fp);
	(void)fseek(fp, 0, SEEK_SET);

	CString buf = (CString)malloc(fsz * sizeof(char) + 1);
	if (!buf) {
#ifdef ENABLE_ERROR_LOGGIN
		fprintf(stderr, "Couldn't create memory-buffer: %s\n", strerror(errno));
#endif // ENABLE_ERROR_LOGGIN
	} else {
		fread(buf, sizeof(char), fsz, fp);
#ifdef ENABLE_ERROR_LOGGIN
		if (!feof(fp)) {
			fprintf(stderr, "Reached end-of-file\n");
		} else if (!ferror(fp)) {
			fprintf(stderr, "Couldn't read file\n");
		}
#endif // ENABLE_ERROR_LOGGIN
	}

	if (!fclose(fp)) {
#ifndef ENABLE_ERROR_LOGGIN
		fprintf(stderr, "Could close file stream: %s\n", strerror(errno));
#endif // ENABLE_ERROR_LOGIN
	}
	return buf;
}

#endif // SLURP_IMPLEMENTATION
