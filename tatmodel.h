#ifndef __TATMODEL_H__
#define __TATMODEL_H__

#include "tatgl.h"
#include <list>

#define OBJ_VERT_3				0
#define OBJ_VERT_4				1
#define OBJ_TEX_2				0
#define OBJ_TEX_3				1
#define OBJ_NORM_3				0
#define OBJ_NORM_4				1
#define OBJ_FACE_VERT			0
#define OBJ_FACE_VERT_TEX		1
#define OBJ_FACE_VERT_NORM		2
#define OBJ_FACE_VERT_TEX_NORM	3

class TATBaseModel
{
protected:
	GLuint BufferPosition;
	GLuint BufferNormal;
	GLuint BufferUVs;
	GLuint BufferTangent;
	GLuint BufferIndex;

	float* DataPositions;
	float* DataNormals;
	float* DataTangents;
	float* DataUVs;
	GLuint* DataIndices;

public:
	TATBaseModel();
	virtual ~TATBaseModel();

	float* GetPositions();
	float* GetNormals();

};

class TATRectangle : public TATBaseModel
{
public:
	float SizeX;
	float SizeY;

public:
	TATRectangle();
	TATRectangle(float sizeX, float sizeY);
	virtual ~TATRectangle();

	void SetSize(float sizeX, float sizeY);

	void InitBuffers(bool isgenuv = false, float su = 1.0f, float sv = 1.0f);
	void Draw();
	void DrawTexture();
};

class TATPolyRectangle : public TATBaseModel
{
private:
	list<TATVector*> Points;
public:
	float Width;
	float Height;

public:
	TATPolyRectangle();
	TATPolyRectangle(float width, float height);
	virtual ~TATPolyRectangle();

	int GetVerticesSize();
	int GetIndicesSize();
	void AddPoint(float x, float y, float z);
	void DeleteAllPoints();

	void InitVertices();
	void InitUVs(float su = 1.0f, float sv = 1.0f);
	void InitIndices();
	void InitBuffers();
	void Draw();
	void DrawTexture();
};

class TATCube : public TATBaseModel
{
private:
	float SizeX;
	float SizeY;
	float SizeZ;

public:
	TATCube();
	TATCube(float sizeX, float sizeY, float sizeZ);
	virtual ~TATCube();

	float GetSizeX();
	void SetSizeX(float size);
	float GetSizeY();
	void SetSizeY(float size);
	float GetSizeZ();
	void SetSizeZ(float size);
	void SetSize(float sizeX, float sizeY, float sizeZ);

	void InitBuffers();
	void Draw();
};

class TATSphere : public TATBaseModel
{
private:
	float Radius;
	int Resolution;

public:
	TATSphere();
	TATSphere(float radius, int resol);
	virtual ~TATSphere();

	float GetRadius();
	void SetRadius(float radius);
	int GetResolution();
	void SetResolution(int resol);

	int GetVerticesSize();
	int GetIndicesSize();

	void InitVertices();
	void InitUVs();
	void InitTangents();
	void InitIndices();
	void InitBuffers();

	void Draw();
};

class TATTorus : public TATBaseModel
{
private:
	float TorusRadius;
	float TubeRadius;
	int TorusResolution;
	int TubeResolution;

public:
	TATTorus();
	TATTorus(float torusRadius, float tubeRadius, int torusResol, int tubeResol);
	virtual ~TATTorus();

	float GetTorusRadius();
	void SetTorusRadius(float radius);
	float GetTubeRadius();
	void SetTubeRadius(float radius);
	int GetTorusResolution();
	void SetTorusResolution(int resol);
	int GetTubeResolution();
	void SetTubeResolution(int resol);

	int GetVerticesSize();
	int GetIndicesSize();

	void InitVertices();
	void InitUVs();
	void InitTangents();
	void InitIndices();
	void InitBuffers();

	void Draw();

};

class TATCone : public TATBaseModel
{
private:
	float Aperture;
	float Height;
	int HeightResolution;
	int ThetaResolution;

public:
	TATCone();
	TATCone(float aperture, float height, int heightResol, int thetaResol);
	virtual ~TATCone();

	float GetAperture();
	void SetAperture(float aperture);
	float GetHeight();
	void SetHeight(float height);
	int GetHeightResolution();
	void SetHeightResolution(int resol);
	int GetThetaResolution();
	void SetThetaResolution(int resol);

	int GetVerticesSize();
	int GetIndicesSize();

	void InitVertices();
	void InitUVs();
	void InitTangents();
	void InitIndices();
	void InitBuffers();

	void Draw();

};

class TATSTLFace
{
public:
	TATVector Normal;
	TATVector Vertex0;
	TATVector Vertex1;
	TATVector Vertex2;

public:
	TATSTLFace();
};

class TATSTLMesh : public TATBaseModel
{
private:
	unsigned int FaceSize;
	bool Loaded;

public:
	TATSTLMesh();
	virtual ~TATSTLMesh();
	int GetVerticesSize();
	int GetIndicesSize();
	bool IsLoaded();
	void LoadBin(char* filePath, TATVector& scale);
	void LoadBin(char* filePath, float scale = 1.0f);
	void LoadASCII(char* filePath, float scale);
	void Load(char* filePath, float scale);
	void CheckFileMode(char* filePath, int &mode);
	void InitVertices(list<TATSTLFace*>& faces);
	void InitIndices();
	void InitBuffers();
	void Draw();
};

#endif;
