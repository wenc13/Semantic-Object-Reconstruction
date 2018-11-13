#include "stdafx.h"

#include "DX11DepthSensor.h"

#include "GlobalAppState.h"

DX11Sensor::DX11Sensor()
{
	m_pDepthTextureUS2D = NULL;
	m_pDepthTextureUSSRV = NULL;

	m_pPreDepthTextureUS2D = NULL;
	m_pPreDepthTextureUSSRV = NULL;

	m_pDepthTextureF2D = NULL;
	m_pDepthTextureFSRV = NULL;
	m_pDepthTextureFUAV = NULL;

	m_pPreDepthTextureF2D = NULL;
	m_pPreDepthTextureFSRV = NULL;
	m_pPreDepthTextureFUAV = NULL;

	m_pDepthTextureFEroded2D = NULL;
	m_pDepthTextureFErodedSRV = NULL;
	m_pDepthTextureFErodedUAV = NULL;

	m_pPreDepthTextureFEroded2D = NULL;
	m_pPreDepthTextureFErodedSRV = NULL;
	m_pPreDepthTextureFErodedUAV = NULL;

	m_pDepthTextureFFiltered2D = NULL;
	m_pDepthTextureFFilteredSRV = NULL;
	m_pDepthTextureFFilteredUAV = NULL;

	m_pPreDepthTextureFFiltered2D = NULL;
	m_pPreDepthTextureFFilteredSRV = NULL;
	m_pPreDepthTextureFFilteredUAV = NULL;

	m_pDepthTextureFloat42D = NULL;
	m_pDepthTextureFloat4SRV = NULL;
	m_pDepthTextureFloat4UAV = NULL;

	m_pPreDepthTextureFloat42D = NULL;
	m_pPreDepthTextureFloat4SRV = NULL;
	m_pPreDepthTextureFloat4UAV = NULL;

	m_pHSVDepthTextureFloat42D = NULL;
	m_pHSVDepthTextureFloat4SRV = NULL;
	m_pHSVDepthTextureFloat4UAV = NULL;

	m_pHSVPreDepthTextureFloat42D = NULL;
	m_pHSVPreDepthTextureFloat4SRV = NULL;
	m_pHSVPreDepthTextureFloat4UAV = NULL;

	m_pNormalTextureFloat42D = NULL;
	m_pNormalTextureFloat4SRV = NULL;
	m_pNormalTextureFloat4UAV = NULL;

	m_pPreNormalTextureFloat42D = NULL;
	m_pPreNormalTextureFloat4SRV = NULL;
	m_pPreNormalTextureFloat4UAV = NULL;

	m_pColorTexture2D = NULL;
	m_pColorTextureSRV= NULL;

	for (int i = 0; i < LABEL_TEXTURE_NUM; i++)
	{
		m_pLabelTexture2D[i] = NULL;
		m_pLabelTextureSRV[i] = NULL;
	}

	m_pPreColorTexture2D = NULL;
	m_pPreColorTextureSRV = NULL;

	for (int i = 0; i < LABEL_TEXTURE_NUM; i++)
	{
		m_pPreLabelTexture2D[i] = NULL;
		m_pPreLabelTextureSRV[i] = NULL;
	}

	m_FrameNumberDepth = 0;
}

DX11Sensor::~DX11Sensor()
{
	OnD3D11DestroyDevice();
}

void DX11Sensor::OnD3D11DestroyDevice()
{
	SAFE_RELEASE(m_pDepthTextureUS2D);
	SAFE_RELEASE(m_pDepthTextureUSSRV);

	SAFE_RELEASE(m_pPreDepthTextureUS2D);
	SAFE_RELEASE(m_pPreDepthTextureUSSRV);

	//! depth texture float
	SAFE_RELEASE(m_pDepthTextureF2D);
	SAFE_RELEASE(m_pDepthTextureFSRV);
	SAFE_RELEASE(m_pDepthTextureFUAV);

	SAFE_RELEASE(m_pPreDepthTextureF2D);
	SAFE_RELEASE(m_pPreDepthTextureFSRV);
	SAFE_RELEASE(m_pPreDepthTextureFUAV);

	SAFE_RELEASE(m_pDepthTextureFEroded2D);
	SAFE_RELEASE(m_pDepthTextureFErodedSRV);
	SAFE_RELEASE(m_pDepthTextureFErodedUAV);

	SAFE_RELEASE(m_pPreDepthTextureFEroded2D);
	SAFE_RELEASE(m_pPreDepthTextureFErodedSRV);
	SAFE_RELEASE(m_pPreDepthTextureFErodedUAV);

	SAFE_RELEASE(m_pDepthTextureFFiltered2D);
	SAFE_RELEASE(m_pDepthTextureFFilteredSRV);
	SAFE_RELEASE(m_pDepthTextureFFilteredUAV);

	SAFE_RELEASE(m_pPreDepthTextureFFiltered2D);
	SAFE_RELEASE(m_pPreDepthTextureFFilteredSRV);
	SAFE_RELEASE(m_pPreDepthTextureFFilteredUAV);

	SAFE_RELEASE(m_pHSVDepthTextureFloat42D);
	SAFE_RELEASE(m_pHSVDepthTextureFloat4SRV);
	SAFE_RELEASE(m_pHSVDepthTextureFloat4UAV);

	SAFE_RELEASE(m_pHSVPreDepthTextureFloat42D);
	SAFE_RELEASE(m_pHSVPreDepthTextureFloat4SRV);
	SAFE_RELEASE(m_pHSVPreDepthTextureFloat4UAV);

	SAFE_RELEASE(m_pDepthTextureFloat42D);
	SAFE_RELEASE(m_pDepthTextureFloat4SRV);
	SAFE_RELEASE(m_pDepthTextureFloat4UAV);

	SAFE_RELEASE(m_pPreDepthTextureFloat42D);
	SAFE_RELEASE(m_pPreDepthTextureFloat4SRV);
	SAFE_RELEASE(m_pPreDepthTextureFloat4UAV);

	SAFE_RELEASE(m_pNormalTextureFloat42D);
	SAFE_RELEASE(m_pNormalTextureFloat4SRV);
	SAFE_RELEASE(m_pNormalTextureFloat4UAV);

	SAFE_RELEASE(m_pPreNormalTextureFloat42D);
	SAFE_RELEASE(m_pPreNormalTextureFloat4SRV);
	SAFE_RELEASE(m_pPreNormalTextureFloat4UAV);

	SAFE_RELEASE(m_pColorTexture2D);
	SAFE_RELEASE(m_pColorTextureSRV);
	
	for (int i = 0; i < LABEL_TEXTURE_NUM; i++)
	{
		SAFE_RELEASE(m_pLabelTexture2D[i]);
		SAFE_RELEASE(m_pLabelTextureSRV[i]);
	}

	SAFE_RELEASE(m_pPreColorTexture2D);
	SAFE_RELEASE(m_pPreColorTextureSRV);

	for (int i = 0; i < LABEL_TEXTURE_NUM; i++)
	{
		SAFE_RELEASE(m_pPreLabelTexture2D[i]);
		SAFE_RELEASE(m_pPreLabelTextureSRV[i]);
	}
}

HRESULT DX11Sensor::OnD3D11CreateDevice( ID3D11Device* device, DepthSensor* depthSensor )
{
	HRESULT hr = S_OK;

	m_depthSensor = depthSensor;
	
	// Create depth texture
	D3D11_TEXTURE2D_DESC depthTexDesc = {0};
	depthTexDesc.Width = GlobalAppState::getInstance().s_windowWidth;
	depthTexDesc.Height = GlobalAppState::getInstance().s_windowHeight;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.Format = DXGI_FORMAT_R16_UINT;
	depthTexDesc.SampleDesc.Count = 1;
	depthTexDesc.SampleDesc.Quality = 0;
	depthTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	depthTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	depthTexDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pDepthTextureUS2D);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pPreDepthTextureUS2D);
	if (FAILED(hr)) { return hr; }

	depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTexDesc.CPUAccessFlags = 0;
	depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	depthTexDesc.Format = DXGI_FORMAT_R32_FLOAT;

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pDepthTextureF2D);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pPreDepthTextureF2D);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pDepthTextureFEroded2D);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pPreDepthTextureFEroded2D);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pDepthTextureFFiltered2D);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pPreDepthTextureFFiltered2D);
	if (FAILED(hr)) { return hr; }

	depthTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pHSVDepthTextureFloat42D);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pHSVPreDepthTextureFloat42D);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pDepthTextureFloat42D);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pPreDepthTextureFloat42D);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pNormalTextureFloat42D);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateTexture2D(&depthTexDesc, NULL, &m_pPreNormalTextureFloat42D);
	if (FAILED(hr)) { return hr; }

	//create shader resource views
	hr = device->CreateShaderResourceView(m_pDepthTextureUS2D, NULL, &m_pDepthTextureUSSRV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pPreDepthTextureUS2D, NULL, &m_pPreDepthTextureUSSRV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pDepthTextureF2D, NULL, &m_pDepthTextureFSRV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pPreDepthTextureF2D, NULL, &m_pPreDepthTextureFSRV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pDepthTextureFEroded2D, NULL, &m_pDepthTextureFErodedSRV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pPreDepthTextureFEroded2D, NULL, &m_pPreDepthTextureFErodedSRV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pDepthTextureFFiltered2D, NULL, &m_pDepthTextureFFilteredSRV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pPreDepthTextureFFiltered2D, NULL, &m_pPreDepthTextureFFilteredSRV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pHSVDepthTextureFloat42D, NULL, &m_pHSVDepthTextureFloat4SRV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pHSVPreDepthTextureFloat42D, NULL, &m_pHSVPreDepthTextureFloat4SRV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pDepthTextureFloat42D, NULL, &m_pDepthTextureFloat4SRV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pPreDepthTextureFloat42D, NULL, &m_pPreDepthTextureFloat4SRV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pNormalTextureFloat42D, NULL, &m_pNormalTextureFloat4SRV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateShaderResourceView(m_pPreNormalTextureFloat42D, NULL, &m_pPreNormalTextureFloat4SRV);
	if (FAILED(hr)) { return hr; }

	//create unordered access views
	hr = device->CreateUnorderedAccessView(m_pDepthTextureF2D, NULL, &m_pDepthTextureFUAV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateUnorderedAccessView(m_pPreDepthTextureF2D, NULL, &m_pPreDepthTextureFUAV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateUnorderedAccessView(m_pDepthTextureFEroded2D, NULL, &m_pDepthTextureFErodedUAV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateUnorderedAccessView(m_pPreDepthTextureFEroded2D, NULL, &m_pPreDepthTextureFErodedUAV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateUnorderedAccessView(m_pDepthTextureFFiltered2D, NULL, &m_pDepthTextureFFilteredUAV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateUnorderedAccessView(m_pPreDepthTextureFFiltered2D, NULL, &m_pPreDepthTextureFFilteredUAV);
	if (FAILED(hr)) { return hr; }

	hr = device->CreateUnorderedAccessView(m_pHSVDepthTextureFloat42D, NULL, &m_pHSVDepthTextureFloat4UAV);
	if ( FAILED(hr)) { return hr; }

	hr = device->CreateUnorderedAccessView(m_pHSVPreDepthTextureFloat42D, NULL, &m_pHSVPreDepthTextureFloat4UAV);
	if (FAILED(hr)) { return hr; }

	V_RETURN(device->CreateUnorderedAccessView(m_pDepthTextureFloat42D, NULL, &m_pDepthTextureFloat4UAV));
	V_RETURN(device->CreateUnorderedAccessView(m_pPreDepthTextureFloat42D, NULL, &m_pPreDepthTextureFloat4UAV));
	V_RETURN(device->CreateUnorderedAccessView(m_pNormalTextureFloat42D, NULL, &m_pNormalTextureFloat4UAV));
	V_RETURN(device->CreateUnorderedAccessView(m_pPreNormalTextureFloat42D, NULL, &m_pPreNormalTextureFloat4UAV));

	// Create color texture
	D3D11_TEXTURE2D_DESC colorTexDesc = {0};
	colorTexDesc.Width = GlobalAppState::getInstance().s_windowWidth;
	colorTexDesc.Height = GlobalAppState::getInstance().s_windowHeight;
	colorTexDesc.MipLevels = 1;
	colorTexDesc.ArraySize = 1;
	colorTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	colorTexDesc.SampleDesc.Count = 1;
	colorTexDesc.SampleDesc.Quality = 0;
	colorTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	colorTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorTexDesc.MiscFlags = 0;

	V_RETURN(device->CreateTexture2D(&colorTexDesc, NULL, &m_pColorTexture2D));
	V_RETURN(device->CreateShaderResourceView(m_pColorTexture2D, NULL, &m_pColorTextureSRV));

	V_RETURN(device->CreateTexture2D(&colorTexDesc, NULL, &m_pPreColorTexture2D));
	V_RETURN(device->CreateShaderResourceView(m_pPreColorTexture2D, NULL, &m_pPreColorTextureSRV));

	// Create label texture
	D3D11_TEXTURE2D_DESC labelTexDesc = {0};
	labelTexDesc.Width = GlobalAppState::getInstance().s_windowWidth;
	labelTexDesc.Height = GlobalAppState::getInstance().s_windowHeight;
	labelTexDesc.MipLevels = 1;
	labelTexDesc.ArraySize = 1;
	labelTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	labelTexDesc.SampleDesc.Count = 1;
	labelTexDesc.SampleDesc.Quality = 0;
	labelTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	labelTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	labelTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	labelTexDesc.MiscFlags = 0;

	for (int i = 0; i < LABEL_TEXTURE_NUM; i++)
	{
		V_RETURN(device->CreateTexture2D(&labelTexDesc, NULL, &m_pLabelTexture2D[i])); //Label Texture2D
		V_RETURN(device->CreateShaderResourceView(m_pLabelTexture2D[i], NULL, &m_pLabelTextureSRV[i])); //Label SRV
		
		V_RETURN(device->CreateTexture2D(&labelTexDesc, NULL, &(m_pPreLabelTexture2D[i]))); //Label Texture2D
		V_RETURN(device->CreateShaderResourceView(m_pPreLabelTexture2D[i], NULL, &m_pPreLabelTextureSRV[i])); //Label SRV
	}

	return hr;  
}

HRESULT DX11Sensor::processDepth( ID3D11DeviceContext* context )
{
	HRESULT hr = S_OK;

	//get data from Kinect to GPU
	if (m_depthSensor->processDepth() == S_FALSE)	return S_FALSE;
	m_FrameNumberDepth++;

	// copy to our d3d 11 depth texture
	D3D11_MAPPED_SUBRESOURCE msT;
	V_RETURN(context->Map(m_pDepthTextureUS2D, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msT));

	if (GlobalAppState::getInstance().s_windowWidth != m_depthSensor->getDepthWidth() || 
		GlobalAppState::getInstance().s_windowHeight != m_depthSensor->getDepthHeight()) {

		float convWidth = (float)m_depthSensor->getDepthWidth() / (float)GlobalAppState::getInstance().s_windowWidth;
		float convHeight = (float)m_depthSensor->getDepthHeight() / (float)GlobalAppState::getInstance().s_windowHeight;

		for (unsigned int i = 0; i < GlobalAppState::getInstance().s_windowHeight; i++) {
			for (unsigned int j = 0; j < GlobalAppState::getInstance().s_windowWidth; j++) {
				const unsigned int tarIdx = i*msT.RowPitch/2 + j;	//2 bytes per entry
				const unsigned int srcIdx = (unsigned int)((float)i*convHeight+0.5f)*m_depthSensor->getDepthWidth() + (unsigned int)((float)j*convWidth+0.5f);
				((USHORT*)msT.pData)[tarIdx] = m_depthSensor->getDepthD16()[srcIdx];
			}
		}
	} else {
		//TODO actually there should be a pitch as well... (for odd width sizes this doesn't work...)
		memcpy(msT.pData, m_depthSensor->getDepthD16(), GlobalAppState::getInstance().s_windowWidth * GlobalAppState::getInstance().s_windowHeight * sizeof(USHORT));
	}
	context->Unmap(m_pDepthTextureUS2D, NULL);

	DX11ImageHelper::applyDepthMap(context, m_pDepthTextureUSSRV, m_pDepthTextureFUAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	// Start query for timing
	if(GlobalAppState::getInstance().s_timingsDetailledEnabled)
	{
		GlobalAppState::getInstance().WaitForGPU();
		m_timer.start();
	}

	DX11ImageHelper::applyErode(context, m_pDepthTextureFSRV, m_pColorTextureSRV, m_pDepthTextureFErodedUAV, 0.1f, 0, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	// Wait for query
	if(GlobalAppState::getInstance().s_timingsDetailledEnabled)
	{
		GlobalAppState::getInstance().WaitForGPU();
		TimingLog::totalTimeErode += m_timer.getElapsedTimeMS(); TimingLog::countErode++;
	}		

	//DX11ImageHelper::applyCameraSpaceProjection(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFloat4NoSmoothingUAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	if (m_bFilterDepthValues)
	{
		{
			// Start query for timing
			if(GlobalAppState::getInstance().s_timingsDetailledEnabled)
			{
				GlobalAppState::getInstance().WaitForGPU();
				m_timer.start();
			}

			DX11ImageHelper::applyBilateralFilter(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFFilteredUAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight, m_fBilateralFilterSigmaD, m_fBilateralFilterSigmaR);

			//DX11ImageHelper::applyBFApprox(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFFilteredUAV, getDepthImageWidth(), getDepthImageHeight(), 10, 0.1f);
			//DX11ImageHelper::applyBlockAveraging(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFFilteredUAV, getDepthImageWidth(), getDepthImageHeight(), m_fBilateralFilterSigmaD, m_fBilateralFilterSigmaR);
			//DX11ImageHelper::applySubSamp(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFFilteredUAV, getDepthImageWidth()/2, getDepthImageHeight()/2);

			// Wait for query
			if(GlobalAppState::getInstance().s_timingsDetailledEnabled)
			{
				GlobalAppState::getInstance().WaitForGPU();
				TimingLog::totalTimeBilateralFiltering += m_timer.getElapsedTimeMS(); TimingLog::countBilateralFiltering++;
			}
		}

		DX11ImageHelper::applyHSVDepth(context, m_pDepthTextureFFilteredSRV, m_pHSVDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

		DX11ImageHelper::applyCameraSpaceProjection(context, m_pDepthTextureFFilteredSRV, m_pDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);
	}
	else
	{
		DX11ImageHelper::applyHSVDepth(context, m_pDepthTextureFErodedSRV, m_pHSVDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

		DX11ImageHelper::applyCameraSpaceProjection(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);
	}

	DX11ImageHelper::applyNormalComputation(context, m_pDepthTextureFloat4SRV, m_pNormalTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	return hr;
}

HRESULT DX11Sensor::processColor( ID3D11DeviceContext* context )
{
	if (m_depthSensor->processColor() == S_FALSE)	return S_FALSE;

	HRESULT hr = S_OK; // open the KEY

	// copy to our d3d 11 color texture
	D3D11_MAPPED_SUBRESOURCE msT;
	V_RETURN(context->Map(m_pColorTexture2D, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msT));

	if (GlobalAppState::getInstance().s_windowWidth != m_depthSensor->getColorWidth() || 
		GlobalAppState::getInstance().s_windowHeight != m_depthSensor->getColorHeight()) {

			float convWidth = (float)m_depthSensor->getColorWidth() / (float)GlobalAppState::getInstance().s_windowWidth;
			float convHeight = (float)m_depthSensor->getColorHeight() / (float)GlobalAppState::getInstance().s_windowHeight;

			for (unsigned int i = 0; i < GlobalAppState::getInstance().s_windowHeight; i++) {
				for (unsigned int j = 0; j < GlobalAppState::getInstance().s_windowWidth; j++) {
					const unsigned int tarIdx = i*msT.RowPitch/4 + j;	//4 bytes per entry
					const unsigned int srcIdx = (unsigned int)((float)i*convHeight + 0.5f)*m_depthSensor->getColorWidth() + (unsigned int)((float)j*convWidth + 0.5f);
					((unsigned int*)msT.pData)[tarIdx] = ((const unsigned int*)m_depthSensor->getColorRGBX())[srcIdx];
				}
			}
	} else {
		//TODO actually there should be a pitch as well... (for odd width sizes this doesn't work...)
		memcpy(msT.pData, m_depthSensor->getColorRGBX(), sizeof(unsigned int)*GlobalAppState::getInstance().s_windowWidth*GlobalAppState::getInstance().s_windowHeight);
	}

	context->Unmap(m_pColorTexture2D, NULL);

	return hr;
}


HRESULT DX11Sensor::processLabel( ID3D11DeviceContext* context )
{
	if (m_depthSensor->processLabel() == S_FALSE)	return S_FALSE;

	HRESULT hr = S_OK; // No remapping of color -> therefore color is off at the moment!!

	// copy to our d3d 11 label texture
	for (int labelno = 0; labelno < LABEL_TEXTURE_NUM; labelno++)
	{
		D3D11_MAPPED_SUBRESOURCE msT;
		V_RETURN(context->Map(m_pLabelTexture2D[labelno], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msT));

		if (GlobalAppState::getInstance().s_windowWidth != m_depthSensor->getColorWidth() || 
			GlobalAppState::getInstance().s_windowHeight != m_depthSensor->getColorHeight()) {

				float convWidth = (float)m_depthSensor->getColorWidth() / (float)GlobalAppState::getInstance().s_windowWidth;
				float convHeight = (float)m_depthSensor->getColorHeight() / (float)GlobalAppState::getInstance().s_windowHeight;

				for (unsigned int i = 0; i < GlobalAppState::getInstance().s_windowHeight; i++) {
					for (unsigned int j = 0; j < GlobalAppState::getInstance().s_windowWidth; j++) {
						const unsigned int tarIdx = i*msT.RowPitch/16 + j;	//16 bytes per entry
						const unsigned int srcIdx = (unsigned int)((float)i*convHeight + 0.5f)*m_depthSensor->getColorWidth() + (unsigned int)((float)j*convWidth + 0.5f);
						((unsigned int*)msT.pData)[tarIdx] = ((const unsigned int*)m_depthSensor->getLabel()[labelno])[srcIdx];
					}
				}
		} else {
			//TODO actually there should be a pitch as well... (for odd width sizes this doesn't work...)
			memcpy(msT.pData, m_depthSensor->getLabel()[labelno],  sizeof(unsigned int)*GlobalAppState::getInstance().s_windowWidth*GlobalAppState::getInstance().s_windowHeight);
		}

		context->Unmap(m_pLabelTexture2D[labelno], NULL);
	}
	
	return hr;
}

HRESULT DX11Sensor::processPreDepth(ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;

	//get data from Kinect to GPU
	if (m_depthSensor->processPreDepth() == S_FALSE)	return S_FALSE;

	// copy to our d3d 11 depth texture
	D3D11_MAPPED_SUBRESOURCE msT;
	V_RETURN(context->Map(m_pPreDepthTextureUS2D, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msT));

	if (GlobalAppState::getInstance().s_windowWidth != m_depthSensor->getDepthWidth() ||
		GlobalAppState::getInstance().s_windowHeight != m_depthSensor->getDepthHeight()) {

		float convWidth = (float)m_depthSensor->getDepthWidth() / (float)GlobalAppState::getInstance().s_windowWidth;
		float convHeight = (float)m_depthSensor->getDepthHeight() / (float)GlobalAppState::getInstance().s_windowHeight;

		for (unsigned int i = 0; i < GlobalAppState::getInstance().s_windowHeight; i++) {
			for (unsigned int j = 0; j < GlobalAppState::getInstance().s_windowWidth; j++) {
				const unsigned int tarIdx = i*msT.RowPitch/2 + j;	//2 bytes per entry
				const unsigned int srcIdx = (unsigned int)((float)i*convHeight + 0.5f)*m_depthSensor->getDepthWidth() + (unsigned int)((float)j*convWidth + 0.5f);
				((USHORT*)msT.pData)[tarIdx] = m_depthSensor->getPreDepthD16()[srcIdx];
			}
		}
	}
	else {
		//TODO actually there should be a pitch as well... (for odd width sizes this doesn't work...)
		memcpy(msT.pData, m_depthSensor->getPreDepthD16(), GlobalAppState::getInstance().s_windowWidth * GlobalAppState::getInstance().s_windowHeight * sizeof(USHORT));
	}
	context->Unmap(m_pPreDepthTextureUS2D, NULL);

	DX11ImageHelper::applyDepthMap(context, m_pPreDepthTextureUSSRV, m_pPreDepthTextureFUAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	// Start query for timing
	if (GlobalAppState::getInstance().s_timingsDetailledEnabled)
	{
		GlobalAppState::getInstance().WaitForGPU();
		m_timer.start();
	}

	DX11ImageHelper::applyErode(context, m_pPreDepthTextureFSRV, m_pPreColorTextureSRV, m_pPreDepthTextureFErodedUAV, 0.1f, 0, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	// Wait for query
	if (GlobalAppState::getInstance().s_timingsDetailledEnabled)
	{
		GlobalAppState::getInstance().WaitForGPU();
		TimingLog::totalTimeErode += m_timer.getElapsedTimeMS(); TimingLog::countErode++;
	}

	//DX11ImageHelper::applyCameraSpaceProjection(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFloat4NoSmoothingUAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	if (m_bFilterDepthValues)
	{
		{
			// Start query for timing
			if (GlobalAppState::getInstance().s_timingsDetailledEnabled)
			{
				GlobalAppState::getInstance().WaitForGPU();
				m_timer.start();
			}

			DX11ImageHelper::applyBilateralFilter(context, m_pPreDepthTextureFErodedSRV, m_pPreDepthTextureFFilteredUAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight, m_fBilateralFilterSigmaD, m_fBilateralFilterSigmaR);

			//DX11ImageHelper::applyBFApprox(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFFilteredUAV, getDepthImageWidth(), getDepthImageHeight(), 10, 0.1f);
			//DX11ImageHelper::applyBlockAveraging(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFFilteredUAV, getDepthImageWidth(), getDepthImageHeight(), m_fBilateralFilterSigmaD, m_fBilateralFilterSigmaR);
			//DX11ImageHelper::applySubSamp(context, m_pDepthTextureFErodedSRV, m_pDepthTextureFFilteredUAV, getDepthImageWidth()/2, getDepthImageHeight()/2);

			// Wait for query
			if (GlobalAppState::getInstance().s_timingsDetailledEnabled)
			{
				GlobalAppState::getInstance().WaitForGPU();
				TimingLog::totalTimeBilateralFiltering += m_timer.getElapsedTimeMS(); TimingLog::countBilateralFiltering++;
			}
		}

		DX11ImageHelper::applyHSVDepth(context, m_pPreDepthTextureFFilteredSRV, m_pHSVPreDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

		DX11ImageHelper::applyCameraSpaceProjection(context, m_pPreDepthTextureFFilteredSRV, m_pPreDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);
	}
	else
	{
		DX11ImageHelper::applyHSVDepth(context, m_pPreDepthTextureFErodedSRV, m_pHSVPreDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

		DX11ImageHelper::applyCameraSpaceProjection(context, m_pPreDepthTextureFErodedSRV, m_pPreDepthTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);
	}

	DX11ImageHelper::applyNormalComputation(context, m_pPreDepthTextureFloat4SRV, m_pPreNormalTextureFloat4UAV, GlobalAppState::getInstance().s_windowWidth, GlobalAppState::getInstance().s_windowHeight);

	return hr;
}

HRESULT DX11Sensor::processPreColor(ID3D11DeviceContext* context)
{
	if (m_depthSensor->processPreColor() == S_FALSE)	return S_FALSE;

	HRESULT hr = S_OK; // open the KEY

	// copy to our d3d 11 color texture
	D3D11_MAPPED_SUBRESOURCE msT;
	V_RETURN(context->Map(m_pPreColorTexture2D, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msT));

	if (GlobalAppState::getInstance().s_windowWidth != m_depthSensor->getColorWidth() ||
		GlobalAppState::getInstance().s_windowHeight != m_depthSensor->getColorHeight()) {

		float convWidth = (float)m_depthSensor->getColorWidth() / (float)GlobalAppState::getInstance().s_windowWidth;
		float convHeight = (float)m_depthSensor->getColorHeight() / (float)GlobalAppState::getInstance().s_windowHeight;

		for (unsigned int i = 0; i < GlobalAppState::getInstance().s_windowHeight; i++) {
			for (unsigned int j = 0; j < GlobalAppState::getInstance().s_windowWidth; j++) {
				const unsigned int tarIdx = i*msT.RowPitch / 4 + j;	//4 bytes per entry
				const unsigned int srcIdx = (unsigned int)((float)i*convHeight + 0.5f)*m_depthSensor->getColorWidth() + (unsigned int)((float)j*convWidth + 0.5f);
				((unsigned int*)msT.pData)[tarIdx] = ((const unsigned int*)m_depthSensor->getPreColorRGBX())[srcIdx];
			}
		}
	}
	else {
		//TODO actually there should be a pitch as well... (for odd width sizes this doesn't work...)
		memcpy(msT.pData, m_depthSensor->getPreColorRGBX(), sizeof(unsigned int)*GlobalAppState::getInstance().s_windowWidth*GlobalAppState::getInstance().s_windowHeight);
	}

	context->Unmap(m_pPreColorTexture2D, NULL);

	return hr;
}

HRESULT DX11Sensor::processPreLabel(ID3D11DeviceContext* context)
{
	if (m_depthSensor->processPreLabel() == S_FALSE)	return S_FALSE;

	HRESULT hr = S_OK; // No remapping of color -> therefore color is off at the moment!!

	// copy to our d3d 11 label texture
	for (int labelno = 0; labelno < LABEL_TEXTURE_NUM; labelno++)
	{
		D3D11_MAPPED_SUBRESOURCE msT;
		V_RETURN(context->Map(m_pPreLabelTexture2D[labelno], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msT));

		if (GlobalAppState::getInstance().s_windowWidth != m_depthSensor->getColorWidth() ||
			GlobalAppState::getInstance().s_windowHeight != m_depthSensor->getColorHeight()) {

			float convWidth = (float)m_depthSensor->getColorWidth() / (float)GlobalAppState::getInstance().s_windowWidth;
			float convHeight = (float)m_depthSensor->getColorHeight() / (float)GlobalAppState::getInstance().s_windowHeight;

			for (unsigned int i = 0; i < GlobalAppState::getInstance().s_windowHeight; i++) {
				for (unsigned int j = 0; j < GlobalAppState::getInstance().s_windowWidth; j++) {
					const unsigned int tarIdx = i*msT.RowPitch / 16 + j;	//16 bytes per entry
					const unsigned int srcIdx = (unsigned int)((float)i*convHeight + 0.5f)*m_depthSensor->getColorWidth() + (unsigned int)((float)j*convWidth + 0.5f);
					((unsigned int*)msT.pData)[tarIdx] = ((const unsigned int*)m_depthSensor->getPreLabel()[labelno])[srcIdx];
				}
			}
		}
		else {
			//TODO actually there should be a pitch as well... (for odd width sizes this doesn't work...)
			memcpy(msT.pData, m_depthSensor->getPreLabel()[labelno], sizeof(unsigned int)*GlobalAppState::getInstance().s_windowWidth*GlobalAppState::getInstance().s_windowHeight);
		}

		context->Unmap(m_pPreLabelTexture2D[labelno], NULL);
	}

	return hr;
}