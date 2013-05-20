//
//  loadbmp.cpp
//  Assignment1
//
//  Created by Pamela Martinez on 4/13/13.
//
//

#include "loadbmp.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <string>


using namespace std;

// Actual RGB data
unsigned char* data;
unsigned int imageSize;   // = width*height*3
unsigned int width, height;

loadbmp::loadbmp(string filename)
{
	Parse(filename);
}

void loadbmp::Parse(string filename)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
  
    
    FILE * file = fopen(filename.c_str(),"rb");
    if (!file){
        printf("Image could not be opened\n");
        exit(1);
    }
    
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        exit(1);
    }
    
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        exit(1);
    }   
    
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);
}


unsigned int loadbmp::getImageSize(){
    return imageSize;
}

unsigned int loadbmp::getWidth(){
    return width;
}

unsigned int loadbmp::getHeight(){
    return height;
}

unsigned char* loadbmp::getData(){
    return data;
}