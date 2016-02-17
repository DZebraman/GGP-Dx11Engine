#pragma once
#include <d3d11.h>
#include "Vertex.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "DirectXGameCore.h"

class Material
{
protected:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
public:
	Material(SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader);
	~Material();
};

