#include "PostProcessBase.h"



PostProcessBase::PostProcessBase()
{
}


PostProcessBase::~PostProcessBase()
{
}

ID3D11ShaderResourceView* PostProcessBase::draw(ID3D11ShaderResourceView* ppSRV,ID3D11DepthStencilView* _depthStencilView) {
	return ppSRV;
}