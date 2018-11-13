#include "stdafx.h"

#include "DX11QuadDrawer.h"

//--------------------------------------------------------------------------------------
// Constant buffers
//--------------------------------------------------------------------------------------
#pragma pack(push, 1)
struct CB_POW_TWO_RATIOS
{
	float	fWidthoverNextPowOfTwo;
	float	fHeightoverNextPowOfTwo;
	float	fScale;
	float	dummy1;
};
#pragma pack(pop)

ID3D11InputLayout*	DX11QuadDrawer::s_VertexLayout = NULL;
ID3D11Buffer*		DX11QuadDrawer::s_VertexBuffer = NULL;
ID3D11Buffer*		DX11QuadDrawer::s_IndexBuffer = NULL;

ID3D11VertexShader*	DX11QuadDrawer::s_VertexShader = NULL;
ID3D11PixelShader*	DX11QuadDrawer::s_PixelShaderFloat = NULL;
ID3D11PixelShader*	DX11QuadDrawer::s_PixelShaderRGBA = NULL;
ID3D11PixelShader*	DX11QuadDrawer::s_PixelShader3 = NULL;

ID3D11Buffer*		DX11QuadDrawer::s_CBquad = NULL;

ID3D11SamplerState*	DX11QuadDrawer::s_PointSampler = NULL;

ID3D11Buffer*       DX11QuadDrawer::s_pcbVSPowTwoRatios = NULL;

ID3D11Texture2D**           DX11QuadDrawer::m_labelTexture = NULL;
ID3D11ShaderResourceView**  DX11QuadDrawer::m_labelTextureSRV = NULL;
ID3D11UnorderedAccessView** DX11QuadDrawer::m_labelTextureUAV = NULL;

ID3D11Texture2D*            DX11QuadDrawer::m_coloredLabelTexture = NULL;
ID3D11ShaderResourceView*   DX11QuadDrawer::m_coloredLabelTextureSRV = NULL;
ID3D11UnorderedAccessView*  DX11QuadDrawer::m_coloredLabelTextureUAV = NULL;

unsigned int DX11QuadDrawer::m_imageWidth = 0;
unsigned int DX11QuadDrawer::m_imageHeight = 0;

// static variables

DX11QuadDrawer::DX11QuadDrawer()
{
}

DX11QuadDrawer::~DX11QuadDrawer()
{
}

HRESULT DX11QuadDrawer::OnD3D11CreateDevice( ID3D11Device* pd3dDevice )
{
	HRESULT hr = S_OK;

	ID3DBlob* pBlob = NULL;

	V_RETURN(CompileShaderFromFile(L"Shaders/QuadDrawer.hlsl", "QuadVS", "vs_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &s_VertexShader));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );
	V_RETURN(pd3dDevice->CreateInputLayout(layout, numElements, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &s_VertexLayout));
	SAFE_RELEASE(pBlob);

	V_RETURN(CompileShaderFromFile(L"Shaders/QuadDrawer.hlsl", "QuadFloatPS", "ps_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &s_PixelShaderFloat));
	SAFE_RELEASE(pBlob);

	V_RETURN(CompileShaderFromFile(L"Shaders/QuadDrawer.hlsl", "QuadRGBAPS", "ps_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &s_PixelShaderRGBA));
	SAFE_RELEASE(pBlob);

	//V_RETURN(CompileShaderFromFile(L"QuadDrawer.hlsl", "QuadPS2", "ps_5_0", &pBlob));
	//V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_PixelShader2));
	//SAFE_RELEASE(pBlob);

	//V_RETURN(CompileShaderFromFile(L"Float4Pyramid.hlsl", "QuadPS3", "ps_5_0", &pBlob));
	//V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &s_PixelShader3));
	//SAFE_RELEASE(pBlob);

	SimpleVertex vertices[] =
	{
		{ float3( -1.0f, -1.0f, 0.0f ), float2( 0.0f, 1.0f ) },
		{ float3( 1.0f, -1.0f, 0.0f ),  float2( 1.0f, 1.0f ) },
		{ float3( 1.0f, 1.0f, 0.0f ),   float2( 1.0f, 0.0f ) },
		{ float3( -1.0f, 1.0f, 0.0f ),  float2( 0.0f, 0.0f ) }
	};

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(SimpleVertex) * 4;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = vertices;
	V_RETURN( pd3dDevice->CreateBuffer( &desc, &InitData, &s_VertexBuffer ) );

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof( CB_QUAD );
	V_RETURN( pd3dDevice->CreateBuffer( &desc, NULL, &s_CBquad ) );

	// Create and set index buffer
	DWORD indices[] =
	{
		2,1,0,//0,1,2,
		0,3,2//2,3,0
	};

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof( DWORD ) * 6;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	InitData.pSysMem = indices;
	V_RETURN(pd3dDevice->CreateBuffer( &desc, &InitData, &s_IndexBuffer ));

	D3D11_SAMPLER_DESC sdesc;
	ZeroMemory(&sdesc, sizeof(sdesc));
	sdesc.AddressU =  D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.AddressV =  D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.AddressW =  D3D11_TEXTURE_ADDRESS_CLAMP;
	sdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	V_RETURN(pd3dDevice->CreateSamplerState(&sdesc, &s_PointSampler));

	// constant buffers---------------------------------------------------------------------------
	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_BUFFER_DESC));
	Desc.Usage = D3D11_USAGE_DYNAMIC;
	Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Desc.MiscFlags = 0;

	Desc.ByteWidth = sizeof( CB_POW_TWO_RATIOS );
	V_RETURN( pd3dDevice->CreateBuffer( &Desc, NULL, &s_pcbVSPowTwoRatios ) );
	//--------------------------------------------------------------------------------------------

	m_labelTexture    = new ID3D11Texture2D*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM];
	m_labelTextureSRV = new ID3D11ShaderResourceView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM];
	m_labelTextureUAV = new ID3D11UnorderedAccessView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM];

	D3D11_TEXTURE2D_DESC TexDesc = { 0 };
	TexDesc.Width = 640;
	TexDesc.Height = 480;
	TexDesc.MipLevels = 1;
	TexDesc.ArraySize = 1;
	TexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.SampleDesc.Quality = 0;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	TexDesc.CPUAccessFlags = 0;
	TexDesc.MiscFlags = 0;

	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		V_RETURN(pd3dDevice->CreateTexture2D(&TexDesc, NULL, &m_labelTexture[i]));
		V_RETURN(pd3dDevice->CreateShaderResourceView(m_labelTexture[i], NULL, &m_labelTextureSRV[i]));
		V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_labelTexture[i], NULL, &m_labelTextureUAV[i]));
	}

	V_RETURN(pd3dDevice->CreateTexture2D(&TexDesc, NULL, &m_coloredLabelTexture));
	V_RETURN(pd3dDevice->CreateShaderResourceView(m_coloredLabelTexture, NULL, &m_coloredLabelTextureSRV));
	V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_coloredLabelTexture, NULL, &m_coloredLabelTextureUAV));

	m_imageWidth = GlobalAppState::getInstance().s_windowWidth;
	m_imageHeight = GlobalAppState::getInstance().s_windowHeight;

	return hr;
}

void DX11QuadDrawer::OnD3D11DestroyDevice()
{
	SAFE_RELEASE(s_VertexShader);
	SAFE_RELEASE(s_PixelShaderFloat);
	SAFE_RELEASE(s_PixelShaderRGBA);
	SAFE_RELEASE(s_PixelShader3);
	SAFE_RELEASE(s_VertexLayout);
	SAFE_RELEASE(s_VertexBuffer);
	SAFE_RELEASE(s_IndexBuffer);
	SAFE_RELEASE(s_CBquad);
	SAFE_RELEASE(s_PointSampler);
	SAFE_RELEASE(s_pcbVSPowTwoRatios);

	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		SAFE_RELEASE(m_labelTexture[i]);
		SAFE_RELEASE(m_labelTextureSRV[i]);
		SAFE_RELEASE(m_labelTextureUAV[i]);
	}

	SAFE_DELETE_ARRAY(m_labelTexture);
	SAFE_DELETE_ARRAY(m_labelTextureSRV);
	SAFE_DELETE_ARRAY(m_labelTextureUAV);

	SAFE_RELEASE(m_coloredLabelTexture);
	SAFE_RELEASE(m_coloredLabelTextureSRV);
	SAFE_RELEASE(m_coloredLabelTextureUAV);
}

void DX11QuadDrawer::RenderQuad( ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* srv, float scale, float2 Pow2Ratios, ID3D11PixelShader* pixelShader)
{
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pd3dDeviceContext->Map( s_pcbVSPowTwoRatios, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	CB_POW_TWO_RATIOS* pCSPowTwoRatios = ( CB_POW_TWO_RATIOS* )MappedResource.pData;
	pCSPowTwoRatios->fWidthoverNextPowOfTwo = Pow2Ratios.x;
	pCSPowTwoRatios->fHeightoverNextPowOfTwo = Pow2Ratios.y;
	pCSPowTwoRatios->fScale = scale;
	pd3dDeviceContext->Unmap( s_pcbVSPowTwoRatios, 0 );

	pd3dDeviceContext->VSSetConstantBuffers( 10, 1, &s_pcbVSPowTwoRatios );
	pd3dDeviceContext->PSSetConstantBuffers( 10, 1, &s_pcbVSPowTwoRatios );

	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetInputLayout(s_VertexLayout);
	pd3dDeviceContext->VSSetShader(s_VertexShader, NULL, 0);

	if (!pixelShader) 
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desSRV;
		srv->GetDesc( &desSRV);
	
		//RGBA
		if (desSRV.Format == DXGI_FORMAT_R8G8B8A8_UNORM || 
			desSRV.Format == DXGI_FORMAT_R32G32B32A32_FLOAT ||
			desSRV.Format == DXGI_FORMAT_R16G16B16A16_FLOAT || 
			desSRV.Format == DXGI_FORMAT_B8G8R8A8_UNORM
			)
		{
			pd3dDeviceContext->PSSetShader(s_PixelShaderRGBA, NULL, 0);
		}
		else if (desSRV.Format == DXGI_FORMAT_R32_FLOAT)
		{
			pd3dDeviceContext->PSSetShader(s_PixelShaderFloat, NULL, 0);
		} 
		else 
		{
			assert(false);
		}
	} 
	else 
	{
		pd3dDeviceContext->PSSetShader(pixelShader, NULL, 0);
	}
	
	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &s_VertexBuffer, &stride, &offset);
	pd3dDeviceContext->IASetIndexBuffer(s_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pd3dDeviceContext->PSSetShaderResources(10, 1, &srv);
	pd3dDeviceContext->PSSetSamplers(10, 1, &s_PointSampler);
	pd3dDeviceContext->DrawIndexed(6, 0, 0);

	ID3D11ShaderResourceView* srvNULL[] = {NULL};
	pd3dDeviceContext->PSSetShaderResources(10, 1, srvNULL);
}

void DX11QuadDrawer::RenderQuad(ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView** srv, float scale, float2 Pow2Ratios, ID3D11PixelShader* pixelShader)
{
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		DX11ImageHelper::applySplitLabelToFloat(pd3dDeviceContext, srv, m_labelTextureUAV[i], m_imageWidth, m_imageHeight, GlobalAppState::getInstance().s_labelNum, i);
	}
	
	Eigen::Vector4i colors[LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL];
	if (GlobalAppState::getInstance().s_labelNum == 3)
	{
		// Table
		colors[0] = Eigen::Vector4i(225, 225, 225, 255); colors[1] = Eigen::Vector4i(255, 219, 100, 255); colors[2]  = Eigen::Vector4i(157, 197, 233, 255); colors[3]  = Eigen::Vector4i(255, 255, 255, 255);
		colors[4] = Eigen::Vector4i(255, 255, 255, 255); colors[5] = Eigen::Vector4i(255, 255, 255, 255); colors[6]  = Eigen::Vector4i(255, 255, 255, 255); colors[7]  = Eigen::Vector4i(255, 255, 255, 255);
		colors[8] = Eigen::Vector4i(255, 255, 255, 255); colors[9] = Eigen::Vector4i(255, 255, 255, 255); colors[10] = Eigen::Vector4i(255, 255, 255, 255); colors[11] = Eigen::Vector4i(255, 255, 255, 255);
	}
	else if (GlobalAppState::getInstance().s_labelNum == 5)
	{
		// Chair
		colors[0] = Eigen::Vector4i(225, 225, 225, 255); colors[1] = Eigen::Vector4i(243, 177, 133, 255); colors[2]  = Eigen::Vector4i(255, 219, 100, 255); colors[3]  = Eigen::Vector4i(157, 233, 225, 255);
		colors[4] = Eigen::Vector4i(157, 197, 233, 255); colors[5] = Eigen::Vector4i(255, 255, 255, 255); colors[6]  = Eigen::Vector4i(255, 255, 255, 255); colors[7]  = Eigen::Vector4i(255, 255, 255, 255);
		colors[8] = Eigen::Vector4i(255, 255, 255, 255); colors[9] = Eigen::Vector4i(255, 255, 255, 255); colors[10] = Eigen::Vector4i(255, 255, 255, 255); colors[11] = Eigen::Vector4i(255, 255, 255, 255);
	}
	else if (GlobalAppState::getInstance().s_labelNum == 7)
	{
		// Motorcycle
		colors[0] = Eigen::Vector4i(225, 225, 225, 255); colors[1] = Eigen::Vector4i(157, 197, 233, 255); colors[2]  = Eigen::Vector4i(243, 177, 133, 255); colors[3]  = Eigen::Vector4i(233, 157, 165, 255);
		colors[4] = Eigen::Vector4i(255, 219, 100, 255); colors[5] = Eigen::Vector4i(157, 233, 225, 255); colors[6]  = Eigen::Vector4i(167, 211, 143, 255); colors[7]  = Eigen::Vector4i(255, 255, 255, 255);
		colors[8] = Eigen::Vector4i(255, 255, 255, 255); colors[9] = Eigen::Vector4i(255, 255, 255, 255); colors[10] = Eigen::Vector4i(255, 255, 255, 255); colors[11] = Eigen::Vector4i(255, 255, 255, 255);
	}

	DX11ImageHelper::applyInputLabelToRGBW(pd3dDeviceContext, m_labelTextureSRV, m_coloredLabelTextureUAV, colors, m_imageWidth, m_imageHeight, GlobalAppState::getInstance().s_labelNum);
	//for (unsigned int i = 0; i < GlobalAppState::getInstance().s_labelNum; i++)
	//{
	//	DX11ImageHelper::applyInputLabelToRGBW(pd3dDeviceContext, m_labelTextureSRV[i], m_coloredLabelTextureUAV, color[i], m_imageWidth, m_imageHeight, i);
	//}

	// test
	//DX11ImageHelper::applyInputLabelToRGBW(pd3dDeviceContext, m_labelTextureSRV[1], m_coloredLabelTextureUAV, color[1], m_imageWidth, m_imageHeight, 0);
	// test

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pd3dDeviceContext->Map(s_pcbVSPowTwoRatios, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	CB_POW_TWO_RATIOS* pCSPowTwoRatios = (CB_POW_TWO_RATIOS*)MappedResource.pData;
	pCSPowTwoRatios->fWidthoverNextPowOfTwo = Pow2Ratios.x;
	pCSPowTwoRatios->fHeightoverNextPowOfTwo = Pow2Ratios.y;
	pCSPowTwoRatios->fScale = scale;
	pd3dDeviceContext->Unmap(s_pcbVSPowTwoRatios, 0);

	pd3dDeviceContext->VSSetConstantBuffers(10, 1, &s_pcbVSPowTwoRatios);
	pd3dDeviceContext->PSSetConstantBuffers(10, 1, &s_pcbVSPowTwoRatios);

	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetInputLayout(s_VertexLayout);
	pd3dDeviceContext->VSSetShader(s_VertexShader, NULL, 0);

	if (!pixelShader)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desSRV;
		m_coloredLabelTextureSRV->GetDesc(&desSRV);

		// Label
		if (desSRV.Format == DXGI_FORMAT_R8G8B8A8_UNORM ||
			desSRV.Format == DXGI_FORMAT_R32G32B32A32_FLOAT ||
			desSRV.Format == DXGI_FORMAT_R16G16B16A16_FLOAT ||
			desSRV.Format == DXGI_FORMAT_B8G8R8A8_UNORM
			)
		{
			pd3dDeviceContext->PSSetShader(s_PixelShaderRGBA, NULL, 0);
		}
		else if (desSRV.Format == DXGI_FORMAT_R32_FLOAT)
		{
			pd3dDeviceContext->PSSetShader(s_PixelShaderFloat, NULL, 0);
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		pd3dDeviceContext->PSSetShader(pixelShader, NULL, 0);
	}

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &s_VertexBuffer, &stride, &offset);
	pd3dDeviceContext->IASetIndexBuffer(s_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pd3dDeviceContext->PSSetShaderResources(10, 1, &m_coloredLabelTextureSRV);
	pd3dDeviceContext->PSSetSamplers(10, 1, &s_PointSampler);
	pd3dDeviceContext->DrawIndexed(6, 0, 0);

	ID3D11ShaderResourceView* srvNULL[] = { NULL };
	pd3dDeviceContext->PSSetShaderResources(10, 1, srvNULL);
}

void DX11QuadDrawer::RenderQuad( ID3D11DeviceContext* pd3dDeviceContext, ID3D11PixelShader* pixelShader, ID3D11ShaderResourceView** srvs, UINT numShaderResourceViews, float2 Pow2Ratios /*= float2(1.0f, 1.0f)*/ )
{
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pd3dDeviceContext->Map( s_pcbVSPowTwoRatios, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
	CB_POW_TWO_RATIOS* pCSPowTwoRatios = ( CB_POW_TWO_RATIOS* )MappedResource.pData;
	pCSPowTwoRatios->fWidthoverNextPowOfTwo = Pow2Ratios.x;
	pCSPowTwoRatios->fHeightoverNextPowOfTwo = Pow2Ratios.y;
	pd3dDeviceContext->Unmap( s_pcbVSPowTwoRatios, 0 );

	pd3dDeviceContext->VSSetConstantBuffers( 10, 1, &s_pcbVSPowTwoRatios );
	pd3dDeviceContext->PSSetConstantBuffers( 10, 1, &s_pcbVSPowTwoRatios );

	pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dDeviceContext->IASetInputLayout(s_VertexLayout);

	pd3dDeviceContext->VSSetShader(s_VertexShader, NULL, 0);
	pd3dDeviceContext->PSSetShader(pixelShader, NULL, 0);

	UINT stride = sizeof( SimpleVertex );
	UINT offset = 0;
	pd3dDeviceContext->IASetVertexBuffers(0, 1, &s_VertexBuffer, &stride, &offset);
	pd3dDeviceContext->IASetIndexBuffer(s_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pd3dDeviceContext->PSSetShaderResources(10, numShaderResourceViews, srvs);
	pd3dDeviceContext->PSSetSamplers(10, 1, &s_PointSampler);

	pd3dDeviceContext->DrawIndexed(6, 0, 0);

	pd3dDeviceContext->VSSetShader(NULL, NULL, 0);
	pd3dDeviceContext->PSSetShader(NULL, NULL, 0);
	ID3D11SamplerState* samplerNULL[] = {NULL};
	pd3dDeviceContext->PSSetSamplers(10, 1, samplerNULL);

	ID3D11ShaderResourceView* srvsNULL[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	pd3dDeviceContext->PSSetShaderResources(10, numShaderResourceViews, srvsNULL);
}