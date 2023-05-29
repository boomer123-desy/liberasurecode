#ifndef EC_CODING_H
#define EC_CODING_H

#include "erasurecode.h"
#define NULL ((void *)0)

typedef unsigned long int __uint64_t;
typedef __uint64_t uint64_t;
// typedef struct back
//     {
//         char* encoded_data;
//         char* encoded_parity;
//         uint64_t encoded_fragment_len;
//     } ret_args;

int ec_encode(char* filename, char* model, int k, int m, char **encoded_data, char **encoded_parity, uint64_t* encoded_fragment_len);

int ec_piece_encode(char* filename, char* model, int k, int m, int piece, int opt);

int ec_update(int k, int m, int num_fragments, char* filename);

int fragment_read(char **encoded_data, char **encoded_parity, int orig_data_size,
        fragment_metadata_t *fragment_metadata, struct ec_args *args);

#endif