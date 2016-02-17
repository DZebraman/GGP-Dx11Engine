#include "Material.h"



Material::Material(SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader)
{
	vertexShader = _vertexShader;
	pixelShader = _pixelShader;
}


Material::~Material()
{
}
