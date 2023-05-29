#include "ec_coding.h"
//#include "erasurecode_helpers_ext.h"




int main(int argc, char const *argv[])
{
    struct ec_args args = {
                .k = 7,
                .m = 2,
                .w = 16,
                .hd = 8
            };
    
    char **encoded_data = NULL; 
    uint64_t encoded_fragment_len = 0;
    char **encoded_parity = NULL;
    int orig_data_size = 1024 * 1024;
    char *file = create_buffer(orig_data_size, 'x');
    //文件读写
    // char* filename = "/home/zuol/Pictures/tree.jpg";
    // char* update_filename = "/home/zuol/Pictures/tree3.jpg";
    // char* model = "r+b";
    //int orig_data_size = 0;

    int ret_encode = ec_encode(file, 7, 2, &encoded_data, &encoded_parity,
							   &encoded_fragment_len, &orig_data_size);

    int ret_update = ec_update(7, 2, 7, file, orig_data_size);

    int ret_piece = ec_piece_encode(file, orig_data_size, 7, 2, 2, 1);

    int ter_read = fragment_read(encoded_data, encoded_parity, orig_data_size, &args);


    return 0;
}

