#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

int generateOffset(char offsetArray[]){
    int length = strlen(offsetArray);
    int offset = offsetArray[0];
    for(int i = 1; i<length; i++){
        offset = offset | offsetArray[i] << 8;
    }
    return offset;
}

unsigned int generateChecksum(char *name, char *secName){
    
    FILE *fptr = fopen(name, "rb");
    if (fptr == NULL){
        perror("Error Opening file");
        return(-1);
    }

    fseek(fptr, 0, SEEK_END);

    int length = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    unsigned char buffer[length + 2];
    
    fread(buffer, length+1, length+1, fptr);

    //Calculate Section Offset
    char sectionOffset[8];
    for(int i = 0; i<8; i++){
        sectionOffset[i] = buffer[i+40];
    }
    int shoffset = generateOffset(sectionOffset); 


    //Calculate shsize
    char sectionSizebuff[2];
    for(int i = 0; i<2; i++){
        sectionSizebuff[i] = buffer[i+58];
    }

    int sectionSize = generateOffset(sectionSizebuff);

    //Calculate shnum
    char sectionNumbuff[2];
    for(int i = 0; i<2; i++){
        sectionNumbuff[i] = buffer[i+60];
    }

    int sectionNum = generateOffset(sectionNumbuff);

    //Calculate shstrndx
    char sectionIndexbuff[2];
    for(int i = 0; i<2; i++){
        sectionIndexbuff[i] = buffer[i+62];
    }

    int sectionIndex = generateOffset(sectionIndexbuff);

    //Calculate strtableLocation
    int strtableLocation = shoffset + (sectionSize * sectionIndex);
    strtableLocation +=24;

    unsigned char strTable[8];
    for( int i = 0; i<8; i++){
        strTable[i] = buffer[strtableLocation+i];
    }
    int strLoc = generateOffset(strTable);
    
    char *tableName, *inputName;
    unsigned char strTableindex[4];
    int k = 0;
    int tableOffset;
    int nullInput;
    //Check Name
    for(int i = 0; i<sectionNum; i++){

        inputName = secName;
        
        for(int a = 0; a<4; a++){
            strTableindex[a] = buffer[shoffset+ a + k];
        }
        //Find Position
        tableOffset = generateOffset(strTableindex);
        
        /*Compare Names then change our value as needed
         *If Length of section is 0 it should not print anything
         */
        tableName = &buffer[strLoc+tableOffset];
       
        int matched = strcmp(tableName, inputName);
        if(strlen(inputName) == 0){
            matched = 1;
        }
        //Calulate the CRC32
        if(matched==0){
            unsigned int reg = -1;
            unsigned int tmp;
            unsigned int tmp2; 
            
            unsigned char crcTemp[64];
            for(int a = 0; a<64; a++){
                crcTemp[a] = buffer[shoffset+ a + k];
            }
            const unsigned char *c = crcTemp;
               
    
    
            for(int a = 0; a<64; a++){
        
            tmp = ( reg >> 8 ) & 0x00FFFFFFL;
            tmp2 = crc_tab32[ ((int) reg ^ *c++) & 0xff];
            reg = tmp ^ tmp2;

            }   

            
            printf("%08X\n", reg ^= 0xffffffffL);
        }
        k = k + 64;
    }
    
    return 0;
    
}

int main(int argc, char* argv[]){
    printf("Assignment #1-3, William Wood, woodw221@gmail.com\n");
    
    if(argv[2] == '\0'){
    printf("Error no section inputted.\n");
    return 1;
    }
    init_crc32_tab();

    generateChecksum(argv[1], argv[2]);
}
