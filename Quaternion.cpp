//  Referenced Sony source code given referenced in handout:
//   Copyright (C) 2006, 2007 Sony Computer Entertainment Inc.
//   All rights reserved.

//  Quaternion.cpp
//  Assignment3
//
//  Created by Pamela Martinez on 4/25/13.
//
//

#include "Quaternion.h"

using namespace std;

Quaternion::Quaternion(){
    qw = 0;
    qx = 0;
    qy = 0;
    qz = 0;
}

Quaternion::Quaternion(float w, float x, float y, float z){
    qw = w;
    qx = x;
    qy = y;
    qz = z;
}

Quaternion::Quaternion(float w, const STVector3 & xyz)
{
    qw = w;
    qx = xyz.x;
    qy = xyz.y;
    qz = xyz.z;
}

Quaternion Quaternion::identity(){
    return Quaternion(1, 0, 0, 0);
}


Quaternion Quaternion::conjugate(const Quaternion & quat){
    return Quaternion( quat.getW(), -quat.getX(), -quat.getY(), -quat.getZ());
}

Quaternion Quaternion::SLERP(float t, const Quaternion & unitQuat0, const Quaternion & unitQuat1){
    float threshhold = .0001f;
    Quaternion start;
    float recipSinAngle, scale0, scale1, cosAngle, angle;
    cosAngle = dot( unitQuat0, unitQuat1 );
    
    if ( cosAngle < 0.0f ) {
        cosAngle = -cosAngle;
        start = ( -unitQuat0 );
    } else {
        start = unitQuat0;
    }
    
    if ( cosAngle < threshhold) {
        angle = acosf( cosAngle );
        recipSinAngle = ( 1.0f / sinf( angle ) );
        scale0 = ( sinf( ( ( 1.0f - t ) * angle ) ) * recipSinAngle );
        scale1 = ( sinf( ( t * angle ) ) * recipSinAngle );
    } else {
        scale0 = ( 1.0f - t );
        scale1 = t;
    }
    return ( ( start * scale0 ) + ( unitQuat1 * scale1 ) );
}


Quaternion & Quaternion::operator =( const Quaternion & quat ){
    qw = quat.getW();
    qx = quat.getX();
    qy = quat.getY();
    qz = quat.getZ();
    return *this;
}


const Quaternion Quaternion::operator +( const Quaternion & quat ) const{
    return Quaternion(qw + quat.getW(), qx + quat.getX(), qy + quat.getY(), qz + quat.getZ());
}


const Quaternion Quaternion::operator -( const Quaternion & quat ) const{
    return Quaternion(qw - quat.getW(), qx - quat.getX(), qy - quat.getY(), qz - quat.getZ());
}


const Quaternion Quaternion::operator *( float scalar ) const{
    return Quaternion(qw * scalar, qx * scalar, qy * scalar, qz * scalar);
}

// Multiplying q1 with q2 applies the rotation q2 to q1
Quaternion Quaternion::operator* (const Quaternion &rq) const
{
	// the constructor takes its arguments as (x, y, z, w)
	return Quaternion(qw * rq.getW() - qx * rq.getX() - qy * rq.getY() - qz * rq.getZ(),
                      qw * rq.getX() + qx * rq.getW() + qy * rq.getZ() - qz * rq.getY(),
	                  qw * rq.getY() + qy * rq.getW() + qz * rq.getX() - qx * rq.getZ(),
	                  qw * rq.getZ() + qz * rq.getW() + qx * rq.getY() - qy * rq.getX());
}


const Quaternion Quaternion::operator /( float scalar ) const{
    return Quaternion(qw / scalar, qx / scalar, qy / scalar, qz / scalar);

}


const Quaternion Quaternion::operator -( ) const{
    return Quaternion(-qw, -qx, -qy, -qz);
}


float Quaternion::dot( const Quaternion & quat0, const Quaternion & quat1 ){
    float result;
    result = ( quat0.getW() * quat1.getW() );
    result = ( result + ( quat0.getX() * quat1.getX() ) );
    result = ( result + ( quat0.getY() * quat1.getY() ) );
    result = ( result + ( quat0.getZ() * quat1.getZ() ) );
    return result;
}

Quaternion Quaternion::cross( const Quaternion & quat0, const Quaternion & quat1 ){
    return Quaternion(
                      quat0.getW()*quat1.getW() - quat0.getX()*quat1.getX() - quat0.getY()*quat1.getY() - quat0.getZ()*quat1.getZ(),
                      quat0.getW()*quat1.getX() + quat0.getX()*quat1.getW() + quat0.getY()*quat1.getZ() - quat0.getZ()*quat1.getY(),
                      quat0.getW()*quat1.getY() - quat0.getX()*quat1.getZ() + quat0.getY()*quat1.getW() + quat0.getZ()*quat1.getX(), quat0.getW()*quat1.getZ() + quat0.getX()*quat1.getY() - quat0.getY()*quat1.getX() + quat0.getZ()*quat1.getW()
                      );
}



float Quaternion::norm( const Quaternion & quat ){
    float result;
    result = ( quat.getX() * quat.getX() );
    result = ( result + ( quat.getY() * quat.getY() ) );
    result = ( result + ( quat.getZ() * quat.getZ() ) );
    result = ( result + ( quat.getW() * quat.getW() ) );
    return result;
}


const Quaternion Quaternion::normalize( const Quaternion & quat ){
    float lenSqr, lenInv;
    lenSqr = norm( quat );
    lenInv = ( 1.0f / sqrtf( lenSqr ) );
    return Quaternion(( quat.getW() * lenInv ), ( quat.getX() * lenInv ),( quat.getY() * lenInv ),( quat.getZ() * lenInv ));
}

const Quaternion Quaternion::rotation( float angle, const STVector3 & vec )
{
    STVector3 temp = STVector3(vec);
    temp.Normalize();
    
    float s, c;// angle;
   // angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return Quaternion( c, temp.x * s, temp.y * s, temp.z * s );
}


const Quaternion Quaternion::rotationX( float angle ){
    float s, c; //angle;
//    angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return Quaternion( c, s, 0.0f, 0.0f );
}


const Quaternion Quaternion::rotationY( float angle ){
    float s, c; // angle;
//    angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return Quaternion(c,  0.0f, s, 0.0f );
}


const Quaternion Quaternion::rotationZ( float angle ){
    float s, c;// angle;
//    angle = ( radians * 0.5f );
    s = sinf( angle );
    c = cosf( angle );
    return Quaternion( c, 0.0f, 0.0f, s );
}


const STVector3 rotate( const Quaternion & quat, const STVector3 & vec )
{
	STVector3 vn(vec);
	vn.Normalize();
    
	Quaternion vecQuat = Quaternion(0.0f, vn.x, vn.y, vn.z);

    Quaternion temp = Quaternion(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
	Quaternion resQuat = vecQuat * temp.conjugate(temp) ;
	resQuat = quat * resQuat;
    
	return (STVector3(resQuat.getX(), resQuat.getY(), resQuat.getZ()));
}


float Quaternion::getW() const{
    return qw;
}

float Quaternion::getX() const{
    return qx;
}

float Quaternion::getY() const{
    return qy;
}

float Quaternion::getZ() const{
    return qz;
}

void Quaternion::print( const Quaternion & quat ){
    
}

// Convert to Matrix
float* Quaternion::getMatrix() const
{
	float x2 = qx * qx;
	float y2 = qy * qy;
	float z2 = qz * qz;
	float xy = qx * qy;
	float xz = qx * qz;
	float yz = qy * qz;
	float wx = qw * qx;
	float wy = qw * qy;
	float wz = qw * qz;
    
    float* matrix4 = new float[16];
    
    
    matrix4[0]=1.0f - 2.0f * (y2 + z2);
    matrix4[1]= 2.0f * (xy - wz);
    matrix4[2] = 2.0f * (xz + wy);
    matrix4[3]= 0.0f;

    matrix4[4] = 2.0f * (xy + wz);
    matrix4[5] = 1.0f - 2.0f * (x2 + z2);
    matrix4[6] = 2.0f * (yz - wx);
    matrix4[7]= 0.0f;

    matrix4[8] = 2.0f * (xz - wy);
    matrix4[9] = 2.0f * (yz + wx);
    matrix4[10] = 1.0f - 2.0f * (x2 + y2);
    matrix4[11]= 0.0f;

    matrix4[12] = 0.0f;
    matrix4[13] = 0.0f;
    matrix4[14] = 0.0f;
    matrix4[15]= 1.0f;
    
//    printf("the elements are %f %f %f, %f %f %f, %f %f %f", matrix4[0], matrix4[1], matrix4[2], matrix4[3], matrix4[4], matrix4[5], matrix4[6], matrix4[7], matrix4[8]);
    
    return matrix4;
}
