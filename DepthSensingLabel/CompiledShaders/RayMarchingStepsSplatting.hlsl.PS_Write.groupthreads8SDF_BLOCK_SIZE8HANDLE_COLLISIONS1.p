DXBCM.ПмЙҐ6NВљpю   DN    8     P  `  †  <  RDEF№     D     <    €€	  ©  RD11<          (   $          №            €€€€         щ            €€€€                     €€€€         1                            6                           g_FragmentPrefixSumBufferSRV g_FragmentCountBufferUAV g_FragmentSortedDepthBufferUAV cbCS cbConstant ЂЂЂ6     t  Р           1  
   0  †           d      @       x      €€€€    €€€€    Ь  @   @       x      €€€€    €€€€    ≠  А         »      €€€€    €€€€    м  Д          »      €€€€    €€€€      И          »      €€€€    €€€€      М          »      €€€€    €€€€    g_ViewMat float4x4 Ђ                            n  g_ViewMatInverse g_RenderTargetWidth dword Ђ                             Ѕ  g_RenderTargetHeight g_dummyRayInteveral337 g_dummyRayInteveral338 Ђј      @       x      €€€€    €€€€    —  @   @       x      €€€€    €€€€    й  А          »      €€€€    €€€€    ъ  Д          »      €€€€    €€€€      И          »      €€€€    €€€€      М          »      €€€€    €€€€    &  Р          @      €€€€    €€€€    d  Ф          @      €€€€    €€€€    Г  Ш          »      €€€€    €€€€    Т  Ь          »      €€€€    €€€€    g_RigidTransform g_RigidTransformInverse g_HashNumBuckets g_HashBucketSize g_ImageWidth g_ImageHeight g_VirtualVoxelSize float Ђ                             9  g_VirtualVoxelResolutionScalar g_NumSDFBlocks g_NumOccupiedSDFBlocks Microsoft (R) HLSL Shader Compiler 9.30.9200.16384 ISGN,                             SV_POSITION OSGN          SHEX8  P   N   j Y  FО     	   Y  FО         X  p    33  Ь  а     33  Ь  а    UU  d  r        h       2      F     #  
            
А        
      і  	      а     
      @  €€€€-  ЙB  А√ћ             F~            
 АA      
      §  та          ¶     >  STATФ                                                                                                                                             SPDB F Microsoft C/C++ MSF 7.00
DS         £   і      Я                                                                                                                                                                                                                                                                                                                                                                                                                                                                           ј€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€8                   €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€       <       €€€€                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         Ф.1ФY   т&ЏЅ±Gљ£Рtґо(“                          AС2                                                                                                                                                                                                                                                                                                                                                                                                                                                                    V : register(t2);
Buffer<float>	g_FragmentSortedDepthBufferSRV : register(t3);

Buffer<float>	g_SDFBlocksSDFSRV : register(t5);
Buffer<int>		g_SDFBlocksRGBWSRV : register(t7);
Buffer<int>		g_DecisionArraySRV : register(t4);


RWBuffer<int>	g_FragmentCountBufferUAV : register(u0);
RWBuffer<int>	g_FragmentPrefixSumBufferUAV : register(u1);
RWBuffer<float>	g_FragmentSortedDepthBufferUAV : register(u2);

RWBuffer<int>	g_DecisionArrayUAV : register(u6);

#define NUM_GROUPS_X 1024 // to be in-sync with the definews  o  OL  J  at  g}  Љ  І?  р1  ,  ‘p  а  ЮV  {h  Ny  €x                                                                                                                                                                                                                                                                                                                                                                                                                                                                 Buffer<int>		g_VoxelHash : register(t0);		

Buffer<int>		g_FragmentCountBufferSRV : register(t1);
Buffer<int>		g_FragmentPrefixSumBufferSRV : register(t2);
Buffer<float>	g_FragmentSortedDepthBufferSRV : register(t3);

Buffer<float>	g_SDFBlocksSDFSRV : register(t5);
Buffer<int>		g_SDFBlocksRGBWSRV : register(t7);
Buffer<int>		g_DecisionArraySRV : register(t4);


RWBuffer<int>	g_FragmentCountBufferUAV : register(u0);
RWBuffer<int>	g_FragmentPrefixSumBufferUAV : register(u1);
RWBuffer<float>	g_FragmentSortedDepthBufferUAV : register(u2);

RWBuffer<int>	g_DecisionArrayUAV : register(u6);

#define NUM_GROUPS_X 1024 // to be in-sync with the define on the Cpu

cbuffer cbConstant : register(b1)
{
	float4x4	g_ViewMat;
	float4x4	g_ViewMatInverse;
	uint		g_RenderTargetWidth;
	uint		g_RenderTargetHeight;
	uint		g_dummyRayInteveral337;
	uint		g_dummyRayInteveral338;
};

#include "GlobalAppStateShaderBuffer.h.hlsl"
#include "SDFShaderBuffer.h.hlsl"
#include "KinectCameraUtil.h.hlsl"
#include "VoxelUtilHashSDF.h.hlsl"

////////////////////////////////////////////////////////////////////
// General setup
////////////////////////////////////////////////////////////////////

static const float4 offsets[4] = 
{
    float4( 0.5, -0.5, 0.0, 0.0),
    float4(-0.5, -0.5, 0.0, 0.0),
	
	float4( 0.5,  0.5, 0.0, 0.0),
    float4(-0.5,  0.5, 0.0, 0.0)
};

struct GS_INPUT
{
};

struct PS_INPUT
{
    float4 position	: SV_POSITION;
};

GS_INPUT VS()
{
    GS_INPUT output = (GS_INPUT)0;
 
    return output;
}

[maxvertexcount(4)]
void GS(point GS_INPUT points[1], uint primID : SV_PrimitiveID, inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT output;

	HashEntry entry = getHashEntry(g_VoxelHash, primID);
	float3 worldCurrentVoxel = SDFBlockToWorld(entry.pos);

	float3 MINV = worldCurrentVoxel.xyz-g_VirtualVoxelSize/2.0;
	float3 maxv = MINV+SDF_BLOCK_SIZE*g_VirtualVoxelSize;

	float3 proj000 = cameraToKinectProj(mul(float4(MINV.x, MINV.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj100 = cameraToKinectProj(mul(float4(maxv.x, MINV.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj010 = cameraToKinectProj(mul(float4(MINV.x, maxv.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj001 = cameraToKinectProj(mul(float4(MINV.x, MINV.y, maxv.z, 1.0f), g_ViewMat).xyz);
	float3 proj110 = cameraToKinectProj(mul(float4(maxv.x, maxv.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj011 = cameraToKinectProj(mul(float4(MINV.x, maxv.y, maxv.z, 1.0f), g_ViewMat).xyz);
	float3 proj101 = cameraToKinectProj(mul(float4(maxv.x, MINV.y, maxv.z, 1.0f), g_ViewMat).xyz);
	float3 proj111 = cameraToKinectProj(mul(float4(maxv.x, maxv.y, maxv.z, 1.0f), g_ViewMat).xyz);

	// Tree Reduction Min
	float3 min00 = min(proj000, proj100);
	float3 min01 = min(proj010, proj001);
	float3 min10 = min(proj110, proj011);
	float3 min11 = min(proj101, proj111);

	float3 min0 = min(min00, min01);
	float3 min1 = min(min10, min11);

	float3 minFinal = min(min0, min1);

	// Tree Reduction Max 
	float3 max00 = max(proj000, proj100);
	float3 max01 = max(proj010, proj001);
	float3 max10 = max(proj110, proj011);
	float3 max11 = max(proj101, proj111);

	float3 max0 = max(max00, max01);
	float3 max1 = max(max10, max11);

	float3 maxFinal = max(max0, max1);
	
	float depth = minFinal.z;

	if(g_DecisionArraySRV[primID] == 1)
	{
		output.position = float4(maxFinal.x, minFinal.y, depth, 1.0f);
		triStream.Append(output);

		output.position = float4(minFinal.x, minFinal.y, depth, 1.0f);
		triStream.Append(output);

		output.position = float4(maxFinal.x, maxFinal.y, depth, 1.0f);
		triStream.Append(output);
		
		output.position = float4(minFinal.x, maxFinal.y, depth, 1.0f);
		triStream.Append(output);
	}
}

////////////////////////////////////////////////////////////////////
// For first pass // Count fragments per pixel
////////////////////////////////////////////////////////////////////

[numthreads(groupthreads*groupthreads, 1, 1)]
void clearCS(int3 dTid : SV_DispatchThreadID)
{
	if(dTid.x >= (int)(g_RenderTargetWidth*g_RenderTargetHeight)) return;
	
	g_FragmentCountBufferUAV[dTid.x] = 0;
}

void PS_Count(PS_INPUT input)
{
	int2 index = (int2)input.position.xy;
	int index1D = index.y*g_RenderTargetWidth+index.x;

	int original;
	InterlockedAdd(g_FragmentCountBufferUAV[index1D], 1, original);
}

////////////////////////////////////////////////////////////////////
// For third pass // Write fragments
////////////////////////////////////////////////////////////////////

void PS_Write(PS_INPUT input) 
{
	int2 index = (int2)input.position.xy;
	int index1D = index.y*g_RenderTargetWidth+index.x;

	int internalIndex;
	InterlockedAdd(g_FragmentCountBufferUAV[index1D], -1, internalIndex);
	int endIndex = g_FragmentPrefixSumBufferSRV[index1D];

	int offset = endIndex-internalIndex;

	g_FragmentSortedDepthBufferUAV[offset] = input.position.z;
}

////////////////////////////////////////////////////////////////////
// For forth pass // Sort fragments
////////////////////////////////////////////////////////////////////

static const uint maxLocalBufferSize = 30;  // adapt also in raycasting shader

[numthreads(groupthreads*groupthreads, 1, 1)]
void sortFragmentsCS(int3 dTid : SV_DispatchThreadID, int3 GTid : SV_GroupThreadID)
{
	if(dTid.x >= (int)(g_RenderTargetWidth*g_RenderTargetHeight)) return;

	// Allocate local memory
	float depthValuesLocalCopy[maxLocalBufferSize];

	int startIndex = 0;
	if(dTid.x != 0) startIndex = g_FragmentPrefixSumBufferSRV[dTid.x-1];
	int endIndex = g_FragmentPrefixSumBufferSRV[dTid.x];
	int length = min(endIndex-startIndex, maxLocalBufferSize);

	// Copy to local memory
	for(int k = 0; k < length; k++)
	{
		depthValuesLocalCopy[k] = g_FragmentSortedDepthBufferUAV[startIndex+k];
	}

	// Sort local copy
	for(int i = 0; i < length-1; i++)
	{
		float d = depthValuesLocalCopy[i];

		int swap_id = i;
		for(int j = i; j < length; j++)
		{
			float dj = depthValuesLocalCopy[j];
			if(dj < d)
			{
				d = dj;
				swap_id = j;
			}
		}

		if(swap_id != i)
		{			
			depthValuesLocalCopy[swap_id] = depthValuesLocalCopy[i];
			depthValuesLocalCopy[i] = d;
		}
	}

	// Copy to output buffer
	for(int l = 0; l < length; l++)
	{
		g_FragmentSortedDepthBufferUAV[startIndex+l] = depthValuesLocalCopy[l];
	}
}

////////////////////////////////////////////////////////////////////
// Pre-pass // Which blocks should be splatted
////////////////////////////////////////////////////////////////////

groupshared float shared_MinSDF[SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2];

[numthreads( SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2, 1, 1)]
void splatIdentifyCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex, uint3 GID : SV_GroupID)
{
	const uint groupID = GID.x + GID.y * NUM_GROUPS_X;
	const uint hashIdx = groupID;
	if(hashIdx < g_NumOccupiedSDFBlocks)
	{ 
		HashEntry entry = getHashEntry(g_VoxelHash, hashIdx);
		uint idx0 = entry.ptr + 2*GTid.x+0;
		uint idx1 = entry.ptr + 2*GTid.x+1;
		Voxel v0 = getVoxel_onlySDF_RGBW(g_SDFBlocksSDFSRV, g_SDFBlocksRGBWSRV, idx0);
		Voxel v1 = getVoxel_onlySDF_RGBW(g_SDFBlocksSDFSRV, g_SDFBlocksRGBWSRV, idx1);
		if (v0.weight == 0)	v0.sdf = PINF;
		if (v1.weight == 0) v1.sdf = PINF;
		shared_MinSDF[GTid.x] = min(abs(v0.sdf), abs(v1.sdf));	//init shared memory
		
		uint numGroupThreads = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2;

		[loop]
		for(uint stride = 2; stride <= numGroupThreads; stride <<= 1)
		{
			GroupMemoryBarrierWithGroupSync();
			if((GTid.x  & (stride-1)) == (stride-1))
			{
				shared_MinSDF[GTid.x] = min(shared_MinSDF[GTid.x-stride/2], shared_MinSDF[GTid.x]);
			}
		}

		if(GTid.x == numGroupThreads - 1)
		{
            float minSDF = shared_MinSDF[GTid.x];
            
			g_DecisionArrayUAV[hashIdx] = 0;
			//float t = getTruncation(g_maxIntegrationDistance);
			
			if(minSDF < rayIncrement+g_VirtualVoxelSize)
			{
				g_DecisionArrayUAV[hashIdx] = 1;
			}
		}
	}
}
                                                                                                                                                                                                                                                                         #ifndef _GLOBAL_APP_STATE_BUFFER_H_
#define _GLOBAL_APP_STATE_BUFFER_H_

cbuffer cbGlobalAppState : register( b8 )
{
	uint	g_WeightSample;			//weight per sample (per integration step)
	uint	g_WeightMax;			//maximum weight per voxel
	float	g_Truncation;	
	float	g_maxIntegrationDistance;

	float3 m_voxelExtends;
	float	g_TruncScale;			//how to adapt the truncation: per distance in meter
	
	int3 m_gridDimensions;
	int nBitsInT;

	int3 m_minGridPos;
	float rayIncrement;

	float g_thresSampleDist;
	float g_thresDist;
	float g_thresMarchingCubes;
	float materialShininess;
	
	float4 materialDiffuse;
	float4 materialSpecular;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
	float4 g_LightDir;

	uint	g_MaxLoopIterCount;
	float	g_thresMarchingCubes2;
	uint	g_useGradients;
	uint	g_enableMultiLayerSplatting;

	//float4x4 g_intrinsics;
	//float4x4 g_intrinsicsInv;
	float4x4 g_intrinsicsCoeff;		//coeffs

	float4x4 g_intrinsicsStereo;
	float4x4 g_intrinsicsInvStereo;

	float4x4 g_intrinsicsStereoOther;
	float4x4 g_intrinsicsInvStereoOther;

	float4x4 g_worldToCamStereo;
	float4x4 g_camToWorldStereo;

	float4x4 g_worldToCamStereoOther;
	float4x4 g_camToWorldStereoOther;

	uint g_stereoEnabled;
	float g_SensorDepthWorldMin;
	float g_SensorDepthWorldMax;
	uint g_dummy02Glob;
};

#endif
                                                                                                                                                                                                                                    #ifndef _SDF_SHADER_BUFFER_H_
#define _SDF_SHADER_BUFFER_H_

cbuffer cbCS : register( b0 )
{
	float4x4	g_RigidTransform;
	float4x4	g_RigidTransformInverse;
	uint		g_HashNumBuckets;
	uint		g_HashBucketSize;
	uint		g_ImageWidth;
	uint		g_ImageHeight;
	float		g_VirtualVoxelSize;
	float		g_VirtualVoxelResolutionScalar;
	uint		g_NumSDFBlocks;
	uint		g_NumOccupiedSDFBlocks;
};

cbuffer cbCS : register( b1 )
{
	float4x4	g_Other_RigidTransform;
	float4x4	g_Other_RigidTransformInverse;
	uint		g_Other_HashNumBuckets;
	uint		g_Other_HashBucketSize;
	uint		g_Other_ImageWidth;
	uint		g_Other_ImageHeight;
	float		g_Other_VirtualVoxelSize;
	float		g_Other_VirtualVoxelResolutionScalar;
	uint		g_Other_NumSDFBlocks;
	uint		g_Other_NumOccupiedSDFBlocks;
};


#endif
                                                                                                                                                                                                                                                                           #ifndef _KINECTCAMERA_UTIL_H_
#define _KINECTCAMERA_UTIL_H_

//#ifndef NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240
//#define NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 (285.63f)
//#endif

//#ifndef NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240
//#define NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 (3.501e-3f)
//#endif

//#define DEPTH_WORLD_MIN 0.4f
//#define DEPTH_WORLD_MAX 12.0f
//set by buffer
#define DEPTH_WORLD_MIN g_SensorDepthWorldMin
#define DEPTH_WORLD_MAX g_SensorDepthWorldMax


///////////////////////////////////////////////////////////////
// Camera to Screen
///////////////////////////////////////////////////////////////

float2 cameraToKinectScreenFloat(float3 pos)
{
	//float4 p = float4(pos.x, pos.y, 0.0f, pos.z);
	//float4x4 projMat = g_intrinsics;
	//if(g_stereoEnabled) projMat = g_intrinsicsStereo;

	//float4 proj = mul(p, projMat);
	//return float2(proj.x/proj.w, proj.y/proj.w);

	//undistort
	float fx = g_intrinsicsCoeff[0][0];
	float fy = g_intrinsicsCoeff[1][0];
	float mx = g_intrinsicsCoeff[2][0];
	float my = g_intrinsicsCoeff[3][0];
	float k1 = g_intrinsicsCoeff[0][1];
	float k2 = g_intrinsicsCoeff[1][1];
	float k3 = g_intrinsicsCoeff[2][1];
	float p1 = g_intrinsicsCoeff[3][1];
	float p2 = g_intrinsicsCoeff[0][2];

	//float2 p = float2(pos.x/pos.z, pos.y/pos.z);
	//float r2 = p.x*p.x + p.y*p.y;

	//float2 pos2;
	//pos2.x = p.x * (1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2) + 2.0f*p1*p.x*p.y + p2*(r2*2.0f*p.x*p.x);
	//pos2.y = p.y * (1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2) + 2.0f*p2*p.x*p.y + p1*(r2*2.0f*p.y*p.y);
	//
	//pos2.x = pos2.x*fx + mx;
	//pos2.y = my - pos2.y*fy;

	//return pos2;
	return float2(pos.x*fx/pos.z + mx, my - pos.y*fy/pos.z);
}

int2 cameraToKinectScreenInt(float3 pos)
{
	float2 pImage = cameraToKinectScreenFloat(pos);
	return int2(pImage + float2(0.5f, 0.5f));
}

uint2 cameraToKinectScreen(float3 pos)
{
    int2 p = cameraToKinectScreenInt(pos);

	return uint2(p.xy);
}

float cameraToKinectProjZ(float z)
{
	return (z - DEPTH_WORLD_MIN)/(DEPTH_WORLD_MAX - DEPTH_WORLD_MIN);
}

float kinectProjZToCamera(float z)
{
	return DEPTH_WORLD_MIN+z*(DEPTH_WORLD_MAX - DEPTH_WORLD_MIN);
}

float3 cameraToKinectProj(float3 pos)
{
	float2 proj = cameraToKinectScreenFloat(pos);

    float3 pImage = float3(proj.x, proj.y, pos.z);

	pImage.x = (2.0f*pImage.x - (g_ImageWidth-1.0f))/(g_ImageWidth-1.0f);
	pImage.y = ((g_ImageHeight-1.0f) - 2.0f*pImage.y)/(g_ImageHeight-1.0f);
	pImage.z = cameraToKinectProjZ(pImage.z);

	return pImage;
}

///////////////////////////////////////////////////////////////
// Screen to Camera (depth in meters)
///////////////////////////////////////////////////////////////

float3 kinectDepthToSkeleton(uint ux, uint uy, float depth)
{
	//float4x4 projInv = g_intrinsicsInv;
	//if(g_stereoEnabled) projInv = g_intrinsicsInvStereo;

	//float4 camera = mul(float4((float)ux*depth, (float)uy*depth, 0.0f, depth), projInv);
	//
	//return float3(camera.x, camera.y, camera.w);
	
	//undistort
	float fx = g_intrinsicsCoeff[0][0];
	float fy = g_intrinsicsCoeff[1][0];
	float mx = g_intrinsicsCoeff[2][0];
	float my = g_intrinsicsCoeff[3][0];
	float k1 = g_intrinsicsCoeff[0][1];
	float k2 = g_intrinsicsCoeff[1][1];
	float k3 = g_intrinsicsCoeff[2][1];
	float p1 = g_intrinsicsCoeff[3][1];
	float p2 = g_intrinsicsCoeff[0][2];

	const float x = ((float)ux-mx) / fx;
	const float y = (my-(float)uy) / fy;

	//float r2 = x*x + y*y;
	//float2 pos2;
	//pos2.x = x * (1 + k1 * r2 + k2 * r2*r2 + k3 * r2*r2*r2) + 2.0f * p1 * x * y + p2 * (r2 * 2.0f * x*x);
	//pos2.y = y * (1 + k1 * r2 + k2 * r2*r2 + k3 * r2*r2*r2) + 2.0f * p2 * x * y + p1 * (r2 * 2.0f * y*y); 

	//return float3(pos2.x*depth, pos2.y*depth, depth);
	return float3(depth*x, depth*y, depth);
}

///////////////////////////////////////////////////////////////
// RenderScreen to Camera -- ATTENTION ASSUMES [1,0]-Z range!!!!
///////////////////////////////////////////////////////////////

float kinectProjToCameraZ(float z)
{
	return z * (DEPTH_WORLD_MAX - DEPTH_WORLD_MIN) + DEPTH_WORLD_MIN;
}

// z has to be in [0, 1]
float3 kinectProjToCamera(uint ux, uint uy, float z)
{
	float fSkeletonZ = kinectProjToCameraZ(z);
	return kinectDepthToSkeleton(ux, uy, fSkeletonZ);
}

#endif
                                                                                                                                                                                                                                                                                                                                                              #ifndef _VOXEL_UTIL_HASH_SDF_H_
#define _VOXEL_UTIL_HASH_SDF_H_

// should be set by application
#ifndef groupthreads
#define groupthreads 1
#endif

//// should be set by application
//#ifndef HASH_BUCKET_SIZE
//#define HASH_BUCKET_SIZE 10
//#endif

#ifndef MINF
#define MINF asfloat(0xff800000)
#endif

#ifndef PINF
#define PINF asfloat(0x7f800000)
#endif

//status flags for hash entries
static const int LOCK_ENTRY = -1;
static const int FREE_ENTRY = -2;
static const int NO_OFFSET = 0;

//static const uint	g_WeightSample = 3;			//weight per sample (per integration step)
//static const uint	g_WeightMax = 255;			//maximum weight per voxel
//static const float	g_Truncation = 0.05f;		//5cm world space
//static const float	g_TruncScale = 0.02f;		//how to adapt the truncation: per distance meter, increase truncation by 1 cm

//#define HANDLE_COLLISIONS 1

struct HashEntry
{
	int3 pos;	//hash position (lower left corner of SDFBlock))
	int offset;	//offset for collisions
	int ptr;	//pointer into heap to SDFBlock
};

struct Voxel
{
	float sdf;
	uint3 color;
	uint weight;
	int label[12];
};

//! computes the (local) virtual voxel pos of an index; idx in [0;511]
int3 delinearizeVoxelIndex(uint idx)
{
	uint x = idx % SDF_BLOCK_SIZE;
	uint y = (idx % (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE)) / SDF_BLOCK_SIZE;
	uint z = idx / (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE);	
	return uint3(x,y,z);
}

//! computes the linearized index of a local virtual voxel pos; pos in [0;7]^3
uint linearizeVoxelPos(int3 virtualVoxelPos)
{
	return  virtualVoxelPos.z * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE +
			virtualVoxelPos.y * SDF_BLOCK_SIZE +
			virtualVoxelPos.x;
}

HashEntry getHashEntry(Buffer<int> hash, in uint id)
{
	HashEntry entry;
	int i0 = hash[3*id+0];
	entry.pos.x = i0 & 0x0000ffff;
	if (entry.pos.x & (0x1 << 15))	entry.pos.x |= 0xffff0000;
	entry.pos.y = i0 >> 16;
	if (entry.pos.y & (0x1 << 15))	entry.pos.y |= 0xffff0000;

	int i1 = hash[3*id+1];
	entry.pos.z = i1 & 0x0000ffff;
	if (entry.pos.z & (0x1 << 15))	entry.pos.z |= 0xffff0000;

	entry.offset = i1 >> 16;
	if (entry.offset & (0x1 << 15)) entry.offset |= 0xffff0000;
	
	entry.ptr = hash[3*id+2];
	return entry;
}

HashEntry getHashEntry(RWBuffer<int> hash, in uint id)
{
	HashEntry entry;
	int i0 = hash[3*id+0];
	entry.pos.x = i0 & 0x0000ffff;
	if (entry.pos.x & (0x1 << 15))	entry.pos.x |= 0xffff0000;
	entry.pos.y = i0 >> 16;
	if (entry.pos.y & (0x1 << 15))	entry.pos.y |= 0xffff0000;

	int i1 = hash[3*id+1];
	entry.pos.z = i1 & 0x0000ffff;
	if (entry.pos.z & (0x1 << 15))	entry.pos.z |= 0xffff0000;

	entry.offset = i1 >> 16;
	if (entry.offset & (0x1 << 15)) entry.offset |= 0xffff0000;

	entry.ptr = hash[3*id+2];
	return entry;
}

void setHashEntry(RWBuffer<int> hash, in uint id, const in HashEntry entry)
{
	hash[3*id+0] = (entry.pos.y << 16) | (entry.pos.x & 0x0000ffff);
	hash[3*id+1] = (entry.offset << 16) | (entry.pos.z & 0x0000ffff);
	hash[3*id+2] = entry.ptr;
}

void deleteHashEntry(RWBuffer<int> hash, in uint id) 
{
	hash[3*id+0] = 0;
	hash[3*id+1] = 0;
	hash[3*id+2] = FREE_ENTRY;
}

Voxel getVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW, RWBuffer<int> sdfBlocksLabel_1 , RWBuffer<int> sdfBlocksLabel_2, RWBuffer<int> sdfBlocksLabel_3, in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];
	int last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;                                                                                                                                                                                                                                                                                                        
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	//voxel.color.z = last;
	
	//Label have 32 bits.
	int last2 = sdfBlocksLabel_1[id]   ;
	voxel.label[0] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[1] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[2] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[3] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_2[id];
	voxel.label[4] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[5] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[6] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[7] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_3[id];
	voxel.label[8] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[9] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[10] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[11] = last2 & 0x000000ff;
	last2 >>= 0x8 ;   

	//last2 = sdfBlocksLabel_4[id];
	//voxel.label[12] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    
	//voxel.label[13] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    
	//voxel.label[14] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    
	//voxel.label[15] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    

	return voxel;
}

Voxel getVoxel(Buffer<float> sdfBlocksSDF, Buffer<int> sdfBlocksRGBW, Buffer<int> sdfBlocksLabel_1, Buffer<int> sdfBlocksLabel_2, Buffer<int> sdfBlocksLabel_3,in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];
	int last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;                                                                                                                                                                                                                                                                                                        
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	//voxel.color.z = last;
	
	//Label have 32 bits.
	int last2 = sdfBlocksLabel_1[id];
	voxel.label[0] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[1] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[2] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[3] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_2[id];
	voxel.label[4] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[5] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[6] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[7] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[8] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_3[id];
	voxel.label[9] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[10] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[11] = last2 & 0x000000ff;
	last2 >>= 0x8 ;   

/*	last2 = sdfBlocksLabel_4[id];
	voxel.label[12] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[13] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[14] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[15] = last2 & 0x000000ff;
	last2 >>= 0x8 ;   */ 

	return voxel;
}

Voxel getVoxel_onlySDF_RGBW(Buffer<float> sdfBlocksSDF, Buffer<int> sdfBlocksRGBW , in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];
	int last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;                                                                                                                                                                                                                                                                                                        
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	//voxel.color.z = last;
	for(int i=0;i<12;i++)
	{
		voxel.label[i]=0;
	}

	return voxel;
}


void setVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW,  RWBuffer<int>  sdfBlocksLabel_1,RWBuffer<int>  sdfBlocksLabel_2,RWBuffer<int>  sdfBlocksLabel_3,in uint id, in Voxel voxel)
{
	sdfBlocksSDF[id] = voxel.sdf;
	int last = 0;
	last |= voxel.color.z & 0x000000ff;
	last <<= 8;
	last |= voxel.color.y & 0x000000ff;
	last <<= 8;
	last |= voxel.color.x & 0x000000ff;
	last <<= 8;
	last |= voxel.weight & 0x000000ff;
	//last |= voxel.color.z;
	//last <<= 8;
	//last |= voxel.color.y;
	//last <<= 8;
	//last |= voxel.color.x;
	//last <<= 8;
	//last |= voxel.weight;
	sdfBlocksRGBW[id] = last;    

	//label's set.32bit.
	

	last |= (uint)voxel.label[0] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[1] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[2] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[3] & 0x000000ff;	
	sdfBlocksLabel_1[id] = last;   

	last |= (uint)voxel.label[4] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[5] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[6] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[7] & 0x000000ff;	
	sdfBlocksLabel_2[id] = last;   

	last |= (uint)voxel.label[8] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[9] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[10] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[11] & 0x000000ff;	
	sdfBlocksLabel_3[id] = last;   

	//last |= (uint)voxel.label[12] & 0x000000ff;
	//last <<= 8;
	//last |= (uint)voxel.label[13] & 0x000000ff;
	//last <<= 8;
	//last |= (uint)voxel.label[14] & 0x000000ff;
	//last <<= 8;
	//last |= (uint)voxel.label[15] & 0x000000ff;	
	//sdfBlocksLabel_4[id] = last;   
}

void starveVoxel(RWBuffer<int> sdfBlocksRGBW,in uint id) {
	int last = sdfBlocksRGBW[id];
	int weight = last & 0x000000ff;
	weight = max(0, weight-1);	
	last &= 0xffffff00;
	last |= weight;
	sdfBlocksRGBW[id] = last;
}

void deleteVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW,  RWBuffer<int>  sdfBlocksLabel_1,RWBuffer<int>  sdfBlocksLabel_2,RWBuffer<int>  sdfBlocksLabel_3,in uint id) {
	sdfBlocksSDF[id] = 0;
	sdfBlocksRGBW[id] = 0;
	sdfBlocksLabel_1[id] = 0;
	sdfBlocksLabel_2[id] = 0;
	sdfBlocksLabel_3[id] = 0;
	//sdfBlocksLabel_4[id] = 0;
}


float3 worldToVirtualVoxelPosFloat(in float3 pos)
{
	return pos*g_VirtualVoxelResolutionScalar;
}

int3 worldToVirtualVoxelPos(in float3 pos)
{
	const float3 p = pos*g_VirtualVoxelResolutionScalar;
	return (int3)(p+sign(p)*0.5f);
}

int3 virtualVoxelPosToSDFBlock(int3 virtualVoxelPos)
{
	if (virtualVoxelPos.x < 0) virtualVoxelPos.x -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.y < 0) virtualVoxelPos.y -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.z < 0) virtualVoxelPos.z -= SDF_BLOCK_SIZE-1;

	return virtualVoxelPos/SDF_BLOCK_SIZE;
}

// Computes virtual voxel position of corner sample position
int3 SDFBlockToVirtualVoxelPos(int3 sdfBlock)
{
	return sdfBlock*SDF_BLOCK_SIZE;
}

float3 virtualVoxelPosToWorld(in int3 pos)
{   
	return float3(pos)*g_VirtualVoxelSize;
}

float3 SDFBlockToWorld(int3 sdfBlock)
{
	return virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock));
}

int3 worldToSDFBlock(float3 worldPos)
{
	const int3 virtualVoxelPos = worldToVirtualVoxelPos(worldPos);
	return virtualVoxelPosToSDFBlock(virtualVoxelPos);
}

int virtualVoxelPosToLocalSDFBlockIndex(int3 virtualVoxelPos)
{
	int3 localVoxelPos = virtualVoxelPos%SDF_BLOCK_SIZE;

	if (localVoxelPos.x < 0) localVoxelPos.x += SDF_BLOCK_SIZE;
	if (localVoxelPos.y < 0) localVoxelPos.y += SDF_BLOCK_SIZE;
	if (localVoxelPos.z < 0) localVoxelPos.z += SDF_BLOCK_SIZE;

	return linearizeVoxelPos(localVoxelPos);
}

int worldToLocalSDFBlockIndex(float3 world)
{
	int3 virtualVoxelPos = worldToVirtualVoxelPos(world);
	return virtualVoxelPosToLocalSDFBlockIndex(virtualVoxelPos);
}

//! see teschner et al. (but with correct prime values)
uint computeHashPos(in int3 virtualVoxelPos)
{ 
	const int p0 = 73856093;
	const int p1 = 19349669;
	const int p2 = 83492791;

	int res = ((virtualVoxelPos.x * p0) ^ (virtualVoxelPos.y * p1) ^ (virtualVoxelPos.z * p2)) % g_HashNumBuckets;
	if (res < 0) res += g_HashNumBuckets;
	return (uint)res;
}

//! see teschner et al. (but with correct prime values)
uint computeHashPosOther(in int3 virtualVoxelPos, in uint hashNumBuckets)
{ 
	const int p0 = 73856093;
	const int p1 = 19349669;
	const int p2 = 83492791;

	int res = ((virtualVoxelPos.x * p0) ^ (virtualVoxelPos.y * p1) ^ (virtualVoxelPos.z * p2)) % hashNumBuckets;
	if (res < 0) res += hashNumBuckets;
	return (uint)res;
}

//merges two voxels (v0 is the input voxel, v1 the currently stored voxel)
Voxel combineVoxel(in Voxel v0, in Voxel v1)
{
	Voxel v;
	//v.color = (10*v0.weight * v0.color + v1.weight * v1.color)/(10*v0.weight + v1.weight);	//give the currently observed color more weight
	//v.color = (v0.weight * v0.color + v1.weight * v1.color)/(v0.weight + v1.weight);
	v.color = 0.5f * (v0.color + v1.color);	//exponential running average

	v.label[0] = 0.5f * (v0.label[0] + v1.label[0]); 
	v.label[1] = 0.5f * (v0.label[1] + v1.label[1]); 
	v.label[2] = 0.5f * (v0.label[2] + v1.label[2]); 
	v.label[3] = 0.5f * (v0.label[3] + v1.label[3]); 
	v.label[4] = 0.5f * (v0.label[4] + v1.label[4]); 
	v.label[5] = 0.5f * (v0.label[5] + v1.label[5]); 
	v.label[6] = 0.5f * (v0.label[6] + v1.label[6]); 
	v.label[7] = 0.5f * (v0.label[7] + v1.label[7]); 
	v.label[8] = 0.5f * (v0.label[6] + v1.label[8]); 
	v.label[9] = 0.5f * (v0.label[9] + v1.label[9]); 
	v.label[10] = 0.5f * (v0.label[10] + v1.label[10]); 
	v.label[11] = 0.5f * (v0.label[11] + v1.label[11]); 
	/*v.label[12] = 0.5f * (v0.label[12] + v1.label[12]); 
	v.label[13] = 0.5f * (v0.label[13] + v1.label[13]); 
	v.label[14] = 0.5f * (v0.label[14] + v1.label[14]); 
	v.label[15] = 0.5f * (v0.label[15] + v1.label[15]); */



	v.sdf = (v0.sdf * v0.weight + v1.sdf * v1.weight) / (v0.weight + v1.weight);
	v.weight = min(g_WeightMax, v0.weight + v1.weight);
	return v;
}

//! returns the hash entry for a given sdf block id; if there was no hash entry the returned entry will have a ptr with FREE_ENTRY set
HashEntry getHashEntryForSDFBlockPos(Buffer<int> hash, int3 sdfBlock) 
{
	uint h = computeHashPos(sdfBlock);	//hash bucket
	uint hp = h * g_HashBucketSize;		//hash position

	HashEntry entry;
	entry.pos = sdfBlock;
	entry.offset = 0;
	entry.ptr = FREE_ENTRY;

	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++) {
		uint i = j + hp;
		HashEntry curr = getHashEntry(hash, i);
		if (curr.pos.x == entry.pos.x && curr.pos.y == entry.pos.y && curr.pos.z == entry.pos.z && curr.ptr != FREE_ENTRY) {
			return curr;
		}
	}

	#ifdef HANDLE_COLLISIONS
		const uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;
		int i = idxLastEntryInBucket;	//start with the last entry of the current bucket
		HashEntry curr;
		//traverse list until end: memorize idx at list end and memorize offset from last element of bucket to list end
		int k = 0;

		unsigned int maxIter = 0;
		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount) {
			curr = getHashEntry(hash, i);
			
			if (curr.pos.x == entry.pos.x && curr.pos.y == entry.pos.y && curr.pos.z == entry.pos.z && curr.ptr != FREE_ENTRY) {
				return curr;
			}

			if (curr.offset == 0) {	//we have found the end of the list
				break;
			}
			i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
			i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

			maxIter++;
		}
	#endif
	return entry;
}

//! returns the hash entry for a given worldPos; if there was no hash entry the returned entry will have a ptr with FREE_ENTRY set
HashEntry getHashEntry(Buffer<int> hash, float3 worldPos) 
{
	//int3 blockID = worldToSDFVirtualVoxelPos(worldPos)/SDF_BLOCK_SIZE;	//position of sdf block
	int3 blockID = worldToSDFBlock(worldPos);
	return getHashEntryForSDFBlockPos(hash, blockID);
}

//! returns the truncation of the SDF for a given distance value
float getTruncation(float z) {
	return g_Truncation + g_TruncScale * z;
}

bool isInCameraFrustum(in float3 pos) {
	float3 pCamera = mul(float4(pos, 1.0f), g_RigidTransformInverse).xyz;
	float3 pProj = cameraToKinectProj(pCamera);
	//pProj *= 0.75f;	//TODO THIS IS A HACK FIX IT :)
	return !(pProj.x < -1.0f || pProj.x > 1.0f || pProj.y < -1.0f || pProj.y > 1.0f || pProj.z < 0.0f || pProj.z > 1.0f);  
}

bool isInCameraFrustumApprox(in float3 pos) {
	float3 pCamera = mul(float4(pos, 1.0f), g_RigidTransformInverse).xyz;
	float3 pProj = cameraToKinectProj(pCamera);
	//pProj *= 1.5f;	//TODO THIS IS A HACK FIX IT :)
	pProj *= 0.95;
	return !(pProj.x < -1.0f || pProj.x > 1.0f || pProj.y < -1.0f || pProj.y > 1.0f || pProj.z < 0.0f || pProj.z > 1.0f);  
}

bool isSDFBlockInCameraFrustumApprox(in int3 sdfBlock) {
	float3 posWorld = virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock)) + g_VirtualVoxelSize * 0.5f * (SDF_BLOCK_SIZE - 1.0f);
	return isInCameraFrustumApprox(posWorld);
}





 

//! deletes a hash entry position for a given sdfBlock index (returns true uppon successful deletion; otherwise returns false)
bool deleteHashEntryElement(RWBuffer<int> hash, AppendStructuredBuffer<uint> heapAppend, RWBuffer<uint> bucketMutex, int3 sdfBlock) {
	uint h = computeHashPos(sdfBlock);	//hash bucket
	uint hp = h * g_HashBucketSize;		//hash position

	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++) {
		uint i = j + hp;
		HashEntry curr = getHashEntry(hash, i);
		if (curr.pos.x == sdfBlock.x && curr.pos.y == sdfBlock.y && curr.pos.z == sdfBlock.z && curr.ptr != FREE_ENTRY) {
			#ifndef HANDLE_COLLISIONS
			const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
			heapAppend.Append(curr.ptr / linBlockSize);
			deleteHashEntry(hash, i);
			return true;
			#endif
			#ifdef HANDLE_COLLISIONS
			if (curr.offset != 0) {	//if there was a pointer set it to the next list element
				int prevValue = 0;
				InterlockedExchange(bucketMutex[h], LOCK_ENTRY, prevValue);	//lock the hash bucket
				if (prevValue == LOCK_ENTRY)	return false;
				if (prevValue != LOCK_ENTRY) {
					const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
					heapAppend.Append(curr.ptr / linBlockSize);
					int nextIdx = (i + curr.offset) % (g_HashBucketSize * g_HashNumBuckets);
					setHashEntry(hash, i, getHashEntry(hash, nextIdx));
					deleteHashEntry(hash, nextIdx);
					return true;
				}
			} else {
				const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
				heapAppend.Append(curr.ptr / linBlockSize);
				deleteHashEntry(hash, i);
				return true;
			}
			#endif
		}
	}	
	#ifdef HANDLE_COLLISIONS
		const uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;
		int i = idxLastEntryInBucket;
		int offset = 0;
		HashEntry curr;
		curr = getHashEntry(hash, i);
		int prevIdx = i;
		i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
		i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

		unsigned int maxIter = 0;
		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount) {
			curr = getHashEntry(hash, i);
			//found that dude that we need/want to delete
			if (curr.pos.x == sdfBlock.x && curr.pos.y == sdfBlock.y && curr.pos.z == sdfBlock.z && curr.ptr != FREE_ENTRY) {
				int prevValue = 0;
				InterlockedExchange(bucketMutex[h], LOCK_ENTRY, prevValue);	//lock the hash bucket
				if (prevValue == LOCK_ENTRY)	return false;
				if (prevValue != LOCK_ENTRY) {
					const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
					heapAppend.Append(curr.ptr / linBlockSize);
					deleteHashEntry(hash, i);
					HashEntry prev = getHashEntry(hash, prevIdx);				
					prev.offset = curr.offset;
					setHashEntry(hash, prevIdx, prev);
					return true;
				}
			}

			if (curr.offset == 0) {	//we have found the end of the list
				return false;	//should actually never happen because we need to find that guy before
			}
			prevIdx = i;
			i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
			i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

			maxIter++;
		}
	#endif
	return false;
}










#endif
                                                                                                                                                                                                                                                           юпюп   Ж   E:\VoxelHashing-master\DepthSensing\Shaders\RayMarchingStepsSplatting.hlsl  e:\voxelhashing-master\depthsensing\shaders\raymarchingstepssplatting.hlsl GlobalAppStateShaderBuffer.h.hlsl globalappstateshaderbuffer.h.hlsl SDFShaderBuffer.h.hlsl sdfshaderbuffer.h.hlsl KinectCameraUtil.h.hlsl kinectcamerautil.h.hlsl VoxelUtilHashSDF.h.hlsl voxelutilhashsdf.h.hlsl Buffer<int>		g_VoxelHash : register(t0);		

Buffer<int>		g_FragmentCountBufferSRV : register(t1);
Buffer<int>		g_FragmentPrefixSumBufferSRв00  ђ%Y/"њ“                                                  
      …      "  (   в0Т)щҐ  
  L   "              у   (   в0Oй§~х  №   L   у               Ї   (   в0≥Т~Ц  Ш   L   Ї               M   (   в0_xњсч     L   M               R  (   в0»Ґ£≤M  :  L   R                                                                                                                                                                                                                                 N <   	  р#ј	  р#јMicrosoft (R) HLSL Shader Compiler 9.30.9200.16384    В =hlslFlags 0x801 hlslTarget ps_5_0 hlslEntry PS_Write hlslDefines  /Dgroupthreads=8 /DSDF_BLOCK_SIZE=8 /DHANDLE_COLLISIONS=1    .            ƒ       ƒ     t    †PS_Write . >  	 input                                  P     t    ƒ      P    t    ƒ     P    t    ƒ     P    t    ƒ    . >   index                                  P      И    (      P     И    ∞    . >t     index1D                                P      ∞    H     6 >t     internalIndex                                  P      ‘    d    . >t     endIndex                               P      ш          . >t     offset                                 P                 ф   x      л~?ацF^a	ф}5Е2  Ш   {њгЯ-8LэШґёpyЭu0  №   ЯИ)ъ#_Н‘dЧ%Ц…RМ  
  ~~hљњЩО"‘qА!m’√И  :  <х2€∆Ѓg"мƒђ^†э  т   ј         8         і   t   Ч  Аt   Ч   И   Ш  АИ   Ш   ∞   Ы  А∞   Ы   ‘   Ь  А‘   Ь   ш   Ю  Аш   Ю     †  А  †   4  °  А4  °    &  %  3  2  F  E  6  5  %  $  ;  :     ц                    L   |   ∞                                                                                                                                                                                                                                                                                                                                                                                    18           €€   А      @   @      H        @       float4 утс       position с               PS_INPUT с
      
              t       int2 с t   @В  тс
     
      t   PА  тс
 
     @   PА  тс
     
 u     
                                                                                                                                                                                                          18              €€   А                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   on the Cpu

cbuffer cbConstant : register(b1)
{
	float4x4	g_ViewMat;
	float4x4	g_ViewMatInverse;
	uint		g_RenderTargetWidth;
	uint		g_RenderTargetHeight;
	uint		g_dummyRayInteveral337;
	uint		g_dummyRayInteveral338;
};

#include "GlobalAppStateShaderBuffer.h.hlsl"
#include "SDFShaderBuffer.h.hlsl"
#include "KinectCameraUtil.h.hlsl"
#include "VoxelUtilHashSDF.h.hlsl"

////////////////////////////////////////////////////////////////////
// General setup
////////////////////////////////////////////////////////////////////

static const float4 offsets[4] = 
{
    float4( 0.5, -0.5, 0.0, 0.0),
    float4(-0.5, -0.5, 0.0, 0.0),
	
	float4( 0.5,  0.5, 0.0, 0.0),
    float4(-0.5,  0.5, 0.0, 0.0)
};

struct GS_INPUT
{
};

struct PS_INPUT
{
    float4 position	: SV_POSITION;
};

GS_INPUT VS()
{
    GS_INPUT output = (GS_INPUT)0;
 
    return output;
}

[maxvertexcount(4)]
void GS(point GS_INPUT points[1], uint primID : SV_PrimitiveID, inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT output;

	HashEntry entry = getHashEntry(g_VoxelHash, primID);
	float3 worldCurrentVoxel = SDFBlockToWorld(entry.pos);

	float3 MINV = worldCurrentVoxel.xyz-g_VirtualVoxelSize/2.0;
	float3 maxv = MINV+SDF_BLOCK_SIZE*g_VirtualVoxelSize;

	float3 proj000 = cameraToKinectProj(mul(float4(MINV.x, MINV.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj100 = cameraToKinectProj(mul(float4(maxv.x, MINV.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj010 = cameraToKinectProj(mul(float4(MINV.x, maxv.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj001 = cameraToKinectProj(mul(float4(MINV.x, MINV.y, maxv.z, 1.0f), g_ViewMat).xyz);
	float3 proj110 = cameraToKinectProj(mul(float4(maxv.x, maxv.y, MINV.z, 1.0f), g_ViewMat).xyz);
	float3 proj011 = cameraToKinectProj(mul(float4(MINV.x, maxv.y, maxv.z, 1.0f), g_ViewMat).xyz);
	float3 proj101 = cameraToKinectProj(mul(float4(maxv.x, MINV.y, maxv.z, 1.0f), g_ViewMat).xyz);
	float3 proj111 = cameraToKinectProj(mul(float4(maxv.x, maxv.y, maxv.z, 1.0f), g_ViewMat).xyz);

	// Tree Reduction Min
	float3 min00 = min(proj000, proj100);
	float3 min01 = min(proj010, proj001);
	float3 min10 = min(proj110, proj011);
	float3 min11 = min(proj101, proj111);

	float3 min0 = min(min00, min01);
	float3 min1 = min(min10, min11);

	float3 minFinal = min(min0, min1);

	// Tree Reduction Max 
	float3 max00 = max(proj000, proj100);
	float3 max01 = max(proj010, proj001);
	float3 max10 = max(proj110, proj011);
	float3 max11 = max(proj101, proj111);

	float3 max0 = max(max00, max01);
	float3 max1 = max(max10, max11);

	float3 maxFinal = max(max0, max1);
	
	float depth = minFinal.z;

	if(g_DecisionArraySRV[primID] == 1)
	{
		output.position = float4(maxFinal.x, minFinal.y, depth, 1.0f);
		triStream.Append(output);

		output.position = float4(minFinal.x, minFinal.y, depth, 1.0f);
		triStream.Append(output);

		output.position = float4(maxFinal.x, maxFinal.y, depth, 1.0f);
		triStream.Append(output);
		
		output.position = float4(minFinal.x, maxFinal.y, depth, 1.0f);
		triStream.Append(output);
	}
}

////////////////////////////////////////////////////////////////////
// For first pass // Count fragments per pixel
////////////////////////////////////////////////////////////////////

[numthreads(groupthreads*groupthreads, 1, 1)]
void clearCS(int3 dTid : SV_DispatchThreadID)
{
	if(dTid.x >= (int)(g_RenderTargetWidth*g_RenderTargetHeight)) return;
	
	g_FragmentCountBufferUAV[dTid.x] = 0;
}

void PS_Count(PS_INPUT input)
{
	int2 index = (int2)input.position.xy;
	int index1D = index.y*g_RenderTargetWidth+index.x;

	int original;
	InterlockedAdd(g_FragmentCountBufferUAV[index1D], 1, original);
}

////////////////////////////////////////////////////////////////////
// For third pass // Write fragments
////////////////////////////////////////////////////////////////////

void PS_Write(PS_INPUT input) 
{
	int2 index = (int2)input.position.xy;
	int index1D = index.y*g_RenderTargetWidth+index.x;

	int internalIndex;
	InterlockedAdd(g_FragmentCountBufferUAV[index1D], -1, internalIndex);
	int endIndex = g_FragmentPrefixSumBufferSRV[index1D];

	int offset = endIndex-internalIndex;

	g_FragmentSortedDepthBufferUAV[offset] = input.position.z;
}

////////////////////////////////////////////////////////////////////
// For forth pass // Sort fragments
////////////////////////////////////////////////////////////////////

static const uint maxLocalBufferSize = 30;  // adapt also in raycasting shader

[numthreads(groupthreads*groupthreads, 1, 1)]
void sortFragmentsCS(int3 dTid : SV_DispatchThreadID, int3 GTid : SV_GroupThreadID)
{
	if(dTid.x >= (int)(g_RenderTargetWidth*g_RenderTargetHeight)) return;

	// Allocate local memory
	float depthValuesLocalCopy[maxLocalBufferSize];

	int startIndex = 0;
	if(dTid.x != 0) startIndex = g_FragmentPrefixSumBufferSRV[dTid.x-1];
	int endIndex = g_FragmentPrefixSumBufferSRV[dTid.x];
	int length = min(endIndex-startIndex, maxLocalBufferSize);

	// Copy to local memory
	for(int k = 0; k < length; k++)
	{
		depthValuesLocalCopy[k] = g_FragmentSortedDepthBufferUAV[startIndex+k];
	}

	// Sort local copy
	for(int i = 0; i < length-1; i++)
	{
		float d = depthValuesLocalCopy[i];

		int swap_id = i;
		for(int j = i; j < length; j++)
		{
			float dj = depthValuesLocalCopy[j];
			if(dj < d)
			{
				d = dj;
				swap_id = j;
			}
		}

		if(swap_id != i)
		{			
			depthValuesLocalCopy[swap_id] = depthValuesLocalCopy[i];
			depthValuesLocalCopy[i] = d;
		}
	}

	// Copy to output buffer
	for(int l = 0; l < length; l++)
	{
		g_FragmentSortedDepthBufferUAV[startIndex+l] = depthValuesLocalCopy[l];
	}
}

////////////////////////////////////////////////////////////////////
// Pre-pass // Which blocks should be splatted
////////////////////////////////////////////////////////////////////

groupshared float shared_MinSDF[SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2];

[numthreads( SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2, 1, 1)]
void splatIdentifyCS(uint3 DTid : SV_DispatchThreadID, uint3 GTid : SV_GroupThreadID, uint GI: SV_GroupIndex, uint3 GID : SV_GroupID)
{
	const uint groupID = GID.x + GID.y * NUM_GROUPS_X;
	const uint hashIdx = groupID;
	if(hashIdx < g_NumOccupiedSDFBlocks)
	{ 
		HashEntry entry = getHashEntry(g_VoxelHash, hashIdx);
		uint idx0 = entry.ptr + 2*GTid.x+0;
		uint idx1 = entry.ptr + 2*GTid.x+1;
		Voxel v0 = getVoxel_onlySDF_RGBW(g_SDFBlocksSDFSRV, g_SDFBlocksRGBWSRV, idx0);
		Voxel v1 = getVoxel_onlySDF_RGBW(g_SDFBlocksSDFSRV, g_SDFBlocksRGBWSRV, idx1);
		if (v0.weight == 0)	v0.sdf = PINF;
		if (v1.weight == 0) v1.sdf = PINF;
		shared_MinSDF[GTid.x] = min(abs(v0.sdf), abs(v1.sdf));	//init shared memory
		
		uint numGroupThreads = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE / 2;

		[loop]
		for(uint stride = 2; stride <= numGroupThreads; stride <<= 1)
		{
			GroupMemoryBarrierWithGroupSync();
			if((GTid.x  & (stride-1)) == (stride-1))
			{
				shared_MinSDF[GTid.x] = min(shared_MinSDF[GTid.x-stride/2], shared_MinSDF[GTid.x]);
			}
		}

		if(GTid.x == numGroupThreads - 1)
		{
            float minSDF = shared_MinSDF[GTid.x];
            
			g_DecisionArrayUAV[hashIdx] = 0;
			//float t = getTruncation(g_maxIntegrationDistance);
			
			if(minSDF < rayIncrement+g_VirtualVoxelSize)
			{
				g_DecisionArrayUAV[hashIdx] = 1;
			}
		}
	}
}
 #ifndef _GLOBAL_APP_STATE_BUFFER_H_
#define _GLOBAL_APP_STATE_BUFFER_H_

cbuffer cbGlobalAppState : register( b8 )
{
	uint	g_WeightSample;			//weight per sample (per integration step)
	uint	g_WeightMax;			//maximum weight per voxel
	float	g_Truncation;	
	float	g_maxIntegrationDistance;

	float3 m_voxelExtends;
	float	g_TruncScale;			//how to adapt the truncation: per distance in meter
	
	int3 m_gridDimensions;
	int nBitsInT;

	int3 m_minGridPos;
	float rayIncrement;

	float g_thresSampleDist;
	float g_thresDist;
	float g_thresMarchingCubes;
	float materialShininess;
	
	float4 materialDiffuse;
	float4 materialSpecular;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
	float4 g_LightDir;

	uint	g_MaxLoopIterCount;
	float	g_thresMarchingCubes2;
	uint	g_useGradients;
	uint	g_enableMultiLayerSplatting;

	//float4x4 g_intrinsics;
	//float4x4 g_intrinsicsInv;
	float4x4 g_intrinsicsCoeff;		//coeffs

	float4x4 g_intrinsicsStereo;
	float4x4 g_intrinsicsInvStereo;

	float4x4 g_intrinsicsStereoOther;
	float4x4 g_intrinsicsInvStereoOther;

	float4x4 g_worldToCamStereo;
	float4x4 g_camToWorldStereo;

	float4x4 g_worldToCamStereoOther;
	float4x4 g_camToWorldStereoOther;

	uint g_stereoEnabled;
	float g_SensorDepthWorldMin;
	float g_SensorDepthWorldMax;
	uint g_dummy02Glob;
};

#endif
 #ifndef _SDF_SHADER_BUFFER_H_
#define _SDF_SHADER_BUFFER_H_

cbuffer cbCS : register( b0 )
{
	float4x4	g_RigidTransform;
	float4x4	g_RigidTransformInverse;
	uint		g_HashNumBuckets;
	uint		g_HashBucketSize;
	uint		g_ImageWidth;
	uint		g_ImageHeight;
	float		g_VirtualVoxelSize;
	float		g_VirtualVoxelResolutionScalar;
	uint		g_NumSDFBlocks;
	uint		g_NumOccupiedSDFBlocks;
};

cbuffer cbCS : register( b1 )
{
	float4x4	g_Other_RigidTransform;
	float4x4	g_Other_RigidTransformInverse;
	uint		g_Other_HashNumBuckets;
	uint		g_Other_HashBucketSize;
	uint		g_Other_ImageWidth;
	uint		g_Other_ImageHeight;
	float		g_Other_VirtualVoxelSize;
	float		g_Other_VirtualVoxelResolutionScalar;
	uint		g_Other_NumSDFBlocks;
	uint		g_Other_NumOccupiedSDFBlocks;
};


#endif
 #ifndef _KINECTCAMERA_UTIL_H_
#define _KINECTCAMERA_UTIL_H_

//#ifndef NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240
//#define NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 (285.63f)
//#endif

//#ifndef NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240
//#define NUI_CAMERA_DEPTH_IMAGE_TO_SKELETON_MULTIPLIER_320x240 (3.501e-3f)
//#endif

//#define DEPTH_WORLD_MIN 0.4f
//#define DEPTH_WORLD_MAX 12.0f
//set by buffer
#define DEPTH_WORLD_MIN g_SensorDepthWorldMin
#define DEPTH_WORLD_MAX g_SensorDepthWorldMax


///////////////////////////////////////////////////////////////
// Camera to Screen
///////////////////////////////////////////////////////////////

float2 cameraToKinectScreenFloat(float3 pos)
{
	//float4 p = float4(pos.x, pos.y, 0.0f, pos.z);
	//float4x4 projMat = g_intrinsics;
	//if(g_stereoEnabled) projMat = g_intrinsicsStereo;

	//float4 proj = mul(p, projMat);
	//return float2(proj.x/proj.w, proj.y/proj.w);

	//undistort
	float fx = g_intrinsicsCoeff[0][0];
	float fy = g_intrinsicsCoeff[1][0];
	float mx = g_intrinsicsCoeff[2][0];
	float my = g_intrinsicsCoeff[3][0];
	float k1 = g_intrinsicsCoeff[0][1];
	float k2 = g_intrinsicsCoeff[1][1];
	float k3 = g_intrinsicsCoeff[2][1];
	float p1 = g_intrinsicsCoeff[3][1];
	float p2 = g_intrinsicsCoeff[0][2];

	//float2 p = float2(pos.x/pos.z, pos.y/pos.z);
	//float r2 = p.x*p.x + p.y*p.y;

	//float2 pos2;
	//pos2.x = p.x * (1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2) + 2.0f*p1*p.x*p.y + p2*(r2*2.0f*p.x*p.x);
	//pos2.y = p.y * (1 + k1*r2 + k2*r2*r2 + k3*r2*r2*r2) + 2.0f*p2*p.x*p.y + p1*(r2*2.0f*p.y*p.y);
	//
	//pos2.x = pos2.x*fx + mx;
	//pos2.y = my - pos2.y*fy;

	//return pos2;
	return float2(pos.x*fx/pos.z + mx, my - pos.y*fy/pos.z);
}

int2 cameraToKinectScreenInt(float3 pos)
{
	float2 pImage = cameraToKinectScreenFloat(pos);
	return int2(pImage + float2(0.5f, 0.5f));
}

uint2 cameraToKinectScreen(float3 pos)
{
    int2 p = cameraToKinectScreenInt(pos);

	return uint2(p.xy);
}

float cameraToKinectProjZ(float z)
{
	return (z - DEPTH_WORLD_MIN)/(DEPTH_WORLD_MAX - DEPTH_WORLD_MIN);
}

float kinectProjZToCamera(float z)
{
	return DEPTH_WORLD_MIN+z*(DEPTH_WORLD_MAX - DEPTH_WORLD_MIN);
}

float3 cameraToKinectProj(float3 pos)
{
	float2 proj = cameraToKinectScreenFloat(pos);

    float3 pImage = float3(proj.x, proj.y, pos.z);

	pImage.x = (2.0f*pImage.x - (g_ImageWidth-1.0f))/(g_ImageWidth-1.0f);
	pImage.y = ((g_ImageHeight-1.0f) - 2.0f*pImage.y)/(g_ImageHeight-1.0f);
	pImage.z = cameraToKinectProjZ(pImage.z);

	return pImage;
}

///////////////////////////////////////////////////////////////
// Screen to Camera (depth in meters)
///////////////////////////////////////////////////////////////

float3 kinectDepthToSkeleton(uint ux, uint uy, float depth)
{
	//float4x4 projInv = g_intrinsicsInv;
	//if(g_stereoEnabled) projInv = g_intrinsicsInvStereo;

	//float4 camera = mul(float4((float)ux*depth, (float)uy*depth, 0.0f, depth), projInv);
	//
	//return float3(camera.x, camera.y, camera.w);
	
	//undistort
	float fx = g_intrinsicsCoeff[0][0];
	float fy = g_intrinsicsCoeff[1][0];
	float mx = g_intrinsicsCoeff[2][0];
	float my = g_intrinsicsCoeff[3][0];
	float k1 = g_intrinsicsCoeff[0][1];
	float k2 = g_intrinsicsCoeff[1][1];
	float k3 = g_intrinsicsCoeff[2][1];
	float p1 = g_intrinsicsCoeff[3][1];
	float p2 = g_intrinsicsCoeff[0][2];

	const float x = ((float)ux-mx) / fx;
	const float y = (my-(float)uy) / fy;

	//float r2 = x*x + y*y;
	//float2 pos2;
	//pos2.x = x * (1 + k1 * r2 + k2 * r2*r2 + k3 * r2*r2*r2) + 2.0f * p1 * x * y + p2 * (r2 * 2.0f * x*x);
	//pos2.y = y * (1 + k1 * r2 + k2 * r2*r2 + k3 * r2*r2*r2) + 2.0f * p2 * x * y + p1 * (r2 * 2.0f * y*y); 

	//return float3(pos2.x*depth, pos2.y*depth, depth);
	return float3(depth*x, depth*y, depth);
}

///////////////////////////////////////////////////////////////
// RenderScreen to Camera -- ATTENTION ASSUMES [1,0]-Z range!!!!
///////////////////////////////////////////////////////////////

float kinectProjToCameraZ(float z)
{
	return z * (DEPTH_WORLD_MAX - DEPTH_WORLD_MIN) + DEPTH_WORLD_MIN;
}

// z has to be in [0, 1]
float3 kinectProjToCamera(uint ux, uint uy, float z)
{
	float fSkeletonZ = kinectProjToCameraZ(z);
	return kinectDepthToSkeleton(ux, uy, fSkeletonZ);
}

#endif
 #ifndef _VOXEL_UTIL_HASH_SDF_H_
#define _VOXEL_UTIL_HASH_SDF_H_

// should be set by application
#ifndef groupthreads
#define groupthreads 1
#endif

//// should be set by application
//#ifndef HASH_BUCKET_SIZE
//#define HASH_BUCKET_SIZE 10
//#endif

#ifndef MINF
#define MINF asfloat(0xff800000)
#endif

#ifndef PINF
#define PINF asfloat(0x7f800000)
#endif

//status flags for hash entries
static const int LOCK_ENTRY = -1;
static const int FREE_ENTRY = -2;
static const int NO_OFFSET = 0;

//static const uint	g_WeightSample = 3;			//weight per sample (per integration step)
//static const uint	g_WeightMax = 255;			//maximum weight per voxel
//static const float	g_Truncation = 0.05f;		//5cm world space
//static const float	g_TruncScale = 0.02f;		//how to adapt the truncation: per distance meter, increase truncation by 1 cm

//#define HANDLE_COLLISIONS 1

struct HashEntry
{
	int3 pos;	//hash position (lower left corner of SDFBlock))
	int offset;	//offset for collisions
	int ptr;	//pointer into heap to SDFBlock
};

struct Voxel
{
	float sdf;
	uint3 color;
	uint weight;
	int label[12];
};

//! computes the (local) virtual voxel pos of an index; idx in [0;511]
int3 delinearizeVoxelIndex(uint idx)
{
	uint x = idx % SDF_BLOCK_SIZE;
	uint y = (idx % (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE)) / SDF_BLOCK_SIZE;
	uint z = idx / (SDF_BLOCK_SIZE * SDF_BLOCK_SIZE);	
	return uint3(x,y,z);
}

//! computes the linearized index of a local virtual voxel pos; pos in [0;7]^3
uint linearizeVoxelPos(int3 virtualVoxelPos)
{
	return  virtualVoxelPos.z * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE +
			virtualVoxelPos.y * SDF_BLOCK_SIZE +
			virtualVoxelPos.x;
}

HashEntry getHashEntry(Buffer<int> hash, in uint id)
{
	HashEntry entry;
	int i0 = hash[3*id+0];
	entry.pos.x = i0 & 0x0000ffff;
	if (entry.pos.x & (0x1 << 15))	entry.pos.x |= 0xffff0000;
	entry.pos.y = i0 >> 16;
	if (entry.pos.y & (0x1 << 15))	entry.pos.y |= 0xffff0000;

	int i1 = hash[3*id+1];
	entry.pos.z = i1 & 0x0000ffff;
	if (entry.pos.z & (0x1 << 15))	entry.pos.z |= 0xffff0000;

	entry.offset = i1 >> 16;
	if (entry.offset & (0x1 << 15)) entry.offset |= 0xffff0000;
	
	entry.ptr = hash[3*id+2];
	return entry;
}

HashEntry getHashEntry(RWBuffer<int> hash, in uint id)
{
	HashEntry entry;
	int i0 = hash[3*id+0];
	entry.pos.x = i0 & 0x0000ffff;
	if (entry.pos.x & (0x1 << 15))	entry.pos.x |= 0xffff0000;
	entry.pos.y = i0 >> 16;
	if (entry.pos.y & (0x1 << 15))	entry.pos.y |= 0xffff0000;

	int i1 = hash[3*id+1];
	entry.pos.z = i1 & 0x0000ffff;
	if (entry.pos.z & (0x1 << 15))	entry.pos.z |= 0xffff0000;

	entry.offset = i1 >> 16;
	if (entry.offset & (0x1 << 15)) entry.offset |= 0xffff0000;

	entry.ptr = hash[3*id+2];
	return entry;
}

void setHashEntry(RWBuffer<int> hash, in uint id, const in HashEntry entry)
{
	hash[3*id+0] = (entry.pos.y << 16) | (entry.pos.x & 0x0000ffff);
	hash[3*id+1] = (entry.offset << 16) | (entry.pos.z & 0x0000ffff);
	hash[3*id+2] = entry.ptr;
}

void deleteHashEntry(RWBuffer<int> hash, in uint id) 
{
	hash[3*id+0] = 0;
	hash[3*id+1] = 0;
	hash[3*id+2] = FREE_ENTRY;
}

Voxel getVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW, RWBuffer<int> sdfBlocksLabel_1 , RWBuffer<int> sdfBlocksLabel_2, RWBuffer<int> sdfBlocksLabel_3, in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];
	int last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;                                                                                                                                                                                                                                                                                                        
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	//voxel.color.z = last;
	
	//Label have 32 bits.
	int last2 = sdfBlocksLabel_1[id]   ;
	voxel.label[0] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[1] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[2] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[3] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_2[id];
	voxel.label[4] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[5] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[6] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[7] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_3[id];
	voxel.label[8] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[9] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[10] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[11] = last2 & 0x000000ff;
	last2 >>= 0x8 ;   

	//last2 = sdfBlocksLabel_4[id];
	//voxel.label[12] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    
	//voxel.label[13] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    
	//voxel.label[14] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    
	//voxel.label[15] = last2 & 0x000000ff;
	//last2 >>= 0x8 ;    

	return voxel;
}

Voxel getVoxel(Buffer<float> sdfBlocksSDF, Buffer<int> sdfBlocksRGBW, Buffer<int> sdfBlocksLabel_1, Buffer<int> sdfBlocksLabel_2, Buffer<int> sdfBlocksLabel_3,in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];
	int last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;                                                                                                                                                                                                                                                                                                        
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	//voxel.color.z = last;
	
	//Label have 32 bits.
	int last2 = sdfBlocksLabel_1[id];
	voxel.label[0] = last2 & 0x000000ff;
	last2 >>= 0x8;
	voxel.label[1] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[2] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[3] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_2[id];
	voxel.label[4] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[5] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[6] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[7] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[8] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    

	last2 = sdfBlocksLabel_3[id];
	voxel.label[9] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[10] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[11] = last2 & 0x000000ff;
	last2 >>= 0x8 ;   

/*	last2 = sdfBlocksLabel_4[id];
	voxel.label[12] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[13] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[14] = last2 & 0x000000ff;
	last2 >>= 0x8 ;    
	voxel.label[15] = last2 & 0x000000ff;
	last2 >>= 0x8 ;   */ 

	return voxel;
}

Voxel getVoxel_onlySDF_RGBW(Buffer<float> sdfBlocksSDF, Buffer<int> sdfBlocksRGBW , in uint id)
{
	Voxel voxel;
	voxel.sdf = sdfBlocksSDF[id];
	int last = sdfBlocksRGBW[id];
	voxel.weight = last & 0x000000ff;
	last >>= 0x8;                                                                                                                                                                                                                                                                                                        
	voxel.color.x = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.y = last & 0x000000ff;
	last >>= 0x8;
	voxel.color.z = last & 0x000000ff;
	//voxel.color.z = last;
	for(int i=0;i<12;i++)
	{
		voxel.label[i]=0;
	}

	return voxel;
}


void setVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW,  RWBuffer<int>  sdfBlocksLabel_1,RWBuffer<int>  sdfBlocksLabel_2,RWBuffer<int>  sdfBlocksLabel_3,in uint id, in Voxel voxel)
{
	sdfBlocksSDF[id] = voxel.sdf;
	int last = 0;
	last |= voxel.color.z & 0x000000ff;
	last <<= 8;
	last |= voxel.color.y & 0x000000ff;
	last <<= 8;
	last |= voxel.color.x & 0x000000ff;
	last <<= 8;
	last |= voxel.weight & 0x000000ff;
	//last |= voxel.color.z;
	//last <<= 8;
	//last |= voxel.color.y;
	//last <<= 8;
	//last |= voxel.color.x;
	//last <<= 8;
	//last |= voxel.weight;
	sdfBlocksRGBW[id] = last;    

	//label's set.32bit.
	

	last |= (uint)voxel.label[0] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[1] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[2] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[3] & 0x000000ff;	
	sdfBlocksLabel_1[id] = last;   

	last |= (uint)voxel.label[4] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[5] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[6] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[7] & 0x000000ff;	
	sdfBlocksLabel_2[id] = last;   

	last |= (uint)voxel.label[8] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[9] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[10] & 0x000000ff;
	last <<= 8;
	last |= (uint)voxel.label[11] & 0x000000ff;	
	sdfBlocksLabel_3[id] = last;   

	//last |= (uint)voxel.label[12] & 0x000000ff;
	//last <<= 8;
	//last |= (uint)voxel.label[13] & 0x000000ff;
	//last <<= 8;
	//last |= (uint)voxel.label[14] & 0x000000ff;
	//last <<= 8;
	//last |= (uint)voxel.label[15] & 0x000000ff;	
	//sdfBlocksLabel_4[id] = last;   
}

void starveVoxel(RWBuffer<int> sdfBlocksRGBW,in uint id) {
	int last = sdfBlocksRGBW[id];
	int weight = last & 0x000000ff;
	weight = max(0, weight-1);	
	last &= 0xffffff00;
	last |= weight;
	sdfBlocksRGBW[id] = last;
}

void deleteVoxel(RWBuffer<float> sdfBlocksSDF, RWBuffer<int> sdfBlocksRGBW,  RWBuffer<int>  sdfBlocksLabel_1,RWBuffer<int>  sdfBlocksLabel_2,RWBuffer<int>  sdfBlocksLabel_3,in uint id) {
	sdfBlocksSDF[id] = 0;
	sdfBlocksRGBW[id] = 0;
	sdfBlocksLabel_1[id] = 0;
	sdfBlocksLabel_2[id] = 0;
	sdfBlocksLabel_3[id] = 0;
	//sdfBlocksLabel_4[id] = 0;
}


float3 worldToVirtualVoxelPosFloat(in float3 pos)
{
	return pos*g_VirtualVoxelResolutionScalar;
}

int3 worldToVirtualVoxelPos(in float3 pos)
{
	const float3 p = pos*g_VirtualVoxelResolutionScalar;
	return (int3)(p+sign(p)*0.5f);
}

int3 virtualVoxelPosToSDFBlock(int3 virtualVoxelPos)
{
	if (virtualVoxelPos.x < 0) virtualVoxelPos.x -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.y < 0) virtualVoxelPos.y -= SDF_BLOCK_SIZE-1;
	if (virtualVoxelPos.z < 0) virtualVoxelPos.z -= SDF_BLOCK_SIZE-1;

	return virtualVoxelPos/SDF_BLOCK_SIZE;
}

// Computes virtual voxel position of corner sample position
int3 SDFBlockToVirtualVoxelPos(int3 sdfBlock)
{
	return sdfBlock*SDF_BLOCK_SIZE;
}

float3 virtualVoxelPosToWorld(in int3 pos)
{   
	return float3(pos)*g_VirtualVoxelSize;
}

float3 SDFBlockToWorld(int3 sdfBlock)
{
	return virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock));
}

int3 worldToSDFBlock(float3 worldPos)
{
	const int3 virtualVoxelPos = worldToVirtualVoxelPos(worldPos);
	return virtualVoxelPosToSDFBlock(virtualVoxelPos);
}

int virtualVoxelPosToLocalSDFBlockIndex(int3 virtualVoxelPos)
{
	int3 localVoxelPos = virtualVoxelPos%SDF_BLOCK_SIZE;

	if (localVoxelPos.x < 0) localVoxelPos.x += SDF_BLOCK_SIZE;
	if (localVoxelPos.y < 0) localVoxelPos.y += SDF_BLOCK_SIZE;
	if (localVoxelPos.z < 0) localVoxelPos.z += SDF_BLOCK_SIZE;

	return linearizeVoxelPos(localVoxelPos);
}

int worldToLocalSDFBlockIndex(float3 world)
{
	int3 virtualVoxelPos = worldToVirtualVoxelPos(world);
	return virtualVoxelPosToLocalSDFBlockIndex(virtualVoxelPos);
}

//! see teschner et al. (but with correct prime values)
uint computeHashPos(in int3 virtualVoxelPos)
{ 
	const int p0 = 73856093;
	const int p1 = 19349669;
	const int p2 = 83492791;

	int res = ((virtualVoxelPos.x * p0) ^ (virtualVoxelPos.y * p1) ^ (virtualVoxelPos.z * p2)) % g_HashNumBuckets;
	if (res < 0) res += g_HashNumBuckets;
	return (uint)res;
}

//! see teschner et al. (but with correct prime values)
uint computeHashPosOther(in int3 virtualVoxelPos, in uint hashNumBuckets)
{ 
	const int p0 = 73856093;
	const int p1 = 19349669;
	const int p2 = 83492791;

	int res = ((virtualVoxelPos.x * p0) ^ (virtualVoxelPos.y * p1) ^ (virtualVoxelPos.z * p2)) % hashNumBuckets;
	if (res < 0) res += hashNumBuckets;
	return (uint)res;
}

//merges two voxels (v0 is the input voxel, v1 the currently stored voxel)
Voxel combineVoxel(in Voxel v0, in Voxel v1)
{
	Voxel v;
	//v.color = (10*v0.weight * v0.color + v1.weight * v1.color)/(10*v0.weight + v1.weight);	//give the currently observed color more weight
	//v.color = (v0.weight * v0.color + v1.weight * v1.color)/(v0.weight + v1.weight);
	v.color = 0.5f * (v0.color + v1.color);	//exponential running average

	v.label[0] = 0.5f * (v0.label[0] + v1.label[0]); 
	v.label[1] = 0.5f * (v0.label[1] + v1.label[1]); 
	v.label[2] = 0.5f * (v0.label[2] + v1.label[2]); 
	v.label[3] = 0.5f * (v0.label[3] + v1.label[3]); 
	v.label[4] = 0.5f * (v0.label[4] + v1.label[4]); 
	v.label[5] = 0.5f * (v0.label[5] + v1.label[5]); 
	v.label[6] = 0.5f * (v0.label[6] + v1.label[6]); 
	v.label[7] = 0.5f * (v0.label[7] + v1.label[7]); 
	v.label[8] = 0.5f * (v0.label[6] + v1.label[8]); 
	v.label[9] = 0.5f * (v0.label[9] + v1.label[9]); 
	v.label[10] = 0.5f * (v0.label[10] + v1.label[10]); 
	v.label[11] = 0.5f * (v0.label[11] + v1.label[11]); 
	/*v.label[12] = 0.5f * (v0.label[12] + v1.label[12]); 
	v.label[13] = 0.5f * (v0.label[13] + v1.label[13]); 
	v.label[14] = 0.5f * (v0.label[14] + v1.label[14]); 
	v.label[15] = 0.5f * (v0.label[15] + v1.label[15]); */



	v.sdf = (v0.sdf * v0.weight + v1.sdf * v1.weight) / (v0.weight + v1.weight);
	v.weight = min(g_WeightMax, v0.weight + v1.weight);
	return v;
}

//! returns the hash entry for a given sdf block id; if there was no hash entry the returned entry will have a ptr with FREE_ENTRY set
HashEntry getHashEntryForSDFBlockPos(Buffer<int> hash, int3 sdfBlock) 
{
	uint h = computeHashPos(sdfBlock);	//hash bucket
	uint hp = h * g_HashBucketSize;		//hash position

	HashEntry entry;
	entry.pos = sdfBlock;
	entry.offset = 0;
	entry.ptr = FREE_ENTRY;

	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++) {
		uint i = j + hp;
		HashEntry curr = getHashEntry(hash, i);
		if (curr.pos.x == entry.pos.x && curr.pos.y == entry.pos.y && curr.pos.z == entry.pos.z && curr.ptr != FREE_ENTRY) {
			return curr;
		}
	}

	#ifdef HANDLE_COLLISIONS
		const uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;
		int i = idxLastEntryInBucket;	//start with the last entry of the current bucket
		HashEntry curr;
		//traverse list until end: memorize idx at list end and memorize offset from last element of bucket to list end
		int k = 0;

		unsigned int maxIter = 0;
		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount) {
			curr = getHashEntry(hash, i);
			
			if (curr.pos.x == entry.pos.x && curr.pos.y == entry.pos.y && curr.pos.z == entry.pos.z && curr.ptr != FREE_ENTRY) {
				return curr;
			}

			if (curr.offset == 0) {	//we have found the end of the list
				break;
			}
			i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
			i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

			maxIter++;
		}
	#endif
	return entry;
}

//! returns the hash entry for a given worldPos; if there was no hash entry the returned entry will have a ptr with FREE_ENTRY set
HashEntry getHashEntry(Buffer<int> hash, float3 worldPos) 
{
	//int3 blockID = worldToSDFVirtualVoxelPos(worldPos)/SDF_BLOCK_SIZE;	//position of sdf block
	int3 blockID = worldToSDFBlock(worldPos);
	return getHashEntryForSDFBlockPos(hash, blockID);
}

//! returns the truncation of the SDF for a given distance value
float getTruncation(float z) {
	return g_Truncation + g_TruncScale * z;
}

bool isInCameraFrustum(in float3 pos) {
	float3 pCamera = mul(float4(pos, 1.0f), g_RigidTransformInverse).xyz;
	float3 pProj = cameraToKinectProj(pCamera);
	//pProj *= 0.75f;	//TODO THIS IS A HACK FIX IT :)
	return !(pProj.x < -1.0f || pProj.x > 1.0f || pProj.y < -1.0f || pProj.y > 1.0f || pProj.z < 0.0f || pProj.z > 1.0f);  
}

bool isInCameraFrustumApprox(in float3 pos) {
	float3 pCamera = mul(float4(pos, 1.0f), g_RigidTransformInverse).xyz;
	float3 pProj = cameraToKinectProj(pCamera);
	//pProj *= 1.5f;	//TODO THIS IS A HACK FIX IT :)
	pProj *= 0.95;
	return !(pProj.x < -1.0f || pProj.x > 1.0f || pProj.y < -1.0f || pProj.y > 1.0f || pProj.z < 0.0f || pProj.z > 1.0f);  
}

bool isSDFBlockInCameraFrustumApprox(in int3 sdfBlock) {
	float3 posWorld = virtualVoxelPosToWorld(SDFBlockToVirtualVoxelPos(sdfBlock)) + g_VirtualVoxelSize * 0.5f * (SDF_BLOCK_SIZE - 1.0f);
	return isInCameraFrustumApprox(posWorld);
}





 

//! deletes a hash entry position for a given sdfBlock index (returns true uppon successful deletion; otherwise returns false)
bool deleteHashEntryElement(RWBuffer<int> hash, AppendStructuredBuffer<uint> heapAppend, RWBuffer<uint> bucketMutex, int3 sdfBlock) {
	uint h = computeHashPos(sdfBlock);	//hash bucket
	uint hp = h * g_HashBucketSize;		//hash position

	[allow_uav_condition]
	for (uint j = 0; j < g_HashBucketSize; j++) {
		uint i = j + hp;
		HashEntry curr = getHashEntry(hash, i);
		if (curr.pos.x == sdfBlock.x && curr.pos.y == sdfBlock.y && curr.pos.z == sdfBlock.z && curr.ptr != FREE_ENTRY) {
			#ifndef HANDLE_COLLISIONS
			const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
			heapAppend.Append(curr.ptr / linBlockSize);
			deleteHashEntry(hash, i);
			return true;
			#endif
			#ifdef HANDLE_COLLISIONS
			if (curr.offset != 0) {	//if there was a pointer set it to the next list element
				int prevValue = 0;
				InterlockedExchange(bucketMutex[h], LOCK_ENTRY, prevValue);	//lock the hash bucket
				if (prevValue == LOCK_ENTRY)	return false;
				if (prevValue != LOCK_ENTRY) {
					const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
					heapAppend.Append(curr.ptr / linBlockSize);
					int nextIdx = (i + curr.offset) % (g_HashBucketSize * g_HashNumBuckets);
					setHashEntry(hash, i, getHashEntry(hash, nextIdx));
					deleteHashEntry(hash, nextIdx);
					return true;
				}
			} else {
				const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
				heapAppend.Append(curr.ptr / linBlockSize);
				deleteHashEntry(hash, i);
				return true;
			}
			#endif
		}
	}	
	#ifdef HANDLE_COLLISIONS
		const uint idxLastEntryInBucket = (h+1)*g_HashBucketSize - 1;
		int i = idxLastEntryInBucket;
		int offset = 0;
		HashEntry curr;
		curr = getHashEntry(hash, i);
		int prevIdx = i;
		i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
		i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

		unsigned int maxIter = 0;
		[allow_uav_condition]
		while (true && maxIter < g_MaxLoopIterCount) {
			curr = getHashEntry(hash, i);
			//found that dude that we need/want to delete
			if (curr.pos.x == sdfBlock.x && curr.pos.y == sdfBlock.y && curr.pos.z == sdfBlock.z && curr.ptr != FREE_ENTRY) {
				int prevValue = 0;
				InterlockedExchange(bucketMutex[h], LOCK_ENTRY, prevValue);	//lock the hash bucket
				if (prevValue == LOCK_ENTRY)	return false;
				if (prevValue != LOCK_ENTRY) {
					const uint linBlockSize = SDF_BLOCK_SIZE * SDF_BLOCK_SIZE * SDF_BLOCK_SIZE;
					heapAppend.Append(curr.ptr / linBlockSize);
					deleteHashEntry(hash, i);
					HashEntry prev = getHashEntry(hash, prevIdx);				
					prev.offset = curr.offset;
					setHashEntry(hash, prevIdx, prev);
					return true;
				}
			}

			if (curr.offset == 0) {	//we have found the end of the list
				return false;	//should actually never happen because we need to find that guy before
			}
			prevIdx = i;
			i = idxLastEntryInBucket + curr.offset;		//go to next element in the list
			i %= (g_HashBucketSize * g_HashNumBuckets);	//check for overflow

			maxIter++;
		}
	#endif
	return false;
}










#endif
    b   9                          M   
  "  Ш   Ї   №   у      %              L   :  R  j  u(                                                                                                                                                                                                                                                                                                                                                                               D3DSHDR 8                             `                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        €€€€	/с(     ±      }                  M                                                                                                                                                                               @                                                                                                                                                                                                                                                                                                                                                                $   0                                                                                                                                                                                                                                                                                                                                                                                                                                                    %    Ў    PS_Write  2 Q	   €€€€ €€€€g_FragmentPrefixSumBufferSRV    . Q   €€€€€€€€  g_FragmentCountBufferUAV    2 Q   €€€€€€€€ g_FragmentSortedDepthBufferUAV  & Q    А €€€€€€g_RenderTargetWidth                                                                                                                                                                                                                                                                                                                          ≠Ї    р≠Ї€€€€	/с                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            Ф.1ФY   т&ЏЅ±Gљ£Рtґо(“  /LinkInfo /names /src/headerblock /src/files/e:\voxelhashing-master\depthsensing\shaders\raymarchingstepssplatting.hlsl /src/files/globalappstateshaderbuffer.h.hlsl /src/files/sdfshaderbuffer.h.hlsl /src/files/kinectcamerautil.h.hlsl /src/files/voxelutilhashsdf.h.hlsl          µ&      к      "      x   	   
                   «      •   
       AС2                                                                                                                       €€€€w	1     Л Ј√   P       ,   –                                    8     `              $      T    @            PS_Write none   -Ї.с ≠Ї    8     `  ≠Ї                €€€€    8        €€€€    €€€€         K   m   Д   Ь   E:\VoxelHashing-master\DepthSensing\Shaders\RayMarchingStepsSplatting.hlsl GlobalAppStateShaderBuffer.h.hlsl SDFShaderBuffer.h.hlsl KinectCameraUtil.h.hlsl VoxelUtilHashSDF.h.hlsl юпюп                  €€€€€€€€€€ €€€€€€€€€€                                            Й  <  џ  8       ЪЖ  0  ч    х  Ґ  M  Р  H       (   P  ,   Ў      Ы   R   Ь   S   M      T   U   V   W   X   Y   Z   [   \   ]   ^   _   `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~      А   Б   В   Г   Д   Е   Ж   З   И   Й   К   Л   М   Н   О   П   Р   С   Т   У   Ф   Х   N      	   
                                                                      !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?   @   A   B   C   D   E   F   G   H   I   J   K   L   O   P   Q      Ц   Ч   Ш   Ъ   Щ                                                                                                                                                                                                                                                                                                                                               Э   Ю                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           