//
//  obj.h
//  Metamorphosis
//
//  Created by Pamela Martinez on 4/7/13.
//
//

#ifndef __Metamorphosis__obj__
#define __Metamorphosis__obj__

#include <iostream>
#include "st.h"
#include "mesh.h"


using namespace std;

class obj
{
    
public:
	obj(std::string sceneFilename);
    mesh* getMesh();

    
private:
    
	/** Parsing helpers **/
	void Parse(std::string sceneFilename);
	void FinishedParsing();
    
    /**stores the mesh**/
    mesh* objMesh;
    int vertexNormals;
    
};


#endif /* defined(__Metamorphosis__obj__) */
