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




// int file_size2(char* filename)
// {
//     struct stat statbuf;
//     stat(filename,&statbuf);
//     int size=statbuf.st_size;
 
//     return size;
// }

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

int main(int argc, char const *argv[])
{


    //int test_k[] = {2, 4, 8};
    //int test_m[] = {1, 2};


    //for (size_t i = 0; i < sizeof(test_k)/sizeof(test_k[0]); i++)
    //{
        //int test_k_args = test_k[i];
        //for (size_t j = 0; j < sizeof(test_m)/sizeof(test_m[0]); j++)
        //{
            //int test_m_args = test_m[j];
            //if (test_m_args >= test_k_args || (test_k_args > 4 && test_m_args > 1)) 
            //{
                //break;
            //}
    struct ec_args null_args = {
                .k = 8,
                .m = 2,
                .w = 16,
                .hd = 8
            };

             
            FILE *fp;
    int i , size = sizeof(int);
    char ch;

    if ((fp = fopen("/home/zuol/Pictures/tree.jpg", "r+b")) == NULL)
    {
        printf("Fail to open file!\n");
        exit(0);
    }

    while( (ch=fgetc(fp)) != EOF )
    {
        putchar(ch);
    }      

    int orig_data_size = fsize(fp);
       
    fclose(fp);
    

    //int orig_data_size = 1024 * 1024;
    
    char **encoded_data = NULL, **encoded_parity = NULL;
    char *orig_data = create_buffer(orig_data_size, ch);
    uint64_t encoded_fragment_len = 0;
    ec_backend_t instance = NULL;
    int (*orig_encode_func)(void *, char **, char **, int);
    fragment_metadata_t cur_frag;
    int force_metadata_checks = 0;
    char *out_data = NULL; uint64_t out_data_len = 0;

    

    
    int desc = liberasurecode_instance_create(&null_args);
    if (-EBACKENDNOTAVAIL == desc) {
        fprintf(stderr, "Backend library not available!\n");
        return 0;
    }
    assert(desc > 0);
    

    int rc = liberasurecode_encode(desc, orig_data, orig_data_size, &encoded_data, &encoded_parity, &encoded_fragment_len);
    
    printf("k:%d  +  m:%d的情况:\n", null_args.k, null_args.m);
    printf("原始数据大小:%d\n", orig_data_size);
    printf("每个碎片长度:%ld\n", encoded_fragment_len);

    
    // int ret = liberasurecode_get_fragment_metadata(*encoded_data, &cur_frag);
    //     //assert(ret != 0);
    // if (ret != 0)
    // {
    //     printf("Failed");
    // }
    
    
    int encoden = liberasurecode_decode(desc, encoded_data, null_args.k, encoded_fragment_len, force_metadata_checks, &out_data, &out_data_len);
    printf("合并数据长度:%ld\n\n\n", out_data_len);


    //Output image
    // if ((fp = fopen("/home/zuol/Pictures/tree2.jpg", "w+")) == NULL)
    // {
    //     printf("Fail to open file!");
    //     exit(0);
    // }
    // if(out_data != NULL){
    //     fprintf(fp, "%s", out_data);
    //     printf("output successed!");
    // }else{
    //     printf("output failed!");
    // }
    // fclose(fp);    




            
        //}
        
    //}
    
    
    
    


    return 0;


}
