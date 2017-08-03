
#include "tatmodel.h"

TATBaseModel::TATBaseModel()
{
	BufferPosition = 0;
	BufferNormal = 0;
	BufferTangent = 0;
	BufferUVs = 0;
	BufferIndex = 0;

	DataPositions = NULL;
	DataNormals = NULL;
	DataTangents = NULL;
	DataUVs = NULL;
	DataIndices = NULL;
}
TATBaseModel::~TATBaseModel()
{
	if (BufferPosition != 0)
		glDeleteBuffers(1, &BufferPosition);
	if (BufferNormal != 0)
		glDeleteBuffers(1, &BufferNormal);
	if (BufferTangent != 0)
		glDeleteBuffers(1, &BufferTangent);
	if (BufferUVs != 0)
		glDeleteBuffers(1, &BufferUVs);
	if (BufferIndex != 0)
		glDeleteBuffers(1, &BufferIndex);

	if (DataPositions != NULL)
		delete[] DataPositions;
	if (DataNormals != NULL)
		delete[] DataNormals;
	if (DataTangents != NULL)
		delete[] DataTangents;
	if (DataUVs != NULL)
		delete[] DataUVs;
	if (DataIndices != NULL)
		delete[] DataIndices;
}
float* TATBaseModel::GetPositions()
{
	return DataPositions;
}
float* TATBaseModel::GetNormals()
{
	return DataNormals;
}

// TATRectangle
TATRectangle::TATRectangle() : TATBaseModel()
{
	SizeX = 1.0f;
	SizeY = 1.0f;
}
TATRectangle::TATRectangle(float sizeX, float sizeY)
{
	SizeX = sizeX;
	SizeY = sizeY;
}
TATRectangle::~TATRectangle()
{

}
void TATRectangle::SetSize(float sizeX, float sizeY)
{
	SizeX = sizeX;
	SizeY = sizeY;
}
void TATRectangle::InitBuffers(bool isgenuv, float su, float sv)
{
	glGenBuffers(1, &BufferPosition);
	glGenBuffers(1, &BufferNormal);
	glGenBuffers(1, &BufferUVs);
	glGenBuffers(1, &BufferIndex);

	float sizeX_2 = SizeX / 2.0f;
	float sizeY_2 = SizeY / 2.0f;
	float positionData[] = {
		-sizeX_2, -sizeY_2, 0.0f,
		sizeX_2, -sizeY_2, 0.0f,
		sizeX_2, sizeY_2, 0.0f,
		-sizeX_2, sizeY_2, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

	TATVector norm(0.0f, 0.0f, 1.0f);
	float normalData[] = {
		norm.X, norm.Y, norm.Z,
		norm.X, norm.Y, norm.Z,
		norm.X, norm.Y, norm.Z,
		norm.X, norm.Y, norm.Z,
	};

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalData), normalData, GL_STATIC_DRAW);

	// Texture coordinates
	if (isgenuv) {
		float texCoordData[] = {
			0.0f, 0.0f,
			su, 0.0f,
			su, sv,
			0.0f, sv,
		};

		glBindBuffer(GL_ARRAY_BUFFER, BufferUVs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordData), texCoordData, GL_STATIC_DRAW);
	}

	GLuint indexData[] = {
		0, 1, 2, 0, 2, 3
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

}
void TATRectangle::Draw() 
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
void TATRectangle::DrawTexture()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	//glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
	//glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	glBindBuffer(GL_ARRAY_BUFFER, BufferUVs);	// Use buffer
	glVertexAttribPointer(TEXTURE_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}

// TATPolyRectangle
TATPolyRectangle::TATPolyRectangle()
{
	Width = 1.0f;
	Height = 1.0f;
}
TATPolyRectangle::TATPolyRectangle(float width, float height)
{
	Width = width;
	Height = height;
}
TATPolyRectangle::~TATPolyRectangle()
{
	DeleteAllPoints();
}
int TATPolyRectangle::GetVerticesSize()
{
	int numOfPoint = Points.size();
	return (4 * numOfPoint);
}
int TATPolyRectangle::GetIndicesSize()
{
	int numOfPoint = Points.size();
	return (4 * 6 * (numOfPoint - 1));
}
void TATPolyRectangle::AddPoint(float x, float y, float z)
{
	Points.push_back(new TATVector(x, y, z));
}
void TATPolyRectangle::DeleteAllPoints()
{
	list<TATVector*>::iterator ite = Points.begin();
	while (ite != Points.end()) {
		delete *ite;
		ite++;
	}
	Points.clear();
}
void TATPolyRectangle::InitVertices()
{
	int numOfPoint = Points.size();
	if (numOfPoint <= 1)
		return;

	float w2 = Width / 2.0f;
	int vertexSize = GetVerticesSize();
	DataPositions = new float[3 * vertexSize];
	DataNormals = new float[3 * vertexSize];
	
	list<TATVector*>::iterator ite = Points.begin();
	for (int iP = 0; iP < numOfPoint; iP ++) {
		TATVector* pt = *ite;
		TATVector v0, v1, v2, v3;
		TATVector n0, n1, n2, n3;
		if (iP == 0) {
			list<TATVector*>::iterator nextite = ite;
			nextite++;
			TATVector* nextpt = *nextite;
			TATVector dir = (*nextpt) - (*pt);
			dir.Z = 0.0f;
			dir.ToUnit();
			n0 = TATMatrixRotZ(90.0f) * dir;
			n1 = TATMatrixRotZ(-90.0f) * dir;
			n2 = n1;
			n3 = n0;
			v0 = (*pt) +  (TATMatrixRotZ(90.0f) * dir).ScaleTo(w2);
			v1 = (*pt) + (TATMatrixRotZ(-90.0f) * dir).ScaleTo(w2);
			v2 = v1 + TATVector(0.0f, 0.0f, Height);
			v3 = v0 + TATVector(0.0f, 0.0f, Height);
		}
		else if (iP == (numOfPoint - 1)) {
			list<TATVector*>::iterator previte = ite;
			previte--;
			TATVector* prevpt = *previte;
			TATVector dir = (*pt) - (*prevpt);
			dir.Z = 0.0f;
			dir.ToUnit();
			n0 = TATMatrixRotZ(90.0f) * dir;
			n1 = TATMatrixRotZ(-90.0f) * dir;
			n2 = n1;
			n3 = n0;
			v0 = (*pt) + (TATMatrixRotZ(90.0f) * dir).ScaleTo(w2);
			v1 = (*pt) + (TATMatrixRotZ(-90.0f) * dir).ScaleTo(w2);
			v2 = v1 + TATVector(0.0f, 0.0f, Height);
			v3 = v0 + TATVector(0.0f, 0.0f, Height);
		}
		else {
			list<TATVector*>::iterator nextite = ite;
			nextite++;
			TATVector* nextpt = *nextite;

			list<TATVector*>::iterator previte = ite;
			previte--;
			TATVector* prevpt = *previte;

			TATVector dir0 = (*nextpt) - (*pt);
			TATVector dir1 = (*prevpt) - (*pt);
			TATVector dir = (dir0 + dir1);
			dir.Z = 0.0f;
			dir.ToUnit();
			n0 = TATMatrixRotZ(90.0f) * dir;
			n1 = TATMatrixRotZ(-90.0f) * dir;
			n2 = n1;
			n3 = n0;
			v0 = (*pt) + dir.ScaleTo(w2);
			v1 = (*pt) + dir.ScaleTo(-w2);
			v2 = v1 + TATVector(0.0f, 0.0f, Height);
			v3 = v0 + TATVector(0.0f, 0.0f, Height);
		}

		int iV0 = 4 * iP;
		int iV1 = iV0 + 1;
		int iV2 = iV0 + 2;
		int iV3 = iV0 + 3;
		int iVF0 = 3 * iV0;
		int iVF1 = 3 * iV1;
		int iVF2 = 3 * iV2;
		int iVF3 = 3 * iV3;

		DataPositions[iVF0] = v0.X;
		DataPositions[iVF0 + 1] = v0.Y;
		DataPositions[iVF0 + 2] = v0.Z;

		DataPositions[iVF1] = v1.X;
		DataPositions[iVF1 + 1] = v1.Y;
		DataPositions[iVF1 + 2] = v1.Z;

		DataPositions[iVF2] = v2.X;
		DataPositions[iVF2 + 1] = v2.Y;
		DataPositions[iVF2 + 2] = v2.Z;

		DataPositions[iVF3] = v3.X;
		DataPositions[iVF3 + 1] = v3.Y;
		DataPositions[iVF3 + 2] = v3.Z;

		DataNormals[iVF0] = n0.X;
		DataNormals[iVF0 + 1] = n0.Y;
		DataNormals[iVF0 + 2] = n0.Z;

		DataNormals[iVF1] = n1.X;
		DataNormals[iVF1 + 1] = n1.Y;
		DataNormals[iVF1 + 2] = n1.Z;

		DataNormals[iVF2] = n2.X;
		DataNormals[iVF2 + 1] = n2.Y;
		DataNormals[iVF2 + 2] = n2.Z;

		DataNormals[iVF3] = n3.X;
		DataNormals[iVF3 + 1] = n3.Y;
		DataNormals[iVF3 + 2] = n3.Z;

		ite++;
	}
}
void TATPolyRectangle::InitUVs(float su, float sv)
{
	int numOfPoint = Points.size();
	if (numOfPoint <= 1)
		return;

	int vertexSize = GetVerticesSize();
	DataUVs = new float[2 * vertexSize];

	float u, v;
	int iV, iVF;
	for (int i = 0; i < numOfPoint; i++){
		for (int j = 0; j < 4; j++){
			u = su * (float)i / (float)(numOfPoint - 1);
			v = sv * (float)j / 3.0f;
			iV = 4 * i + j;
			iVF = 2 * iV;
			DataUVs[iVF] = u;
			DataUVs[iVF + 1] = v;
		}
	}
}
void TATPolyRectangle::InitIndices()
{
	int numOfPoint = Points.size();
	if (numOfPoint <= 1)
		return;

	int indexSize = GetIndicesSize();
	DataIndices = new GLuint[indexSize];

	for (int i = 0; i < numOfPoint - 1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int iQ = 3 * i + j;
			int iVL = 4 * i + j;
			int iVR = 4 * (i + 1) + j;

			DataIndices[6 * iQ] = iVL;
			DataIndices[6 * iQ + 1] = iVR;
			DataIndices[6 * iQ + 2] = iVR + 1;
			DataIndices[6 * iQ + 3] = iVL;
			DataIndices[6 * iQ + 4] = iVR + 1;
			DataIndices[6 * iQ + 5] = iVL + 1;
		}
	}

	for (int i = 0; i < numOfPoint - 1; i++)
	{
		int iQ = 3 * i + 3;
		int iVL0 = 4 * i + 3;
		int iVR0 = 4 * (i + 1) + 3;
		int iVL1 = 4 * i;
		int iVR1 = 4 * (i + 1);

		DataIndices[6 * iQ] = iVL0;
		DataIndices[6 * iQ + 1] = iVR0;
		DataIndices[6 * iQ + 2] = iVR1;
		DataIndices[6 * iQ + 3] = iVL0;
		DataIndices[6 * iQ + 4] = iVR1;
		DataIndices[6 * iQ + 5] = iVL1;
	}
}
void TATPolyRectangle::InitBuffers()
{
	if (DataPositions == NULL)
		return;
	if (DataNormals == NULL)
		return;
	if (DataIndices == NULL)
		return;

	int verticesSize = GetVerticesSize();
	int indicesSize = GetIndicesSize();

	glGenBuffers(1, &BufferPosition);
	glGenBuffers(1, &BufferNormal);
	glGenBuffers(1, &BufferIndex);

	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataNormals, GL_STATIC_DRAW);

	if (DataUVs != NULL) {
		glGenBuffers(1, &BufferUVs);
		glBindBuffer(GL_ARRAY_BUFFER, BufferUVs);
		glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataUVs, GL_STATIC_DRAW);
		delete DataUVs;
		DataUVs = NULL;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLuint), DataIndices, GL_STATIC_DRAW);

	delete[]DataPositions;
	DataPositions = NULL;
	delete[]DataNormals;
	DataNormals = NULL;
	
	delete[]DataIndices;
	DataIndices = NULL;
}
void TATPolyRectangle::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	int indicesSize = GetIndicesSize();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void*)0);
}
void TATPolyRectangle::DrawTexture()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, BufferUVs);	// Use buffer
	glVertexAttribPointer(TEXTURE_ATTRIBUTE, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	int indicesSize = GetIndicesSize();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void*)0);
}

// TATCube
TATCube::TATCube() : TATBaseModel()
{
	SizeX = 1.0f;
	SizeY = 1.0f;
	SizeZ = 1.0f;
}
TATCube::TATCube(float sizeX, float sizeY, float sizeZ) : TATBaseModel()
{
	SizeX = sizeX;
	SizeY = sizeY;
	SizeZ = sizeZ;
}
TATCube::~TATCube()
{

}
float TATCube::GetSizeX()
{
	return SizeX;
}
void TATCube::SetSizeX(float size)
{
	SizeX = size;
}
float TATCube::GetSizeY()
{
	return SizeY;
}
void TATCube::SetSizeY(float size)
{
	SizeY = size;
}
float TATCube::GetSizeZ()
{
	return SizeZ;
}
void TATCube::SetSizeZ(float size)
{
	SizeZ = size;
}
void TATCube::SetSize(float sizeX, float sizeY, float sizeZ)
{
	SizeX = sizeX;
	SizeY = sizeY;
	SizeZ = sizeZ;
}
void TATCube::InitBuffers()
{
	glGenBuffers(1, &BufferPosition);
	glGenBuffers(1, &BufferNormal);
	glGenBuffers(1, &BufferIndex);

	float sizeX_2 = SizeX / 2.0f;
	float sizeY_2 = SizeY / 2.0f;
	float sizeZ_2 = SizeZ / 2.0f;
	float positionData[] = {
		-sizeX_2, -sizeY_2, -sizeZ_2,
		sizeX_2, -sizeY_2, -sizeZ_2,
		sizeX_2, sizeY_2, -sizeZ_2,
		-sizeX_2, sizeY_2, -sizeZ_2,
		-sizeX_2, -sizeY_2, sizeZ_2,
		sizeX_2, -sizeY_2, sizeZ_2,
		sizeX_2, sizeY_2, sizeZ_2,
		-sizeX_2, sizeY_2, sizeZ_2,
	};

	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

	TATVector tv(SizeX, SizeY, SizeZ);
	tv.ToUnit();
	float normalData[] = {
		-tv.X, -tv.Y, -tv.Z,
		tv.X, -tv.Y, -tv.Z,
		tv.X, tv.Y, -tv.Z,
		-tv.X, tv.Y, -tv.Z,
		-tv.X, -tv.Y, tv.Z,
		tv.X, -tv.Y, tv.Z,
		tv.X, tv.Y, tv.Z,
		-tv.X, tv.Y, tv.Z,
	};

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalData), normalData, GL_STATIC_DRAW);

	GLuint indexData[] = {
		4, 5, 6, 4, 6, 7,    // Front face
		0, 3, 2, 0, 2, 1,    // Back face
		3, 7, 6, 3, 6, 2,    // Top face
		0, 1, 5, 0, 5, 4,    // Bottom face
		1, 2, 6, 1, 6, 5,    // Right face
		0, 4, 7, 0, 7, 3,    // Left face
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

}
void TATCube::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
}

// TATSphere
TATSphere::TATSphere() : TATBaseModel()
{
	Radius = 1.0f;
}
TATSphere::TATSphere(float radius, int resol) : TATBaseModel()
{
	Radius = radius;
	Resolution = resol;
}
TATSphere::~TATSphere()
{

}
float TATSphere::GetRadius()
{
	return Radius;
}
void TATSphere::SetRadius(float radius)
{
	Radius = radius;
}
int TATSphere::GetResolution()
{
	return Resolution;
}
void TATSphere::SetResolution(int resol)
{
	Resolution = resol;
}
int TATSphere::GetVerticesSize()
{
	int resol_teta = 2 * Resolution;
	return (resol_teta * Resolution);
}
int TATSphere::GetIndicesSize()
{
	int resol_teta = 2 * Resolution;
	return (6 * (resol_teta - 1) * (Resolution - 1));
}
void TATSphere::InitVertices()
{
	int verticesSize = GetVerticesSize();
	DataPositions = new float[3 * verticesSize];
	DataNormals = new float[3 * verticesSize];
	int resol_teta = 2 * Resolution;
	for (int i = 0; i < resol_teta; i++)
	{
		float teta = ((float)i / (float)(resol_teta - 1)) * 2.0f*PI;
		for (int j = 0; j < Resolution; j++)
		{
			int iV = Resolution*i + j;
			int iVF = 3 * iV;

			float phi = -PI / 2.0f + ((float)j / (float)(Resolution - 1)) * PI;

			TATVector vertex;
			vertex.X = Radius * cosf(phi)*cosf(teta);
			vertex.Y = Radius * cosf(phi)*sinf(teta);
			vertex.Z = Radius * sinf(phi);
			DataPositions[iVF] = vertex.X;
			DataPositions[iVF + 1] = vertex.Y;
			DataPositions[iVF + 2] = vertex.Z;

			vertex.ToUnit();
			DataNormals[iVF] = vertex.X;
			DataNormals[iVF + 1] = vertex.Y;
			DataNormals[iVF + 2] = vertex.Z;
		}
	}
}
void TATSphere::InitUVs()
{
	int verticesSize = GetVerticesSize();
	DataUVs = new float[2 * verticesSize];
	int resol_teta = 2 * Resolution;
	float c = 2.0f * PI * Radius;
	for (int i = 0; i < resol_teta; i++)
	{
		float teta = ((float)i / (float)(resol_teta - 1)) * 2.0f*PI;
		for (int j = 0; j < Resolution; j++)
		{
			int iV = Resolution*i + j;
			float phi = -PI / 2.0f + ((float)j / (float)(Resolution - 1)) * PI;	// -PI/2 - Pi/2
			float phi_off = phi + PI / 2.0f;	// 0 - PI
			float len = Radius * phi_off;

			float u = len * cosf(teta) / c + 0.5f;
			float v = len * sinf(teta) / c + 0.5f;
			DataUVs[2 * iV] = u;
			DataUVs[2 * iV + 1] = v;
		}
	}
}
void TATSphere::InitTangents()
{
	int verticesSize = GetVerticesSize();
	DataTangents = new float[3 * verticesSize];
	memset(DataTangents, 0, 3 * verticesSize * sizeof(float));
	int resol_teta = 2 * Resolution;

	int indicesSize = GetIndicesSize();
	for (int i = 0; i < indicesSize; i += 3)	// For each triangle
	{
		int idxV0 = 3 * DataIndices[i];
		int idxV1 = 3 * DataIndices[i + 1];
		int idxV2 = 3 * DataIndices[i + 2];
		int idxUV0 = 2 * DataIndices[i];
		int idxUV1 = 2 * DataIndices[i + 1];
		int idxUV2 = 2 * DataIndices[i + 2];

		TATVector v0(DataPositions[idxV0], DataPositions[idxV0 + 1], DataPositions[idxV0 + 2]);
		TATVector v1(DataPositions[idxV1], DataPositions[idxV1 + 1], DataPositions[idxV1 + 2]);
		TATVector v2(DataPositions[idxV2], DataPositions[idxV2 + 1], DataPositions[idxV2 + 2]);

		TATVector2 uv0(DataUVs[idxUV0], DataUVs[idxUV0 + 1]);
		TATVector2 uv1(DataUVs[idxUV1], DataUVs[idxUV1 + 1]);
		TATVector2 uv2(DataUVs[idxUV2], DataUVs[idxUV2 + 1]);

		TATVector dv1 = v1 - v0;
		TATVector dv2 = v2 - v0;

		TATVector2 duv1 = uv1 - uv0;
		TATVector2 duv2 = uv2 - uv0;

		float cr = duv1.X * duv2.Y - duv1.Y * duv2.X;
		if (cr == 0.0)
		{
			TATVector tangent(1.0, 0.0, 0.0);

			DataTangents[idxV0] += tangent.X;
			DataTangents[idxV0 + 1] += tangent.Y;
			DataTangents[idxV0 + 2] += tangent.Z;

			DataTangents[idxV1] += tangent.X;
			DataTangents[idxV1 + 1] += tangent.Y;
			DataTangents[idxV1 + 2] += tangent.Z;

			DataTangents[idxV2] += tangent.X;
			DataTangents[idxV2 + 1] += tangent.Y;
			DataTangents[idxV2 + 2] += tangent.Z;

		}
		else
		{
			float r = 1.0f / cr;
			TATVector tangent = (dv1.ScaleTo(duv2.Y) - dv2.ScaleTo(duv1.Y)).ScaleTo(r);

			DataTangents[idxV0] += tangent.X;
			DataTangents[idxV0 + 1] += tangent.Y;
			DataTangents[idxV0 + 2] += tangent.Z;

			DataTangents[idxV1] += tangent.X;
			DataTangents[idxV1 + 1] += tangent.Y;
			DataTangents[idxV1 + 2] += tangent.Z;

			DataTangents[idxV2] += tangent.X;
			DataTangents[idxV2 + 1] += tangent.Y;
			DataTangents[idxV2 + 2] += tangent.Z;
		}
	}

	for (int i = 0; i < verticesSize; i += 1)	// For each vertex
	{
		int idxX = 3 * i;
		int idxY = 3 * i + 1;
		int idxZ = 3 * i + 2;
		TATVector tangent(DataTangents[idxX], DataTangents[idxY], DataTangents[idxZ]);
		tangent.ToUnit();
		DataTangents[idxX] = tangent.X;
		DataTangents[idxY] = tangent.Y;
		DataTangents[idxZ] = tangent.Z;
	}

}
void TATSphere::InitIndices()
{
	int indicesSize = GetIndicesSize();
	DataIndices = new GLuint[indicesSize];
	int resol_teta = 2 * Resolution;
	for (int i = 0; i < resol_teta - 1; i++)
	{
		for (int j = 0; j < Resolution - 1; j++)
		{
			int iQ = (Resolution - 1) * i + j;
			int iVL = (Resolution)* i + j;
			int iVR = (Resolution)* (i + 1) + j;

			DataIndices[6 * iQ] = iVL;
			DataIndices[6 * iQ + 1] = iVR;
			DataIndices[6 * iQ + 2] = iVR + 1;
			DataIndices[6 * iQ + 3] = iVL;
			DataIndices[6 * iQ + 4] = iVR + 1;
			DataIndices[6 * iQ + 5] = iVL + 1;
		}
	}
}
void TATSphere::InitBuffers()
{
	if (DataPositions == NULL)
		return;
	if (DataNormals == NULL)
		return;
	if (DataIndices == NULL)
		return;

	int verticesSize = GetVerticesSize();
	int indicesSize = GetIndicesSize();

	glGenBuffers(1, &BufferPosition);
	glGenBuffers(1, &BufferNormal);
	glGenBuffers(1, &BufferIndex);

	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataNormals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLuint), DataIndices, GL_STATIC_DRAW);

	delete[]DataPositions;
	DataPositions = NULL;
	delete[]DataNormals;
	DataNormals = NULL;
	delete[]DataIndices;
	DataIndices = NULL;
}
void TATSphere::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	int indicesSize = GetIndicesSize();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void*)0);
}

// class TATTorus
TATTorus::TATTorus() : TATBaseModel()
{
	TorusRadius = 1.0f;
	TubeRadius = 0.20f;
	TorusResolution = 10;
	TubeResolution = 10;
}
TATTorus::TATTorus(float torusRadius, float tubeRadius, int torusResol, int tubeResol)
{
	TorusRadius = torusRadius;
	TubeRadius = tubeRadius;
	TorusResolution = torusResol;
	TubeResolution = tubeResol;
}
TATTorus::~TATTorus()
{

}
float TATTorus::GetTorusRadius()
{
	return TorusRadius;
}
void TATTorus::SetTorusRadius(float radius)
{
	TorusRadius = radius;
}
float TATTorus::GetTubeRadius()
{
	return TubeRadius;
}
void TATTorus::SetTubeRadius(float radius)
{
	TubeRadius = radius;
}
int TATTorus::GetTorusResolution()
{
	return TorusResolution;
}
void TATTorus::SetTorusResolution(int resol)
{
	TorusResolution = resol;
}
int TATTorus::GetTubeResolution()
{
	return TubeResolution;
}
void TATTorus::SetTubeResolution(int resol)
{
	TubeResolution = resol;
}
int TATTorus::GetVerticesSize()
{
	return (TorusResolution * TubeResolution);
}
int TATTorus::GetIndicesSize()
{
	return (6 * (TorusResolution - 1) * (TubeResolution - 1));
}
void TATTorus::InitVertices()
{
	int verticesSize = GetVerticesSize();
	DataPositions = new float[3 * verticesSize];
	DataNormals = new float[3 * verticesSize];
	for (int i = 0; i < TorusResolution; i++)
	{
		float teta = ((float)i / (float)(TorusResolution - 1)) * 2.0f*PI;
		for (int j = 0; j < TubeResolution; j++)
		{
			int iV = TubeResolution*i + j;
			int iVF = 3 * iV;

			float phi = ((float)j / (float)(TubeResolution - 1)) * 2.0f*PI;

			float x = (TorusRadius + TubeRadius * cosf(teta)) * cosf(phi);
			float y = (TorusRadius + TubeRadius * cosf(teta)) * sinf(phi);
			float z = TubeRadius * sinf(teta);
			DataPositions[iVF] = x;
			DataPositions[iVF + 1] = y;
			DataPositions[iVF + 2] = z;

			TATVector norm;
			norm.X = TubeRadius * cosf(teta) * cosf(phi);
			norm.Y = TubeRadius * cosf(teta) * sinf(phi);
			norm.Z = TubeRadius * sinf(teta);
			norm.ToUnit();
			DataNormals[iVF] = norm.X;
			DataNormals[iVF + 1] = norm.Y;
			DataNormals[iVF + 2] = norm.Z;

		}
	}
}
void TATTorus::InitUVs()
{

}
void TATTorus::InitTangents()
{

}
void TATTorus::InitIndices()
{
	int indicesSize = GetIndicesSize();
	DataIndices = new GLuint[indicesSize];
	for (int i = 0; i < TorusResolution - 1; i++)
	{
		for (int j = 0; j < TubeResolution - 1; j++)
		{
			int iQ = (TubeResolution - 1) * i + j;
			int iVL = (TubeResolution)* i + j;
			int iVR = (TubeResolution)* (i + 1) + j;

			DataIndices[6 * iQ] = iVL;
			DataIndices[6 * iQ + 1] = iVR;
			DataIndices[6 * iQ + 2] = iVR + 1;
			DataIndices[6 * iQ + 3] = iVL;
			DataIndices[6 * iQ + 4] = iVR + 1;
			DataIndices[6 * iQ + 5] = iVL + 1;
		}
	}
}
void TATTorus::InitBuffers()
{
	if (DataPositions == NULL)
		return;
	if (DataNormals == NULL)
		return;
	if (DataIndices == NULL)
		return;

	int verticesSize = GetVerticesSize();
	int indicesSize = GetIndicesSize();

	glGenBuffers(1, &BufferPosition);
	glGenBuffers(1, &BufferNormal);
	glGenBuffers(1, &BufferIndex);

	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataNormals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLuint), DataIndices, GL_STATIC_DRAW);

	delete[]DataPositions;
	DataPositions = NULL;
	delete[]DataNormals;
	DataNormals = NULL;
	delete[]DataIndices;
	DataIndices = NULL;
}
void TATTorus::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	int indicesSize = GetIndicesSize();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void*)0);
}
// class TATCone
TATCone::TATCone() : TATBaseModel()
{
	Aperture = 30.0f;
	Height = 1.0f;
	HeightResolution = 20;
	ThetaResolution = 20;
}
TATCone::TATCone(float aperture, float height, int heightResol, int thetaResol)
{
	Aperture = aperture;
	Height = height;
	HeightResolution = heightResol;
	ThetaResolution = thetaResol;
}
TATCone::~TATCone()
{

}
float TATCone::GetAperture()
{
	return Aperture;
}
void TATCone::SetAperture(float aperture)
{
	Aperture = aperture;
}
float TATCone::GetHeight()
{
	return Height;
}
void TATCone::SetHeight(float height)
{
	Height = height;
}
int TATCone::GetHeightResolution()
{
	return HeightResolution;
}
void TATCone::SetHeightResolution(int resol)
{
	HeightResolution = resol;
}
int TATCone::GetThetaResolution()
{
	return ThetaResolution;
}
void TATCone::SetThetaResolution(int resol)
{
	ThetaResolution = resol;
}
int TATCone::GetVerticesSize()
{
	return (ThetaResolution * HeightResolution);
}
int TATCone::GetIndicesSize()
{
	return (6 * (ThetaResolution - 1) * (HeightResolution - 1));
}
void TATCone::InitVertices()
{
	int verticesSize = GetVerticesSize();
	DataPositions = new float[3 * verticesSize];
	DataNormals = new float[3 * verticesSize];
	for (int i = 0; i < ThetaResolution; i++)
	{
		float teta = ((float)i / (float)(ThetaResolution - 1)) * 2.0f*PI;
		float tetaDeg = teta * 180.0f / PI;

		TATVector norm(1.0f, 0.0f, 0.0f);
		norm = TATMatrixRotZ(tetaDeg) * TATMatrixRotY(-Aperture / 2.0f) * norm;

		for (int j = 0; j < HeightResolution; j++)
		{
			int iV = HeightResolution*i + j;
			int iVF = 3 * iV;

			float h = ((float)j / (float)(HeightResolution - 1)) * Height;
			float r = h * tanf((Aperture / 2.0f) * (PI / 180.0f));

			float x = r * cosf(teta);
			float y = r * sinf(teta);
			float z = Height - h;
			DataPositions[iVF] = x;
			DataPositions[iVF + 1] = y;
			DataPositions[iVF + 2] = z;

			DataNormals[iVF] = norm.X;
			DataNormals[iVF + 1] = norm.Y;
			DataNormals[iVF + 2] = norm.Z;

		}
	}
}
void TATCone::InitUVs()
{

}
void TATCone::InitTangents()
{

}
void TATCone::InitIndices()
{
	int indicesSize = GetIndicesSize();
	DataIndices = new GLuint[indicesSize];
	for (int i = 0; i < ThetaResolution - 1; i++)
	{
		for (int j = 0; j < HeightResolution - 1; j++)
		{
			int iQ = (HeightResolution - 1) * i + j;
			int iVL = (HeightResolution)* i + j;
			int iVR = (HeightResolution)* (i + 1) + j;

			DataIndices[6 * iQ] = iVL;
			DataIndices[6 * iQ + 1] = iVR;
			DataIndices[6 * iQ + 2] = iVR + 1;
			DataIndices[6 * iQ + 3] = iVL;
			DataIndices[6 * iQ + 4] = iVR + 1;
			DataIndices[6 * iQ + 5] = iVL + 1;
		}
	}
}
void TATCone::InitBuffers()
{
	if (DataPositions == NULL)
		return;
	if (DataNormals == NULL)
		return;
	if (DataIndices == NULL)
		return;

	int verticesSize = GetVerticesSize();
	int indicesSize = GetIndicesSize();

	glGenBuffers(1, &BufferPosition);
	glGenBuffers(1, &BufferNormal);
	glGenBuffers(1, &BufferIndex);

	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataNormals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLuint), DataIndices, GL_STATIC_DRAW);

	delete[]DataPositions;
	DataPositions = NULL;
	delete[]DataNormals;
	DataNormals = NULL;
	delete[]DataIndices;
	DataIndices = NULL;
}
void TATCone::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
	glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
	glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

	int indicesSize = GetIndicesSize();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void*)0);
}

// TATSTLFace
TATSTLFace::TATSTLFace()
{

}

// TATSTLMesh
TATSTLMesh::TATSTLMesh() : TATBaseModel()
{
	Loaded = false;
}
TATSTLMesh::~TATSTLMesh()
{

}
int TATSTLMesh::GetVerticesSize()
{
	return 3 * FaceSize;
}
int TATSTLMesh::GetIndicesSize()
{
	return 3 * FaceSize;
}
bool TATSTLMesh::IsLoaded()
{
	return Loaded;
}
void TATSTLMesh::LoadBin(char* filePath, TATVector& scale)
{
	FILE* fInput = fopen(filePath, "rb");
	if (fInput == NULL)
		return;

	// Read header
	char header[80];
	fread(header, 1, 80, fInput);
	// Read number of triangles
	fread(&FaceSize, 4, 1, fInput);

	// Read data for all triangles
	TATVector normal;
	TATVector v0, v1, v2;
	unsigned short byteCount = 0;
	unsigned int count = 0;
	list<TATSTLFace*> faces;

	while (count < FaceSize)
	{
		fread(&normal, 12, 1, fInput);
		fread(&v0, 12, 1, fInput);
		fread(&v1, 12, 1, fInput);
		fread(&v2, 12, 1, fInput);
		fread(&byteCount, 2, 1, fInput);

		v0.X *= scale.X; v0.Y *= scale.Y; v0.Z *= scale.Z;
		v1.X *= scale.X; v1.Y *= scale.Y; v1.Z *= scale.Z;
		v2.X *= scale.X; v2.Y *= scale.Y; v2.Z *= scale.Z;

		TATSTLFace* newFace = new TATSTLFace();
		faces.push_back(newFace);
		newFace->Normal = normal;
		
		newFace->Vertex0 = v0;
		newFace->Vertex1 = v1;
		newFace->Vertex2 = v2;

		count++;
	}
	fclose(fInput);

	// Init for opengl
	InitVertices(faces);
	InitIndices();
	InitBuffers();
	Loaded = true;

	// Delete faces
	list<TATSTLFace*>::iterator iteF = faces.begin();
	while (iteF != faces.end()) {
		TATSTLFace* face = *iteF;
		delete face;
		iteF++;
	}
	faces.clear();
}
void TATSTLMesh::LoadBin(char* filePath, float scale)
{
	FILE* fInput = fopen(filePath, "rb");
	if (fInput == NULL)
		return;

	// Read header
	char header[80];
	fread(header, 1, 80, fInput);
	// Read number of triangles
	fread(&FaceSize, 4, 1, fInput);

	// Read data for all triangles
	TATVector normal;
	TATVector v0, v1, v2;
	unsigned short byteCount = 0;
	unsigned int count = 0;
	list<TATSTLFace*> faces;

	while (count < FaceSize)
	{
		fread(&normal, 12, 1, fInput);
		fread(&v0, 12, 1, fInput);
		fread(&v1, 12, 1, fInput);
		fread(&v2, 12, 1, fInput);
		fread(&byteCount, 2, 1, fInput);

		TATSTLFace* newFace = new TATSTLFace();
		faces.push_back(newFace);
		newFace->Normal = normal;
		newFace->Vertex0 = v0.ScaleMe(scale);
		newFace->Vertex1 = v1.ScaleMe(scale);
		newFace->Vertex2 = v2.ScaleMe(scale);

		count++;
	}
	fclose(fInput);

	// Init for opengl
	InitVertices(faces);
	InitIndices();
	InitBuffers();
	Loaded = true;

	// Delete faces
	list<TATSTLFace*>::iterator iteF = faces.begin();
	while (iteF != faces.end()) {
		TATSTLFace* face = *iteF;
		delete face;
		iteF++;
	}
	faces.clear();
}
void TATSTLMesh::LoadASCII(char* filePath, float scale)
{

}
void TATSTLMesh::Load(char* filePath, float scale)
{

}
void TATSTLMesh::CheckFileMode(char* filePath, int &mode)
{

}
void TATSTLMesh::InitVertices(list<TATSTLFace*>& faces)
{
	int verticesSize = 3 * FaceSize;
	DataPositions = new float[3 * verticesSize];
	DataNormals = new float[3 * verticesSize];
	int iVF = 0;
	list<TATSTLFace*>::iterator iteF = faces.begin();
	for (int iF = 0; iF < FaceSize; iF++) {
		TATSTLFace* face = *iteF;
		iVF = 9 * iF;

		DataPositions[iVF] = face->Vertex0.X;
		DataPositions[iVF + 1] = face->Vertex0.Y;
		DataPositions[iVF + 2] = face->Vertex0.Z;
		DataNormals[iVF] = face->Normal.X;
		DataNormals[iVF + 1] = face->Normal.Y;
		DataNormals[iVF + 2] = face->Normal.Z;

		DataPositions[iVF + 3] = face->Vertex1.X;
		DataPositions[iVF + 4] = face->Vertex1.Y;
		DataPositions[iVF + 5] = face->Vertex1.Z;
		DataNormals[iVF + 3] = face->Normal.X;
		DataNormals[iVF + 4] = face->Normal.Y;
		DataNormals[iVF + 5] = face->Normal.Z;

		DataPositions[iVF + 6] = face->Vertex2.X;
		DataPositions[iVF + 7] = face->Vertex2.Y;
		DataPositions[iVF + 8] = face->Vertex2.Z;
		DataNormals[iVF + 6] = face->Normal.X;
		DataNormals[iVF + 7] = face->Normal.Y;
		DataNormals[iVF + 8] = face->Normal.Z;

		iteF++;
	}
}
void TATSTLMesh::InitIndices()
{
	int indicesSize = 3 * FaceSize;
	DataIndices = new GLuint[indicesSize];
	for (int i = 0; i < indicesSize; i++) {
		DataIndices[i] = i;
	}
}
void TATSTLMesh::InitBuffers()
{
	if (DataPositions == NULL)
		return;
	if (DataNormals == NULL)
		return;
	if (DataIndices == NULL)
		return;

	int verticesSize = GetVerticesSize();
	int indicesSize = GetIndicesSize();

	glGenBuffers(1, &BufferPosition);
	glGenBuffers(1, &BufferNormal);
	glGenBuffers(1, &BufferIndex);

	glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 * sizeof(float), DataNormals, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(GLuint), DataIndices, GL_STATIC_DRAW);

	delete[]DataPositions;
	DataPositions = NULL;
	delete[]DataNormals;
	DataNormals = NULL;
	delete[]DataIndices;
	DataIndices = NULL;
}
void TATSTLMesh::Draw()
{
	if (Loaded) {
		glBindBuffer(GL_ARRAY_BUFFER, BufferPosition);	// Use buffer
		glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a vertex

		glBindBuffer(GL_ARRAY_BUFFER, BufferNormal);	// Use buffer
		glVertexAttribPointer(NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	// Parameters of a normal

		int indicesSize = GetIndicesSize();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIndex);
		glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void*)0);
	}
	
}




