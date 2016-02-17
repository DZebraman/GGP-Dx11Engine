#include "Mesh.h"

using namespace DirectX;

Mesh::Mesh(ID3D11Device* _device, ID3D11DeviceContext * _deviceContext) {
	device = _device;
	deviceContext = _deviceContext;
}

void Mesh::CreateGeometry(const char * path) {
	MeshLoader mesh = MeshLoader(path);
	CreateGeometry(mesh.getVerts(), mesh.getIndices(), mesh.getNumVerts(), mesh.getNumInd());
}

void Mesh::CreateGeometry(Vertex * vertexArray, unsigned int * indices, int numVerts, int _numIndices)
{
	//LoadShaders(device, deviceContext);

	numIndices = _numIndices;

	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVerts;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertexArray;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&vbd, &initialVertexData, &vBuffer));

	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * numIndices;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&ibd, &initialIndexData, &iBuffer));
}


Mesh::~Mesh()
{
	ReleaseMacro(vBuffer);
	ReleaseMacro(iBuffer);
	/*ReleaseMacro(device);
	ReleaseMacro(deviceContext);*/


	delete vBuffer;
	delete iBuffer;
	/*delete device;
	delete deviceContext;*/
}

ID3D11Buffer * Mesh::GetVertexBuffer() {
	return vBuffer;
}

ID3D11Buffer * Mesh::GetIndexBuffer() {
	return iBuffer;
}

int Mesh::GetIndexCount() {
	return numIndices;
}


void Mesh::Draw() {

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	deviceContext->DrawIndexed(
		numIndices,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

}