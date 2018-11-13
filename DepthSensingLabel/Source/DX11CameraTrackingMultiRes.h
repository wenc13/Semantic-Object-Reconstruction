#pragma once

/************************************************************************/
/* Main ICP class used for tracking                                     */
/************************************************************************/

#include "MatrixConversion.h"
#include "DX11QuadDrawer.h"
#include "DX11NormalizeReduction.h"
#include "DX11ImageHelper.h"
#include "DX11BuildLinearSystem.h"
#include "ICPErrorLog.h"
#include "Eigen.h"

#include <D3D11.h>
#include "DX11Utils.h"

#include "AbsoluteOrientation.h"

using namespace MatrixConversion;

class DX11CameraTrackingMultiRes
{
public:

	static HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice);

	static mat4f applyCT(ID3D11DeviceContext* context, 
		                 ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputColorsSRV, ID3D11ShaderResourceView** inputLabelsSRV,
		                 ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelColorsSRV, ID3D11ShaderResourceView** modelLabelsSRV,
		                 const mat4f& lastTransform, const std::vector<unsigned int>& maxInnerIter, const std::vector<unsigned int>& maxOuterIter, const std::vector<float>& distThres, const std::vector<float>& normalThres, const std::vector<float>& colorThres, std::vector<float>& labelThres, float condThres, float angleThres, const mat4f& deltaTransformEstimate, const float focalx, const float focaly, const std::vector<float>& earlyOutResidual, ICPErrorLog* errorLog);

	static void OnD3D11DestroyDevice();

private:
	// angleThres in radians, distThres in meter
	static bool checkRigidTransformation(Eigen::Matrix3f& R, Eigen::Vector3f& t, float angleThres, float distThres);

	static Eigen::Matrix4f delinearizeTransformation(Vector6f& x, Eigen::Vector3f& mean, float meanStDev, unsigned int level);
	
	static Eigen::Matrix4f computeBestRigidAlignment(ID3D11DeviceContext* context, 
		                                             ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputIntensitiesSRV, ID3D11ShaderResourceView** inputLabelsSRV, 
		                                             ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelIntensitiesSRV, ID3D11ShaderResourceView** modelLabelsSRV,
		                                             D3DXVECTOR3& mean, float meanStDev, float nValidCorres, const Eigen::Matrix4f& globalDeltaTransform, unsigned int level, unsigned int maxInnerIter, float condThres, float angleThres, float focalx, float focaly, LinearSystemConfidence& conf);

	static Eigen::Matrix4f align(ID3D11DeviceContext* context, 
		                         ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputIntensitiesSRV, ID3D11ShaderResourceView** inputLabelsSRV,
		                         ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelIntensitiesSRV, ID3D11ShaderResourceView** modelLabelsSRV, 
								 Eigen::Matrix4f& deltaTransform, unsigned int level, unsigned int maxInnerIter, unsigned int maxOuterIter, float distThres, float normalThres, float colorThres, float labelThres, float condThres, float angleThres, float focalx, float focaly, float earlyOut, ICPErrorLog* errorLog);

	static HRESULT computeCorrespondences(ID3D11DeviceContext* context, 
		                                  ID3D11ShaderResourceView* inputSRV, ID3D11ShaderResourceView* inputNormalsSRV, ID3D11ShaderResourceView* inputIntensitiesSRV, ID3D11ShaderResourceView** inputLabelsSRV,
		                                  ID3D11ShaderResourceView* modelSRV, ID3D11ShaderResourceView* modelNormalsSRV, ID3D11ShaderResourceView* modelIntensitiesSRV, ID3D11ShaderResourceView** modelLabelsSRV,
		                                  D3DXVECTOR3& mean, float& meanStDev, float& nValidCorres, const Eigen::Matrix4f& deltaTransform, unsigned int level, float distThres, float normalThres, float colorThres, float labelThres, int labelNo);

private:

	static Eigen::Matrix4f m_matrixTrackingLost;

	/////////////////////////////////////////////////////
	// Camera Tracking Multi Res
	/////////////////////////////////////////////////////

	// Correspondences
	static ID3D11Texture2D** m_pCorrespondenceTextureFloat42D;
	static ID3D11ShaderResourceView** m_pCorrespondenceTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_pCorrespondenceTextureFloat4UAV;

	static ID3D11Texture2D** m_pCorrespondenceNormalTextureFloat42D;
	static ID3D11ShaderResourceView** m_pCorrespondenceNormalTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_pCorrespondenceNormalTextureFloat4UAV;

	// Input
	static ID3D11Texture2D** m_inputTextureFloat42D;
	static ID3D11ShaderResourceView** m_inputTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_inputTextureFloat4UAV;

	static ID3D11Texture2D** m_inputNormalTextureFloat42D;
	static ID3D11ShaderResourceView** m_inputNormalTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_inputNormalTextureFloat4UAV;

	static ID3D11Texture2D** m_inputColorTextureFloat42D;
	static ID3D11ShaderResourceView** m_inputColorTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_inputColorTextureFloat4UAV;

	static ID3D11Texture2D** m_inputLabelTextureFloat42D;
	static ID3D11ShaderResourceView** m_inputLabelTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_inputLabelTextureFloat4UAV;

	static ID3D11Texture2D** m_inputIntensityTextureFloat42D;
	static ID3D11ShaderResourceView** m_inputIntensityTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_inputIntensityTextureFloat4UAV;

	// Model
	static ID3D11Texture2D** m_modelTextureFloat42D;
	static ID3D11ShaderResourceView** m_modelTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_modelTextureFloat4UAV;

	static ID3D11Texture2D** m_modelNormalTextureFloat42D;
	static ID3D11ShaderResourceView** m_modelNormalTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_modelNormalTextureFloat4UAV;

	static ID3D11Texture2D** m_modelColorTextureFloat42D;
	static ID3D11ShaderResourceView** m_modelColorTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_modelColorTextureFloat4UAV;

	static ID3D11Texture2D** m_modelLabelTextureFloat42D;
	static ID3D11ShaderResourceView** m_modelLabelTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_modelLabelTextureFloat4UAV;

	static ID3D11Texture2D** m_modelIntensityTextureFloat42D;
	static ID3D11ShaderResourceView** m_modelIntensityTextureFloat4SRV;
	static ID3D11UnorderedAccessView** m_modelIntensityTextureFloat4UAV;

	// Weight
	static ID3D11Buffer* m_inputConfidenceWithoutBGFloat;
	static ID3D11Buffer* m_inputConfidenceWithoutBGFloatCPU;
	static ID3D11UnorderedAccessView* m_inputConfidenceWithoutBGFloatUAV;

	static ID3D11Buffer* m_inputNormalsWithoutBGFloat4;
	static ID3D11Buffer* m_inputNormalsWithoutBGFloat4CPU;
	static ID3D11UnorderedAccessView* m_inputNormalsWithoutBGFloat4UAV;

	static ID3D11Buffer* m_inputPointsWithoutBGFloat4;
	static ID3D11Buffer* m_inputPointsWithoutBGFloat4CPU;
	static ID3D11UnorderedAccessView* m_inputPointsWithoutBGFloat4UAV;

	static ID3D11Buffer* m_inputIndexWithoutBGInt;
	static ID3D11Buffer* m_inputIndexWithoutBGIntCPU;
	static ID3D11UnorderedAccessView* m_inputIndexWithoutBGIntUAV;

	// Image Pyramid Dimensions
	static unsigned int* m_imageWidth;
	static unsigned int* m_imageHeight;

	/////////////////////////////////////////////////////
	// Query
	/////////////////////////////////////////////////////

	static Timer m_timer;
};