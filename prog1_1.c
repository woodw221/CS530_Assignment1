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
    
    fread(buffer, length+1, length+1, fptr);

    const unsigned char *c = buffer;    
    
    for(int a = 0; a<length; a++){
        
        tmp = ( reg >> 8 ) & 0x00FFFFFFL;
        tmp2 = crc_tab32[ ((int) reg ^ *c++) & 0xff];
        reg = tmp ^ tmp2;

    }   

    return reg ^= 0xffffffffL;
}

int main(int argc, char* argv[]){
    printf("Assignment #1-1, William Wood, woodw221@gmail.com\n");

    init_crc32_tab();

    printf("%08X\n",generateChecksum(argv[1]));
}
