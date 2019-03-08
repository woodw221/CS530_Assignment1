#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

//Attributed to LibCRC.ORG, Lammert Bies, Nov. 27 2016
unsigned int crc_tab32[256];

int init_crc32_tab(){
    unsigned int i;
    unsigned int j;
    unsigned int crc;

    for( i=0; i<256; i++){

        crc = i;

        for (j=0; j<8; j++){

            if(  crc & 0x00000001L ){
                crc = (crc >>1) ^ 0xEDB88320L;
            }

            else{
                crc >>=1;
            }
            
            crc_tab32[i] = crc;
        }

    }
    return 0;
}

unsigned int generateChecksum(char *name){
    
    FILE *fptr = fopen(name, "rb");
    if (fptr == NULL){
        perror("Error Opening file");
        return(-1);
    }
    fseek(fptr, 0, SEEK_END);

    int length = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    
    unsigned long reg = -1;
    unsigned long tmp;
    unsigned long tmp2;
    unsigned char buffer[length + 2];
    
    fread(buffer, length, 1, fptr);
    
    //Calculate offset
    char phoff[8];
    for(int i = 0; i<8; i++){
        
        phoff[i] = buffer[i+32];
        
    }
 
    int offset = phoff[0];
    for(int i=1; i <8; i++){
        offset = offset | phoff[i] << 8;
    }
    //Calculate phsize
    char phsize[2];
    for(int i = 0; i<2; i++){
        phsize[i] = buffer[i+54];
    }

    int phoffset = phsize[0];
    phoffset = phoffset | phsize[1] << 8;

    //Calculate phnum
    char phnum[2];
    for(int i = 0; i<2; i++){
        phnum[i] = buffer[i+56];
    }

    int phnumOffset = phnum[0];
    phnumOffset = phnumOffset | phnum[1] << 8;

    //Copy Header
    int totalBytes = phnumOffset * phoffset;
    char header[totalBytes];

    for(int i = 0; i<totalBytes; i++){
        header[i] = buffer[i+offset];
    }

    const unsigned char *c = header;    
    
    //Calculate the crc32 checksum
    for(int a = 0; a<totalBytes; a++){
        
        tmp = ( reg >> 8 ) & 0x00FFFFFFL;
        tmp2 = crc_tab32[ ((int) reg ^ *c++) & 0xff];
        reg = tmp ^ tmp2;

    }   

    return reg ^= 0xffffffffL;
}

int main(int agrc, char* argv[]){
    printf("Assignment #1-2, William Wood, woodw221@gmail.com\n");

    init_crc32_tab();

    printf("%08X\n",generateChecksum(argv[1])); 
   
}
