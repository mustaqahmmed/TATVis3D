#include "tatgl.h"

// TATShader
TATShader::TATShader()
{
	Program = 0;
}
TATShader::~TATShader()
{

}
unsigned int TATShader::GetProgram()
{
	return Program;
}
unsigned int TATShader::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	Program = glCreateProgram();
	glAttachShader(Program, VertexShaderID);
	glAttachShader(Program, FragmentShaderID);
	glLinkProgram(Program);

	// Check the program
	glGetProgramiv(Program, GL_LINK_STATUS, &Result);
	glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
	glGetProgramInfoLog(Program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return Program;
}
void TATShader::Use()
{
	glUseProgram(Program);
}

void TATShader::SetUniformMatrix4(char* name, float* matrix, int count)
{
	int id = glGetUniformLocation(Program, name);
	glUniformMatrix4fv(id, count, GL_FALSE, matrix);
}
void TATShader::SetUniformMatrix4(char* name, TATHomoMatrix& matrix)
{
	int id = glGetUniformLocation(Program, name);
	float m[16];
	matrix.ToArray(m);
	glUniformMatrix4fv(id, 1, GL_FALSE, m);
}

void TATShader::SetUniformVector3(char* name, float* vector, int count)
{
	int id = glGetUniformLocation(Program, name);
	glUniform3fv(id, count, vector);
}
void TATShader::SetUniformVector3(char* name, TATVector& vector)
{
	int id = glGetUniformLocation(Program, name);
	glUniform3f(id, vector.X, vector.Y, vector.Z);
}
void TATShader::SetUniformVector3(char* name, float v0, float v1, float v2)
{
	int id = glGetUniformLocation(Program, name);
	glUniform3f(id, v0, v1, v2);
}

void TATShader::SetUniformValue(char* name, float* value, int count)
{
	int id = glGetUniformLocation(Program, name);
	glUniform1fv(id, count, value);
}
void TATShader::SetUniformValue(char* name, int* value, int count)
{
	int id = glGetUniformLocation(Program, name);
	glUniform1iv(id, count, value);
}
void TATShader::SetUniformValue(char* name, float value)
{
	int id = glGetUniformLocation(Program, name);
	glUniform1f(id, value);
}
void TATShader::SetUniformValue(char* name, int value)
{
	int id = glGetUniformLocation(Program, name);
	glUniform1i(id, value);
}

void TATShader::SetSampler(char* name, int index)
{
	GLuint samplerID = glGetUniformLocation(Program, name);
	glUniform1i(samplerID, index);
}

// class TATTexture
TATTexture::TATTexture()
{
	TextureId = -1;
}
TATTexture::~TATTexture()
{

}
unsigned int TATTexture::GetTextureIndex()
{
	return TextureId;
}
void TATTexture::SetTextureIndex(unsigned int index)
{
	TextureId = index;
}
bool TATTexture::LoadTexture(char* fileName)
{
	TATImage* image = new TATImage();
	if (!image->Load(fileName))
	{
		delete image;
		return false;
	}

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &TextureId);
	
	glBindTexture(GL_TEXTURE_2D, TextureId);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0,
		GL_RGB, GL_UNSIGNED_BYTE, image->GetPixelData());
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 6778, 11368,
	//	GL_RGB, GL_UNSIGNED_BYTE, image->GetPixelData());

	delete image;
	return true;
}
bool TATTexture::IsLoad()
{
	return (TextureId != -1);
}

// class TATColors
TATColors::TATColors()
{

}
TATColors::~TATColors()
{

}
void TATColors::SetAmbientColor(TATVector color)
{
	AmbientColor = color;
}
void TATColors::SetDiffuseColor(TATVector color)
{
	DiffuseColor = color;
}
void TATColors::SetSpecularColor(TATVector color)
{
	SpecularColor = color;
}
TATVector TATColors::GetAmbientColor()
{
	return AmbientColor;
}
TATVector TATColors::GetDiffuseColor()
{
	return DiffuseColor;
}
TATVector TATColors::GetSpecularColor()
{
	return SpecularColor;
}

// class TATOBJMaterial
TATOBJMaterial::TATOBJMaterial()
{
	Colors.SetAmbientColor(TATVector(0.5f, 0.5f, 0.5f));
	Colors.SetDiffuseColor(TATVector(0.5f, 0.5f, 0.5f));
	Colors.SetSpecularColor(TATVector(0.5f, 0.5f, 0.5f));
}
TATOBJMaterial::~TATOBJMaterial()
{

}
char* TATOBJMaterial::GetName()
{
	return Name;
}
void TATOBJMaterial::SetName(char* name)
{
	strcpy(Name, name);
}
TATColors* TATOBJMaterial::GetColors()
{
	return &Colors;
}
TATTexture* TATOBJMaterial::GetTexture()
{
	return &Texture;
}
void TATOBJMaterial::SetAmbientColor(TATVector color)
{
	Colors.SetAmbientColor(color);
}
void TATOBJMaterial::SetDiffuseColor(TATVector color)
{
	Colors.SetDiffuseColor(color);
}
void TATOBJMaterial::SetSpecularColor(TATVector color)
{
	Colors.SetSpecularColor(color);
}
void TATOBJMaterial::SetColors(TATColors& colors)
{
	Colors.SetAmbientColor(colors.GetAmbientColor());
	Colors.SetDiffuseColor(colors.GetDiffuseColor());
	Colors.SetSpecularColor(colors.GetSpecularColor());
}
void TATOBJMaterial::SetTextureIndex(unsigned int index)
{
	Texture.SetTextureIndex(index);
}
bool TATOBJMaterial::LoadTexture(char* fileName)
{
	return Texture.LoadTexture(fileName);
}