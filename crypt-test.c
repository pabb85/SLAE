#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * MCrypt API available online:
 * http://linux.die.net/man/3/mcrypt
 */
#include <mcrypt.h>

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

int encrypt(
    void* buffer,
    int buffer_len, /* Because the plaintext could include null bytes*/
    char* IV, 
    char* key,
    int key_len 
){
  MCRYPT td = mcrypt_module_open("rijndael-128", NULL, "cbc", NULL);
  int blocksize = mcrypt_enc_get_block_size(td);
  if( buffer_len % blocksize != 0 ){return 1;}

  mcrypt_generic_init(td, key, key_len, IV);
  mcrypt_generic(td, buffer, buffer_len);
  mcrypt_generic_deinit (td);
  mcrypt_module_close(td);
  
  return 0;
}

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

void display(char* ciphertext, int len){
  int v;
  for (v=0; v<len; v++){
    printf("%c", ciphertext[v]);
  }
  printf("\n");
}

int main()
{
  MCRYPT td, td2;
  unsigned char * plaintext = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80";
  char* IV = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  char *key = "0123456789abcdef0123456789abcdef";
  int keysize = 32; /* 128 bits */
  unsigned char* buffer;
  int buffer_len = 32;

  buffer = calloc(1, buffer_len);
  strncpy(buffer, plaintext, buffer_len);

  printf("==C==\n");
  printf("plain:   %s\n", plaintext);
  encrypt(buffer, buffer_len, IV, key, keysize); 
  printf("cipher:  "); display(buffer , buffer_len);
  decrypt(buffer, buffer_len, IV, key, keysize);
  printf("decrypt: %s\n", buffer);

  return 0;
}
