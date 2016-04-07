#pragma once
#include "DirectXGameCore.h"
class PostProcessBase
{
public:
	PostProcessBase();
	~PostProcessBase();
	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);
};

