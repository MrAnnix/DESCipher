/**DESCipher*******************************************************************
  File        des.c
  
  Resume      Data Encryption Standard (DES) implementation in c
  
  See also    des.h
  
  Autor       Raúl San Martín Aniceto
  
  Copyright (c) 2019 Raúl San Martín Aniceto
  
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "des.h"

//PC1 permutation 
const uint8_t PC1[] = {57, 49, 41, 33, 25, 17,  9,
                        1, 58, 50, 42, 34, 26, 18,
                       10,  2, 59, 51, 43, 35, 27,
                       19, 11,  3, 60, 52, 44, 36,
                       63, 55, 47, 39, 31, 23, 15,
                        7, 62, 54, 46, 38, 30, 22,
                       14,  6, 61, 53, 45, 37, 29,
                       21, 13,  5, 28, 20, 12,  4};

// Key's circular left shift sizes
const uint8_t key_shift_sizes[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

//PC2 permutation
const uint8_t PC2[] = {14, 17, 11, 24,  1,  5,
                        3, 28, 15,  6, 21, 10,
                        23, 19, 12,  4, 26,  8,
                        16,  7, 27, 20, 13,  2,
                        41, 52, 31, 37, 47, 55,
                        30, 40, 51, 45, 33, 48,
                        44, 49, 39, 56, 34, 53,
                        46, 42, 50, 36, 29, 32};

//S Boxes                                         
const uint8_t S1[] = {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
                       0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
                       4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
                       15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13};

const uint8_t S2[] = {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
                       3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
                       0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
                      13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9};

const uint8_t S3[] = {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
                      13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
                      13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
                       1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12};

const uint8_t S4[] = { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
                      13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
                      10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
                       3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14};

const uint8_t S5[] = { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
                      14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
                       4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
                      11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3};

const uint8_t S6[] = {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
                      10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
                       9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
                       4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13};

const uint8_t S7[] = { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
                      13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
                       1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
                       6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12};

const uint8_t S8[] = {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
                       1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
                       7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
                       2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11};


//F function permutation with expansion            
const uint8_t message_expansion[] = {32,  1,  2,  3,  4,  5,
                                      4,  5,  6,  7,  8,  9,
                                      8,  9, 10, 11, 12, 13,
                                     12, 13, 14, 15, 16, 17,
                                     16, 17, 18, 19, 20, 21,
                                     20, 21, 22, 23, 24, 25,
                                     24, 25, 26, 27, 28, 29,
                                     28, 29, 30, 31, 32,  1};
                            
//F function final permutation
const uint8_t right_sub_message_permutation[] = {16,  7, 20, 21,
                                                 29, 12, 28, 17,
                                                  1, 15, 23, 26,
                                                  5, 18, 31, 10,
                                                  2,  8, 24, 14,
                                                 32, 27,  3,  9,
                                                 19, 13, 30,  6,
                                                 22, 11,  4, 25};

const uint8_t initial_message_permutation[] ={58, 50, 42, 34, 26, 18, 10, 2,
                                              60, 52, 44, 36, 28, 20, 12, 4,
                                              62, 54, 46, 38, 30, 22, 14, 6,
                                              64, 56, 48, 40, 32, 24, 16, 8,
                                              57, 49, 41, 33, 25, 17,  9, 1,
                                              59, 51, 43, 35, 27, 19, 11, 3,
                                              61, 53, 45, 37, 29, 21, 13, 5,
                                              63, 55, 47, 39, 31, 23, 15, 7};

int shift_key(uint64_t* key, uint32_t* fpart, uint32_t* spart){
    
}

int generate_subkeys(key_set_t *key){
    //First apply PC1 to mix the key
    int i;
    for(i=0; i<56; i++){
        key->key56 <<= 1;
        if(key->key64 & (1 << PC1[i-1]))
            key->key56 |= 1;
    }
    
    /*FOR DEBUGGING
    printf("%lx\n", key->key56);
    
    char clave[7] = {0};
    for(i=0; i<7; i++){
        clave[i] = (key->key56 >> (8*i)) & 0xFF;
        printf("%x", clave[i]);
    }printf("\n");*/
    
    //Subkeys generation 16 rounds
    for(i=0; i<16; i++){
        //Split the actual mixed key of 56 bits into 2 parts of 28 bits
        key->key_split[0] = (key->key56 >> 28) & 0xFFFFFFF;
        key->key_split[1] = (key->key56 >>  0) & 0xFFFFFFF;
        
        //Shift splited keys as key_shift_sizes determines
        key->key_split[0] <<= key_shift_sizes[i];
        key->key_split[1] <<= key_shift_sizes[i];
        
        uint64_t merged_key = (key->key_split[0] << 28 & 0x00FFFFFFF0000000) 
                            | (key->key_split[0] <<  0 & 0x000000000FFFFFFF);
        
        //Apply PC2 to the merged key and generate the i_th subkey
        int j;
        for(j=0; j<48; j++){
            key->subkey[j] <<= 1;
            if(merged_key & (1 << PC2[j-1]))
                key->subkey[j] |= 1;
        }       
    }
}               