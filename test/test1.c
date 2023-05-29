#include "ec_coding.h"
#include "erasurecode_helpers_ext.h"
int main(int argc, char const *argv[])
{
    //ret_args *mes; 
    char** encoded_data = NULL; 
    uint64_t* encoded_fragment_len = NULL;
    char** encoded_parity = NULL;
    char* filename = "/home/zuol/Pictures/tree.jpg";
    char* update_filename = "/home/zuol/Pictures/tree3.jpg";
    char* model = "r+b";
    int ret_encode = ec_encode(filename, model, 7, 2, encoded_data, encoded_parity, encoded_fragment_len);
    
    // if( mes->encoded_fragment_len > 0){
    //     printf("ewrasdfasdfa");
    //     printf("%ld\n", mes->encoded_fragment_len);
    // }else{}
    //printf("%ld\n", *encoded_fragment_len);
    //  printf("%s\n", update_filename);
    // char** available_fragments = NULL;
    // available_fragments = &(mes->encoded_data);
    // printf("123456\n");
    //  printf("%s\n", update_filename);
    // // if(update_filename == NULL){
    // //     update_filename = "/home/zuol/Pictures/tree3.jpg";
    // // }
    // printf("123456\n");
    // // int ret_update = 
    // int a = is_invalid_fragment_header((fragment_header_t *) mes->encoded_data[0]);
    // // printf("%d\n",a);
    // // ec_update(7, 2, 7,filename, mes);
    // // printf("%d\n", ret_update);
    // // ec_piece_encode(filename, model, 7, 2, 2, 1);


    return 0;
}

