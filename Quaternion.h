//
//  Quaternion.h
//  Assignment3
//
//  Created by Pamela Martinez on 4/25/13.
//
//

#ifndef __Assignment3__Quaternion__
#define __Assignment3__Quaternion__

#include <iostream>
#include "st.h"


class Quaternion
{
    
public:
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    Quaternion(float w, const STVector3 & xyz);
    Quaternion identity();
    Quaternion conjugate( const Quaternion & quat );
    //Quaternion conjugate();

    
    Quaternion SLERP(float t, const Quaternion & unitQuat0, const Quaternion & unitQuat1);
    
    //operators
    Quaternion & operator =( const Quaternion & quat );
    const Quaternion operator +( const Quaternion & quat ) const;
    const Quaternion operator -( const Quaternion & quat ) const;
    const Quaternion operator *( float scalar ) const;
    Quaternion operator* (const Quaternion &rq) const;
    const Quaternion operator /( float scalar ) const;
    const Quaternion operator -( ) const;

    float dot( const Quaternion & quat0, const Quaternion & quat1 );
    Quaternion cross( const Quaternion & quat0, const Quaternion & quat1 );
    
    float norm( const Quaternion & quat );
    const Quaternion normalize( const Quaternion & quat );
    
    //rotations
    const Quaternion rotation( float radians, const STVector3 & vec );
    const Quaternion rotationX( float radians );
    const Quaternion rotationY( float radians );
    const Quaternion rotationZ( float radians );
    const STVector3 rotate( const Quaternion & quat, const STVector3 & vec );
                       
    void print( const Quaternion & quat );
    
    
    //gets values
    float getW() const;
    float getX() const;
    float getY() const;
    float getZ() const;
    
    float* getMatrix() const;

    
private:
    float qw, qx, qy, qz;
    
};
#endif /* defined(__Assignment3__Quaternion__) */
