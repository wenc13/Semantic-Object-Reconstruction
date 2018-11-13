#include "stdafx.h"
#include "DX11ImageHelper.h"
#include "GlobalAppState.h"
#include "GlobalCameraTrackingState.h"

#include <vector>
#include <iostream>

/////////////////////////////////////////////////////
// Bilateral Completion
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeBilateralCompletion = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferBilateralCompletion = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderBilateralCompletion = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderBilateralCompletionWithColor = NULL;

/////////////////////////////////////////////////////
// Bilateral Filter
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeBilateralFilter = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferBilateralFilter = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderBilateralFilter = NULL;

/////////////////////////////////////////////////////
// Bilateral Filter SSAO
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeBilateralFilterForSSAO = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferBilateralFilterForSSAO = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderBilateralFilterForSSAO = NULL;

/////////////////////////////////////////////////////
// Bilateral Approximation
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeBilateralFilterApprox = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferBilateralFilterApprox = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderBilateralFilterApprox = NULL;

/////////////////////////////////////////////////////
// Bilateral Filter 4F
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeBilateralFilter4F = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferBilateralFilter4F = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderBilateralFilter4F = NULL;

/////////////////////////////////////////////////////
// Normal Computation
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeNormalComputation = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferNormalComputation = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderNormalComputation = NULL;

/////////////////////////////////////////////////////
// Camera Space To Depth Map
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeCameraSpace = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferCameraSpace = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderCameraSpace = NULL;

/////////////////////////////////////////////////////
// Compute Depth Map
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeDepthMap = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferDepthMap = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderDepthMap = NULL;

/////////////////////////////////////////////////////
// Compute SSAO Map
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeSSAOMap = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferSSAOMap = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderSSAOMap = NULL;

std::vector<vec4f> DX11ImageHelper::m_randomRotations;

/////////////////////////////////////////////////////
// Compute HSV Depth Map
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeHSVDepth = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferHSVDepth = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderHSVDepth = NULL;

/////////////////////////////////////////////////////
// Camera Space Projection
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeCameraSpaceProjection = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferCameraSpaceProjection = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderCameraSpaceProjection = NULL;

/////////////////////////////////////////////////////
// Stereo Mask Camera Space Projection
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeStereoCameraSpaceProjection = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferStereoCameraSpaceProjection = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderStereoCameraSpaceProjection = NULL;

/////////////////////////////////////////////////////
// Projective Correspondences
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeProjectiveCorrespondences = 16;

ID3D11Buffer* DX11ImageHelper::m_constantBufferProjectiveCorrespondences = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderProjectiveCorrespondences = NULL;

/////////////////////////////////////////////////////
// Depth aware block averaging
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeBlockAverage = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferBlockAverage = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderBlockAverage = NULL;

/////////////////////////////////////////////////////
// Subsample
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeSubSamp = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferSubSamp = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderSubSamp = NULL;

/////////////////////////////////////////////////////
// Downsampling
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeDownsampling = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferDownsampling = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderDownsampling = NULL;

/////////////////////////////////////////////////////
// Copy
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeCopy = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferCopy = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderCopy = NULL;

/////////////////////////////////////////////////////
// Convert color to intensity float
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeConvertColorToIntensityFloat = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferConvertColorToIntensityFloat = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderConvertColorToIntensityFloat = NULL;

/////////////////////////////////////////////////////
// Convert label
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeConvertLabel = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferConvertLabel = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderConvertLabel = NULL;

/////////////////////////////////////////////////////
// Split label to float
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeSplitLabelToFloat = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferSplitLabelToFloat = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderSplitLabelToFloat = NULL;

/////////////////////////////////////////////////////
// Save as Texture
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeSaveAsTexture = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferSaveAsTexture = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderSaveAsTexture = NULL;

/////////////////////////////////////////////////////
// Convert input label to RGBW
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeInputLabelToRGBW = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferInputLabelToRGBW = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderInputLabelToRGBW = NULL;

/////////////////////////////////////////////////////
// Convert model label to RGBW
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeModelLabelToRGBW = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferModelLabelToRGBW = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderModelLabelToRGBW = NULL;

/////////////////////////////////////////////////////
// Compute gradient
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeComputeGradient = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferComputeGradient = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderComputeGradient = NULL;

/////////////////////////////////////////////////////
// Compute confidence and entropy
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeComputeConfidenceAndEntropy = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferComputeConfidenceAndEntropy = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderComputeConfidenceAndEntropy = NULL;

/////////////////////////////////////////////////////
// Get points
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeGetPoints = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferGetPoints = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderGetPoints = NULL;

/////////////////////////////////////////////////////
// Get points without background
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeGetPointsWithoutBG = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferGetPointsWithoutBG = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderGetPointsWithoutBG = NULL;

/////////////////////////////////////////////////////
// Get texture without background
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeGetTextureWithoutBG = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferGetTextureWithoutBG = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderGetTextureWithoutBG = NULL;

/////////////////////////////////////////////////////
// Get confidence, normal and point without background
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeGetConfidenceNormalPointWithoutBG = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferGetConfidenceNormalPointWithoutBG = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderGetConfidenceNormalPointWithoutBG = NULL;

/////////////////////////////////////////////////////
// Get contour
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeGetContour = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferGetContour = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderGetContour = NULL;

/////////////////////////////////////////////////////
// Get contour confidence
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeGetContourConfidence = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferGetContourConfidence = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderGetContourConfidence = NULL;

/////////////////////////////////////////////////////
// Center of gravity
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeCenterOfGravity = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferCenterOfGravity = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderCenterOfGravity = NULL;

/////////////////////////////////////////////////////
// Erode
/////////////////////////////////////////////////////

unsigned int DX11ImageHelper::m_blockSizeErode = 16;
ID3D11Buffer* DX11ImageHelper::m_constantBufferErode = NULL;
ID3D11ComputeShader* DX11ImageHelper::m_pComputeShaderErode = NULL;

/////////////////////////////////////////////////////
// Timer
/////////////////////////////////////////////////////

Timer DX11ImageHelper::m_timer;

HRESULT DX11ImageHelper::OnD3D11CreateDevice( ID3D11Device* pd3dDevice )
{
	HRESULT hr = S_OK;

	/////////////////////////////////////////////////////
	// Bilateral Completion
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_BilateralCompletion[5];
	sprintf_s(BLOCK_SIZE_BilateralCompletion, "%d", m_blockSizeBilateralCompletion);

	D3D_SHADER_MACRO shaderDefinesBilateralCompletion[] = { { "groupthreads", BLOCK_SIZE_BilateralCompletion }, { 0 } };

	ID3DBlob* pBlob = NULL;
	hr = CompileShaderFromFile(L"Shaders\\BilateralFilter.hlsl", "bilateralCompletionCS", "cs_5_0", &pBlob, shaderDefinesBilateralCompletion);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderBilateralCompletion);
	if(FAILED(hr)) return hr;

	D3D_SHADER_MACRO shaderDefinesBilateralCompletionWithColor[] = { { "groupthreads", BLOCK_SIZE_BilateralCompletion }, { "WITH_COLOR", "1"}, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\BilateralFilter.hlsl", "bilateralCompletionCS", "cs_5_0", &pBlob, shaderDefinesBilateralCompletionWithColor);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderBilateralCompletionWithColor);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	D3D11_BUFFER_DESC bDesc;
	bDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	bDesc.Usage	= D3D11_USAGE_DYNAMIC;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags	= 0;

	bDesc.ByteWidth	= sizeof(CBufferBilateralCompletion) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferBilateralCompletion);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Bilateral Filter
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_BF[5];
	sprintf_s(BLOCK_SIZE_BF, "%d", m_blockSizeBilateralFilter);

	D3D_SHADER_MACRO shaderDefinesBF[] = { { "groupthreads", BLOCK_SIZE_BF }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\BilateralFilter.hlsl", "bilateralFilterCS", "cs_5_0", &pBlob, shaderDefinesBF);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderBilateralFilter);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	bDesc.Usage	= D3D11_USAGE_DYNAMIC;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags	= 0;

	bDesc.ByteWidth	= sizeof(CBufferBilateralFilter) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferBilateralFilter);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Bilateral Filter SSAO
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_BFSSAO[5];
	sprintf_s(BLOCK_SIZE_BFSSAO, "%d", m_blockSizeBilateralFilterForSSAO);

	D3D_SHADER_MACRO shaderDefinesBFSSAO[] = { { "groupthreads", BLOCK_SIZE_BFSSAO }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\BilateralFilterSSAO.hlsl", "bilateralFilterSSAOCS", "cs_5_0", &pBlob, shaderDefinesBFSSAO);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderBilateralFilterForSSAO);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	bDesc.Usage	= D3D11_USAGE_DYNAMIC;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags	= 0;

	bDesc.ByteWidth	= sizeof(CBufferBilateralFilterSSAO) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferBilateralFilterForSSAO);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Bilateral Filter Approximation
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_BFApprox[5];
	sprintf_s(BLOCK_SIZE_BFApprox, "%d", m_blockSizeBilateralFilterApprox);

	D3D_SHADER_MACRO shaderDefinesBFApprox[] = { { "groupthreads", BLOCK_SIZE_BFApprox }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\BilateralFilterApprox.hlsl", "bilateralFilterApproxCS", "cs_5_0", &pBlob, shaderDefinesBFApprox);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderBilateralFilterApprox);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	bDesc.Usage	= D3D11_USAGE_DYNAMIC;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags	= 0;

	bDesc.ByteWidth	= sizeof(CBufferBilateralFilterApprox) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferBilateralFilterApprox);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Bilateral Filter 4F
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_BF4F[5];
	sprintf_s(BLOCK_SIZE_BF4F, "%d", m_blockSizeBilateralFilter4F);

	D3D_SHADER_MACRO shaderDefinesBF4F[] = { { "groupthreads", BLOCK_SIZE_BF4F }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\BilateralFilter.hlsl", "bilateralFilter4FCS", "cs_5_0", &pBlob, shaderDefinesBF4F);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderBilateralFilter4F);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.BindFlags	= D3D11_BIND_CONSTANT_BUFFER;
	bDesc.Usage	= D3D11_USAGE_DYNAMIC;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags	= 0;

	bDesc.ByteWidth	= sizeof(CBufferBilateralFilter4F) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferBilateralFilter4F);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Normal Computation
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_NC[5];
	sprintf_s(BLOCK_SIZE_NC, "%d", m_blockSizeNormalComputation);

	D3D_SHADER_MACRO shaderDefinesNC[] = { { "groupthreads", BLOCK_SIZE_NC }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\NormalComputation.hlsl", "normalComputationCS", "cs_5_0", &pBlob, shaderDefinesNC);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderNormalComputation);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferNormalComputation) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferNormalComputation);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Camera Space To Depth Map
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_CS[5];
	sprintf_s(BLOCK_SIZE_CS, "%d", m_blockSizeCameraSpace);

	D3D_SHADER_MACRO shaderDefinesCS[] = { { "groupthreads", BLOCK_SIZE_CS }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\CameraSpaceProjection.hlsl", "cameraSpaceToDepthMapCS", "cs_5_0", &pBlob, shaderDefinesCS);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderCameraSpace);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferCameraSpace) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferCameraSpace);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Compute Depth Maps
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_DM[5];
	sprintf_s(BLOCK_SIZE_DM, "%d", m_blockSizeDepthMap);

	D3D_SHADER_MACRO shaderDefinesDM[] = { { "groupthreads", BLOCK_SIZE_DM }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\DepthMapComputation.hlsl", "depthMapComputationCS", "cs_5_0", &pBlob, shaderDefinesDM);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderDepthMap);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferDepthMap);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferDepthMap);
	if(FAILED(hr)) return hr;


	/////////////////////////////////////////////////////
	// Compute SSAO Map
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_SSAOMap[5];
	sprintf_s(BLOCK_SIZE_SSAOMap, "%d", m_blockSizeSSAOMap);

	D3D_SHADER_MACRO shaderDefinesSSAOMap[] = { { "groupthreads", BLOCK_SIZE_SSAOMap }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\ComputeSSAOMap.hlsl", "computeSSAOMapCS", "cs_5_0", &pBlob, shaderDefinesSSAOMap);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderSSAOMap);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferSSAOMap);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferSSAOMap);
	if(FAILED(hr)) return hr;

	for(unsigned int i = 0; i<16; i++)
	{
		float r0 = (float)std::rand()/(float)RAND_MAX; r0-=0.5f; r0*=2.0f;
		float r1 = (float)std::rand()/(float)RAND_MAX; r1-=0.5f; r1*=2.0f;
		float r2 = (float)std::rand()/(float)RAND_MAX; r2-=0.5f; r2*=2.0f;

		m_randomRotations.push_back(vec4f(r0, r1, r2, 1.0));
	}

	/////////////////////////////////////////////////////
	// Compute HSV Depth Maps
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_HSVDepth[5];
	sprintf_s(BLOCK_SIZE_HSVDepth, "%d", m_blockSizeHSVDepth);

	D3D_SHADER_MACRO shaderDefinesHSVDepth[] = { { "groupthreads", BLOCK_SIZE_HSVDepth }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\HSVDepthMapComputation.hlsl", "HSVdepthMapComputationCS", "cs_5_0", &pBlob, shaderDefinesHSVDepth);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderHSVDepth);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferHSVDepth);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferHSVDepth);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Camera Space Projection
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_CP[5];
	sprintf_s(BLOCK_SIZE_CP, "%d", m_blockSizeCameraSpaceProjection);

	D3D_SHADER_MACRO shaderDefinesCP[] = { { "groupthreads", BLOCK_SIZE_CP }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\CameraSpaceProjection.hlsl", "cameraSpaceProjectionCS", "cs_5_0", &pBlob, shaderDefinesCP);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderCameraSpaceProjection);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferCameraSpaceProjection) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferCameraSpaceProjection);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Stereo Mask Camera Space Projection
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_SM[5];
	sprintf_s(BLOCK_SIZE_SM, "%d", m_blockSizeStereoCameraSpaceProjection);

	D3D_SHADER_MACRO shaderDefinesSM[] = { { "groupthreads", BLOCK_SIZE_SM }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\StereoMaskCameraSpaceProjection.hlsl", "stereoMaskCameraSpaceProjectionCS", "cs_5_0", &pBlob, shaderDefinesSM);
	if(FAILED(hr)) return hr;

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderStereoCameraSpaceProjection);
	if(FAILED(hr)) return hr;

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferStereoCameraSpaceProjection) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferStereoCameraSpaceProjection);
	if(FAILED(hr)) return hr;

	/////////////////////////////////////////////////////
	// Projective Correspondences
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_PC[5];
	sprintf_s(BLOCK_SIZE_PC, "%d", m_blockSizeProjectiveCorrespondences);

	D3D_SHADER_MACRO shaderDefinesPC[] = { { "groupthreads", BLOCK_SIZE_PC }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\ProjectiveCorrespondences.hlsl", "projectiveCorrespondencesCS", "cs_5_0", &pBlob, shaderDefinesPC);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderProjectiveCorrespondences);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferProjectiveCorrespondences) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferProjectiveCorrespondences);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Downsampling
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_DS[5];
	sprintf_s(BLOCK_SIZE_DS, "%d", m_blockSizeDownsampling);

	D3D_SHADER_MACRO shaderDefinesDS[] = { { "groupthreads", BLOCK_SIZE_DS }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\Downsampling.hlsl", "downsamplingCS", "cs_5_0", &pBlob, shaderDefinesDS);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderDownsampling);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferDS) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferDownsampling);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Subsample
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_SubSamp[5];
	sprintf_s(BLOCK_SIZE_SubSamp, "%d", m_blockSizeSubSamp);

	D3D_SHADER_MACRO shaderDefinesSubSamp[] = { { "groupthreads", BLOCK_SIZE_SubSamp }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\SubSampling.hlsl", "subSampleCS", "cs_5_0", &pBlob, shaderDefinesSubSamp);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderSubSamp);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferSubSamp) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferSubSamp);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Depth aware block averaging
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_BlockAverage[5];
	sprintf_s(BLOCK_SIZE_BlockAverage, "%d", m_blockSizeBlockAverage);

	D3D_SHADER_MACRO shaderDefinesBlockAverage[] = { { "groupthreads", BLOCK_SIZE_BlockAverage }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\BlockAveraging.hlsl", "averageCS", "cs_5_0", &pBlob, shaderDefinesBlockAverage);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderBlockAverage);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferBlockAverage) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferBlockAverage);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Copy
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_Copy[5];
	sprintf_s(BLOCK_SIZE_Copy, "%d", m_blockSizeCopy);

	D3D_SHADER_MACRO shaderDefinesCopy[] = { { "groupthreads", BLOCK_SIZE_Copy }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\Copy.hlsl", "copyCS", "cs_5_0", &pBlob, shaderDefinesCopy);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderCopy);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferCopy) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferCopy);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Convert color to intensity float
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_ConvertColorToIntensityFloat[5];
	sprintf_s(BLOCK_SIZE_ConvertColorToIntensityFloat, "%d", m_blockSizeConvertColorToIntensityFloat);

	D3D_SHADER_MACRO shaderDefinesConvertColorToIntensityFloat[] = { { "groupthreads", BLOCK_SIZE_ConvertColorToIntensityFloat }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\ConvertColorToIntensityFloat.hlsl", "convertColorToIntensityFloatCS", "cs_5_0", &pBlob, shaderDefinesConvertColorToIntensityFloat);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderConvertColorToIntensityFloat);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferConvertColorToIntensityFloat);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferConvertColorToIntensityFloat);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Convert label
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_ConvertLabel[5];
	sprintf_s(BLOCK_SIZE_ConvertLabel, "%d", m_blockSizeConvertLabel);

	D3D_SHADER_MACRO shaderDefinesConvertLabel[] = { { "groupthreads", BLOCK_SIZE_ConvertLabel },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\ConvertLabel.hlsl", "convertLabelCS", "cs_5_0", &pBlob, shaderDefinesConvertLabel);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderConvertLabel);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferConvertLabel);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferConvertLabel);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Split label to float
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_SplitLabelToFloat[5];
	sprintf_s(BLOCK_SIZE_SplitLabelToFloat, "%d", m_blockSizeSplitLabelToFloat);

	D3D_SHADER_MACRO shaderDefinesSplitLabelToFloat[] = { { "groupthreads", BLOCK_SIZE_SplitLabelToFloat },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\SplitLabelToFloat.hlsl", "splitLabelToFloatCS", "cs_5_0", &pBlob, shaderDefinesSplitLabelToFloat);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderSplitLabelToFloat);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferSplitLabelToFloat);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferSplitLabelToFloat);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Save as Texture
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_SaveAsTexture[5];
	sprintf_s(BLOCK_SIZE_SaveAsTexture, "%d", m_blockSizeSaveAsTexture);

	D3D_SHADER_MACRO shaderDefinesSaveAsTexture[] = { { "groupthreads", BLOCK_SIZE_SaveAsTexture },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\SaveAsTexture.hlsl", "saveAsTextureCS", "cs_5_0", &pBlob, shaderDefinesSaveAsTexture);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderSaveAsTexture);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferSaveAsTexture);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferSaveAsTexture);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Convert input label to RGBW
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_InputLabelToRGBW[5];
	sprintf_s(BLOCK_SIZE_InputLabelToRGBW, "%d", m_blockSizeInputLabelToRGBW);

	D3D_SHADER_MACRO shaderDefinesInputLabelToRGBW[] = { { "groupthreads", BLOCK_SIZE_InputLabelToRGBW },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\InputLabelToRGBW.hlsl", "inputLabelToRGBWCS", "cs_5_0", &pBlob, shaderDefinesInputLabelToRGBW);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderInputLabelToRGBW);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferInputLabelToRGBW);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferInputLabelToRGBW);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Convert model label to RGBW
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_ModelLabelToRGBW[5];
	sprintf_s(BLOCK_SIZE_ModelLabelToRGBW, "%d", m_blockSizeModelLabelToRGBW);

	D3D_SHADER_MACRO shaderDefinesModelLabelToRGBW[] = { { "groupthreads", BLOCK_SIZE_ModelLabelToRGBW },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\ModelLabelToRGBW.hlsl", "modelLabelToRGBWCS", "cs_5_0", &pBlob, shaderDefinesModelLabelToRGBW);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderModelLabelToRGBW);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferModelLabelToRGBW);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferModelLabelToRGBW);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Compute gradient
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_ComputeGradient[5];
	sprintf_s(BLOCK_SIZE_ComputeGradient, "%d", m_blockSizeComputeGradient);

	D3D_SHADER_MACRO shaderDefinesComputeGradient[] = { { "groupthreads", BLOCK_SIZE_ComputeGradient }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\ComputeGradient.hlsl", "computeGradientCS", "cs_5_0", &pBlob, shaderDefinesComputeGradient);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderComputeGradient);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferComputeGradient);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferComputeGradient);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Compute confidence and entropy
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_ComputeConfidenceAndEntropy[5];
	sprintf_s(BLOCK_SIZE_ComputeConfidenceAndEntropy, "%d", m_blockSizeComputeConfidenceAndEntropy);

	D3D_SHADER_MACRO shaderDefinesComputeConfidenceAndEntropy[] = { { "groupthreads", BLOCK_SIZE_ComputeConfidenceAndEntropy },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\ComputeConfidenceAndEntropy.hlsl", "computeConfidenceAndEntropyCS", "cs_5_0", &pBlob, shaderDefinesComputeConfidenceAndEntropy);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderComputeConfidenceAndEntropy);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferComputeConfidenceAndEntropy);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferComputeConfidenceAndEntropy);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Get points
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_GetPoints[5];
	sprintf_s(BLOCK_SIZE_GetPoints, "%d", m_blockSizeGetPoints);

	D3D_SHADER_MACRO shaderDefinesGetPoints[] = { { "groupthreads", BLOCK_SIZE_GetPoints },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\GetPoints.hlsl", "getPointsCS", "cs_5_0", &pBlob, shaderDefinesGetPoints);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderGetPoints);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferGetPoints);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferGetPoints);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Get points without background
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_GetPointsWithoutBG[5];
	sprintf_s(BLOCK_SIZE_GetPointsWithoutBG, "%d", m_blockSizeGetPointsWithoutBG);

	D3D_SHADER_MACRO shaderDefinesGetPointsWithoutBG[] = { { "groupthreads", BLOCK_SIZE_GetPointsWithoutBG },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\GetPointsWithoutBG.hlsl", "getPointsWithoutBGCS", "cs_5_0", &pBlob, shaderDefinesGetPointsWithoutBG);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderGetPointsWithoutBG);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferGetPointsWithoutBG);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferGetPointsWithoutBG);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Get texture without background
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_GetTextureWithoutBG[5];
	sprintf_s(BLOCK_SIZE_GetTextureWithoutBG, "%d", m_blockSizeGetTextureWithoutBG);

	D3D_SHADER_MACRO shaderDefinesGetTextureWithoutBG[] = { { "groupthreads", BLOCK_SIZE_GetTextureWithoutBG },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\GetTextureWithoutBG.hlsl", "getTextureWithoutBGCS", "cs_5_0", &pBlob, shaderDefinesGetTextureWithoutBG);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderGetTextureWithoutBG);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferGetTextureWithoutBG);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferGetTextureWithoutBG);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Get confidence, normal and point without background
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_GetConfidenceNormalPointWithoutBG[5];
	sprintf_s(BLOCK_SIZE_GetConfidenceNormalPointWithoutBG, "%d", m_blockSizeGetConfidenceNormalPointWithoutBG);

	D3D_SHADER_MACRO shaderDefinesGetConfidenceNormalPointWithoutBG[] = { { "groupthreads", BLOCK_SIZE_GetConfidenceNormalPointWithoutBG },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\GetConfidenceNormalPointWithoutBG.hlsl", "getConfidenceNormalPointWithoutBGCS", "cs_5_0", &pBlob, shaderDefinesGetConfidenceNormalPointWithoutBG);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderGetConfidenceNormalPointWithoutBG);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferGetConfidenceNormalPointWithoutBG);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferGetConfidenceNormalPointWithoutBG);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Get contour
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_GetContour[5];
	sprintf_s(BLOCK_SIZE_GetContour, "%d", m_blockSizeGetContour);

	D3D_SHADER_MACRO shaderDefinesGetContour[] = { { "groupthreads", BLOCK_SIZE_GetContour },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\GetContour.hlsl", "getContourCS", "cs_5_0", &pBlob, shaderDefinesGetContour);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderGetContour);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferGetContour);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferGetContour);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Get contour confidence
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_GetContourConfidence[5];
	sprintf_s(BLOCK_SIZE_GetContourConfidence, "%d", m_blockSizeGetContourConfidence);

	D3D_SHADER_MACRO shaderDefinesGetContourConfidence[] = { { "groupthreads", BLOCK_SIZE_GetContourConfidence },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\GetContourConfidence.hlsl", "getContourConfidenceCS", "cs_5_0", &pBlob, shaderDefinesGetContourConfidence);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderGetContourConfidence);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferGetContourConfidence);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferGetContourConfidence);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Center of gravity
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_CenterOfGravity[5];
	sprintf_s(BLOCK_SIZE_CenterOfGravity, "%d", m_blockSizeCenterOfGravity);

	D3D_SHADER_MACRO shaderDefinesCenterOfGravity[] = { { "groupthreads", BLOCK_SIZE_CenterOfGravity },{ 0 } };

	hr = CompileShaderFromFile(L"Shaders\\CenterOfGravity.hlsl", "centerOfGravityCS", "cs_5_0", &pBlob, shaderDefinesCenterOfGravity);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderCenterOfGravity);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth = sizeof(CBufferCenterOfGravity);
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferCenterOfGravity);
	V_RETURN(hr);

	/////////////////////////////////////////////////////
	// Erode
	/////////////////////////////////////////////////////

	char BLOCK_SIZE_Erode[5];
	sprintf_s(BLOCK_SIZE_Erode, "%d", m_blockSizeErode);

	D3D_SHADER_MACRO shaderDefinesErode[] = { { "groupthreads", BLOCK_SIZE_Erode }, { 0 } };

	hr = CompileShaderFromFile(L"Shaders\\Erode.hlsl", "erodeCS", "cs_5_0", &pBlob, shaderDefinesErode);
	V_RETURN(hr);

	hr = pd3dDevice->CreateComputeShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pComputeShaderErode);
	V_RETURN(hr);

	SAFE_RELEASE(pBlob);

	bDesc.ByteWidth	= sizeof(CBufferErode) ;
	hr = pd3dDevice->CreateBuffer(&bDesc, NULL, &m_constantBufferErode);
	V_RETURN(hr);

	return  hr;
}

HRESULT DX11ImageHelper::applyBilateralFilter( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, float sigmaD /*= 5.0f*/, float sigmaR /*= 0.1f*/ )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferBilateralFilter, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferBilateralFilter *cbuffer = (CBufferBilateralFilter*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->sigmaD = sigmaD;
	cbuffer->sigmaR = sigmaR;

	context->Unmap(m_constantBufferBilateralFilter, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferBilateralFilter);
	context->CSSetShader(m_pComputeShaderBilateralFilter, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeBilateralFilter);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeBilateralFilter);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyBilateralFilterForSSAO( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputDepthSRV, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, float sigmaD /*= 5.0f*/, float sigmaR /*= 0.1f*/ )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferBilateralFilterForSSAO, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferBilateralFilterSSAO *cbuffer = (CBufferBilateralFilterSSAO*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->sigmaD = sigmaD;
	cbuffer->sigmaR = sigmaR;

	context->Unmap(m_constantBufferBilateralFilterForSSAO, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputDepthSRV);
	context->CSSetShaderResources(1, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferBilateralFilterForSSAO);
	context->CSSetShader(m_pComputeShaderBilateralFilterForSSAO, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeBilateralFilterForSSAO);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeBilateralFilterForSSAO);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[] = { NULL, NULL };
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 2, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyBilateralFilterApprox( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, unsigned int kernelRadius, float distThres )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferBilateralFilterApprox, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferBilateralFilterApprox *cbuffer = (CBufferBilateralFilterApprox*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->kernelRadius = kernelRadius;
	cbuffer->thres = distThres;

	context->Unmap(m_constantBufferBilateralFilterApprox, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferBilateralFilterApprox);
	context->CSSetShader(m_pComputeShaderBilateralFilterApprox, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeBilateralFilterApprox);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeBilateralFilterApprox);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyBilateralCompletion( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, float sigmaD /*= 5.0f*/, float sigmaR /*= 0.1f*/ )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferBilateralCompletion, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferBilateralCompletion *cbuffer = (CBufferBilateralCompletion*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->sigmaD = sigmaD;
	cbuffer->sigmaR = sigmaR;

	context->Unmap(m_constantBufferBilateralCompletion, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferBilateralCompletion);
	context->CSSetShader(m_pComputeShaderBilateralCompletion, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeBilateralCompletion);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeBilateralCompletion);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyBilateralCompletionWithColor( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputDepthSRV, ID3D11UnorderedAccessView* outputDepthUAV, ID3D11ShaderResourceView* inputColorSRV, ID3D11UnorderedAccessView* outputColorUAV, unsigned int imageWidth, unsigned int imageHeight, float sigmaD /*= 5.0f*/, float sigmaR /*= 0.1f*/ )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferBilateralCompletion, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferBilateralCompletion *cbuffer = (CBufferBilateralCompletion*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->sigmaD = sigmaD;
	cbuffer->sigmaR = sigmaR;

	context->Unmap(m_constantBufferBilateralCompletion, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputDepthSRV);
	context->CSSetShaderResources(1, 1, &inputColorSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputDepthUAV, 0);
	context->CSSetUnorderedAccessViews( 1, 1, &outputColorUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferBilateralCompletion);
	context->CSSetShader(m_pComputeShaderBilateralCompletionWithColor, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeBilateralCompletion);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeBilateralCompletion);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSRV[] = { NULL, NULL };
	ID3D11UnorderedAccessView* nullUAV[] = { NULL, NULL };
	ID3D11Buffer* nullCB[] = { NULL };

	context->CSSetShaderResources(0, 2, nullSRV);
	context->CSSetUnorderedAccessViews(0, 2, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyBilateralFilter4F( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, float sigmaD /*= 5.0f*/, float sigmaR /*= 0.1f*/ )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferBilateralFilter4F, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferBilateralFilter4F *cbuffer = (CBufferBilateralFilter4F*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->sigmaD = sigmaD;
	cbuffer->sigmaR = sigmaR;

	context->Unmap(m_constantBufferBilateralFilter4F, 0);

	// Setup Pipeline
	context->CSSetShaderResources(1, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(1, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferBilateralFilter4F);
	context->CSSetShader(m_pComputeShaderBilateralFilter4F, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeBilateralFilter4F);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeBilateralFilter4F);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(1, 1, nullSAV);
	context->CSSetUnorderedAccessViews(1, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyNormalComputation( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferNormalComputation, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	CBufferNormalComputation *cbuffer = (CBufferNormalComputation*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferNormalComputation, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferNormalComputation);
	context->CSSetShader(m_pComputeShaderNormalComputation, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeNormalComputation);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeNormalComputation);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyCameraSpace( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferCameraSpace, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	CBufferCameraSpace *cbuffer = (CBufferCameraSpace*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferCameraSpace, 0);

	// Setup Pipeline
	context->CSSetShaderResources(1, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(1, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferCameraSpace);
	context->CSSetShader(m_pComputeShaderCameraSpace, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeCameraSpace);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeCameraSpace);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(1, 1, nullSAV);
	context->CSSetUnorderedAccessViews(1, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyCameraSpaceProjection( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferCameraSpaceProjection, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	CBufferCameraSpaceProjection *cbuffer = (CBufferCameraSpaceProjection*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferCameraSpaceProjection, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferCameraSpaceProjection);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(context);
	context->CSSetConstantBuffers(8, 1, &CBGlobalAppState);
	context->CSSetShader(m_pComputeShaderCameraSpaceProjection, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeCameraSpaceProjection);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeCameraSpaceProjection);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetConstantBuffers(8, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::StereoCameraSpaceProjection( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferStereoCameraSpaceProjection, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	CBufferStereoCameraSpaceProjection *cbuffer = (CBufferStereoCameraSpaceProjection*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferStereoCameraSpaceProjection, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferStereoCameraSpaceProjection);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(context);
	context->CSSetConstantBuffers(8, 1, &CBGlobalAppState);
	context->CSSetShader(m_pComputeShaderStereoCameraSpaceProjection, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeStereoCameraSpaceProjection);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeStereoCameraSpaceProjection);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetConstantBuffers(8, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyDepthMap( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferDepthMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	CBufferDepthMap *cbuffer = (CBufferDepthMap*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferDepthMap, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferDepthMap);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(context);
	context->CSSetConstantBuffers(8, 1, &CBGlobalAppState);
	context->CSSetShader(m_pComputeShaderDepthMap, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeDepthMap);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeDepthMap);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetConstantBuffers(8, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applySSAOMap( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferSSAOMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	CBufferSSAOMap *cbuffer = (CBufferSSAOMap*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	memcpy(&cbuffer->rotationVectors[0], &m_randomRotations[0], 16*sizeof(float4));

	context->Unmap(m_constantBufferSSAOMap, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferSSAOMap);
	context->CSSetShader(m_pComputeShaderSSAOMap, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeSSAOMap);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeSSAOMap);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyHSVDepth( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferHSVDepth, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	CBufferHSVDepth *cbuffer = (CBufferHSVDepth*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferHSVDepth, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferHSVDepth);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(context);
	context->CSSetConstantBuffers(8, 1, &CBGlobalAppState);
	context->CSSetShader(m_pComputeShaderHSVDepth, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeHSVDepth);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeHSVDepth);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetConstantBuffers(8, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

void FindMaxAndMin(float *pArr, int nStart, int nEnd, float &nMax, float &nMin)
{
	if (nEnd - nStart <= 1)
	{
		if (pArr[nStart] > pArr[nEnd])
		{
			nMax = pArr[nStart];
			nMin = pArr[nEnd];
		}
		else
		{
			nMax = pArr[nEnd];
			nMin = pArr[nStart];
		}

		return;
	}

	float nLeftMax = 0;
	float nLeftMin = 0;

	float nRightMax = 0;
	float nRightMin = 0;

	FindMaxAndMin(pArr, nStart, nStart + (nEnd - nStart) / 2, nLeftMax, nLeftMin);
	FindMaxAndMin(pArr, nStart + (nEnd - nStart) / 2 + 1, nEnd, nRightMax, nRightMin);

	nMax = nLeftMax > nRightMax ? nLeftMax : nRightMax;
	nMin = nLeftMin < nRightMin ? nLeftMin : nRightMin;
}

HRESULT DX11ImageHelper::applyProjectiveCorrespondences(ID3D11DeviceContext* context, 
	                                                    ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputIntensitiesSRV, ID3D11ShaderResourceView** inputLabelsSRV, 
	                                                    ID3D11ShaderResourceView* targetSRV, ID3D11ShaderResourceView* targetNormalsSRV, ID3D11ShaderResourceView* targetIntensitiesSRV, ID3D11ShaderResourceView** targetLabelsSRV,
	                                                    ID3D11UnorderedAccessView* outputUAV, ID3D11UnorderedAccessView* outputNormalUAV,
	                                                    const Eigen::Matrix4f& deltaTransform, unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum, unsigned int level, 
														float distWeight, float normalWeight, float colorWeight, float labelWeight, float distThres, float normalThres, float colorThres, float labelThres, float levelFactor, unsigned int labelNo)
{
	HRESULT hr = S_OK;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferProjectiveCorrespondences, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr)) return hr;

	Eigen::Matrix4f deltaTransformT = deltaTransform.transpose();

	CBufferProjectiveCorrespondences *cbuffer = (CBufferProjectiveCorrespondences*)mappedResource.pData;
	cbuffer->imageWidth   = (int)imageWidth;
	cbuffer->imageHeigth  = (int)imageHeight;

	cbuffer->labelNum     = (int)labelNum;
	cbuffer->level        = (int)level;

	cbuffer->distWeight   = distWeight;
	cbuffer->normalWeight = normalWeight;
	cbuffer->colorWeight  = colorWeight;
	cbuffer->labelWeight  = labelWeight;

	cbuffer->distThres    = distThres;
	cbuffer->normalThres  = normalThres;
	cbuffer->colorThres   = colorThres;
	cbuffer->labelThres   = labelThres;
	
	memcpy(cbuffer->transform, deltaTransformT.data(), 16*sizeof(float));
	cbuffer->levelFactor  = levelFactor;
	cbuffer->labelNo      = (int)labelNo;

	context->Unmap(m_constantBufferProjectiveCorrespondences, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &targetSRV);
	context->CSSetShaderResources(1, 1, &targetNormalsSRV);
	context->CSSetShaderResources(2, 1, &targetIntensitiesSRV);
	context->CSSetShaderResources(3, 1, &inputSRV);
	context->CSSetShaderResources(4, 1, &inputNormalsSRV);
	context->CSSetShaderResources(5, 1, &inputIntensitiesSRV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+6, 1, &targetLabelsSRV[i]);
	}
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+18, 1, &inputLabelsSRV[i]);
	}
		
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, &outputNormalUAV, 0);

	context->CSSetConstantBuffers(0, 1, &m_constantBufferProjectiveCorrespondences);
	ID3D11Buffer* CBGlobalAppState = GlobalAppState::getInstance().MapAndGetConstantBuffer(context);
	context->CSSetConstantBuffers(8, 1, &CBGlobalAppState);
	context->CSSetShader(m_pComputeShaderProjectiveCorrespondences, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeProjectiveCorrespondences);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeProjectiveCorrespondences);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSRV[] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	for (unsigned int i = 0; i < 30; i++)
	{
		context->CSSetShaderResources(i, 1, nullSRV);
	}

	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, nullUAV, 0);

	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetConstantBuffers(8, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applySubSamp( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int targetImageWidth, unsigned int targetImageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferSubSamp, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferSubSamp *cbuffer = (CBufferSubSamp*)mappedResource.pData;
	cbuffer->imageWidth = (int)targetImageWidth;
	cbuffer->imageHeigth = (int)targetImageHeight;

	context->Unmap(m_constantBufferSubSamp, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferSubSamp);
	context->CSSetShader(m_pComputeShaderSubSamp, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)targetImageWidth)/m_blockSizeSubSamp);
	unsigned int dimY = (unsigned int)ceil(((float)targetImageHeight)/m_blockSizeSubSamp);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyDownsampling( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int targetImageWidth, unsigned int targetImageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferDownsampling, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferDS *cbuffer = (CBufferDS*)mappedResource.pData;
	cbuffer->imageWidth = (int)targetImageWidth;
	cbuffer->imageHeigth = (int)targetImageHeight;

	context->Unmap(m_constantBufferDownsampling, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferDownsampling);
	context->CSSetShader(m_pComputeShaderDownsampling, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)targetImageWidth)/m_blockSizeDownsampling);
	unsigned int dimY = (unsigned int)ceil(((float)targetImageHeight)/m_blockSizeDownsampling);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyBlockAveraging( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, float sigmaD, float sigmaR )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	V_RETURN(context->Map(m_constantBufferBlockAverage, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	CBufferBlockAverage *cbuffer = (CBufferBlockAverage*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->sigmaD = sigmaD;
	cbuffer->sigmaR = sigmaR;

	context->Unmap(m_constantBufferBlockAverage, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferBlockAverage);
	context->CSSetShader(m_pComputeShaderBlockAverage, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeBlockAverage);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeBlockAverage);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyCopy( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferCopy, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferCopy *cbuffer = (CBufferCopy*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferCopy, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferCopy);
	context->CSSetShader(m_pComputeShaderCopy, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeCopy);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeCopy);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyConvertColorToIntensityFloat(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight)
{
	HRESULT hr = S_OK;
	
	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferConvertColorToIntensityFloat, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferConvertColorToIntensityFloat *cbuffer = (CBufferConvertColorToIntensityFloat*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferConvertColorToIntensityFloat, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferConvertColorToIntensityFloat);
	context->CSSetShader(m_pComputeShaderConvertColorToIntensityFloat, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeConvertColorToIntensityFloat);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeConvertColorToIntensityFloat);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);
	
	return hr;
}

HRESULT DX11ImageHelper::applyConvertLabel(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, unsigned int initial)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferConvertLabel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferConvertLabel *cbuffer = (CBufferConvertLabel*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->initial = (int)initial;

	context->Unmap(m_constantBufferConvertLabel, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferConvertLabel);
	context->CSSetShader(m_pComputeShaderConvertLabel, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeConvertLabel);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeConvertLabel);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applySplitLabelToFloat(ID3D11DeviceContext* context, ID3D11ShaderResourceView** inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum, unsigned int currentLabel)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferSplitLabelToFloat, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferSplitLabelToFloat *cbuffer = (CBufferSplitLabelToFloat*)mappedResource.pData;
	cbuffer->imageWidth   = (int)imageWidth;
	cbuffer->imageHeigth  = (int)imageHeight;
	cbuffer->labelNum     = (int)labelNum;
	cbuffer->currentLabel = (int)currentLabel;

	context->Unmap(m_constantBufferSplitLabelToFloat, 0);

	// Setup Pipeline
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM; i++)
	{
		context->CSSetShaderResources(i, 1, &inputSRV[i]);
	}
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferSplitLabelToFloat);
	context->CSSetShader(m_pComputeShaderSplitLabelToFloat, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeSplitLabelToFloat);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeSplitLabelToFloat);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM; i++)
	{
		context->CSSetShaderResources(i, 1, nullSAV);
	}
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applySaveAsTexture(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferSaveAsTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferSaveAsTexture *cbuffer = (CBufferSaveAsTexture*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferSaveAsTexture, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferSaveAsTexture);
	context->CSSetShader(m_pComputeShaderSaveAsTexture, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeSaveAsTexture);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeSaveAsTexture);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyInputLabelToRGBW(ID3D11DeviceContext* context, ID3D11ShaderResourceView** inputLabelSRV, ID3D11UnorderedAccessView* outputUAV, Eigen::Vector4i* color, unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferInputLabelToRGBW, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferInputLabelToRGBW *cbuffer = (CBufferInputLabelToRGBW*)mappedResource.pData;
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		memcpy(&cbuffer->color[i*4], color[i].data(), 4*sizeof(int));
	}
	cbuffer->imageWidth  = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->labelNum    = (int)labelNum;

	context->Unmap(m_constantBufferInputLabelToRGBW, 0);

	// Setup Pipeline
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, &inputLabelSRV[i]);
	}
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferInputLabelToRGBW);
	context->CSSetShader(m_pComputeShaderInputLabelToRGBW, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeInputLabelToRGBW);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeInputLabelToRGBW);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, nullSAV);
	}
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyModelLabelToRGBW(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView** inputLabelSRV, ID3D11UnorderedAccessView* outputUAV, Eigen::Vector4i* color, unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferModelLabelToRGBW, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferModelLabelToRGBW *cbuffer = (CBufferModelLabelToRGBW*)mappedResource.pData;
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		memcpy(&cbuffer->color[i*4], color[i].data(), 4*sizeof(int));
	}
	cbuffer->imageWidth  = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->labelNum    = (int)labelNum;

	context->Unmap(m_constantBufferModelLabelToRGBW, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+1, 1, &inputLabelSRV[i]);
	}
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferModelLabelToRGBW);
	context->CSSetShader(m_pComputeShaderModelLabelToRGBW, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeModelLabelToRGBW);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeModelLabelToRGBW);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+1, 1, nullSAV);
	}
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyComputeGradient(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight)
{
	HRESULT hr = S_OK;
	
	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferComputeGradient, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferComputeGradient *cbuffer = (CBufferComputeGradient*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferComputeGradient, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferComputeGradient);
	context->CSSetShader(m_pComputeShaderComputeGradient, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeComputeGradient);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeComputeGradient);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);
	
	return hr;
}

HRESULT DX11ImageHelper::applyComputeConfidenceAndEntropy(ID3D11DeviceContext* context, ID3D11ShaderResourceView** inputLabelsSRV, ID3D11ShaderResourceView* modelPointsSRV, ID3D11ShaderResourceView** modelLabelsSRV, 
	                                                      ID3D11UnorderedAccessView* outputInputConfidenceUAV, ID3D11UnorderedAccessView* outputModelConfidenceUAV, ID3D11UnorderedAccessView* outputFusionConfidenceUAV, ID3D11UnorderedAccessView* outputCrossEntropyUAV, ID3D11UnorderedAccessView* outputFusionImageUAV,
	                                                      unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferComputeConfidenceAndEntropy, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferComputeConfidenceAndEntropy *cbuffer = (CBufferComputeConfidenceAndEntropy*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->labelNum = (int)labelNum;

	context->Unmap(m_constantBufferComputeConfidenceAndEntropy, 0);

	// Setup Pipeline
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, &inputLabelsSRV[i]);
	}
	context->CSSetShaderResources(12, 1, &modelPointsSRV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+13, 1, &modelLabelsSRV[i]);
	}

	context->CSSetUnorderedAccessViews(0, 1, &outputInputConfidenceUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, &outputModelConfidenceUAV, 0);
	context->CSSetUnorderedAccessViews(2, 1, &outputFusionConfidenceUAV, 0);
	context->CSSetUnorderedAccessViews(3, 1, &outputCrossEntropyUAV, 0);
	context->CSSetUnorderedAccessViews(4, 1, &outputFusionImageUAV, 0);

	context->CSSetConstantBuffers(0, 1, &m_constantBufferComputeConfidenceAndEntropy);
	context->CSSetShader(m_pComputeShaderComputeConfidenceAndEntropy, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeComputeConfidenceAndEntropy);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeComputeConfidenceAndEntropy);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, nullSAV);
	}
	context->CSSetShaderResources(12, 1, nullSAV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+13, 1, nullSAV);
	}

	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(2, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(3, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(4, 1, nullUAV, 0);

	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyGetPoints(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferGetPoints, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferGetPoints *cbuffer = (CBufferGetPoints*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferGetPoints, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferGetPoints);
	context->CSSetShader(m_pComputeShaderGetPoints, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeGetPoints);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeGetPoints);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyGetPointsWithoutBG(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView** labelSRV, ID3D11UnorderedAccessView* outputUAV, ID3D11UnorderedAccessView* outputLabelIndexUAV, unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferGetPointsWithoutBG, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferGetPointsWithoutBG *cbuffer = (CBufferGetPointsWithoutBG*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->labelNum = (int)labelNum;

	context->Unmap(m_constantBufferGetPointsWithoutBG, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	for (unsigned int i = 1; i <= LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, &labelSRV[i-1]);
	}
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, &outputLabelIndexUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferGetPointsWithoutBG);
	context->CSSetShader(m_pComputeShaderGetPointsWithoutBG, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeGetPointsWithoutBG);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeGetPointsWithoutBG);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	for (unsigned int i = 1; i <= LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, nullSAV);
	}
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyGetTextureWithoutBG(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView** labelSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferGetTextureWithoutBG, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferGetTextureWithoutBG *cbuffer = (CBufferGetTextureWithoutBG*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->labelNum = (int)labelNum;

	context->Unmap(m_constantBufferGetTextureWithoutBG, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	for (unsigned int i = 1; i <= LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, &labelSRV[i-1]);
	}
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferGetTextureWithoutBG);
	context->CSSetShader(m_pComputeShaderGetTextureWithoutBG, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeGetTextureWithoutBG);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeGetTextureWithoutBG);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	for (unsigned int i = 1; i <= LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i, 1, nullSAV);
	}
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyGetConfidenceNormalPointWithoutBG(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalSRV, ID3D11ShaderResourceView** labelSRV, 
	                                                            ID3D11UnorderedAccessView* outputConfidenceUAV, ID3D11UnorderedAccessView* outputNormalUAV, ID3D11UnorderedAccessView* outputPointUAV, ID3D11UnorderedAccessView* outputIndexUAV, 
	                                                            unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferGetConfidenceNormalPointWithoutBG, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferGetConfidenceNormalPointWithoutBG *cbuffer = (CBufferGetConfidenceNormalPointWithoutBG*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->labelNum = (int)labelNum;

	context->Unmap(m_constantBufferGetConfidenceNormalPointWithoutBG, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetShaderResources(1, 1, &inputNormalSRV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+2, 1, &labelSRV[i]);
	}
	context->CSSetUnorderedAccessViews(0, 1, &outputConfidenceUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, &outputNormalUAV, 0);
	context->CSSetUnorderedAccessViews(2, 1, &outputPointUAV, 0);
	context->CSSetUnorderedAccessViews(3, 1, &outputIndexUAV, 0);
	
	context->CSSetConstantBuffers(0, 1, &m_constantBufferGetConfidenceNormalPointWithoutBG);
	context->CSSetShader(m_pComputeShaderGetConfidenceNormalPointWithoutBG, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeGetConfidenceNormalPointWithoutBG);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeGetConfidenceNormalPointWithoutBG);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetShaderResources(1, 1, nullSAV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+2, 1, nullSAV);
	}
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(2, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(3, 1, nullUAV, 0);
	
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyGetContour(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferGetContour, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferGetContour *cbuffer = (CBufferGetContour*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferGetContour, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferGetContour);
	context->CSSetShader(m_pComputeShaderGetContour, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeGetContour);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeGetContour);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyGetContourConfidence(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView** labelSRV, ID3D11UnorderedAccessView* outputUAV, ID3D11UnorderedAccessView* outputIndexUAV, unsigned int imageWidth, unsigned int imageHeight, unsigned int labelNum)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferGetContourConfidence, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferGetContourConfidence *cbuffer = (CBufferGetContourConfidence*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->labelNum = (int)labelNum;

	context->Unmap(m_constantBufferGetContourConfidence, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+1, 1, &labelSRV[i]);
	}

	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, &outputIndexUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferGetContourConfidence);
	context->CSSetShader(m_pComputeShaderGetContourConfidence, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeGetContourConfidence);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeGetContourConfidence);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	for (unsigned int i = 0; i < LABEL_TEXTURE_NUM*LABEL_TEXTURE_CHANNEL; i++)
	{
		context->CSSetShaderResources(i+1, 1, nullSAV);
	}
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetUnorderedAccessViews(1, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyCenterOfGravity(ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* labelSRV, ID3D11UnorderedAccessView* outputUAV, unsigned int imageWidth, unsigned int imageHeight)
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = context->Map(m_constantBufferCenterOfGravity, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	V_RETURN(hr);

	CBufferCenterOfGravity *cbuffer = (CBufferCenterOfGravity*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;

	context->Unmap(m_constantBufferCenterOfGravity, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetShaderResources(1, 1, &labelSRV);
	context->CSSetUnorderedAccessViews(0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferCenterOfGravity);
	context->CSSetShader(m_pComputeShaderCenterOfGravity, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth) / m_blockSizeCenterOfGravity);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight) / m_blockSizeCenterOfGravity);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[1] = { NULL };
	ID3D11UnorderedAccessView* nullUAV[1] = { NULL };
	ID3D11Buffer* nullCB[1] = { NULL };

	context->CSSetShaderResources(0, 1, nullSAV);
	context->CSSetShaderResources(1, 1, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

HRESULT DX11ImageHelper::applyErode( ID3D11DeviceContext* context, ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputColorSRV, ID3D11UnorderedAccessView* outputUAV, float distThres, int stencilSize, unsigned int imageWidth, unsigned int imageHeight )
{
	HRESULT hr = S_OK;

	// Initialize Constant Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	V_RETURN(context->Map(m_constantBufferErode, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	CBufferErode *cbuffer = (CBufferErode*)mappedResource.pData;
	cbuffer->imageWidth = (int)imageWidth;
	cbuffer->imageHeigth = (int)imageHeight;
	cbuffer->distThres = distThres;
	cbuffer->stencilSize = stencilSize;
	context->Unmap(m_constantBufferErode, 0);

	// Setup Pipeline
	context->CSSetShaderResources(0, 1, &inputSRV);
	context->CSSetShaderResources(1, 1, &inputColorSRV);
	context->CSSetUnorderedAccessViews( 0, 1, &outputUAV, 0);
	context->CSSetConstantBuffers(0, 1, &m_constantBufferErode);
	context->CSSetShader(m_pComputeShaderErode, 0, 0);

	// Start Compute Shader
	unsigned int dimX = (unsigned int)ceil(((float)imageWidth)/m_blockSizeErode);
	unsigned int dimY = (unsigned int)ceil(((float)imageHeight)/m_blockSizeErode);
	context->Dispatch(dimX, dimY, 1);
	assert(dimX <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);
	assert(dimY <= D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION);

	// De-Initialize Pipeline
	ID3D11ShaderResourceView* nullSAV[] = { NULL, NULL};
	ID3D11UnorderedAccessView* nullUAV[] = { NULL };
	ID3D11Buffer* nullCB[] = { NULL };

	context->CSSetShaderResources(0, 2, nullSAV);
	context->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
	context->CSSetConstantBuffers(0, 1, nullCB);
	context->CSSetShader(0, 0, 0);

	return hr;
}

void DX11ImageHelper::OnD3D11DestroyDevice()
{
	/////////////////////////////////////////////////////
	// Bilateral Completion
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferBilateralCompletion);
	SAFE_RELEASE(m_pComputeShaderBilateralCompletion);
	SAFE_RELEASE(m_pComputeShaderBilateralCompletionWithColor);

	/////////////////////////////////////////////////////
	// Bilateral Filter
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferBilateralFilter);
	SAFE_RELEASE(m_pComputeShaderBilateralFilter);

	/////////////////////////////////////////////////////
	// Bilateral Filter
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferBilateralFilterForSSAO);
	SAFE_RELEASE(m_pComputeShaderBilateralFilterForSSAO);

	/////////////////////////////////////////////////////
	// Bilateral Filter Approximation
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferBilateralFilterApprox);
	SAFE_RELEASE(m_pComputeShaderBilateralFilterApprox);

	/////////////////////////////////////////////////////
	// Bilateral Filter 4F
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferBilateralFilter4F);
	SAFE_RELEASE(m_pComputeShaderBilateralFilter4F);

	/////////////////////////////////////////////////////
	// Normal Computation
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferNormalComputation);
	SAFE_RELEASE(m_pComputeShaderNormalComputation);

	/////////////////////////////////////////////////////
	// Camera Space To Dept Map
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferCameraSpace);
	SAFE_RELEASE(m_pComputeShaderCameraSpace);

	/////////////////////////////////////////////////////
	// Compute Depth Maps
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferDepthMap);
	SAFE_RELEASE(m_pComputeShaderDepthMap);

	/////////////////////////////////////////////////////
	// Compute SSAO Map
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferSSAOMap);
	SAFE_RELEASE(m_pComputeShaderSSAOMap);

	/////////////////////////////////////////////////////
	// Compute HSV Depth Maps
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferHSVDepth);
	SAFE_RELEASE(m_pComputeShaderHSVDepth);

	/////////////////////////////////////////////////////
	// Camera Space Projection
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferCameraSpaceProjection);
	SAFE_RELEASE(m_pComputeShaderCameraSpaceProjection);

	/////////////////////////////////////////////////////
	// Stereo Mask Camera Space Projection
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferStereoCameraSpaceProjection);
	SAFE_RELEASE(m_pComputeShaderStereoCameraSpaceProjection);

	/////////////////////////////////////////////////////
	// Projective Correspondences
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferProjectiveCorrespondences);
	SAFE_RELEASE(m_pComputeShaderProjectiveCorrespondences);

	/////////////////////////////////////////////////////
	// Subsample
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferSubSamp);
	SAFE_RELEASE(m_pComputeShaderSubSamp);

	/////////////////////////////////////////////////////
	// Downsampling
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferDownsampling);
	SAFE_RELEASE(m_pComputeShaderDownsampling);

	/////////////////////////////////////////////////////
	// Depth aware block averaging
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferBlockAverage);
	SAFE_RELEASE(m_pComputeShaderBlockAverage);

	/////////////////////////////////////////////////////
	// Copy
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferCopy);
	SAFE_RELEASE(m_pComputeShaderCopy);

	/////////////////////////////////////////////////////
	// Convert color to intensity float
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferConvertColorToIntensityFloat);
	SAFE_RELEASE(m_pComputeShaderConvertColorToIntensityFloat);

	/////////////////////////////////////////////////////
	// Convert label
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferConvertLabel);
	SAFE_RELEASE(m_pComputeShaderConvertLabel);

	/////////////////////////////////////////////////////
	// Split label to float
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferSplitLabelToFloat);
	SAFE_RELEASE(m_pComputeShaderSplitLabelToFloat);

	/////////////////////////////////////////////////////
	// Save as Texture
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferSaveAsTexture);
	SAFE_RELEASE(m_pComputeShaderSaveAsTexture);

	/////////////////////////////////////////////////////
	// Convert input label to RGBW
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferInputLabelToRGBW);
	SAFE_RELEASE(m_pComputeShaderInputLabelToRGBW);

	/////////////////////////////////////////////////////
	// Convert model label to RGBW
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferModelLabelToRGBW);
	SAFE_RELEASE(m_pComputeShaderModelLabelToRGBW);

	/////////////////////////////////////////////////////
	// Compute gradient
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferComputeGradient);
	SAFE_RELEASE(m_pComputeShaderComputeGradient);

	/////////////////////////////////////////////////////
	// Compute confidence and entropy
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferComputeConfidenceAndEntropy);
	SAFE_RELEASE(m_pComputeShaderComputeConfidenceAndEntropy);

	/////////////////////////////////////////////////////
	// Get points
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferGetPoints);
	SAFE_RELEASE(m_pComputeShaderGetPoints);

	/////////////////////////////////////////////////////
	// Get points without background
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferGetPointsWithoutBG);
	SAFE_RELEASE(m_pComputeShaderGetPointsWithoutBG);

	/////////////////////////////////////////////////////
	// Get texture without background
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferGetTextureWithoutBG);
	SAFE_RELEASE(m_pComputeShaderGetTextureWithoutBG);

	/////////////////////////////////////////////////////
	// Get confidence, normal and point without background
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferGetConfidenceNormalPointWithoutBG);
	SAFE_RELEASE(m_pComputeShaderGetConfidenceNormalPointWithoutBG);

	/////////////////////////////////////////////////////
	// Get contour
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferGetContour);
	SAFE_RELEASE(m_pComputeShaderGetContour);

	/////////////////////////////////////////////////////
	// Get contour confidence
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferGetContourConfidence);
	SAFE_RELEASE(m_pComputeShaderGetContourConfidence);

	/////////////////////////////////////////////////////
	// Center of gravity
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferCenterOfGravity);
	SAFE_RELEASE(m_pComputeShaderCenterOfGravity);

	/////////////////////////////////////////////////////
	// Erode
	/////////////////////////////////////////////////////

	SAFE_RELEASE(m_constantBufferErode);
	SAFE_RELEASE(m_pComputeShaderErode);
}