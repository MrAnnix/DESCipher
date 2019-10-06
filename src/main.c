/**DESCipher*******************************************************************
  File        DESCipher.c

  Resume      Data Encryption Standard (DES) implementation in c

  See also    des.c, des.h

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

int main(int argc, char** argv){
    key_set_t key;
        
    uint8_t clave_str[9] = "holahola";
    memcpy(&key.key64, clave_str, 8);
    
    key.key64 = 0x133457799BBCDFF1;//Overwrite key
    
    generate_subkeys(&key);
    
    uint64_t message[] = {0x0123456789ABCDEF,
                          0x727DA00BCA5A84EE,
                          0x47F269A4D6438190,
                          0x9DD52F78F5358499,
                          0x828AC9B453E0E653};
    
    uint64_t out[5] = {0};
    
    process_block(message, out, &key, 1);
}