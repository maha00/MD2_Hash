#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include "md2_alternative.h"
#include "md2_hash.h"

extern uint8_t s[];
// Function for the 16-Byte Padding. Size of the input.
void padding_alt(MD2_HASH *hash)
{
    // Getting the number of bytes that we still need to write.
    int toWrite = 16 - (hash->length);
    // Putting the necessary size inside the index of the buffers.
    while (hash->length < 16)
    {
        hash->input[(hash->length)++] = toWrite;
    }
}
// Initializing the with MEMSET for the starting status.
void initialize_alt(MD2_HASH *hash)
{
    
    memset(hash->checksum, 0, 16);
    memset(hash->output, 0, 48);
    hash->length = 0;
}
// Alternative Update.
void md2_update_alt(MD2_HASH *hash, const uint8_t buf[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        hash->input[hash->length] = buf[i];
        hash->length++;
        if (hash->length == 16)
        {
            transform_alt(hash, hash->input);
            hash->length = 0;
        }
    }
}
// Alternative Checksum.
void md2_checksum_alt(const uint8_t *buf, MD2_HASH *hash)
{

    uint8_t cTemp = hash->checksum[15];
    for (int j = 0; j < 16; j++)
    {
        hash->checksum[j] ^= s[cTemp ^ buf[j]];
        cTemp = hash->checksum[j];
    }
}
// Alternative Transform.
void transform_alt(MD2_HASH *hash, const uint8_t buf[])
{
    uint8_t j, k, t;
    for (j = 0; j < 16; j++)
    {
        // Starting to write from 16.
        hash->output[16 + j] = buf[j];
        hash->output[32 + j] = (hash->output[16 + j] ^ hash->output[j]);
    }
    t = 0;
    for (j = 0; j < 18; j++)
    {

        for (k = 0; k < 48; k++)
        {
            // Iterating through all the bytes in the digest.
            hash->output[k] ^= s[t];
            // Changing digest array.
            t = hash->output[k];
        }

        t = (t + j) % 256;
    }

    md2_checksum_alt(buf, hash);
}
// Calling all the alternative functions sequentially.
void md2_hash_alt(uint8_t out[16], MD2_HASH *hash)
{
    padding_alt(hash);
    transform_alt(hash, hash->input);
    transform_alt(hash, hash->checksum);
    memcpy(out, hash->output, 16);
}