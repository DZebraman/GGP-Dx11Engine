#include "PostProcessBase.h"



PostProcessBase::PostProcessBase()
{
}


PostProcessBase::~PostProcessBase()
{
}

ID3D11ShaderResourceView* PostProcessBase::draw(ID3D11ShaderResourceView* ppSRV){
	return ppSRV;
}