/* NIST Secure Hash Algorithm */

#include "my_file.h"
#include "sha.h"

extern unsigned char data[32743];

int main(void) {
    struct my_FILE fin;
    fin.data = data;
    fin.size = 32743;  // set size = 3247552 for input_large
    fin.cur_pos = 0;
    struct SHA_INFO sha_info;
    sha_stream(&sha_info, &fin);
        
    return 0;
}

