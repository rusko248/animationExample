//
//  obj.cpp
//  Metamorphosis
//
//  Created by Pamela Martinez on 4/7/13.
//
//

#include "obj.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <string>


using namespace std;

obj::obj(std::string objFilename)
{
    objMesh = new mesh();
    vertexNormals = 0;
	Parse(objFilename);
}


void obj::Parse(std::string objFilename)
{
    
	std::ifstream objFile(objFilename.c_str());
    
	// Die if we couldn't find the file
	if (objFile.fail())
	{
		printf("obj::Parse - Could not find input obj file '%s'\n", objFilename.c_str());
		exit(1);
	}
    
	char line[1024];
	while (!objFile.eof())
	{
		objFile.getline(line, 1023);
		std::stringstream ss;
		ss.str(line);
		std::string command;
		ss >> command;
        
        
		if (command == "v")
		{
			float vx, vy, vz;
			ss >> vx >> vy >> vz;
            objMesh->addVertex(vx, vy, vz);
		}
        
        if (command == "vt")
        {
            float vt1, vt2;
            ss >> vt1 >> vt2;
            objMesh->addVertex_t(vt1, vt2);
        }
        
        
        if (command == "vn")
        {
            float vn1, vn2, vn3;
            ss >> vn1 >> vn2 >> vn3;
            objMesh->addVertex_n(vn1, vn2, vn3);
            vertexNormals++;
        }
        
        if (command == "f")
        {
            int v1a, v2a, v3a;
            int v1b, v2b, v3b;
            int v1c, v2c, v3c;
            string lin;
            lin = string(line);
           
            //finds v1a v2a v3a
            int ind = lin.find('/');
            if (ind == string::npos){
                int v1, v2, v3;
                ss >> v1 >> v2 >> v3;
                objMesh->addFace(v1, v2, v3);
            }
            else {
                string temp = lin.substr(2, ind-2);
                v1a = atoi(temp.c_str());
                
                lin = lin.substr(ind+1);
                ind = lin.find('/');
                temp = lin.substr(0, ind);
                v2a = atoi(temp.c_str());
                
                lin = lin.substr(ind+1);
                ind = lin.find(' ');
                temp = lin.substr(0, ind);
                v3a = atoi(temp.c_str());
                
                
                //finds v1b v2b v3b
                lin = lin.substr(ind+1);
                ind = lin.find('/');
                temp = lin.substr(0, ind);
                v1b = atoi(temp.c_str());
                
                lin = lin.substr(ind+1);
                ind = lin.find('/');
                temp = lin.substr(0, ind);
                v2b = atoi(temp.c_str());
                
                lin = lin.substr(ind+1);
                ind = lin.find(' ');
                temp = lin.substr(0, ind);
                v3b = atoi(temp.c_str());
                
                
                //finds v1c v2c v3c
                lin = lin.substr(ind+1);
                ind = lin.find('/');
                temp = lin.substr(0, ind);
                v1c = atoi(temp.c_str());
                
                lin = lin.substr(ind+1);
                ind = lin.find('/');
                temp = lin.substr(0, ind);
                v2c = atoi(temp.c_str());
                
                lin = lin.substr(ind+1);
                ind = lin.find(' ');
                temp = lin.substr(0, ind);
                v3c = atoi(temp.c_str());
                
                objMesh->addFace(v1a, v1b, v1c);
                objMesh->addFace_vt(v2a, v2b, v2c);
                objMesh->addFace_vn(v3a, v3b, v3c);
            }
        }
	}
	objFile.close();
    
    FinishedParsing();
}



void obj::FinishedParsing()
{
    //this means we had no vertex Normal info, and have to define our own
    if (vertexNormals == 0) {
        
       // printf("we're in trouble");
        
        vector<STPoint3>* vertices = objMesh->getVertices();        
        vector<STPoint3>* faces = objMesh->getFaces();
        
        //creates the normals for vertices
        objMesh->makeNormals();
        
        //saves the face normal for each vertex
        for (int i=0; i < faces->size(); i++){
            STPoint3 fac = faces->at(i);
            
            STPoint3 v1 = vertices->at((int)fac.x);
            STPoint3 v2 = vertices->at((int)fac.y);
            STPoint3 v3 = vertices->at((int)fac.z);
            
            STVector3 faceNormal = STVector3::Cross(STVector3(v1-v2), STVector3(v1-v3));
            faceNormal.Normalize();
            
            objMesh->addFaceNormal(faceNormal, (int)fac.x);
            objMesh->addFaceNormal(faceNormal, (int)fac.y);
            objMesh->addFaceNormal(faceNormal, (int)fac.z);
            
            objMesh->addFace_vn((int)fac.x+1, (int)fac.y+1, (int)fac.z+1);
        }
        
        //goes through each vertex and averages its face normals, to calculate vertex normals
        for (int i=0; i < vertices->size(); i++){
            vector<STVector3>* vNormals = objMesh->getVNormals(i);
            
            STVector3 vertexNormal = STVector3(0, 0, 0);
            int totalVN = vNormals->size();
            for (int j=0; j < totalVN; j++){
                vertexNormal += vNormals->at(j);
            }
            
            vertexNormal = vertexNormal/totalVN;
            objMesh->addVertex_n(vertexNormal.x, vertexNormal.y, vertexNormal.z);
        }
    }
    
}


mesh* obj::getMesh(){
    return objMesh;
}
