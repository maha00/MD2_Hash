
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"

// Reading the input from the given path.
const uint8_t *read_file(const char *path, size_t *length)
{
   FILE *fp;
   // Checking if given file is present.
   if (!(fp = fopen(path, "rb")))
   {
      perror("file does not exist, please provide a valid file name");
      goto end;
   }
   // Returns 0 if successful, reads from end of the file: move file pointer to specific location
   fseek(fp, 0, SEEK_END);
   // fseek used to get the length of the file Fsize is the size of the given input.
   size_t fsize = ftell(fp);
   *length = fsize;
   // Now points to beginning of file.
   fseek(fp, 0, SEEK_SET);
   // TODO malloc tests
   uint8_t *stringBuf = malloc(fsize + 1);
   //Fehler.
   if(stringBuf == NULL){
      printf("Memory Allocation failed.\n");
      return NULL;
   } 
   // allocate data into memory starting from the pointer's position in file
   int w = fread(stringBuf, fsize, 1, fp);
   w--;
end:
   if (fp)
      fclose(fp);
   return stringBuf;
}
