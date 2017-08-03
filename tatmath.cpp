//#pragma warning( disable : 4996 )

#include "tatmath.h"


// class TATVector2
TATVector2::TATVector2()
{
	X = 0.0f;
	Y = 0.0f;
}
TATVector2::TATVector2(float x, float y)
{
	X = x;
	Y = y;
}
TATVector2::TATVector2(const TATVector2& vector)
{
	X = vector.X;
	Y = vector.Y;
}
TATVector2::~TATVector2()
{

}
void TATVector2::Set(float x, float y)
{
	X = x;
	Y = y;
}
float TATVector2::GetModule()
{
	return sqrtf(X*X + Y*Y);
}
TATVector2& TATVector2::ScaleMe(float scale)
{
	X *= scale;
	Y *= scale;
	return *this;
}
TATVector2 TATVector2::ScaleTo(float scale)
{
	TATVector2 retVec = *this;
	retVec.X *= scale;
	retVec.Y *= scale;
	return retVec;
}
TATVector2& TATVector2::ToUnit()
{
	float module = GetModule();
	return ScaleMe(1.0f / module);
}
TATVector2& TATVector2::operator = (const TATVector2& vector)
{
	X = vector.X;
	Y = vector.Y;
	return *this;
}
float TATVector2::operator * (const TATVector2& vector)
{
	return (X*vector.X + Y*vector.Y);
}
TATVector2 TATVector2::operator + (const TATVector2& vector)
{
	TATVector2 retVec;
	retVec.X = X + vector.X;
	retVec.Y = Y + vector.Y;
	return retVec;
}
TATVector2 TATVector2::operator - (const TATVector2& vector)
{
	TATVector2 retVec;
	retVec.X = X - vector.X;
	retVec.Y = Y - vector.Y;
	return retVec;
}
bool TATVector2::operator == (const TATVector2& vector)
{
	if (X != vector.X)
		return false;
	if (Y != vector.Y)
		return false;
	return true;
}
bool TATVector2::operator != (const TATVector2& vector)
{
	if (X != vector.X)
		return true;
	if (Y != vector.Y)
		return true;
	return false;
}

// class TATVector
TATVector::TATVector()
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}
TATVector::TATVector(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}
TATVector::TATVector(const TATVector& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
}
TATVector::~TATVector()
{

}
void TATVector::Set(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}
float TATVector::GetModule()
{
	return sqrtf(X*X + Y*Y + Z*Z);
}
TATVector& TATVector::ScaleMe(float scale)
{
	X *= scale;
	Y *= scale;
	Z *= scale;
	return *this;
}
TATVector TATVector::ScaleTo(float scale)
{
	TATVector retVec = *this;
	retVec.X *= scale;
	retVec.Y *= scale;
	retVec.Z *= scale;
	return retVec;
}
TATVector& TATVector::ToUnit()
{
	float module = GetModule();
	ScaleMe(1.0f/module);
	return *this;
}
TATVector& TATVector::operator = (const TATVector& vector)
{
	X = vector.X;
	Y = vector.Y;
	Z = vector.Z;
	return *this;
}
float TATVector::operator * (const TATVector& vector)
{
	return (X*vector.X + Y*vector.Y + Z*vector.Z);
}
TATVector TATVector::operator ^ (const TATVector& vector)
{
	TATVector retVec;
	retVec.X = Y*vector.Z - Z*vector.Y;
	retVec.Y = Z*vector.X - X*vector.Z;
	retVec.Z = X*vector.Y - Y*vector.X;
	return retVec;
}
TATVector TATVector::operator + (const TATVector& vector)
{
	TATVector retVec;
	retVec.X = X + vector.X;
	retVec.Y = Y + vector.Y;
	retVec.Z = Z + vector.Z;
	return retVec;
}
TATVector TATVector::operator - (const TATVector& vector)
{
	TATVector retVec;
	retVec.X = X - vector.X;
	retVec.Y = Y - vector.Y;
	retVec.Z = Z - vector.Z;
	return retVec;
}
bool TATVector::operator == (const TATVector& vector)
{
	if( X != vector.X )
		return false;
	if( Y != vector.Y )
		return false;
	if( Z != vector.Z )
		return false;
	return true;
}
bool TATVector::operator != (const TATVector& vector)
{
	if( X != vector.X )
		return true;
	if( Y != vector.Y )
		return true;
	if( Z != vector.Z )
		return true;
	return false;
}
void TATVector::ReadSTL(FILE* stlFile)
{
	fread(this, sizeof(TATVector), 1, stlFile);
}
void TATVector::WriteSTL(FILE* stlFile)
{
	fwrite(this, sizeof(TATVector), 1, stlFile);
}

// class TATHomoMatrix
TATHomoMatrix::TATHomoMatrix()
{
	N = TATVector(1.0f, 0.0f, 0.0f);
	S = TATVector(0.0f, 1.0f, 0.0f);
	A = TATVector(0.0f, 0.0f, 1.0f);
	P = TATVector(0.0f, 0.0f, 0.0f);
}
TATHomoMatrix::TATHomoMatrix(const TATHomoMatrix& matrix)
{
	N = matrix.N;
	S = matrix.S;
	A = matrix.A;
	P = matrix.P;
}
TATHomoMatrix::~TATHomoMatrix()
{

}
TATVector* TATHomoMatrix::GetN()
{
	return &N;
}
void TATHomoMatrix::SetN(TATVector n)
{
	N = n;
}
void TATHomoMatrix::SetN(float x, float y, float z)
{
	N.Set(x, y, z);
}
TATVector* TATHomoMatrix::GetS()
{
	return &S;
}
void TATHomoMatrix::SetS(TATVector s)
{
	S = s;
}
void TATHomoMatrix::SetS(float x, float y, float z)
{
	S.Set(x, y, z);
}
TATVector* TATHomoMatrix::GetA()
{
	return &A;
}
void TATHomoMatrix::SetA(TATVector a)
{
	A = a;
}
void TATHomoMatrix::SetA(float x, float y, float z)
{
	A.Set(x, y, z);
}
TATVector* TATHomoMatrix::GetP()
{
	return &P;
}
void TATHomoMatrix::SetP(TATVector p)
{
	P = p;
}
void TATHomoMatrix::SetP(float x, float y, float z)
{
	P.Set(x, y, z);
}
TATHomoMatrix& TATHomoMatrix::operator = (const TATHomoMatrix& matrix)
{
	N = matrix.N;
	S = matrix.S;
	A = matrix.A;
	P = matrix.P;
	return *this;
}
TATHomoMatrix TATHomoMatrix::operator * (const TATHomoMatrix& matrix)
{
	TATHomoMatrix retMat;
	TATVector tempVec;
	// N
	TATVector nsap = matrix.N;
	tempVec.X = N.X*nsap.X + S.X*nsap.Y + A.X*nsap.Z;
	tempVec.Y = N.Y*nsap.X + S.Y*nsap.Y + A.Y*nsap.Z;
	tempVec.Z = N.Z*nsap.X + S.Z*nsap.Y + A.Z*nsap.Z;
	retMat.SetN(tempVec);
	// S
	nsap = matrix.S;
	tempVec.X = N.X*nsap.X + S.X*nsap.Y + A.X*nsap.Z;
	tempVec.Y = N.Y*nsap.X + S.Y*nsap.Y + A.Y*nsap.Z;
	tempVec.Z = N.Z*nsap.X + S.Z*nsap.Y + A.Z*nsap.Z;
	retMat.SetS(tempVec);
	// A
	nsap = matrix.A;
	tempVec.X = N.X*nsap.X + S.X*nsap.Y + A.X*nsap.Z;
	tempVec.Y = N.Y*nsap.X + S.Y*nsap.Y + A.Y*nsap.Z;
	tempVec.Z = N.Z*nsap.X + S.Z*nsap.Y + A.Z*nsap.Z;
	retMat.SetA(tempVec);
	// A
	nsap = matrix.P;
	tempVec.X = N.X*nsap.X + S.X*nsap.Y + A.X*nsap.Z + P.X;
	tempVec.Y = N.Y*nsap.X + S.Y*nsap.Y + A.Y*nsap.Z + P.Y;
	tempVec.Z = N.Z*nsap.X + S.Z*nsap.Y + A.Z*nsap.Z + P.Z;
	retMat.SetP(tempVec);

	return retMat;
}
TATVector TATHomoMatrix::operator * (const TATVector& vector)
{
	TATVector retVec;
	retVec.X = N.X*vector.X + S.X*vector.Y + A.X*vector.Z + P.X;
	retVec.Y = N.Y*vector.X + S.Y*vector.Y + A.Y*vector.Z + P.Y;
	retVec.Z = N.Z*vector.X + S.Z*vector.Y + A.Z*vector.Z + P.Z;
	return retVec;
}
void TATHomoMatrix::Reset()
{
	N = TATVector(1.0f, 0.0f, 0.0f);
	S = TATVector(0.0f, 1.0f, 0.0f);
	A = TATVector(0.0f, 0.0f, 1.0f);
	P = TATVector(0.0f, 0.0f, 0.0f);
}
TATHomoMatrix& TATHomoMatrix::Invert()
{
	float det_nsa = TAT::DetMatrix33(N, S, A);
	float scale = 1.0f / det_nsa;
	TATVector invN(S.Y*A.Z - S.Z*A.Y, A.Y*N.Z - A.Z*N.Y, N.Y*S.Z - N.Z*S.Y);
	invN.ScaleMe(scale);
	TATVector invS(S.Z*A.X - S.X*A.Z, A.Z*N.X - A.X*N.Z, N.Z*S.X - N.X*S.Z);
	invS.ScaleMe(scale);
	TATVector invA(S.X*A.Y - S.Y*A.X, A.X*N.Y - A.Y*N.X, N.X*S.Y - N.Y*S.X);
	invA.ScaleMe(scale);
	TATVector invP( -TAT::DetMatrix33(S, A, P),
					TAT::DetMatrix33(N, A, P),
					-TAT::DetMatrix33(N, S, P));
	invP.ScaleMe(scale);
	// ------------------
	N = invN;
	S = invS;
	A = invA;
	P = invP;
	return *this;
}
TATHomoMatrix& TATHomoMatrix::ToIdentity()
{
	N = TATVector(1.0f, 0.0f, 0.0f);
	S = TATVector(0.0f, 1.0f, 0.0f);
	A = TATVector(0.0f, 0.0f, 1.0f);
	P = TATVector(0.0f, 0.0f, 0.0f);
	return *this;
}
void TATHomoMatrix::ToArray(float m[16])
{
	m[0] = N.X;
	m[1] = N.Y;
	m[2] = N.Z;
	m[3] = 0.0f;
	//---------------
	m[4] = S.X;
	m[5] = S.Y;
	m[6] = S.Z;
	m[7] = 0.0f;
	//---------------
	m[8] = A.X;
	m[9] = A.Y;
	m[10] = A.Z;
	m[11] = 0.0f;
	//---------------
	m[12] = P.X;
	m[13] = P.Y;
	m[14] = P.Z;
	m[15] = 1.0f;
}

//class TATMatrixTransX
TATMatrixTransX::TATMatrixTransX() : TATHomoMatrix()
{
	Translate = 0.0f;
}
TATMatrixTransX::TATMatrixTransX(float translate) : TATHomoMatrix()
{
	Translate = translate;
	SetP( TATVector(translate, 0.0f, 0.0f) );
}
TATMatrixTransX::TATMatrixTransX(const TATMatrixTransX& matrix) : TATHomoMatrix(matrix)
{
	Translate = matrix.Translate;
}
float TATMatrixTransX::GetTranslate()
{
	return Translate;
}
void TATMatrixTransX::SetTranslate(float translate)
{
	Translate = translate;
}

//class TATMatrixTransY
TATMatrixTransY::TATMatrixTransY() : TATHomoMatrix()
{
	Translate = 0.0f;
}
TATMatrixTransY::TATMatrixTransY(float translate) : TATHomoMatrix()
{
	Translate = translate;
	SetP( TATVector(0.0f, translate, 0.0f) );
}
TATMatrixTransY::TATMatrixTransY(const TATMatrixTransY& matrix) : TATHomoMatrix(matrix)
{
	Translate = matrix.Translate;
}
float TATMatrixTransY::GetTranslate()
{
	return Translate;
}
void TATMatrixTransY::SetTranslate(float translate)
{
	Translate = translate;
}

//class TATMatrixTransZ
TATMatrixTransZ::TATMatrixTransZ() : TATHomoMatrix()
{
	Translate = 0.0f;
}
TATMatrixTransZ::TATMatrixTransZ(float translate) : TATHomoMatrix()
{
	Translate = translate;
	SetP( TATVector(0.0f, 0.0f, translate) );
}
TATMatrixTransZ::TATMatrixTransZ(const TATMatrixTransZ& matrix) : TATHomoMatrix(matrix)
{
	Translate = matrix.Translate;
}
float TATMatrixTransZ::GetTranslate()
{
	return Translate;
}
void TATMatrixTransZ::SetTranslate(float translate)
{
	Translate = translate;
}

//class TATMatrixRotX
TATMatrixRotX::TATMatrixRotX() : TATHomoMatrix()
{
	Rotate = 0.0f;
}
TATMatrixRotX::TATMatrixRotX(float rotate) : TATHomoMatrix()
{
	Rotate = rotate;
	float angle = rotate*PI/180.0f;
	SetS(TATVector(0.0f, cos(angle), sin(angle)));
	SetA(TATVector(0.0f, -sin(angle), cos(angle)));
}
TATMatrixRotX::TATMatrixRotX(const TATMatrixRotX& matrix) : TATHomoMatrix(matrix)
{
	Rotate = matrix.Rotate;
}
float TATMatrixRotX::GetRotate()
{
	return Rotate;
}
void TATMatrixRotX::SetRotate(float rotate)
{
	Rotate = rotate;
}

//class TATMatrixRotY
TATMatrixRotY::TATMatrixRotY() : TATHomoMatrix()
{
	Rotate = 0.0f;
}
TATMatrixRotY::TATMatrixRotY(float rotate) : TATHomoMatrix()
{
	Rotate = rotate;
	float angle = rotate*PI/180.0f;
	SetA(TATVector(sin(angle), 0.0f, cos(angle)));
	SetN(TATVector(cos(angle), 0.0f, -sin(angle)));
}
TATMatrixRotY::TATMatrixRotY(const TATMatrixRotY& matrix) : TATHomoMatrix(matrix)
{
	Rotate = matrix.Rotate;
}
float TATMatrixRotY::GetRotate()
{
	return Rotate;
}
void TATMatrixRotY::SetRotate(float rotate)
{
	Rotate = rotate;
}

//class TATMatrixRotZ
TATMatrixRotZ::TATMatrixRotZ() : TATHomoMatrix()
{
	Rotate = 0.0f;
}
TATMatrixRotZ::TATMatrixRotZ(float rotate) : TATHomoMatrix()
{
	Rotate = rotate;
	float angle = rotate*PI/180.0f;
	SetN(TATVector(cos(angle), sin(angle), 0.0f));
	SetS(TATVector(-sin(angle), cos(angle), 0.0f));
}
TATMatrixRotZ::TATMatrixRotZ(const TATMatrixRotZ& matrix) : TATHomoMatrix(matrix)
{
	Rotate = matrix.Rotate;
}
float TATMatrixRotZ::GetRotate()
{
	return Rotate;
}
void TATMatrixRotZ::SetRotate(float rotate)
{
	Rotate = rotate;
}

// class TATImage
TATImage::TATImage()
{
	PixelData = NULL;
	Width = 0;
	Height = 0;
}
TATImage::~TATImage()
{
	if (PixelData != NULL)
		delete[] PixelData;
}
int TATImage::SpecifyImageType(char* fileName)
{
	int len = strlen(fileName);
	char* ext = &fileName[len - 1];
	int idx = len - 1;
	while (idx >= 0)
	{
		if (*ext == '.')
			break;
		ext--;
		idx--;
	}

	if ((!strcmp(ext, ".bmp")) || (!strcmp(ext, ".BMP")))
	{
		return IMG_TYPE_BMP;
	}
	if ((!strcmp(ext, ".tga")) || (!strcmp(ext, ".TGA")))
	{
		return IMG_TYPE_TGA;
	}
	else
		return IMG_TYPE_NA;
}
bool TATImage::LoadBMPFile(char* fileName)
{
	FILE* fbmp = NULL;
	fbmp = fopen(fileName, "rb");
	if (fbmp == NULL)
		return false;

	unsigned short usRead;
	unsigned int ulRead;
	int lRead;

	unsigned int ulOffBits;
	unsigned int ulSize;
	unsigned short usBitCount;
	// Header
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&ulSize, sizeof(unsigned int), 1, fbmp);		// Size
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&ulOffBits, sizeof(unsigned int), 1, fbmp);	// Off Bits

														// Info
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&Width, sizeof(int), 1, fbmp);
	fread(&Height, sizeof(int), 1, fbmp);
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&usBitCount, sizeof(unsigned short), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&lRead, sizeof(int), 1, fbmp);
	fread(&lRead, sizeof(int), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);

	// Data
	unsigned short numColours = 1 << usBitCount;
	if (usBitCount == 8)
	{
		Colours = new TATRGB[numColours];
		fread(Colours, sizeof(TATRGB), numColours, fbmp);
	}
	unsigned long size = ulSize - ulOffBits;
	PixelData = new unsigned char[sizeof(unsigned char)*size];
	fread(PixelData, sizeof(unsigned char), size, fbmp);
	int we = size / 3;
	for (int i = 0; i < we; i++)
	{
		unsigned char tb = PixelData[3 * i];
		PixelData[3 * i] = PixelData[3 * i + 2];
		PixelData[3 * i + 2] = tb;
	}

	fclose(fbmp);
	return true;
}
bool TATImage::LoadBMPFileResize(char* fileName)
{
	FILE* fbmp = NULL;
	fbmp = fopen(fileName, "rb");
	if (fbmp == NULL)
		return false;

	unsigned short usRead;
	unsigned int ulRead;
	int lRead;

	unsigned int ulOffBits;
	unsigned int ulSize;
	unsigned short usBitCount;

	int imgWidth;
	int imgHeight;
	// Header
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&ulSize, sizeof(unsigned int), 1, fbmp);		// Size
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&ulOffBits, sizeof(unsigned int), 1, fbmp);	// Off Bits

														// Info
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&imgWidth, sizeof(int), 1, fbmp);
	fread(&imgHeight, sizeof(int), 1, fbmp);
	fread(&usRead, sizeof(unsigned short), 1, fbmp);
	fread(&usBitCount, sizeof(unsigned short), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&lRead, sizeof(int), 1, fbmp);
	fread(&lRead, sizeof(int), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);
	fread(&ulRead, sizeof(unsigned int), 1, fbmp);

	// Colour
	unsigned short numColours = 1 << usBitCount;
	if (usBitCount == 8)
	{
		Colours = new TATRGB[numColours];
		fread(Colours, sizeof(TATRGB), numColours, fbmp);
	}

	// Refine Width/Height
	int i = 0;
	int rWidth = 1;
	int rHeight = 1;
	while (rWidth < imgWidth)
	{
		rWidth *= 2;
	}
	Width = rWidth;
	while (rHeight < imgHeight)
	{
		rHeight *= 2;
	}
	Height = rHeight;
	// Create pixels
	PixelData = new unsigned char[sizeof(unsigned char)*Width*Height * 3];
	memset(PixelData, 0, Width*Height * 3);
	// Read pixels
	unsigned long size = ulSize - ulOffBits;
	unsigned char* pixels = new unsigned char[sizeof(unsigned char)*size];
	fread(pixels, sizeof(unsigned char), size, fbmp);
	// Swap BGR -> RGB
	int we = size / 3;
	for (int i = 0; i < we; i++)
	{
		unsigned char tb = pixels[3 * i];
		pixels[3 * i] = pixels[3 * i + 2];
		pixels[3 * i + 2] = tb;
	}

	// Apply to PixelData
	for (int i = 0; i < imgHeight; i++)
	{
		for (int j = 0; j < imgWidth; j++)
		{
			int iBP = 3 * (imgWidth*i + j);
			int iTP = 3 * (Width*i + j);
			PixelData[iTP] = pixels[iBP];
			PixelData[iTP + 1] = pixels[iBP + 1];
			PixelData[iTP + 2] = pixels[iBP + 2];
			/*PixelData[iTP] = 128;
			PixelData[iTP + 1] =0;
			PixelData[iTP + 2] = 0;*/
		}
	}
	/*for (int i = 0; i < imgWidth; i++)
	{
	for (int j = 0; j < imgHeight; j++)
	{
	int iBP = imgHeight*i + j;
	int iTP = Height*i + j;
	PixelData[3 * iTP] = pixels[3 * iBP];
	PixelData[3 * iTP + 1] = pixels[3 * iBP + 1];
	PixelData[3 * iTP + 2] = pixels[3 * iBP + 2];
	}
	}*/
	delete[] pixels;
	fclose(fbmp);
	return true;
}
bool TATImage::LoadUncompTGAFile(char* fileName)
{
	FILE *ftga = NULL;
	unsigned char typeCode;
	unsigned char ucharBad;
	unsigned char colorSwap;
	unsigned char bitCount;
	short sintBad;
	int size;
	int colorMode;

	// Open the TGA file.
	ftga = fopen(fileName, "rb");
	if (ftga == NULL)
	{
		return false;
	}

	// Info
	fread(&ucharBad, sizeof(unsigned char), 1, ftga);
	fread(&ucharBad, sizeof(unsigned char), 1, ftga);
	fread(&typeCode, sizeof(unsigned char), 1, ftga);
	// For our purposes, the type code should be 2 (uncompressed RGB image)
	// or 3 (uncompressed black-and-white images).
	if (typeCode != 2 && typeCode != 3)
	{
		fclose(ftga);
		return false;
	}
	// Read 13 unsigned chars of data we don't need.
	fread(&sintBad, sizeof(short), 1, ftga);
	fread(&sintBad, sizeof(short), 1, ftga);
	fread(&ucharBad, sizeof(unsigned char), 1, ftga);
	fread(&sintBad, sizeof(short), 1, ftga);
	fread(&sintBad, sizeof(short), 1, ftga);
	// Read the image's width and height.
	fread(&Width, sizeof(short), 1, ftga);
	fread(&Height, sizeof(short), 1, ftga);
	// Read the bit depth.
	fread(&bitCount, sizeof(unsigned char), 1, ftga);
	// Read one unsigned char of data we don't need.
	fread(&ucharBad, sizeof(unsigned char), 1, ftga);
	// Color mode -> 3 = BGR, 4 = BGRA.
	colorMode = bitCount / 8;
	size = Width * Height * colorMode;

	// Data
	PixelData = new unsigned char[sizeof(unsigned char)*size];
	fread(PixelData, sizeof(unsigned char), size, ftga);
	for (int idx = 0; idx < size; idx += colorMode)
	{
		colorSwap = PixelData[idx];
		PixelData[idx] = PixelData[idx + 2];
		PixelData[idx + 2] = colorSwap;
	}

	fclose(ftga);
	return true;
}
bool TATImage::Load(char* fileName)
{
	int type = SpecifyImageType(fileName);
	switch (type)
	{
	case IMG_TYPE_BMP:
		//LoadBMPFileResize(fileName);
		LoadBMPFile(fileName);
		return true;
	case IMG_TYPE_TGA:
		LoadUncompTGAFile(fileName);
		return true;
	default: return false;
	}
}
int TATImage::GetWidth()
{
	return Width;
}
int TATImage::GetHeight()
{
	return Height;
}
unsigned char* TATImage::GetPixelData()
{
	return PixelData;
}

// namespace TAT
void TAT::GetFileName(char* path, char* fileName)
{
	int len = strlen(path);
	int lastIndex = 0;
	for (int idx = len - 1; idx >= 0; idx--)
	{
		if ((path[idx] == '\\') || (path[idx] == '/'))
		{
			lastIndex = idx;
			break;
		}
	}

	for (int idx = lastIndex + 1; idx < len; idx ++)
	{
		fileName[idx - lastIndex - 1] = path[idx];
	}
	fileName[len - lastIndex - 1] = 0;
	len = strlen(fileName);
	for (int idx = 0; idx < len; idx++)
	{
		if (fileName[idx] == '.')
		{
			fileName[idx] = 0;
			break;
		}
	}
}
void TAT::GetFolderPath(char* path, char* folderPath)
{
	strcpy(folderPath, path);

	int len = strlen(folderPath);
	int lastIndex = 0;
	for (int idx = len - 1; idx >= 0; idx--)
	{
		if ((folderPath[idx] == '\\') || (folderPath[idx] == '/'))
		{
			folderPath[idx] = 0;
			break;
		}
	}
}
void TAT::GetFileExtension(char* path, char* ext)
{
	int len = strlen(path);
	int lastIndex = 0;
	for (int idx = len - 1; idx >= 0; idx--)
	{
		if (path[idx] == '.')
		{
			lastIndex = idx;
			break;
		}
	}

	for (int idx = lastIndex+1; idx < len; idx++)
	{
		ext[idx - lastIndex - 1] = path[idx];
	}
	ext[len - lastIndex] = 0;
}
unsigned int TAT::RGB2UI(unsigned char red, unsigned char green, unsigned char blue)
{
	return (red + green * 256 + blue * 256 * 256);
}
void TAT::UI2RGB(unsigned int rgb, unsigned char& red, unsigned char& green, unsigned char& blue)
{
	blue = (unsigned char)(rgb / (256 * 256));
	int tv = rgb - blue * 256 * 256;
	green = (unsigned char)(tv / 256);
	red = tv - green * 256;
}
float TAT::DetMatrix33(TATVector& v1, TATVector& v2, TATVector& v3)
{
	return v1.X * ( v2.Y*v3.Z - v2.Z*v3.Y )
		 + v1.Y * ( v2.Z*v3.X - v2.X*v3.Z )
		 + v1.Z * ( v2.X*v3.Y - v2.Y*v3.X );
}
void TAT::Perspective(float fovy, float aspect, float zNear, float zFar, float* matrix)
{
	if (matrix == NULL)
		return;

	float t = zNear * tanf(fovy / 2.0f);
	float b = -t;
	float r = aspect * t;
	float l = aspect * b;

	matrix[0] = 2.0f * zNear / (r - l);		matrix[1] = 0.0f;		matrix[2] = 0.0f;		matrix[3] = 0.0f;
	matrix[4] = 0.0f;		matrix[5] = 2.0f * zNear / (t - b);	matrix[6] = 0.0f;	matrix[7] = 0.0f;
	matrix[8] = (r + l) / (r - l);		matrix[9] = (t + b) / (t - b);		matrix[10] = -(zFar + zNear) / (zFar - zNear);		matrix[11] = -1.0f;
	matrix[12] = 0.0f;		matrix[13] = 0.0f;	matrix[14] = -2.0f * zFar * zNear / (zFar - zNear);	matrix[15] = 0.0f;
}
void TAT::Ortho(float left, float right, float bottom, float top, float zNear, float zFar, float* matrix)
{
	if (matrix == NULL)
		return;

	matrix[0] = 2.0f / (right - left);		matrix[1] = 0.0f;		matrix[2] = 0.0f;		matrix[3] = 0.0f;
	matrix[4] = 0.0f;		matrix[5] = 2.0f / (top - bottom);	matrix[6] = 0.0f;	matrix[7] = 0.0f;
	matrix[8] = 0.0f;		matrix[9] = 0.0f;		matrix[10] = - 2.0f / (zFar - zNear);		matrix[11] = 0.0f;
	matrix[12] = - (right + left) / (right - left);
	matrix[13] = - (top + bottom) / (top - bottom);
	matrix[14] = - (zFar + zNear) / (zFar - zNear);
	matrix[15] = 1.0f;
}
void TAT::TranslateO(TATHomoMatrix& matrix, TATVector& u)
{
	TATHomoMatrix transMatrix;
	transMatrix.SetP(u);
	matrix = matrix * transMatrix;
}
void TAT::TranslateO(TATHomoMatrix& matrix, float x, float y, float z)
{
	TATHomoMatrix transMatrix;
	transMatrix.SetP(x, y, z);
	matrix = matrix * transMatrix;
}
void TAT::RotateXO(TATHomoMatrix& matrix, float angle)
{
	matrix = matrix * TATMatrixRotX(angle);
}
void TAT::RotateYO(TATHomoMatrix& matrix, float angle)
{
	matrix = matrix * TATMatrixRotY(angle);
}
void TAT::RotateZO(TATHomoMatrix& matrix, float angle)
{
	matrix = matrix * TATMatrixRotZ(angle);
}
void TAT::RotateXW(TATHomoMatrix& matrix, float angle)
{
	matrix = TATMatrixRotX(angle) * matrix;
}
void TAT::RotateYW(TATHomoMatrix& matrix, float angle)
{
	matrix = TATMatrixRotY(angle) * matrix;
}
void TAT::RotateZW(TATHomoMatrix& matrix, float angle)
{
	matrix = TATMatrixRotZ(angle) * matrix;
}
void TAT::RotateW(TATHomoMatrix& matrix, float angle, TATVector& u)
{
	float c = cosf(angle);
	float s = sinf(angle);
	TATHomoMatrix rotMatrix;
	rotMatrix.SetN((1.0f - c)*u.X*u.X + c, (1.0f - c)*u.X*u.Y + s*u.Z, (1.0f - c)*u.X*u.Z - s*u.Y);
	rotMatrix.SetS((1.0f - c)*u.X*u.Y - s*u.Z, (1.0f - c)*u.Y*u.Y + c, (1.0f - c)*u.Y*u.Z + s*u.X);
	rotMatrix.SetA((1.0f - c)*u.X*u.Z + s*u.Y, (1.0f - c)*u.Y*u.Z - s*u.X, (1.0f - c)*u.Z*u.Z + c);
	rotMatrix.SetP(0.0f, 0.0f, 0.0f);
	matrix = rotMatrix * matrix;
}