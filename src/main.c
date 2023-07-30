#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parser.h"

#define READ_MAX 16
#define STREAM_CHUNK 512

char* gora_read_file(char* filename)
{
    int fd = open(filename, O_RDONLY);

    if (fd == -1)
        return NULL;

    size_t read_sz;
    char   buff[READ_MAX + 1];

    size_t stream_sz    = 0;
    size_t stream_alloc = STREAM_CHUNK;
    char*  stream       = calloc(STREAM_CHUNK, sizeof(char));

    while ((read_sz = read(fd, buff, READ_MAX)) > 0) {
        long unsigned int req_sz = stream_sz + read_sz;

        if (req_sz >= stream_alloc) {
            char* new_stream;

            if ((new_stream = realloc(stream, stream_alloc + STREAM_CHUNK)) == NULL) {
                goto free_err;
            }

            stream_alloc += STREAM_CHUNK;
            stream = new_stream;
        }

        strncpy(stream + stream_sz, buff, read_sz);
        stream_sz = req_sz;
    }

    if (read_sz == -1)
        goto free_err;

    stream[stream_sz + 1] = '\0';

    return stream;

free_err:
    free(stream);
    return NULL;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* stream;
    if ((stream = gora_read_file(argv[1])) == NULL)
        goto file_err;

    struct gora_list* token_lst = gora_parser_parse(stream);

    free(stream);
    gora_parser_free_token_list(token_lst);

    return EXIT_SUCCESS;

file_err:
    perror("error: could not process input file");
    return EXIT_FAILURE;
}
