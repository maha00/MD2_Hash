#ifndef MD2_HASH_ALTERNATIVE__
#define MD2_HASH_ALTERNATIVE__
#include <stdint.h>
#include <stddef.h>
typedef struct {
   uint8_t input[16];
   uint8_t output[48];
   uint8_t checksum[16];
   size_t length;
} MD2_HASH;
void padding_alt(MD2_HASH *hash);  
void initialize_alt(MD2_HASH *hash); 
void md2_hash_alt( uint8_t out[16],MD2_HASH *hash);
void md2_update_alt(MD2_HASH *hash, const uint8_t buf[], size_t len);
void md2_checksum_alt( const uint8_t* buf,MD2_HASH *hash);
void transform_alt(MD2_HASH *hash , const uint8_t buf[]); 
#endif