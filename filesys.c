/* filesys.c
 * 
 * provides interface to virtual disk
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "filesys.h"


diskblock_t  virtualDisk [MAXBLOCKS] ;           // define our in-memory virtual, with MAXBLOCKS blocks
fatentry_t   FAT         [MAXBLOCKS] ;           // define a file allocation table with MAXBLOCKS 16-bit entries
fatentry_t   rootDirIndex            = 0 ;       // rootDir will be set by format
direntry_t * currentDir              = NULL ;
fatentry_t   currentDirIndex         = 0 ;

/* writedisk : writes virtual disk out to physical disk
 * 
 * in: file name of stored virtual disk
 */

void writedisk ( const char * filename )
{
   printf ( "writedisk> virtualdisk[0] = %s\n", virtualDisk[0].data ) ;
   FILE * dest = fopen( filename, "w" ) ;
   if ( fwrite ( virtualDisk, sizeof(virtualDisk), 1, dest ) < 0 )
      fprintf ( stderr, "write virtual disk to disk failed\n" ) ;
   //write( dest, virtualDisk, sizeof(virtualDisk) ) ;
   fclose(dest) ;
   
}

void readdisk ( const char * filename )
{
   FILE * dest = fopen( filename, "r" ) ;
   if ( fread ( virtualDisk, sizeof(virtualDisk), 1, dest ) < 0 )
      fprintf ( stderr, "write virtual disk to disk failed\n" ) ;
   //write( dest, virtualDisk, sizeof(virtualDisk) ) ;
      fclose(dest) ;
}


// the basic interface to the virtual disk this moves memory around

void writeblock ( diskblock_t * block, int block_address )
{
   //printf ( "writeblock> block %d = %s\n", block_address, block->data ) ;
   memmove ( virtualDisk[block_address].data, block->data, BLOCKSIZE ) ;
   //printf ( "writeblock> virtualdisk[%d] = %s / %d\n", block_address, virtualDisk[block_address].data, (int)virtualDisk[block_address].data ) ;
}


void copy_fat()
{
   diskblock_t block;
   int x;
   int y;
   int fatblocksneeded = (MAXBLOCKS / FATENTRYCOUNT);

   for (x = 0; x < fatblocksneeded; x++)
   {
      for (y = 0; y < FATENTRYCOUNT; y++)
      {
         block.fat[y] = FAT[((x*FATENTRYCOUNT)+y)];
      }
      writeblock(&block, x + 1);
   }
}


// Implementation of format function
void format()
{
   diskblock_t block ;
   direntry_t  rootDir ;
   int         pos             = 0 ;
   int         fatentry        = 0 ;
   int         fatblocksneeded =  (MAXBLOCKS / FATENTRYCOUNT ) ;

   // Prepare FAT Table
   // Preparing block 0, then filled with '\0'

   for (int i = 0; i < BLOCKSIZE; i++ ) block.data[i] = '\0';
      for (int i = 0; i < BLOCKSIZE; i++)
      {
         block.data[i] = '\0';
      }

      strcpy(block.data, "CS3026 Operating Systems Assessment");  
      writeblock(&block, 0);

      FAT[0] = ENDOFCHAIN;
      FAT[1] = 2;
      FAT[2] = ENDOFCHAIN;
      FAT[3] = ENDOFCHAIN;
      

      for(int i =4; i < MAXBLOCKS; i++){
         FAT[i] = UNUSED;
      }

      copy_fat(); // Copy contents of fat arry and write it to the virtual disk

      for(int i = 0; i < BLOCKSIZE; i++)
      {
         block.data[i] = '\0';
      }


      // Prepare root directory
      block.dir.isdir = 1;
      block.dir.nextEntry = 0;
      writeblock(&block, fatblocksneeded + 1);

      rootDirIndex = fatblocksneeded + 1;
      currentDirIndex = rootDirIndex;


      
}


/* use this for testing
 */
void printBlock ( int blockIndex )
{
   printf ( "virtualdisk[%d] = %s\n", blockIndex, virtualDisk[blockIndex].data ) ;
}

