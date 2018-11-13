#include "stdafx.h"
#include "GlobalAppState.h"
#include "DX11PhongLighting.h"

ID3D11Buffer*               DX11PhongLighting::s_ConstantBuffer = 0;
ID3D11PixelShader*          DX11PhongLighting::s_PixelShaderPhong = 0;

ID3D11Texture2D**           DX11PhongLighting::m_labelTexture = NULL;
ID3D11ShaderResourceView**  DX11PhongLighting::m_labelTextureSRV = NULL;
ID3D11UnorderedAccessView** DX11PhongLighting::m_labelTextureUAV = NULL;

ID3D11Texture2D*            DX11PhongLighting::m_coloredLabelTexture = NULL;
ID3D11ShaderResourceView*   DX11PhongLighting::m_coloredLabelTextureSRV = NULL;
ID3D11UnorderedAccessView*  DX11PhongLighting::m_coloredLabelTextureUAV = NULL;

// Stereo
ID3D11Texture2D*            DX11PhongLighting::s_pDepthStencilStereo = NULL;
ID3D11DepthStencilView*     DX11PhongLighting::s_pDepthStencilStereoDSV = NULL;
ID3D11ShaderResourceView*   DX11PhongLighting::s_pDepthStencilStereoSRV = NULL;

ID3D11Texture2D*            DX11PhongLighting::s_pColorsStereo = NULL;
ID3D11ShaderResourceView*   DX11PhongLighting::s_pColorsStereoSRV = NULL;
ID3D11RenderTargetView*     DX11PhongLighting::s_pColorsStereoRTV = NULL;

unsigned int DX11PhongLighting::m_imageWidth = 0;
unsigned int DX11PhongLighting::m_imageHeight = 0;

HRESULT DX11PhongLighting::OnD3D11CreateDevice( ID3D11Device* pd3dDevice )
{
	HRESULT hr = S_OK;

	ID3DBlob* pBlob = NULL;

	V_RETURN(CompileShaderFromFile(L"Shaders/PhongLighting.hlsl", "PhongPS", "ps_5_0", &pBlob));
	V_RETURN(pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &s_PixelShaderPhong));

	SAFE_RELEASE(pBlob);

	// Constant Buffer
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.ByteWidth = sizeof(cbConstant);

	V_RETURN(pd3dDevice->CreateBuffer(&cbDesc, NULL, &s_ConstantBuffer));

	m_labelTexture = new ID3D11Texture2D*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM];
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

	// Stereo off screen buffer
	if (GlobalAppState::getInstance().s_stereoEnabled)
	{
		D3D11_TEXTURE2D_DESC descTex;
		ZeroMemory(&descTex, sizeof(D3D11_TEXTURE2D_DESC));
		descTex.Usage = D3D11_USAGE_DEFAULT;
		descTex.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		descTex.CPUAccessFlags = 0;
		descTex.MiscFlags = 0;
		descTex.SampleDesc.Count = 1;
		descTex.SampleDesc.Quality = 0;
		descTex.ArraySize = 1;
		descTex.MipLevels = 1;
		descTex.Format = DXGI_FORMAT_R32_TYPELESS;
		descTex.Width = GlobalAppState::getInstance().s_windowWidthStereo;
		descTex.Height = GlobalAppState::getInstance().s_windowHeightStereo;
		V_RETURN(pd3dDevice->CreateTexture2D(&descTex, NULL, &s_pDepthStencilStereo));

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC) );
		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		V_RETURN(pd3dDevice->CreateDepthStencilView(s_pDepthStencilStereo, &descDSV, &s_pDepthStencilStereoDSV));

		D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
		ZeroMemory(&descSRV, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		descSRV.Format = DXGI_FORMAT_R32_FLOAT;
		descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		descSRV.Texture2D.MipLevels = 1;
		descSRV.Texture2D.MostDetailedMip = 0;
		V_RETURN(pd3dDevice->CreateShaderResourceView(s_pDepthStencilStereo, &descSRV, &s_pDepthStencilStereoSRV));

		descTex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		descTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//_SRGB; //DXGI_FORMAT_R8G8B8A8_UNORM_SRGB?
		V_RETURN(pd3dDevice->CreateTexture2D(&descTex, NULL, &s_pColorsStereo));
		V_RETURN(pd3dDevice->CreateShaderResourceView(s_pColorsStereo, NULL, &s_pColorsStereoSRV));
		V_RETURN(pd3dDevice->CreateRenderTargetView(s_pColorsStereo, NULL, &s_pColorsStereoRTV));
	}

	m_imageWidth = GlobalAppState::getInstance().s_windowWidth;
	m_imageHeight = GlobalAppState::getInstance().s_windowHeight;

	return hr;
}

void DX11PhongLighting::render(ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* positions, ID3D11ShaderResourceView* normals, ID3D11ShaderResourceView* colors, ID3D11ShaderResourceView* ssaoMap, bool useMaterial, bool useSSAO)
{
	// Initialize Constant Buffers
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pd3dDeviceContext->Map(s_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) return;

	cbConstant *cbufferConstant = (cbConstant*)mappedResource.pData;
	cbufferConstant->useMaterial = (int)useMaterial;
	cbufferConstant->useSSAO = (int)useSSAO;
	pd3dDeviceContext->Unmap(s_ConstantBuffer, 0);

	pd3dDeviceContext->PSSetConstantBuffers(0, 1, &s_ConstantBuffer);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(pd3dDeviceContext);
	pd3dDeviceContext->PSSetConstantBuffers(8, 1, &CBGlobalAppState);

	ID3D11ShaderResourceView* srvs[] = { positions, normals, colors, ssaoMap };
	DX11QuadDrawer::RenderQuad(pd3dDeviceContext, s_PixelShaderPhong, srvs, 4);

	ID3D11Buffer* nullCB[] = { NULL };
	pd3dDeviceContext->PSSetConstantBuffers(0, 1, nullCB);
	pd3dDeviceContext->PSSetConstantBuffers(8, 1, nullCB);
}

void DX11PhongLighting::render( ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* positions, ID3D11ShaderResourceView* normals, ID3D11ShaderResourceView** labels, ID3D11ShaderResourceView* ssaoMap, bool useMaterial, bool useSSAO )
{
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		DX11ImageHelper::applySplitLabelToFloat(pd3dDeviceContext, labels, m_labelTextureUAV[i], m_imageWidth, m_imageHeight, GlobalAppState::getInstance().s_labelNum, i);
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
	//else if (GlobalAppState::getInstance().s_labelNum == 7)
	//{
	//	// Motorcycle
	//	colors[0] = Eigen::Vector4i(225, 225, 225, 255); colors[1] = Eigen::Vector4i(157, 197, 233, 255); colors[2]  = Eigen::Vector4i(243, 177, 133, 255); colors[3]  = Eigen::Vector4i(233, 157, 165, 255);
	//	colors[4] = Eigen::Vector4i(255, 219, 100, 255); colors[5] = Eigen::Vector4i(157, 233, 225, 255); colors[6]  = Eigen::Vector4i(167, 211, 143, 255); colors[7]  = Eigen::Vector4i(255, 255, 255, 255);
	//	colors[8] = Eigen::Vector4i(255, 255, 255, 255); colors[9] = Eigen::Vector4i(255, 255, 255, 255); colors[10] = Eigen::Vector4i(255, 255, 255, 255); colors[11] = Eigen::Vector4i(255, 255, 255, 255);
	//}
	else if (GlobalAppState::getInstance().s_labelNum == 7)
	{
		// Motorcycle
		colors[0] = Eigen::Vector4i(225, 225, 225, 255); colors[1] = Eigen::Vector4i(157, 197, 233, 255); colors[2]  = Eigen::Vector4i(157, 233, 225, 255); colors[3]  = Eigen::Vector4i(233, 157, 165, 255);
		colors[4] = Eigen::Vector4i(167, 211, 143, 255); colors[5] = Eigen::Vector4i(255, 219, 100, 255); colors[6]  = Eigen::Vector4i(243, 177, 133, 255); colors[7]  = Eigen::Vector4i(255, 255, 255, 255);
		colors[8] = Eigen::Vector4i(255, 255, 255, 255); colors[9] = Eigen::Vector4i(255, 255, 255, 255); colors[10] = Eigen::Vector4i(255, 255, 255, 255); colors[11] = Eigen::Vector4i(255, 255, 255, 255);
	}

	DX11ImageHelper::applyInputLabelToRGBW(pd3dDeviceContext, m_labelTextureSRV, m_coloredLabelTextureUAV, colors, m_imageWidth, m_imageHeight, GlobalAppState::getInstance().s_labelNum);
	//for (unsigned int i = 0; i < GlobalAppState::getInstance().s_labelNum; i++)
	//{
	//	DX11ImageHelper::applyInputLabelToRGBW(pd3dDeviceContext, m_labelTextureSRV[i], m_coloredLabelTextureUAV, color[i], m_imageWidth, m_imageHeight, i);
	//}

	// test
	//DX11ImageHelper::applyInputLabelToRGBW(pd3dDeviceContext, m_labelTextureSRV[0], m_coloredLabelTextureUAV, color[0], m_imageWidth, m_imageHeight, 0);
	// test
	
	// Initialize Constant Buffers
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pd3dDeviceContext->Map(s_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) return;

	cbConstant *cbufferConstant = (cbConstant*)mappedResource.pData;
	cbufferConstant->useMaterial = (int)useMaterial;
	cbufferConstant->useSSAO = (int)useSSAO;
	pd3dDeviceContext->Unmap(s_ConstantBuffer, 0);

	pd3dDeviceContext->PSSetConstantBuffers(0, 1, &s_ConstantBuffer);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(pd3dDeviceContext);
	pd3dDeviceContext->PSSetConstantBuffers(8, 1, &CBGlobalAppState);

	ID3D11ShaderResourceView* srvs[] = { positions, normals, m_coloredLabelTextureSRV, ssaoMap };
	DX11QuadDrawer::RenderQuad(pd3dDeviceContext, s_PixelShaderPhong, srvs, 4);

	ID3D11Buffer* nullCB[] = { NULL };
	pd3dDeviceContext->PSSetConstantBuffers(0, 1, nullCB);
	pd3dDeviceContext->PSSetConstantBuffers(8, 1, nullCB);
}

/*
void DX11PhongLighting::renderStereo( ID3D11DeviceContext* pd3dDeviceContext, ID3D11ShaderResourceView* positions, ID3D11ShaderResourceView* normals, ID3D11ShaderResourceView* colors, ID3D11ShaderResourceView** labels, ID3D11ShaderResourceView* ssaoMap, bool useMaterial, bool useSSAO )
{
	// Initialize Constant Buffers
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = pd3dDeviceContext->Map(s_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr)) return;

	cbConstant *cbufferConstant = (cbConstant*)mappedResource.pData;
	cbufferConstant->useMaterial = (int)useMaterial;
	cbufferConstant->useSSAO = (int)useSSAO;
	pd3dDeviceContext->Unmap(s_ConstantBuffer, 0);

	pd3dDeviceContext->PSSetConstantBuffers(0, 1, &s_ConstantBuffer);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(pd3dDeviceContext);
	pd3dDeviceContext->PSSetConstantBuffers(8, 1, &CBGlobalAppState);

	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
	pd3dDeviceContext->ClearRenderTargetView(s_pColorsStereoRTV, ClearColor);
	pd3dDeviceContext->ClearDepthStencilView(s_pDepthStencilStereoDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	pd3dDeviceContext->OMSetRenderTargets(1, &s_pColorsStereoRTV, s_pDepthStencilStereoDSV);

	ID3D11ShaderResourceView* srvs[] = {positions, normals, colors, ssaoMap};
	DX11QuadDrawer::RenderQuad(pd3dDeviceContext, s_PixelShaderPhong, srvs, 4);

	ID3D11RenderTargetView* rtv = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* dsv = DXUTGetD3D11DepthStencilView();
	pd3dDeviceContext->OMSetRenderTargets(1, &rtv, dsv);

	ID3D11Buffer* nullCB[] = { NULL };
	pd3dDeviceContext->PSSetConstantBuffers(0, 1, nullCB);
	pd3dDeviceContext->PSSetConstantBuffers(8, 1, nullCB);
}
*/

void DX11PhongLighting::OnD3D11DestroyDevice()
{
	SAFE_RELEASE(s_PixelShaderPhong);
	SAFE_RELEASE(s_ConstantBuffer);

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

	// Stereo
	SAFE_RELEASE(s_pDepthStencilStereo);
	SAFE_RELEASE(s_pDepthStencilStereoDSV);
	SAFE_RELEASE(s_pDepthStencilStereoSRV);

	SAFE_RELEASE(s_pColorsStereo);
	SAFE_RELEASE(s_pColorsStereoSRV);
	SAFE_RELEASE(s_pColorsStereoRTV);
}