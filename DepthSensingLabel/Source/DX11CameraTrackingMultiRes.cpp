#include "stdafx.h"

#include "DX11CameraTrackingMultiRes.h"

#include "GlobalAppState.h"

#include <iostream>
#include <limits>

/////////////////////////////////////////////////////
// Camera Tracking Multi Res
/////////////////////////////////////////////////////

Eigen::Matrix4f DX11CameraTrackingMultiRes::m_matrixTrackingLost;

// Correspondences
ID3D11Texture2D** DX11CameraTrackingMultiRes::m_pCorrespondenceTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_pCorrespondenceTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_pCorrespondenceTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_pCorrespondenceNormalTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_pCorrespondenceNormalTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_pCorrespondenceNormalTextureFloat4UAV = NULL;

// Input
ID3D11Texture2D** DX11CameraTrackingMultiRes::m_inputTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_inputTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_inputTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_inputNormalTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_inputNormalTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_inputNormalTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_inputColorTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_inputColorTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_inputColorTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_inputLabelTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_inputLabelTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_inputLabelTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_inputIntensityTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_inputIntensityTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_inputIntensityTextureFloat4UAV = NULL;

// Model
ID3D11Texture2D** DX11CameraTrackingMultiRes::m_modelTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_modelTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_modelTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_modelNormalTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_modelNormalTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_modelNormalTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_modelColorTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_modelColorTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_modelColorTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_modelLabelTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_modelLabelTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_modelLabelTextureFloat4UAV = NULL;

ID3D11Texture2D** DX11CameraTrackingMultiRes::m_modelIntensityTextureFloat42D = NULL;
ID3D11ShaderResourceView** DX11CameraTrackingMultiRes::m_modelIntensityTextureFloat4SRV = NULL;
ID3D11UnorderedAccessView** DX11CameraTrackingMultiRes::m_modelIntensityTextureFloat4UAV = NULL;

// Weight
ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputConfidenceWithoutBGFloat = NULL;
ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputConfidenceWithoutBGFloatCPU = NULL;
ID3D11UnorderedAccessView* DX11CameraTrackingMultiRes::m_inputConfidenceWithoutBGFloatUAV = NULL;

ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputNormalsWithoutBGFloat4 = NULL;
ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputNormalsWithoutBGFloat4CPU = NULL;
ID3D11UnorderedAccessView* DX11CameraTrackingMultiRes::m_inputNormalsWithoutBGFloat4UAV = NULL;

ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputPointsWithoutBGFloat4 = NULL;
ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputPointsWithoutBGFloat4CPU = NULL;
ID3D11UnorderedAccessView* DX11CameraTrackingMultiRes::m_inputPointsWithoutBGFloat4UAV = NULL;

ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputIndexWithoutBGInt = NULL;
ID3D11Buffer* DX11CameraTrackingMultiRes::m_inputIndexWithoutBGIntCPU = NULL;
ID3D11UnorderedAccessView* DX11CameraTrackingMultiRes::m_inputIndexWithoutBGIntUAV = NULL;

// Image Pyramid Dimensions
unsigned int* DX11CameraTrackingMultiRes::m_imageWidth = NULL;
unsigned int* DX11CameraTrackingMultiRes::m_imageHeight = NULL;

/////////////////////////////////////////////////////
// Timer
/////////////////////////////////////////////////////

Timer DX11CameraTrackingMultiRes::m_timer;

// test
//inline void matmul(const float *A, const float *b, float *x, int numRows, int numCols)
//{
//	for (int r = 0; r < numRows; ++r)
//	{
//		float res = 0.0f;
//		for (int c = 0; c < numCols; ++c) 
//			res += A[r*numCols + c] * b[c];
//
//		x[r] = res;
//	}
//}
//
//inline float stepQuality(float f_old, float f_new, const float *b, const float *grad, const float *A, int numPara)
//{
//	// actual reduction and predicted reduction
//	float a_reduction = f_old - f_new;
//	float p_reduction = 0.0;
//
//	float *x = new float[numPara];
//
//	matmul(A, b, x, numPara, numPara);
//	for (int i = 0; i < numPara; i++) 
//		p_reduction -= grad[i] * b[i] + 0.5*b[i] * x[i];
//	
//	delete[] x;
//
//	if (p_reduction < 0) 
//		return a_reduction / fabs(p_reduction);
//
//	return a_reduction / p_reduction;
//}
//
//inline float computeConfidence(float *partLabel, int labelNum)
//{
//	float entropy = 0;
//	for (int i = 0; i < labelNum; i++)
//	{
//		float p = partLabel[i];
//		if (p != 0)
//		{
//			entropy -= p * log(p);
//		}
//	}
//	float confidence = 1 - entropy / log(labelNum);
//
//	return confidence;
//}
// test

//void HSB2RGB(float hue, float saturation, float brightness, int &red, int &green, int &blue)
//{
//	if (saturation == 0.0f)
//		red = green = blue = (int)(brightness * 255.0f + 0.5f);
//	else
//	{
//		if (hue == 360.0f) hue = 0.0f;
//		hue /= 60.0f;
//		int h = (int)hue;
//		float f = hue - h;
//
//		float p = brightness * (1.0f - saturation);
//		float q = brightness * (1.0f - saturation * f);
//		float t = brightness * (1.0f - saturation * (1.0f - f));
//
//		switch ((int)h)
//		{
//		case 0:
//			red = (int)(brightness * 255.0f + 0.5f);
//			green = (int)(t * 255.0f + 0.5f);
//			blue = (int)(p * 255.0f + 0.5f);
//			break;
//		case 1:
//			red = (int)(q * 255.0f + 0.5f);
//			green = (int)(brightness * 255.0f + 0.5f);
//			blue = (int)(p * 255.0f + 0.5f);
//			break;
//		case 2:
//			red = (int)(p * 255.0f + 0.5f);
//			green = (int)(brightness * 255.0f + 0.5f);
//			blue = (int)(t * 255.0f + 0.5f);
//			break;
//		case 3:
//			red = (int)(p * 255.0f + 0.5f);
//			green = (int)(q * 255.0f + 0.5f);
//			blue = (int)(brightness * 255.0f + 0.5f);
//			break;
//		case 4:
//			red = (int)(t * 255.0f + 0.5f);
//			green = (int)(p * 255.0f + 0.5f);
//			blue = (int)(brightness * 255.0f + 0.5f);
//			break;
//		case 5:
//			red = (int)(brightness * 255.0f + 0.5f);
//			green = (int)(p * 255.0f + 0.5f);
//			blue = (int)(q * 255.0f + 0.5f);
//			break;
//		}
//	}
//}

HRESULT DX11CameraTrackingMultiRes::OnD3D11CreateDevice(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	// Correspondences
	m_pCorrespondenceTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_pCorrespondenceTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_pCorrespondenceTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_pCorrespondenceNormalTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_pCorrespondenceNormalTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_pCorrespondenceNormalTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	// Input
	m_inputTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_inputNormalTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputNormalTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputNormalTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_inputColorTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputColorTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputColorTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_inputLabelTextureFloat42D = new ID3D11Texture2D*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM*GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputLabelTextureFloat4SRV = new ID3D11ShaderResourceView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM*GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputLabelTextureFloat4UAV = new ID3D11UnorderedAccessView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM*GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_inputIntensityTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputIntensityTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_inputIntensityTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	// Model
	m_modelTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_modelNormalTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelNormalTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelNormalTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_modelColorTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelColorTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelColorTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_modelLabelTextureFloat42D = new ID3D11Texture2D*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM*GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelLabelTextureFloat4SRV = new ID3D11ShaderResourceView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM*GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelLabelTextureFloat4UAV = new ID3D11UnorderedAccessView*[LABEL_TEXTURE_CHANNEL*LABEL_TEXTURE_NUM*GlobalCameraTrackingState::getInstance().s_maxLevels];

	m_modelIntensityTextureFloat42D = new ID3D11Texture2D*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelIntensityTextureFloat4SRV = new ID3D11ShaderResourceView*[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_modelIntensityTextureFloat4UAV = new ID3D11UnorderedAccessView*[GlobalCameraTrackingState::getInstance().s_maxLevels];

	// Image Pyramid Dimensions
	m_imageWidth = new unsigned int[GlobalCameraTrackingState::getInstance().s_maxLevels];
	m_imageHeight = new unsigned int[GlobalCameraTrackingState::getInstance().s_maxLevels];

	unsigned int fac = 1;
	for (unsigned int i = 0; i < GlobalCameraTrackingState::getInstance().s_maxLevels; i++) 
	{
		m_imageWidth[i] = GlobalAppState::getInstance().s_windowWidth/fac;
		m_imageHeight[i] = GlobalAppState::getInstance().s_windowHeight/fac;

		// Create depth texture
		D3D11_TEXTURE2D_DESC depthTexDesc = {0};
		depthTexDesc.Width = m_imageWidth[i];
		depthTexDesc.Height = m_imageHeight[i];
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;

		////////////////////////////////////////////////
		// Correspondences
		////////////////////////////////////////////////

		V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_pCorrespondenceTextureFloat42D[i]));
		V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_pCorrespondenceNormalTextureFloat42D[i]));
		
		//create shader resource views
		V_RETURN(pd3dDevice->CreateShaderResourceView(m_pCorrespondenceTextureFloat42D[i], NULL, &m_pCorrespondenceTextureFloat4SRV[i]));
		V_RETURN(pd3dDevice->CreateShaderResourceView(m_pCorrespondenceNormalTextureFloat42D[i], NULL, &m_pCorrespondenceNormalTextureFloat4SRV[i]));
		
		//create unordered access views
		V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_pCorrespondenceTextureFloat42D[i], NULL, &m_pCorrespondenceTextureFloat4UAV[i]));
		V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_pCorrespondenceNormalTextureFloat42D[i], NULL, &m_pCorrespondenceNormalTextureFloat4UAV[i]));
		
		////////////////////////////////////////////////
		// Input
		////////////////////////////////////////////////
		if (i != 0) 
		{  
			// Not finest level
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_inputTextureFloat42D[i]));
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_inputNormalTextureFloat42D[i]));
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_inputColorTextureFloat42D[i]));

			//create shader resource views
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_inputTextureFloat42D[i], NULL, &m_inputTextureFloat4SRV[i]));
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_inputNormalTextureFloat42D[i], NULL, &m_inputNormalTextureFloat4SRV[i]));
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_inputColorTextureFloat42D[i], NULL, &m_inputColorTextureFloat4SRV[i]));
			
			//create unordered access views
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputTextureFloat42D[i], NULL, &m_inputTextureFloat4UAV[i]));
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputNormalTextureFloat42D[i], NULL, &m_inputNormalTextureFloat4UAV[i]));
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputColorTextureFloat42D[i], NULL, &m_inputColorTextureFloat4UAV[i]));
		} 
		else 
		{
			m_inputTextureFloat42D[i] = NULL;
			m_inputNormalTextureFloat42D[i] = NULL;
			m_inputColorTextureFloat42D[i] = NULL;

			m_inputTextureFloat4SRV[i] = NULL;
			m_inputNormalTextureFloat4SRV[i] = NULL;
			m_inputColorTextureFloat4SRV[i] = NULL;

			m_inputTextureFloat4UAV[i] = NULL;
			m_inputNormalTextureFloat4UAV[i] = NULL;
			m_inputColorTextureFloat4UAV[i] = NULL;
		}

		V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_inputIntensityTextureFloat42D[i]));
		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_inputLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]));
		}

		//create shader resource views
		V_RETURN(pd3dDevice->CreateShaderResourceView(m_inputIntensityTextureFloat42D[i], NULL, &m_inputIntensityTextureFloat4SRV[i]));
		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_inputLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], NULL, &m_inputLabelTextureFloat4SRV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]));
		}
		
		//create unordered access views
		V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputIntensityTextureFloat42D[i], NULL, &m_inputIntensityTextureFloat4UAV[i]));
		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], NULL, &m_inputLabelTextureFloat4UAV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]));
		}
		
		////////////////////////////////////////////////
		// Model
		////////////////////////////////////////////////
		if (i != 0) 
		{ 
			// Not fines level
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_modelTextureFloat42D[i]));
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_modelNormalTextureFloat42D[i]));
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_modelColorTextureFloat42D[i]));
			
			//create shader resource views
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_modelTextureFloat42D[i], NULL, &m_modelTextureFloat4SRV[i]));
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_modelNormalTextureFloat42D[i], NULL, &m_modelNormalTextureFloat4SRV[i]));
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_modelColorTextureFloat42D[i], NULL, &m_modelColorTextureFloat4SRV[i]));
			
			//create unordered access views
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_modelTextureFloat42D[i], NULL, &m_modelTextureFloat4UAV[i]));
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_modelNormalTextureFloat42D[i], NULL, &m_modelNormalTextureFloat4UAV[i]));
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_modelColorTextureFloat42D[i], NULL, &m_modelColorTextureFloat4UAV[i]));
		} 
		else 
		{
			m_modelTextureFloat42D[i] = NULL;
			m_modelNormalTextureFloat42D[i] = NULL;
			m_modelColorTextureFloat42D[i] = NULL;
			
			m_modelTextureFloat4SRV[i] = NULL;
			m_modelNormalTextureFloat4SRV[i] = NULL;
			m_modelColorTextureFloat4SRV[i] = NULL;
			
			m_modelTextureFloat4UAV[i] = NULL;
			m_modelNormalTextureFloat4UAV[i] = NULL;
			m_modelColorTextureFloat4UAV[i] = NULL;
		}

		V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_modelIntensityTextureFloat42D[i]));
		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			V_RETURN(pd3dDevice->CreateTexture2D(&depthTexDesc, NULL, &m_modelLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]));
		}
		
		//create shader resource views
		V_RETURN(pd3dDevice->CreateShaderResourceView(m_modelIntensityTextureFloat42D[i], NULL, &m_modelIntensityTextureFloat4SRV[i]));
		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			V_RETURN(pd3dDevice->CreateShaderResourceView(m_modelLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], NULL, &m_modelLabelTextureFloat4SRV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]));
		}
		
		//create unordered access views
		V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_modelIntensityTextureFloat42D[i], NULL, &m_modelIntensityTextureFloat4UAV[i]));
		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_modelLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], NULL, &m_modelLabelTextureFloat4UAV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]));
		}

		fac*=2;
	}

	// Create Normals and Points Output Buffer****
	D3D11_BUFFER_DESC pointsDesc;
	ZeroMemory(&pointsDesc, sizeof(pointsDesc));
	pointsDesc.Usage = D3D11_USAGE_DEFAULT;
	pointsDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	pointsDesc.CPUAccessFlags = 0;
	pointsDesc.MiscFlags = 0;
	pointsDesc.ByteWidth = 4*sizeof(float)*640*480;

	D3D11_SUBRESOURCE_DATA InitPointsData;
	ZeroMemory(&InitPointsData, sizeof(D3D11_SUBRESOURCE_DATA));
	float* cpuPointsNull = new float[4*640*480];
	for (unsigned int i = 0; i < 4*640*480; i++)
	{
		cpuPointsNull[i] = 0.0f;
	}
	InitPointsData.pSysMem = cpuPointsNull;
	V_RETURN(pd3dDevice->CreateBuffer(&pointsDesc, &InitPointsData, &m_inputNormalsWithoutBGFloat4));
	V_RETURN(pd3dDevice->CreateBuffer(&pointsDesc, &InitPointsData, &m_inputPointsWithoutBGFloat4));
	SAFE_DELETE_ARRAY(cpuPointsNull);

	pointsDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	pointsDesc.BindFlags = 0;
	pointsDesc.Usage = D3D11_USAGE_STAGING;
	V_RETURN(pd3dDevice->CreateBuffer(&pointsDesc, NULL, &m_inputNormalsWithoutBGFloat4CPU));
	V_RETURN(pd3dDevice->CreateBuffer(&pointsDesc, NULL, &m_inputPointsWithoutBGFloat4CPU));

	D3D11_UNORDERED_ACCESS_VIEW_DESC pointsDescUAV;
	ZeroMemory(&pointsDescUAV, sizeof(pointsDescUAV));
	pointsDescUAV.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pointsDescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	pointsDescUAV.Buffer.FirstElement = 0;
	pointsDescUAV.Buffer.NumElements = 640*480;
	V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputNormalsWithoutBGFloat4, &pointsDescUAV, &m_inputNormalsWithoutBGFloat4UAV));
	V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputPointsWithoutBGFloat4, &pointsDescUAV, &m_inputPointsWithoutBGFloat4UAV));

	// Create Index Output Buffer****
	D3D11_BUFFER_DESC pointsIndexDesc;
	ZeroMemory(&pointsIndexDesc, sizeof(pointsIndexDesc));
	pointsIndexDesc.Usage = D3D11_USAGE_DEFAULT;
	pointsIndexDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	pointsIndexDesc.CPUAccessFlags = 0;
	pointsIndexDesc.MiscFlags = 0;
	pointsIndexDesc.ByteWidth = sizeof(int)*640*480;

	D3D11_SUBRESOURCE_DATA InitPointsIndexData;
	ZeroMemory(&InitPointsIndexData, sizeof(D3D11_SUBRESOURCE_DATA));
	int* cpuPointsIndexNull = new int[640*480];
	for (unsigned int i = 0; i < 640*480; i++)
	{
		cpuPointsIndexNull[i] = 0;
	}
	InitPointsIndexData.pSysMem = cpuPointsIndexNull;
	V_RETURN(pd3dDevice->CreateBuffer(&pointsIndexDesc, &InitPointsIndexData, &m_inputIndexWithoutBGInt));
	SAFE_DELETE_ARRAY(cpuPointsIndexNull);

	pointsIndexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	pointsIndexDesc.BindFlags = 0;
	pointsIndexDesc.Usage = D3D11_USAGE_STAGING;
	V_RETURN(pd3dDevice->CreateBuffer(&pointsIndexDesc, NULL, &m_inputIndexWithoutBGIntCPU));
	
	D3D11_UNORDERED_ACCESS_VIEW_DESC pointsDescIndexUAV;
	ZeroMemory(&pointsDescIndexUAV, sizeof(pointsDescIndexUAV));
	pointsDescIndexUAV.Format = DXGI_FORMAT_R32_SINT;
	pointsDescIndexUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	pointsDescIndexUAV.Buffer.FirstElement = 0;
	pointsDescIndexUAV.Buffer.NumElements = 640*480;
	V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputIndexWithoutBGInt, &pointsDescIndexUAV, &m_inputIndexWithoutBGIntUAV));
	
	// Create Confidence Output Buffer****
	D3D11_BUFFER_DESC confidenceDesc;
	ZeroMemory(&confidenceDesc, sizeof(confidenceDesc));
	confidenceDesc.Usage = D3D11_USAGE_DEFAULT;
	confidenceDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	confidenceDesc.CPUAccessFlags = 0;
	confidenceDesc.MiscFlags = 0;
	confidenceDesc.ByteWidth = sizeof(float)*640*480;

	D3D11_SUBRESOURCE_DATA InitConfidenceData;
	ZeroMemory(&InitConfidenceData, sizeof(D3D11_SUBRESOURCE_DATA));
	float* cpuConfidenceNull = new float[640*480];
	for (unsigned int i = 0; i < 640*480; i++)
	{
		cpuConfidenceNull[i] = 0.0f;
	}
	InitConfidenceData.pSysMem = cpuConfidenceNull;
	V_RETURN(pd3dDevice->CreateBuffer(&confidenceDesc, &InitConfidenceData, &m_inputConfidenceWithoutBGFloat));
	SAFE_DELETE_ARRAY(cpuConfidenceNull);

	confidenceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	confidenceDesc.BindFlags = 0;
	confidenceDesc.Usage = D3D11_USAGE_STAGING;
	V_RETURN(pd3dDevice->CreateBuffer(&confidenceDesc, NULL, &m_inputConfidenceWithoutBGFloatCPU));
	
	D3D11_UNORDERED_ACCESS_VIEW_DESC confidenceDescUAV;
	ZeroMemory(&confidenceDescUAV, sizeof(confidenceDescUAV));
	confidenceDescUAV.Format = DXGI_FORMAT_R32_FLOAT;
	confidenceDescUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	confidenceDescUAV.Buffer.FirstElement = 0;
	confidenceDescUAV.Buffer.NumElements = 640*480;
	V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_inputConfidenceWithoutBGFloat, &confidenceDescUAV, &m_inputConfidenceWithoutBGFloatUAV));
	
	m_matrixTrackingLost.fill(-std::numeric_limits<float>::infinity());

	return  hr;
}

bool DX11CameraTrackingMultiRes::checkRigidTransformation(Eigen::Matrix3f& R, Eigen::Vector3f& t, float angleThres, float distThres)
{
	Eigen::AngleAxisf aa(R);

	if(aa.angle() > angleThres || t.norm() > distThres)
	{
		std::cout << "Tracking lost: angle " << (aa.angle()/M_PI)*180.0f << " translation " << t.norm() << std::endl;
		return false;
	}

	return true;
}

Eigen::Matrix4f DX11CameraTrackingMultiRes::delinearizeTransformation(Vector6f& x, Eigen::Vector3f& mean, float meanStDev, unsigned int level)
{
	Eigen::Matrix3f R =	 Eigen::AngleAxisf(x[0], Eigen::Vector3f::UnitZ()).toRotationMatrix()  // Rot Z
		                *Eigen::AngleAxisf(x[1], Eigen::Vector3f::UnitY()).toRotationMatrix()  // Rot Y
		                *Eigen::AngleAxisf(x[2], Eigen::Vector3f::UnitX()).toRotationMatrix(); // Rot X

	Eigen::Vector3f t = x.segment(3, 3);

	if(!checkRigidTransformation(R, t, GlobalCameraTrackingState::getInstance().s_angleTransThres[level], GlobalCameraTrackingState::getInstance().s_distTransThres[level]))
	{
		return m_matrixTrackingLost;
	}

	Eigen::Matrix4f res; res.setIdentity();
	res.block(0, 0, 3, 3) = R;
	res.block(0, 3, 3, 1) = meanStDev*t+mean-R*mean;

	return res;
}

Eigen::Matrix4f DX11CameraTrackingMultiRes::computeBestRigidAlignment(ID3D11DeviceContext* context, 
	                                                                  ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputIntensitiesSRV, ID3D11ShaderResourceView** inputLabelsSRV, 
	                                                                  ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelIntensitiesSRV, ID3D11ShaderResourceView** modelLabelsSRV, 
	                                                                  D3DXVECTOR3& mean, float meanStDev, float nValidCorres, const Eigen::Matrix4f& globalDeltaTransform, unsigned int level, unsigned int maxInnerIter, float condThres, float angleThres, float focalx, float focaly, LinearSystemConfidence& conf)
{
	Eigen::Matrix4f deltaTransform = globalDeltaTransform;

	for (unsigned int i = 0; i < maxInnerIter; i++)
	{
		conf.reset();

		Matrix6x7f system;

		DX11BuildLinearSystem::applyBL(context, 
			                           inputSRV, inputNormalsSRV,
			                           m_pCorrespondenceTextureFloat4SRV[level], m_pCorrespondenceNormalTextureFloat4SRV[level],
									   mean, meanStDev, deltaTransform, m_imageWidth[level], m_imageHeight[level], level, system, conf, 
			                           focalx, focaly);

		Matrix6x6f ATA = system.block(0, 0, 6, 6);
		Vector6f ATb = system.block(0, 6, 6, 1);

		if (true)
		{
			bool testisnan = false;

			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					if (std::isnan(ATA(i, j)) || std::isinf(ATA(i, j)))
					{
						testisnan = true;
						break;
					}
				}
				if (testisnan)
					break;

				if (std::isnan(ATb(i)) || std::isinf(ATb(i)))
				{
					testisnan = true;
					break;
				}
			}

			if (testisnan)
			{
				conf.trackingLostTresh = true;
				return m_matrixTrackingLost;
			}
		}

		Eigen::JacobiSVD<Matrix6x6f> SVD(ATA, Eigen::ComputeFullU | Eigen::ComputeFullV);
		Vector6f x = SVD.solve(ATb);

		//computing the matrix condition
		Vector6f evs = SVD.singularValues();
		conf.matrixCondition = evs[0]/evs[5];

		Eigen::Matrix4f t = delinearizeTransformation(x, Eigen::Vector3f(mean.x, mean.y, mean.z), meanStDev, level);
		if (t(0, 0) == -std::numeric_limits<float>::infinity())
		{
			conf.trackingLostTresh = true;
			return m_matrixTrackingLost;
		}

		deltaTransform = t*deltaTransform;
	}

	return deltaTransform;
}

mat4f DX11CameraTrackingMultiRes::applyCT(ID3D11DeviceContext* context, 
	                                      ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputColorsSRV, ID3D11ShaderResourceView** inputLabelsSRV,
	                                      ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelColorsSRV, ID3D11ShaderResourceView** modelLabelsSRV,
										  const mat4f& lastTransform, const std::vector<unsigned int>& maxInnerIter, const std::vector<unsigned int>& maxOuterIter, const std::vector<float>& distThres, const std::vector<float>& normalThres, const std::vector<float>& colorThres, std::vector<float>& labelThres, float condThres, float angleThres, const mat4f& deltaTransformEstimate, const float focalx, const float focaly, const std::vector<float>& earlyOutResidual, ICPErrorLog* errorLog)
{
	// Input
	m_inputTextureFloat4SRV[0]       = inputSRV;
	m_inputNormalTextureFloat4SRV[0] = inputNormalsSRV;
	m_inputColorTextureFloat4SRV[0]  = inputColorsSRV;
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		DX11ImageHelper::applySplitLabelToFloat(context, inputLabelsSRV, m_inputLabelTextureFloat4UAV[i], m_imageWidth[0], m_imageHeight[0], GlobalAppState::getInstance().s_labelNum, i);
	}

	// Model
	m_modelTextureFloat4SRV[0]       = modelSRV;
	m_modelNormalTextureFloat4SRV[0] = modelNormalsSRV;
	m_modelColorTextureFloat4SRV[0]  = modelColorsSRV;
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		DX11ImageHelper::applySplitLabelToFloat(context, modelLabelsSRV, m_modelLabelTextureFloat4UAV[i], m_imageWidth[0], m_imageHeight[0], GlobalAppState::getInstance().s_labelNum, i);
	}

	DX11ImageHelper::applyConvertColorToIntensityFloat(context, m_inputColorTextureFloat4SRV[0], m_inputIntensityTextureFloat4UAV[0], m_imageWidth[0], m_imageHeight[0]);
	DX11ImageHelper::applyConvertColorToIntensityFloat(context, m_modelColorTextureFloat4SRV[0], m_modelIntensityTextureFloat4UAV[0], m_imageWidth[0], m_imageHeight[0]);
	
	// Start query for timing
	if (GlobalAppState::getInstance().s_timingsDetailledEnabled)
	{
		GlobalAppState::getInstance().WaitForGPU();
		m_timer.start();
	}

	for (unsigned int i = 0; i < GlobalCameraTrackingState::getInstance().s_maxLevels-1; i++)
	{
		// Downsample Depth Maps directly ? -> better ?
		DX11ImageHelper::applyDownsampling(context, m_inputTextureFloat4SRV[i], m_inputTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);
		DX11ImageHelper::applyDownsampling(context, m_modelTextureFloat4SRV[i], m_modelTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);

		DX11ImageHelper::applyNormalComputation(context, m_inputTextureFloat4SRV[i+1], m_inputNormalTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);
		DX11ImageHelper::applyNormalComputation(context, m_modelTextureFloat4SRV[i+1], m_modelNormalTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);

		DX11ImageHelper::applyDownsampling(context, m_inputColorTextureFloat4SRV[i], m_inputColorTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);
		DX11ImageHelper::applyDownsampling(context, m_modelColorTextureFloat4SRV[i], m_modelColorTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);
		
		DX11ImageHelper::applyDownsampling(context, m_inputIntensityTextureFloat4SRV[i], m_inputIntensityTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);
		DX11ImageHelper::applyDownsampling(context, m_modelIntensityTextureFloat4SRV[i], m_modelIntensityTextureFloat4UAV[i+1], m_imageWidth[i+1], m_imageHeight[i+1]);

		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			DX11ImageHelper::applyDownsampling(context, m_inputLabelTextureFloat4SRV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], m_inputLabelTextureFloat4UAV[(i+1)*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], m_imageWidth[i+1], m_imageHeight[i+1]);
			DX11ImageHelper::applyDownsampling(context, m_modelLabelTextureFloat4SRV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], m_modelLabelTextureFloat4UAV[(i+1)*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j], m_imageWidth[i+1], m_imageHeight[i+1]);
		}
	}

	Eigen::Matrix4f deltaTransform;
	deltaTransform = MatToEig(deltaTransformEstimate);
	for (int level = GlobalCameraTrackingState::getInstance().s_maxLevels-1; level >= 0; level--)
	{	
		if (errorLog) 
		{
			errorLog->newICPFrame(level);
		}

		deltaTransform = align(context, 
			                   m_inputTextureFloat4SRV[level], m_inputNormalTextureFloat4SRV[level], m_inputIntensityTextureFloat4SRV[level], m_inputLabelTextureFloat4SRV,
			                   m_modelTextureFloat4SRV[level], m_modelNormalTextureFloat4SRV[level], m_modelIntensityTextureFloat4SRV[level], m_modelLabelTextureFloat4SRV,
			                   deltaTransform, level, maxInnerIter[level], maxOuterIter[level], distThres[level], normalThres[level], colorThres[level], labelThres[level], condThres, angleThres, focalx, focaly, earlyOutResidual[level], errorLog);

		if (deltaTransform(0, 0) == -std::numeric_limits<float>::infinity()) 
		{
			return EigToMat(m_matrixTrackingLost);
		}
	}

	// Wait for query
	if (GlobalAppState::getInstance().s_timingsDetailledEnabled)
	{
		GlobalAppState::getInstance().WaitForGPU();
		TimingLog::totalTimeTrackCamera += m_timer.getElapsedTimeMS();
		TimingLog::countTrackCamera++;
	}

	return lastTransform * EigToMat(deltaTransform);
}


Eigen::Matrix4f DX11CameraTrackingMultiRes::align(ID3D11DeviceContext* context, 
	                                              ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputIntensitiesSRV, ID3D11ShaderResourceView** inputLabelsSRV,
	                                              ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelIntensitiesSRV, ID3D11ShaderResourceView** modelLabelsSRV,
												  Eigen::Matrix4f& deltaTransform, unsigned int level, unsigned int maxInnerIter, unsigned int maxOuterIter, float distThres, float normalThres, float colorThres, float labelThres, float condThres, float angleThres, float focalx, float focaly, float earlyOut, ICPErrorLog* errorLog)
{	
	bool partLabel = false;
	if (!partLabel)
	{
		float lastICPError = -1.0f;
		for (unsigned int i = 0; i < maxOuterIter; i++)
		{
			D3DXVECTOR3 mean;
			float meanStDev;
			float nValidCorres;

			LinearSystemConfidence currConfNoReject;
			LinearSystemConfidence currConfWiReject;

			if (errorLog)
			{
				//run ICP without correspondence rejection (must be run before because it needs the old delta transform)
				float dThresh = 1000.0f;	float nThresh = 0.0f;	float cThresh = 0.0f;	float lThresh = 0.0f;
				computeCorrespondences(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaTransform, level, distThres, normalThres, colorThres, labelThres, 0);
				computeBestRigidAlignment(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaTransform, level, maxInnerIter, condThres, angleThres, focalx, focaly, currConfNoReject);
				errorLog->addCurrentICPIteration(currConfNoReject, level);
			}

			//standard correspondence search and alignment
			computeCorrespondences(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaTransform, level, distThres, normalThres, colorThres, labelThres, 0);
			deltaTransform = computeBestRigidAlignment(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaTransform, level, maxInnerIter, condThres, angleThres, focalx, focaly, currConfWiReject);

			if (std::abs(lastICPError - currConfWiReject.sumRegError) < earlyOut)
			{
				break;
			}

			lastICPError = currConfWiReject.sumRegError;
		}

		return deltaTransform;
	}
	else
	{
		Eigen::Matrix4f *deltaLabelTransform = new Eigen::Matrix4f[GlobalAppState::getInstance().s_labelNum-1];
		int *deltaLabelTransformIndicator = new int[GlobalAppState::getInstance().s_labelNum-1];

		for (int labelno = 1; labelno < GlobalAppState::getInstance().s_labelNum; labelno++)
		{
			deltaLabelTransform[labelno-1].setIdentity();
			deltaLabelTransformIndicator[labelno-1] = 1;

			float lastICPError = -1.0f;
			for (unsigned int i = 0; i < maxOuterIter; i++)
			{
				D3DXVECTOR3 mean;
				float meanStDev;
				float nValidCorres;

				LinearSystemConfidence currConfNoReject;
				LinearSystemConfidence currConfWiReject;

				if (errorLog)
				{
					//run ICP without correspondence rejection (must be run before because it needs the old delta transform)
					float dThresh = 1000.0f;	float nThresh = 0.0f;	float cThresh = 0.0f;	float lThresh = 0.0f;
					computeCorrespondences(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaLabelTransform[labelno-1], level, distThres, normalThres, colorThres, labelThres, labelno);
					computeBestRigidAlignment(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaLabelTransform[labelno-1], level, maxInnerIter, condThres, angleThres, focalx, focaly, currConfNoReject);
					errorLog->addCurrentICPIteration(currConfNoReject, level);
				}

				//standard correspondence search and alignment
				computeCorrespondences(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaLabelTransform[labelno-1], level, distThres, normalThres, colorThres, labelThres, labelno);
				deltaLabelTransform[labelno-1] = computeBestRigidAlignment(context, inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV, modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV, mean, meanStDev, nValidCorres, deltaLabelTransform[labelno-1], level, maxInnerIter, condThres, angleThres, focalx, focaly, currConfWiReject);

				if (deltaLabelTransform[labelno-1](0, 0) == -std::numeric_limits<float>::infinity())
				{
					deltaLabelTransformIndicator[labelno-1] = 0;
					break;
				}

				if (std::abs(lastICPError - currConfWiReject.sumRegError) < earlyOut)
				{
					break;
				}

				lastICPError = currConfWiReject.sumRegError;
			}
		}

		// Check all part transformation
		int sumIndicator = 0;
		for (int labelno = 0; labelno < GlobalAppState::getInstance().s_labelNum - 1; labelno++)
		{
			sumIndicator += deltaLabelTransformIndicator[labelno];
		}
		if (sumIndicator == 0)
		{
			return m_matrixTrackingLost;
		}

		// Optimize part matrix
		DX11ImageHelper::applyGetConfidenceNormalPointWithoutBG(context, inputSRV, inputNormalsSRV, inputLabelsSRV, m_inputConfidenceWithoutBGFloatUAV, m_inputNormalsWithoutBGFloat4UAV, m_inputPointsWithoutBGFloat4UAV, m_inputIndexWithoutBGIntUAV, m_imageWidth[0], m_imageHeight[0], GlobalAppState::getInstance().s_labelNum);

		HRESULT hr = S_OK;
		D3D11_MAPPED_SUBRESOURCE mappedConfidenceResource, mappedNormalsResource, mappedPointsResource, mappedIndexResource;

		context->CopyResource(m_inputConfidenceWithoutBGFloatCPU, m_inputConfidenceWithoutBGFloat);
		hr = context->Map(m_inputConfidenceWithoutBGFloatCPU, 0, D3D11_MAP_READ, 0, &mappedConfidenceResource);
		if (hr == S_FALSE)
			MessageBox(NULL, L"Save Input Confidence Failed", L"Failed", MB_OK);

		context->CopyResource(m_inputNormalsWithoutBGFloat4CPU, m_inputNormalsWithoutBGFloat4);
		hr = context->Map(m_inputNormalsWithoutBGFloat4CPU, 0, D3D11_MAP_READ, 0, &mappedNormalsResource);
		if (hr == S_FALSE)
			MessageBox(NULL, L"Save Input Normals Without Background Failed", L"Failed", MB_OK);

		context->CopyResource(m_inputPointsWithoutBGFloat4CPU, m_inputPointsWithoutBGFloat4);
		hr = context->Map(m_inputPointsWithoutBGFloat4CPU, 0, D3D11_MAP_READ, 0, &mappedPointsResource);
		if (hr == S_FALSE)
			MessageBox(NULL, L"Save Input Points Without Background Failed", L"Failed", MB_OK);

		context->CopyResource(m_inputIndexWithoutBGIntCPU, m_inputIndexWithoutBGInt);
		hr = context->Map(m_inputIndexWithoutBGIntCPU, 0, D3D11_MAP_READ, 0, &mappedIndexResource);
		if (hr == S_FALSE)
			MessageBox(NULL, L"Save Input Index Failed", L"Failed", MB_OK);

		float* confidenceInput = (float*)mappedConfidenceResource.pData;
		float* normalsInput = (float*)mappedNormalsResource.pData;
		float* pointsInput = (float*)mappedPointsResource.pData;
		int* pointsIndex = (int*)mappedIndexResource.pData;

		std::vector<float> *sourceConfidence = new std::vector<float>[GlobalAppState::getInstance().s_labelNum-1];
		std::vector<cv::Point3f> *sourceNormals = new std::vector<cv::Point3f>[GlobalAppState::getInstance().s_labelNum-1];

		float* partPoints = new float[GlobalAppState::getInstance().s_labelNum-1]; float sumPoints = 0.0f;
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			partPoints[i] = 0.0f;
		}

		for (int j = 0; j < 480; j++)
		{
			for (int i = 0; i < 640; i++)
			{
				float c = confidenceInput[i + 640 * j];

				float px = pointsInput[(i + 640 * j) * 4 + 0];
				float py = pointsInput[(i + 640 * j) * 4 + 1];
				float pz = pointsInput[(i + 640 * j) * 4 + 2];
				float pw = pointsInput[(i + 640 * j) * 4 + 3];

				float nx = normalsInput[(i + 640 * j) * 4 + 0];
				float ny = normalsInput[(i + 640 * j) * 4 + 1];
				float nz = normalsInput[(i + 640 * j) * 4 + 2];
				float nw = normalsInput[(i + 640 * j) * 4 + 3];

				int index = pointsIndex[i + 640 * j];

				if (isfinite(px) && isfinite(nx) && (index != 0))
				{
					sourceConfidence[index-1].push_back(c);
					sourceNormals[index-1].push_back(cv::Point3f(nx, ny, nz));
					
					partPoints[index-1] += 1;
					sumPoints += 1;
				}
			}
		}

		float *pointsWeight = new float[GlobalAppState::getInstance().s_labelNum-1];
		float *normalWeight = new float[GlobalAppState::getInstance().s_labelNum-1];
		float *confidWeight = new float[GlobalAppState::getInstance().s_labelNum-1];
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			pointsWeight[i] = 0.0f;
			normalWeight[i] = 0.0f;
			confidWeight[i] = 0.0f;
		}

		// size weight
		if (sumPoints != 0)
		{
			for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
			{
				pointsWeight[i] = partPoints[i] / sumPoints;
			}
		}

		// normal weight
		cv::Point3f *averageNormal = new cv::Point3f[GlobalAppState::getInstance().s_labelNum - 1];
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			averageNormal[i] = cv::Point3f(0.0f, 0.0f, 0.0f);
		}
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			for (int j = 0; j < sourceNormals[i].size(); j++)
			{
				averageNormal[i] += sourceNormals[i][j];
			}
		}
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			if (sourceNormals[i].size() != 0)
			{
				averageNormal[i].x /= (float)sourceNormals[i].size();
				averageNormal[i].y /= (float)sourceNormals[i].size();
				averageNormal[i].z /= (float)sourceNormals[i].size();
			}
		}
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			for (int j = 0; j < sourceNormals[i].size(); j++)
			{
				cv::Point3f tmp_normal = sourceNormals[i][j] - averageNormal[i];
				normalWeight[i] += (tmp_normal.x*tmp_normal.x + tmp_normal.y*tmp_normal.y + tmp_normal.z*tmp_normal.z);
			}
		}
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			if (sourceNormals[i].size() != 0)
			{
				normalWeight[i] /= (float)sourceNormals[i].size();
			}
		}
		delete[] averageNormal;

		// confidence weight
		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			for (int j = 0; j < sourceConfidence[i].size(); j++)
			{
				confidWeight[i] += sourceConfidence[i][j];
			}
		}

		for (int i = 0; i < GlobalAppState::getInstance().s_labelNum - 1; i++)
		{
			if (sourceConfidence[i].size() != 0)
			{
				confidWeight[i] /= (float)sourceConfidence[i].size();
			}
		}
		
		std::vector<cv::Point3f> sourcePointsTest, targetPointsTest;
		std::vector<float> weights;

		for (int j = 0; j < 480; j++)
		{
			for (int i = 0; i < 640; i++)
			{
				float x = pointsInput[(i + 640 * j) * 4 + 0];
				float y = pointsInput[(i + 640 * j) * 4 + 1];
				float z = pointsInput[(i + 640 * j) * 4 + 2];
				float w = pointsInput[(i + 640 * j) * 4 + 3];

				int index = pointsIndex[i + 640 * j];
				if (isfinite(x) && isfinite(y) && isfinite(z))
				{
					for (int labelno = 0; labelno < GlobalAppState::getInstance().s_labelNum - 1; labelno++)
					{
						if (deltaLabelTransformIndicator[labelno])
						{
							sourcePointsTest.push_back(cv::Point3f(x, y, z));

							Eigen::Vector4f transPoint = deltaLabelTransform[labelno] * Eigen::Vector4f(x, y, z, 1.0f);
							targetPointsTest.push_back(cv::Point3f(transPoint.x(), transPoint.y(), transPoint.z()));

							weights.push_back(confidWeight[index-1]+normalWeight[index-1]+pointsWeight[index-1]);
						}
					}
				}
			}
		}

		Eigen::Matrix3d matR; float fScale;
		Eigen::Vector3d vTranslation;

		bool weighted = false;
		if (!weighted)
		{
			CAbsoluteOrientation::ApplyToPoint(sourcePointsTest, targetPointsTest, matR, fScale, vTranslation);
		}
		else
		{
			CAbsoluteOrientation::ApplyToPoint(sourcePointsTest, targetPointsTest, weights, matR, fScale, vTranslation);
		}

		deltaTransform.setIdentity();
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				deltaTransform(i, j) = matR(i, j);
		for (int i = 0; i < 3; i++)
			deltaTransform(i, 3) = vTranslation(i, 0);

		delete[] pointsWeight;
		delete[] normalWeight;
		delete[] confidWeight;

		delete[] partPoints;
		delete[] sourceNormals;
		delete[] sourceConfidence;

		context->Unmap(m_inputConfidenceWithoutBGFloatCPU, 0);
		context->Unmap(m_inputNormalsWithoutBGFloat4CPU, 0);
		context->Unmap(m_inputPointsWithoutBGFloat4CPU, 0);
		context->Unmap(m_inputIndexWithoutBGIntCPU, 0);
		// Optimize part matrix

		delete[] deltaLabelTransformIndicator;
		delete[] deltaLabelTransform;

		return deltaTransform;
	}
}

HRESULT DX11CameraTrackingMultiRes::computeCorrespondences(ID3D11DeviceContext* context, 
	                                                       ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputIntensitiesSRV, ID3D11ShaderResourceView** inputLabelsSRV,
	                                                       ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelIntensitiesSRV, ID3D11ShaderResourceView** modelLabelsSRV,
	                                                       D3DXVECTOR3& mean, float& meanStDev, float& nValidCorres, const Eigen::Matrix4f& deltaTransform, unsigned int level, float distThres, float normalThres, float colorThres, float labelThres, int labelNo)
{
	float levelFactor = pow(2.0f, (float)level);

	HRESULT hr;
	hr = DX11ImageHelper::applyProjectiveCorrespondences(context,
														 inputSRV, inputNormalsSRV, inputIntensitiesSRV, inputLabelsSRV,
														 modelSRV, modelNormalsSRV, modelIntensitiesSRV, modelLabelsSRV,
														 m_pCorrespondenceTextureFloat4UAV[level], m_pCorrespondenceNormalTextureFloat4UAV[level],
														 deltaTransform, m_imageWidth[level], m_imageHeight[level],
														 GlobalAppState::getInstance().s_labelNum, level, GlobalCameraTrackingState::getInstance().s_distWeight, GlobalCameraTrackingState::getInstance().s_normalWeight, GlobalCameraTrackingState::getInstance().s_colorWeight, GlobalCameraTrackingState::getInstance().s_labelWeight,
														 distThres, normalThres, colorThres, labelThres, levelFactor, labelNo);
	hr = DX11NormalizeReduction::applyNorm(context, m_pCorrespondenceTextureFloat4SRV[level], level, m_imageWidth[level], m_imageHeight[level], mean, meanStDev, nValidCorres);

	return hr;
}

void DX11CameraTrackingMultiRes::OnD3D11DestroyDevice()
{
	// Do not free input buffers of last applyCT call
	m_inputTextureFloat4SRV[0] = NULL;
	m_inputNormalTextureFloat4SRV[0] = NULL;
	m_inputColorTextureFloat4SRV[0] = NULL;

	m_modelTextureFloat4SRV[0] = NULL;
	m_modelNormalTextureFloat4SRV[0] = NULL;
	m_modelColorTextureFloat4SRV[0] = NULL;

	/////////////////////////////////////////////////////
	// Camera Tracking Multi Res
	/////////////////////////////////////////////////////

	for(unsigned int i = 0; i<GlobalCameraTrackingState::getInstance().s_maxLevels; i++)
	{
		SAFE_RELEASE(m_pCorrespondenceTextureFloat42D[i]);
		SAFE_RELEASE(m_pCorrespondenceTextureFloat4SRV[i]);
		SAFE_RELEASE(m_pCorrespondenceTextureFloat4UAV[i]);

		SAFE_RELEASE(m_pCorrespondenceNormalTextureFloat42D[i]);
		SAFE_RELEASE(m_pCorrespondenceNormalTextureFloat4SRV[i]);
		SAFE_RELEASE(m_pCorrespondenceNormalTextureFloat4UAV[i]);

		SAFE_RELEASE(m_inputTextureFloat42D[i]);
		SAFE_RELEASE(m_inputTextureFloat4SRV[i]);
		SAFE_RELEASE(m_inputTextureFloat4UAV[i]);

		SAFE_RELEASE(m_inputNormalTextureFloat42D[i]);
		SAFE_RELEASE(m_inputNormalTextureFloat4SRV[i]);
		SAFE_RELEASE(m_inputNormalTextureFloat4UAV[i]);

		SAFE_RELEASE(m_inputColorTextureFloat42D[i]);
		SAFE_RELEASE(m_inputColorTextureFloat4SRV[i]);
		SAFE_RELEASE(m_inputColorTextureFloat4UAV[i]);

		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			SAFE_RELEASE(m_inputLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]);
			SAFE_RELEASE(m_inputLabelTextureFloat4SRV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]);
			SAFE_RELEASE(m_inputLabelTextureFloat4UAV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]);
		}

		SAFE_RELEASE(m_inputIntensityTextureFloat42D[i]);
		SAFE_RELEASE(m_inputIntensityTextureFloat4SRV[i]);
		SAFE_RELEASE(m_inputIntensityTextureFloat4UAV[i]);

		SAFE_RELEASE(m_modelTextureFloat42D[i]);
		SAFE_RELEASE(m_modelTextureFloat4SRV[i]);
		SAFE_RELEASE(m_modelTextureFloat4UAV[i]);

		SAFE_RELEASE(m_modelNormalTextureFloat42D[i]);
		SAFE_RELEASE(m_modelNormalTextureFloat4SRV[i]);
		SAFE_RELEASE(m_modelNormalTextureFloat4UAV[i]);

		SAFE_RELEASE(m_modelColorTextureFloat42D[i]);
		SAFE_RELEASE(m_modelColorTextureFloat4SRV[i]);
		SAFE_RELEASE(m_modelColorTextureFloat4UAV[i]);

		for (unsigned int j = 0; j < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; j++)
		{
			SAFE_RELEASE(m_modelLabelTextureFloat42D[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]);
			SAFE_RELEASE(m_modelLabelTextureFloat4SRV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]);
			SAFE_RELEASE(m_modelLabelTextureFloat4UAV[i*LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL + j]);
		}

		SAFE_RELEASE(m_modelIntensityTextureFloat42D[i]);
		SAFE_RELEASE(m_modelIntensityTextureFloat4SRV[i]);
		SAFE_RELEASE(m_modelIntensityTextureFloat4UAV[i]);
	}

	SAFE_DELETE_ARRAY(m_pCorrespondenceTextureFloat42D);
	SAFE_DELETE_ARRAY(m_pCorrespondenceTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_pCorrespondenceTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_pCorrespondenceNormalTextureFloat42D);
	SAFE_DELETE_ARRAY(m_pCorrespondenceNormalTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_pCorrespondenceNormalTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_inputTextureFloat42D);
	SAFE_DELETE_ARRAY(m_inputTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_inputTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_inputNormalTextureFloat42D);
	SAFE_DELETE_ARRAY(m_inputNormalTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_inputNormalTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_inputColorTextureFloat42D);
	SAFE_DELETE_ARRAY(m_inputColorTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_inputColorTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_inputLabelTextureFloat42D);
	SAFE_DELETE_ARRAY(m_inputLabelTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_inputLabelTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_inputIntensityTextureFloat42D);
	SAFE_DELETE_ARRAY(m_inputIntensityTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_inputIntensityTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_modelTextureFloat42D);
	SAFE_DELETE_ARRAY(m_modelTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_modelTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_modelNormalTextureFloat42D);
	SAFE_DELETE_ARRAY(m_modelNormalTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_modelNormalTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_modelColorTextureFloat42D);
	SAFE_DELETE_ARRAY(m_modelColorTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_modelColorTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_modelLabelTextureFloat42D);
	SAFE_DELETE_ARRAY(m_modelLabelTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_modelLabelTextureFloat4UAV);

	SAFE_DELETE_ARRAY(m_modelIntensityTextureFloat42D);
	SAFE_DELETE_ARRAY(m_modelIntensityTextureFloat4SRV);
	SAFE_DELETE_ARRAY(m_modelIntensityTextureFloat4UAV);

	SAFE_RELEASE(m_inputConfidenceWithoutBGFloat);
	SAFE_RELEASE(m_inputConfidenceWithoutBGFloatCPU);
	SAFE_RELEASE(m_inputConfidenceWithoutBGFloatUAV);

	SAFE_RELEASE(m_inputNormalsWithoutBGFloat4);
	SAFE_RELEASE(m_inputNormalsWithoutBGFloat4CPU);
	SAFE_RELEASE(m_inputNormalsWithoutBGFloat4UAV);

	SAFE_RELEASE(m_inputPointsWithoutBGFloat4);
	SAFE_RELEASE(m_inputPointsWithoutBGFloat4CPU);
	SAFE_RELEASE(m_inputPointsWithoutBGFloat4UAV);

	SAFE_RELEASE(m_inputIndexWithoutBGInt);
	SAFE_RELEASE(m_inputIndexWithoutBGIntCPU);
	SAFE_RELEASE(m_inputIndexWithoutBGIntUAV);

	SAFE_DELETE_ARRAY(m_imageHeight);
	SAFE_DELETE_ARRAY(m_imageWidth);
}