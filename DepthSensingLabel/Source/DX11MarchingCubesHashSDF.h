#pragma once

/*****************************************************************************/
/* Marching cubes for a HashSDF on the GPU, needs to be run for every chunk  */
/*****************************************************************************/

#include "stdafx.h"

#include <D3D11.h>
#include "DXUT.h"
#include "DX11Utils.h"
#include "TimingLog.h"
#include "removeDuplicate.h"

#include <vector>
#include <string>

#define NUM_GROUPS_X 1024 // to be in-sync with the define in the shader

class DX11MarchingCubesHashSDF
{
	public:
		
		static HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice);

		static void OnD3D11DestroyDevice();

		static void clearMeshBuffer();

		static void clearVectors();

		static void saveMesh(const std::string& filename, const mat4f *transform = NULL);

		static HRESULT extractIsoSurface(ID3D11DeviceContext* context, ID3D11ShaderResourceView* hash, ID3D11ShaderResourceView* sdfBlocksSDF, ID3D11ShaderResourceView* sdfBlocksRGBW, ID3D11ShaderResourceView** sdfBlocksLabel, ID3D11ShaderResourceView* sdfBlocksLabelFrequency, ID3D11Buffer* CBsceneRepSDF, unsigned int hashNumBuckets, unsigned int hashBucketSize, vec3f& minCorner = vec3f(0.0f, 0.0f, 0.0f), vec3f& maxCorner = vec3f(0.0f, 0.0f, 0.0f), bool boxEnabled = false);

		// Helper
		static std::vector<vec3f>& getVertices()
		{
			return s_Vertices;
		}

		static std::vector<vec3f>& getNormals()
		{
			return s_Normals;
		}

		static std::vector<vec4f>& getColors()
		{
			return s_Colors;
		}

		static std::vector<vec3i>& getLabels()
		{
			return s_Labels;
		}

		static std::vector<vec1f>& getConfidence()
		{
			return s_Confidence;
		}

		static std::vector<vec3i>& getVirtualVoxels()
		{
			return s_VirtualVoxels;
		}

	private:

		static HRESULT initialize(ID3D11Device* pd3dDevice);

		static void destroy()
		{
			SAFE_RELEASE(m_pComputeShader);
			SAFE_RELEASE(m_constantBuffer);

			// Triangles
			SAFE_RELEASE(s_pTriangles);
			SAFE_RELEASE(s_pTrianglesUAV);
			SAFE_RELEASE(s_pOutputTrianglesFloatCPU);
			SAFE_RELEASE(s_BuffCountTriangles);

			// Normals
			SAFE_RELEASE(s_pNormals);
			SAFE_RELEASE(s_pNormalsUAV);
			SAFE_RELEASE(s_pOutputNormalsFloat3CPU);
			SAFE_RELEASE(s_BuffCountNormals);

			// Labels
			SAFE_RELEASE(s_pLabels);
			SAFE_RELEASE(s_pLabelsUAV);
			SAFE_RELEASE(s_pOutputLabelsInt3CPU);
			SAFE_RELEASE(s_BuffCountLabels);

			// Confidence
			SAFE_RELEASE(s_pConfidence);
			SAFE_RELEASE(s_pConfidenceUAV);
			SAFE_RELEASE(s_pOutputConfidenceFloatCPU);
			SAFE_RELEASE(s_BuffCountConfidence);

			// Virtual Voxels
			SAFE_RELEASE(s_pVirtualVoxels);
			SAFE_RELEASE(s_pVirtualVoxelsUAV);
			SAFE_RELEASE(s_pOutputVirtualVoxelsInt3CPU);
			SAFE_RELEASE(s_BuffCountVirtualVoxels);
		}
		
		// State
		struct CBuffer
		{
			unsigned int boxEnabled;
			float3 minCorner;

			unsigned int labelNum;
			float3 maxCorner;		
		};
										
		static unsigned int m_blockSize;
		static unsigned int s_maxNumberOfTriangles;

		static ID3D11Buffer* m_constantBuffer;
		static ID3D11ComputeShader* m_pComputeShader;
		
		// Triangles
		static ID3D11Buffer* s_pTriangles;
		static ID3D11UnorderedAccessView* s_pTrianglesUAV;

		static ID3D11Buffer* s_pOutputTrianglesFloatCPU;
		static ID3D11Buffer* s_BuffCountTriangles;

		// Normals
		static ID3D11Buffer* s_pNormals;
		static ID3D11UnorderedAccessView* s_pNormalsUAV;

		static ID3D11Buffer* s_pOutputNormalsFloat3CPU;
		static ID3D11Buffer* s_BuffCountNormals;

		// Labels
		static ID3D11Buffer* s_pLabels;
		static ID3D11UnorderedAccessView* s_pLabelsUAV;

		static ID3D11Buffer* s_pOutputLabelsInt3CPU;
		static ID3D11Buffer* s_BuffCountLabels;

		// Confidence
		static ID3D11Buffer* s_pConfidence;
		static ID3D11UnorderedAccessView* s_pConfidenceUAV;

		static ID3D11Buffer* s_pOutputConfidenceFloatCPU;
		static ID3D11Buffer* s_BuffCountConfidence;

		// Virtual Voxels
		static ID3D11Buffer* s_pVirtualVoxels;
		static ID3D11UnorderedAccessView* s_pVirtualVoxelsUAV;

		static ID3D11Buffer* s_pOutputVirtualVoxelsInt3CPU;
		static ID3D11Buffer* s_BuffCountVirtualVoxels;

		static MeshDataf s_meshData;

		static std::vector<vec3f> s_Vertices;
		static std::vector<vec3f> s_Normals;
		static std::vector<vec4f> s_Colors;
		static std::vector<vec3i> s_Labels;
		static std::vector<vec1f> s_Confidence;
		static std::vector<vec3i> s_VirtualVoxels;

		static Timer m_timer;
};