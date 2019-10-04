#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "des.h"

int main(int argc, char** argv){
    key_set_t key;
        
    uint8_t clave_str[9] = "holahola";
    memcpy(&key.key64, clave_str, 8);
    
    printf("%s\n", clave_str);
    int i;
    for(i=0; i<8; i++){
        printf("%c", (key.key64 >> (8*i)) & 0xFF);
    }printf("\n");
    generate_subkeys(&key);
    
    uint64_t full = 0xAAAAAAAFFFFFFF;
    uint32_t partial[2] = {0};
    partial[0] = (full >> 28) & 0xFFFFFFF;
    partial[1] = (full >>  0) & 0xFFFFFFF;
    
    printf("%lx\n", full);
    printf("%x\n", partial[0]);
    printf("%x\n", partial[1]);
}