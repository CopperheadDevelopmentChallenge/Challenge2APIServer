#include "database.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * Place the contents of the specified file into a memory buffer
 *
 * @param[in] filename The path and name of the file to read
 * @param[out] filebuffer A pointer to the contents in memory
 * @return status 0 success, 1 on failure
 */
int get_file_contents(const char* filename, char** outbuffer) {
    FILE* file = NULL;
    long filesize;
    const int blocksize = 1;
    size_t readsize;
    char* filebuffer;

    // Open the file
    file = fopen(filename, "r");
    if (NULL == file) {
        printf("'%s' not opened\n", filename);
        return EXIT_FAILURE);
    }

    // Determine the file size
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);
    rewind(file);

    // Allocate memory for the file contents
    filebuffer = (char*) malloc(sizeof(char) * filesize);
    *outbuffer = filebuffer;
    if (filebuffer == NULL) {
        fputs ("malloc out-of-memory", stderr);
        exit(EXIT_FAILURE);
    }

    // Read in the file
    readsize = fread(filebuffer, blocksize, filesize, file);
    if (readsize != filesize) {
        fputs("didn't read file completely",stderr);
        return 1;
    }

    fclose(file);
    return 0;
}