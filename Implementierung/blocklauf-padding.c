#include "blocklauf-padding.h"
#include "md2_hash.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// Getting the non-empty bytes of the last block.
extern size_t length;
// Getting the Buffer to fill up the Bytes.
void padding(uint8_t *buf) // Function for the 16-Byte Padding. Size of the input.
{
   //***OPTIMIZATION**
   size_t padLen = 16 - length; // Getting the number of bytes that we still need to write.
   // while ( length <16) //Putting the necessary size inside the index of the buffers.
   // {
   //    buf[length++] =  padLen;
   // printf("%d: %c\n", i,buffer[i]);

   //  }
   //  *len =*len+padLen;
   // Padding even the last block is full.
   if (padLen == 0)
   {
      padLen = 16;
   }
   // Filling the empty bytes with MEMSET.
   memset(&buf[length], padLen, padLen);
}