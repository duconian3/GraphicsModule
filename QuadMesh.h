// QuadMesh.h
// Simple single square mesh (for example purposes).

#ifndef _QUADMESH_H_
#define _QUADMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class QuadMesh : public BaseMesh
{

public:
	QuadMesh(ID3D11Device* device, WCHAR* textureFilename);
	~QuadMesh();

protected:
	void InitBuffers(ID3D11Device* device);

};

#endif