#ifndef __TATMATH_H__
#define __TATMATH_H__

//#pragma warning (disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>


using namespace std;

#define PI 3.141593f
#define IMG_TYPE_NA		-1
#define IMG_TYPE_BMP	0
#define IMG_TYPE_TGA	1

struct TATRGB {
	unsigned char    rgbBlue;
	unsigned char    rgbGreen;
	unsigned char    rgbRed;
	unsigned char    rgbReserved;
};

class TATVector2
{
public:
	float X;
	float Y;

	TATVector2();
	TATVector2(float x, float y);
	TATVector2(const TATVector2& vector);
	~TATVector2();

	void Set(float x, float y);
	float GetModule();
	TATVector2& ScaleMe(float scale);
	TATVector2 ScaleTo(float scale);
	TATVector2& ToUnit();

	TATVector2& operator = (const TATVector2& vector);
	float operator * (const TATVector2& vector);
	TATVector2 operator + (const TATVector2& vector);
	TATVector2 operator - (const TATVector2& vector);
	bool operator == (const TATVector2& vector);
	bool operator != (const TATVector2& vector);
};
class TATVector
{
// Propeties:
public:
	float X;
	float Y;
	float Z;
// Methods
public:
	TATVector();
	TATVector(float x, float y, float z);
	TATVector(const TATVector& vector);
	~TATVector();

	void Set(float x, float y, float z);
	float GetModule();
	TATVector& ScaleMe(float scale);
	TATVector ScaleTo(float scale);
	TATVector& ToUnit();
	
	TATVector& operator = (const TATVector& vector);
	float operator * (const TATVector& vector);
	TATVector operator ^ (const TATVector& vector);
	TATVector operator + (const TATVector& vector);
	TATVector operator - (const TATVector& vector);
	bool operator == (const TATVector& vector);
	bool operator != (const TATVector& vector);

	void ReadSTL(FILE* stlFile);
	void WriteSTL(FILE* stlFile);

};
class TATHomoMatrix
{
// Properties:
private:
	TATVector N;
	TATVector S;
	TATVector A;
	TATVector P;
// Methods:
public:
	TATHomoMatrix();
	TATHomoMatrix(const TATHomoMatrix& matrix);
	~TATHomoMatrix();

	TATVector* GetN();
	void SetN(TATVector n);
	void SetN(float x, float y, float z);
	TATVector* GetS();
	void SetS(TATVector s);
	void SetS(float x, float y, float z);
	TATVector* GetA();
	void SetA(TATVector a);
	void SetA(float x, float y, float z);
	TATVector* GetP();
	void SetP(TATVector p);
	void SetP(float x, float y, float z);

	TATHomoMatrix& operator = (const TATHomoMatrix& matrix);
	TATHomoMatrix operator * (const TATHomoMatrix& matrix);
	TATVector operator * (const TATVector& vector);

	void Reset();
	TATHomoMatrix& Invert();
	TATHomoMatrix& ToIdentity();
	void ToArray(float m[16]);

};
class TATMatrixTransX : public TATHomoMatrix
{
private:
	float Translate;
public:
	TATMatrixTransX();
	TATMatrixTransX(float translate);
	TATMatrixTransX(const TATMatrixTransX& matrix);
	float GetTranslate();
	void SetTranslate(float translate);

};
class TATMatrixTransY : public TATHomoMatrix
{
private:
	float Translate;
public:
	TATMatrixTransY();
	TATMatrixTransY(float translate);
	TATMatrixTransY(const TATMatrixTransY& matrix);
	float GetTranslate();
	void SetTranslate(float translate);

};
class TATMatrixTransZ : public TATHomoMatrix
{
private:
	float Translate;
public:
	TATMatrixTransZ();
	TATMatrixTransZ(float translate);
	TATMatrixTransZ(const TATMatrixTransZ& matrix);
	float GetTranslate();
	void SetTranslate(float translate);

};
class TATMatrixRotX : public TATHomoMatrix
{
private:
	float Rotate;
public:
	TATMatrixRotX();
	TATMatrixRotX(float rotate);
	TATMatrixRotX(const TATMatrixRotX& matrix);
	float GetRotate();
	void SetRotate(float rotate);
};
class TATMatrixRotY : public TATHomoMatrix
{
private:
	float Rotate;
public:
	TATMatrixRotY();
	TATMatrixRotY(float rotate);
	TATMatrixRotY(const TATMatrixRotY& matrix);
	float GetRotate();
	void SetRotate(float rotate);
	
};
class TATMatrixRotZ : public TATHomoMatrix
{
private:
	float Rotate;
public:
	TATMatrixRotZ();
	TATMatrixRotZ(float rotate);
	TATMatrixRotZ(const TATMatrixRotZ& matrix);
	float GetRotate();
	void SetRotate(float rotate);

};
class TATImage
{
private:
	unsigned char *PixelData;
	int Width;
	int Height;
	TATRGB *Colours;

public:
	TATImage();
	~TATImage();
	int SpecifyImageType(char* fileName);
	bool LoadBMPFile(char* fileName);
	bool LoadBMPFileResize(char* fileName);
	bool LoadUncompTGAFile(char* fileName);
	bool Load(char* fileName);

	int GetWidth();
	int GetHeight();
	unsigned char* GetPixelData();

};
namespace TAT
{
	void GetFileName(char* path, char* fileName);
	void GetFolderPath(char* path, char* folderPath);
	void GetFileExtension(char* path, char* ext);

	unsigned int RGB2UI(unsigned char red, unsigned char green, unsigned char blue);
	void UI2RGB(unsigned int rgb, unsigned char& red, unsigned char& green, unsigned char& blue);

	float DetMatrix33(TATVector& v1, TATVector& v2, TATVector& v3);
	
	void Perspective(float fovy, float aspect, float zNear, float zFar, float* matrix);
	void Ortho(float left, float right, float bottom, float top, float zNear, float zFar, float* matrix);

	void TranslateO(TATHomoMatrix& matrix, TATVector& u);
	void TranslateO(TATHomoMatrix& matrix, float x, float y, float z);

	void RotateXO(TATHomoMatrix& matrix, float angle);
	void RotateYO(TATHomoMatrix& matrix, float angle);
	void RotateZO(TATHomoMatrix& matrix, float angle);

	void RotateXW(TATHomoMatrix& matrix, float angle);
	void RotateYW(TATHomoMatrix& matrix, float angle);
	void RotateZW(TATHomoMatrix& matrix, float angle);
	void RotateW(TATHomoMatrix& matrix, float angle, TATVector& u);
	
};

#endif
