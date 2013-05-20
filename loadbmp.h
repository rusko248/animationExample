//
//  loadbmp.h
//  Assignment1
//
//  Created by Pamela Martinez on 4/13/13.
//
//

#ifndef __Assignment1__loadbmp__
#define __Assignment1__loadbmp__

#include <iostream>
#include "st.h"
using namespace std;

class loadbmp
{
    
public:
	loadbmp(std::string filename);
    unsigned char* getData();
    unsigned int getImageSize();
    unsigned int getWidth();
    unsigned int getHeight();
    
private:
    void Parse(string filename);
    
};

#endif /* defined(__Assignment1__loadbmp__) */
