//
//  CatmullRom.h
//  CatmullSpline
//
//  Created by Pamela Martinez on 4/24/13.
//
//

#ifndef __CatmullSpline__CatmullRom__
#define __CatmullSpline__CatmullRom__

#include <iostream>

#include <iostream>
#include "st.h"
#include <vector>

using namespace std;

class CatmullRom
{
    
public:
	CatmullRom();
    void readFile(string filename);
    
    void addControlPoint(float x, float y, float z);
    
    //finds point at given interval, where 0<=u<=1
    //the i interval has to be i<numControlPoints-1
    STPoint3 curveAt(float u, int i);
   
    //stores number of control points in catmull-rom
    int numControlPoints;

    
private:
    
    void newSegment(int i); //determines segment between point p(i) and p(i+1)
    vector<STPoint3>* controlPoints; //stores all vertices
    int segmentNum;
    STVector3 c0, c1, c2, c3;
};



#endif /* defined(__CatmullSpline__CatmullRom__) */
