#if !defined (_MESH_CLASS)
#define _MESH_CLASS

#include "supportClass.h"

#define SAFE_DELETE(ptr) if(ptr != NULL) delete ptr
#define SAFE_DELETE_ARRAY(ptr) if(ptr != NULL) delete[] ptr;

class VertexID
{
public:
	int		vertIndex;
	int		colorIndex;
	int		normalIndex;
};

class Face
{
public:
	int			nVerts;
	VertexID*	vert;
	Vector3		facenorm;
	unsigned char color[4];
	Face()
	{
		nVerts	= 0;
		vert	= NULL;
	}
	~Face()
	{
		if(vert !=NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int			numVerts;
	Point3*		pt;

	int			numFaces;
	Face*		face;
	int			numNorm;
	Vector3*	norm;

	float		slideX, slideY, slideZ;
	float		rotateX, rotateY, rotateZ;
	float		scaleX, scaleY, scaleZ;

public:
	Mesh()
	{
		numVerts	= 0;
		pt			= NULL;
		numFaces	= 0;
		face		= NULL;

		slideX = slideY = slideZ = 0;
		rotateX = rotateY = rotateZ = 0;
		scaleX = scaleY = scaleZ = 0;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}	
		if(face != NULL)
		{
			delete[] face;
		}

		SAFE_DELETE_ARRAY(norm);
		
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe();
	void Draw();

	void Draw2();
	void SetColor(int colorIdx);
	void setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
};

#endif