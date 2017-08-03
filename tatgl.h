
//#pragma comment(lib, "opengl32.lib")  
//#pragma comment(lib, "glew32.lib")  

#include "tatmath.h"
//#include <///usr/local/Cellar/glew/2.0.0/include/GL/glew.h>
//#include <///usr/local/Cellar/glew/2.0.0/include/GL/glxew.h>
//#include <///usr/local/Cellar/glew/2.0.0/include/GL/glut.h>

//#include <OpenGL/gl3.h>
//#include <OpenGL/gl3ext.h>

#include <GL/glew.h>
#include <GL/glxew.h>

#include <GLUT/GLUT.h>

#define POSITION_ATTRIBUTE	0
#define NORMAL_ATTRIBUTE	1
#define TEXTURE_ATTRIBUTE	2

#define LIGHT_MAX_COUNT		10

class TATShader
{
private:
	GLuint Program;

public:
	TATShader();
	~TATShader();
	unsigned int GetProgram();

	unsigned int LoadShaders(const char * vertexFilePath, const char * fragmentFilePath);
	void Use();

	void SetUniformMatrix4(char* name, float* matrix, int count = 1);
	void SetUniformMatrix4(char* name, TATHomoMatrix& matrix);

	void SetUniformVector3(char* name, float* vector, int count = 1);
	void SetUniformVector3(char* name, TATVector& vector);
	void SetUniformVector3(char* name, float v0, float v1, float v2);

	void SetUniformValue(char* name, float* value, int count = 1);
	void SetUniformValue(char* name, int* value, int count = 1);
	void SetUniformValue(char* name, float value);
	void SetUniformValue(char* name, int value);

	void SetSampler(char* name, int index);

};
class TATTexture
{
private:
	unsigned int TextureId;

public:
	TATTexture();
	~TATTexture();
	unsigned int GetTextureIndex();
	void SetTextureIndex(unsigned int index);
	bool LoadTexture(char* fileName);
	bool IsLoad();
};
class TATColors
{
private:
	TATVector AmbientColor;
	TATVector DiffuseColor;
	TATVector SpecularColor;

public:
	TATColors();
	~TATColors();

	void SetAmbientColor(TATVector color);
	void SetDiffuseColor(TATVector color);
	void SetSpecularColor(TATVector color);
	TATVector GetAmbientColor();
	TATVector GetDiffuseColor();
	TATVector GetSpecularColor();

};
class TATOBJMaterial
{
private:
	char Name[32];
	TATColors Colors;
	TATTexture Texture;

public:
	TATOBJMaterial();
	~TATOBJMaterial();

	char* GetName();
	void SetName(char* name);

	TATColors* GetColors();
	TATTexture* GetTexture();

	void SetAmbientColor(TATVector color);
	void SetDiffuseColor(TATVector color);
	void SetSpecularColor(TATVector color);
	void SetColors(TATColors& colors);
	void SetTextureIndex(unsigned int index);
	bool LoadTexture(char* fileName);

};
