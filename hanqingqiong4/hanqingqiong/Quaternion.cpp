#include "pch.h"
#include "Quaternion.h"
#include <math.h>

#define CLAMP(x , min , max) ((x) > (max) ? (max) : ((x) < (min) ? (min) : x))

Quaternion::Quaternion(void)
{
	qw=0;
	qx=0;
	qy=0;
	qz=0;
}


Quaternion::~Quaternion(void)
{
}

void Quaternion::SetEulerAngle(float x_rotate, float y_rotate, float z_rotate)
{
	x_rotate = x_rotate/180.0f*3.14159f;
	y_rotate = y_rotate/180.0f*3.14159f;
	z_rotate = z_rotate/180.0f*3.14159f;

	float fCosZ = (float)cos(z_rotate * .5f);
	float fSinZ = (float)sin(z_rotate * .5f);
	float fCosX = (float)cos(x_rotate * .5f);
	float fSinX = (float)sin(x_rotate * .5f);
	float fCosY = (float)cos(y_rotate * .5f);
	float fSinY = (float)sin(y_rotate * .5f);

	qw = fCosZ * fCosX * fCosY + fSinZ * fSinX * fSinY;
	qx = fCosZ * fSinX * fCosY - fSinZ * fCosX * fSinY;
	qy = fCosZ * fCosX * fSinY + fSinZ * fSinX * fCosY;
	qz = fSinZ * fCosX * fCosY - fCosZ * fSinX * fSinY;
}


 void Quaternion::GetEulerAngle(float& x_rotate, float& y_rotate, float& z_rotate) 
{
	x_rotate  = (float)atan2(2 * (qw * qx + qz * qy) , 1 - 2 * (qy * qy + qx * qx));
	y_rotate  = (float)asin(CLAMP(2 * (qw * qy - qx * qz) , -1.0f , 1.0f));
	z_rotate  = (float)atan2(2 * (qw * qz + qx * qy) , 1 - 2 * (qz * qz + qy * qy));

	x_rotate = x_rotate/3.14159f*180.0f;
	y_rotate = y_rotate/3.14159f*180.0f;
	z_rotate = z_rotate/3.14159f*180.0f;
}