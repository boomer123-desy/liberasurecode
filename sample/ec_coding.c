#include <bits/types.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <zlib.h>
#include "erasurecode.h"
#include "erasurecode_helpers.h"
#include "erasurecode_helpers_ext.h"
#include "erasurecode_preprocessing.h"
#include "erasurecode_backend.h"
#include "erasurecode_postprocessing.h"
#include "erasurecode_stdinc.h"
#include "ec_coding.h"

int desc;

int fsize(FILE *fp){
    int n;
    fpos_t fpos;  //当前位置
    fgetpos(fp, &fpos);  //获取当前位置
    fseek(fp, 0, SEEK_END);
    n = ftell(fp);
    fsetpos(fp,&fpos);  //恢复之前的位置 
    return n;
}

char *create_buffer(int size, char fill)
{
    char *buf = malloc(size);
    memset(buf, fill, size);
    return buf;
}


int ec_encode(char *orig_data, int k, int m,
        char ***encoded_data, char ***encoded_parity, uint64_t *encoded_fragment_len, int *orig_data_size){

    struct ec_args null_args = {
                .k = k,
                .m = m,
                .w = 16,
                .hd = 8
            };
   
    
    //文件读写
    // FILE *fp;
    // char ch;
    // if ((fp = fopen(filename, model)) == NULL)
    // {
    //     printf("Fail to open file!\n");
    //     exit(0);
    // }
    // while( (ch=fgetc(fp)) != EOF )
    // {
    //     putchar(ch);
    // }      

    // int file_size = fsize(fp);
    // *orig_data_size = file_size;
    // fclose(fp);

    
    // char *orig_data = create_buffer(*orig_data_size, ch);
    
    desc = liberasurecode_instance_create(&null_args);
    if (-EBACKENDNOTAVAIL == desc) {
        fprintf(stderr, "Backend library not available!\n");
        return 0;
    }
    // assert(desc > 0);
    

    int rc = liberasurecode_encode(desc, orig_data, *orig_data_size, encoded_data, encoded_parity, encoded_fragment_len);
    // assert(rc == 0);

    printf("k:%d  +  m:%d的情况:\n", null_args.k, null_args.m);
    printf("原始数据大小:%d\n", *orig_data_size);
    printf("每个碎片长度:%ld\n\n", *encoded_fragment_len);

    return 0;
}


int ec_piece_encode(char* file, int orig_data_size, int k, int m, int piece, int opt){
    struct ec_args null_args = {
                .k = k,
                .m = m,
                .w = 16,
                .hd = 8
            };
    
    
    //文件读取
    // FILE *fp;
    // char ch;
    // if ((fp = fopen(filename, model)) == NULL)
    // {
    //     printf("Fail to open file!\n");
    //     exit(0);
    // }

    // while( (ch=fgetc(fp)) != EOF )
    // {
    //     putchar(ch);
    // }      

    // int orig_data_size = fsize(fp)/piece*opt;
    // fclose(fp);

    //内存分配page对齐
    int aligned_data_size = liberasurecode_get_aligned_data_size(desc, orig_data_size/piece*opt);
    char **encoded_data = NULL, **encoded_parity = NULL;
    char *orig_data = create_buffer(aligned_data_size, *file);
    uint64_t encoded_fragment_len = 0;
    
    int rc = liberasurecode_encode(desc, orig_data, orig_data_size, &encoded_data, &encoded_parity, &encoded_fragment_len);
    assert(rc == 0);

    printf("k:%d  +  m:%d的情况:\n", null_args.k, null_args.m);
    printf("原始数据大小:%d\n", orig_data_size);
    printf("每个碎片长度:%ld\n\n", encoded_fragment_len);

    return 0;
}

int ec_update(int k, int m, int num_fragments, char* file, int orig_data_size){

    char *out_data = NULL;
    struct ec_args null_args = {
                .k = k,
                .m = m,
                .w = 16,
                .hd = 8
            };

    int desc = liberasurecode_instance_create(&null_args);
    if (-EBACKENDNOTAVAIL == desc) {
        fprintf(stderr, "Backend library not available!\n");
        return 0;
    }
    assert(desc > 0);


    //文件读取
    // FILE *fp;
    // char ch;
    // if ((fp = fopen(filename, "r+b")) == NULL)
    // {
    //     printf("Fail to open file!\n");
    //     exit(0);
    // }

    // while( (ch=fgetc(fp)) != EOF )
    // {
    //     putchar(ch);
    // }      

    // int orig_data_size = fsize(fp);
    // fclose(fp);

    out_data = malloc(orig_data_size);
    if(out_data == NULL){
        printf("Error: memory allocation failed.\n");
        return -1;
    }
    memset(out_data, *file, orig_data_size);

    return 0;
}


int fragment_read(char **encoded_data, char **encoded_parity, int orig_data_size, struct ec_args *args){

    int i = 0;
    int rc = 0;
    int data_ret = 0;
    int parity_ret = 0;
    fragment_metadata_t cur_frag;
    int num_fragments = args-> k + args->m;

    int desc = liberasurecode_instance_create(args);
    if (-EBACKENDNOTAVAIL == desc) {
        fprintf(stderr, "Backend library not available!\n");
        return 0;
    }
    assert(desc > 0);

    for (i = 0; i < num_fragments; i++) {
        //char *data = NULL;
        char *header = NULL;
        memset(&cur_frag, -1, sizeof(fragment_metadata_t));
        if (i < args->k) {
            rc = liberasurecode_get_fragment_metadata(encoded_data[i], &cur_frag);
            //data = get_data_ptr_from_fragment(encoded_data[i]);
            header = encoded_data[i];
            data_ret = is_invalid_fragment(desc, header);
            
        } else {
            rc = liberasurecode_get_fragment_metadata(encoded_parity[i - args->k], &cur_frag);
            //data = get_data_ptr_from_fragment(encoded_parity[i - args->k]);
            header = encoded_parity[i - args->k];
        }
        assert(rc == 0);
        assert(cur_frag.orig_data_size == orig_data_size);
        assert(cur_frag.size != 0);
        assert(cur_frag.chksum_type == args->ct);
    }
    if(data_ret == 0 && parity_ret == 0){
                printf("数据片段和校验片段正常\n");
    }
    if(data_ret != 0 && parity_ret == 0){
                printf("数据片段不正常，校验片段正常\n");
    }
    if(data_ret == 0 && parity_ret != 0){
                printf("数据片段正常,校验片段不正常\n");
    }
    if(data_ret != 0 && parity_ret != 0){
                printf("数据片段和校验片段都不正常\n");
            }

    return 0;
}


