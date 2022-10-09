
#ifndef MD2_HASH__
#define MD2_HASH__
#include <stdint.h>
#include <stddef.h>
 // accessing data example
 size_t length; 
 void init(); 
void print(const uint8_t *buff); 
void md2_hash(size_t len, const uint8_t buf[len], uint8_t out[16]);
void md2_checksum(size_t len, uint8_t* buf);
void md2_update(const uint8_t* buf, size_t len); 
void transform(size_t len ,uint8_t* buf);

#endif