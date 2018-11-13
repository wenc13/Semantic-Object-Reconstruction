Buffer<int>	     g_Hash			  : register(t0);
Buffer<float>	 g_SDFBlocksSDF	  : register(t1);
Texture2D<float> g_RayIntervalMin : register(t2);
Texture2D<float> g_RayIntervalMax : register(t3);
Buffer<int>		 g_SDFBlocksRGBW  : register(t4);

Buffer<int>		g_FragmentPrefixSumBufferSRV   : register(t5);
Buffer<float>	g_FragmentSortedDepthBufferSRV : register(t6);
Buffer<int>     g_SDFBlocksLabel_1	           : register(t7);
Buffer<int>     g_SDFBlocksLabel_2	           : register(t8);
Buffer<int>     g_SDFBlocksLabel_3             : register(t9);
Buffer<int>     g_SDFBlocksLabelFrequency      : register(t10);
 
#include "GlobalAppStateShaderBuffer.h.hlsl"
#include "SDFShaderBuffer.h.hlsl"
#include "KinectCameraUtil.h.hlsl"
#include "VoxelUtilHashSDF.h.hlsl"
#include "RayCastingUtilHashSDF.h.hlsl"
      
RWTexture2D<float>  g_output        : register(u0);
RWTexture2D<float4> g_outputColor   : register(u1);
RWTexture2D<float4> g_outputNormals : register(u2);

RWTexture2D<float4> g_outputLabel_1 : register(u3);
RWTexture2D<float4> g_outputLabel_2 : register(u4);
RWTexture2D<float4> g_outputLabel_3 : register(u5);
  
cbuffer cbConstant : register(b1)
{ 
	float4x4	g_ViewMat;
	float4x4	g_ViewMatInverse;
	uint		g_RenderTargetWidth;
	uint		g_RenderTargetHeight;
	uint		g_splatMinimum;
	uint		g_dummyRayInteveral337;
};

float3 gradientForPoint(float3 pos)
{
    float3 offset = g_VirtualVoxelSize;

    float distp00; float3 colorp00; float4 label_1p00; float4 label_2p00; float4 label_3p00; trilinearInterpolationSimpleFastFast(pos - float3(0.5f * offset.x, 0.0f, 0.0f), distp00, colorp00, label_1p00, label_2p00, label_3p00);
    float dist0p0; float3 color0p0; float4 label_10p0; float4 label_20p0; float4 label_30p0; trilinearInterpolationSimpleFastFast(pos - float3(0.0f, 0.5f * offset.y, 0.0f), dist0p0, color0p0, label_10p0, label_20p0, label_30p0);
    float dist00p; float3 color00p; float4 label_100p; float4 label_200p; float4 label_300p; trilinearInterpolationSimpleFastFast(pos - float3(0.0f, 0.0f, 0.5f * offset.z), dist00p, color00p, label_100p, label_200p, label_300p);

    float dist100; float3 color100; float4 label_1100; float4 label_2100; float4 label_3100; trilinearInterpolationSimpleFastFast(pos + float3(0.5f * offset.x, 0.0f, 0.0f), dist100, color100, label_1100, label_2100, label_3100);
    float dist010; float3 color010; float4 label_1010; float4 label_2010; float4 label_3010; trilinearInterpolationSimpleFastFast(pos + float3(0.0f, 0.5f * offset.y, 0.0f), dist010, color010, label_1010, label_2010, label_3010);
    float dist001; float3 color001; float4 label_1001; float4 label_2001; float4 label_3001; trilinearInterpolationSimpleFastFast(pos + float3(0.0f, 0.0f, 0.5f * offset.z), dist001, color001, label_1001, label_2001, label_3001);

    float3 grad = float3((distp00 - dist100) / offset.x, (dist0p0 - dist010) / offset.y, (dist00p - dist001) / offset.z);

    float l = length(grad);
    if (l == 0.0f)
    {
        return float3(0.0f, 0.0f, 0.0f);
    }

    return -grad / l;
}

/*
void traverseCoarseGridSimpleSampleAllMultiLayer(float3 worldCamPos, float3 worldDir, float3 camDir, int3 dTid)
{
    const float depthToRayLength = 1.0f / camDir.z; // scale factor to convert from depth to ray length
    int index1D = dTid.y * g_RenderTargetWidth + dTid.x;

    int startIndex = 0;
    if (index1D > 0) startIndex = g_FragmentPrefixSumBufferSRV[index1D - 1];
    int endIndex = g_FragmentPrefixSumBufferSRV[index1D] - 1;
    int currentIndex = startIndex;
	
    float rayCurrent = g_SensorDepthWorldMin;

	// Last Sample
    Sample lastSample; lastSample.sdf = 0.0f; lastSample.alpha = 0.0f; lastSample.weight = 0;

	[allow_uav_condition]
    while (currentIndex <= endIndex)
    {
        float rayStart = depthToRayLength * kinectProjZToCamera(g_FragmentSortedDepthBufferSRV[currentIndex]);
        rayCurrent = max(rayStart, rayCurrent);
				
		[allow_uav_condition]
        while (rayCurrent <= rayStart + 1.74f * SDF_BLOCK_SIZE * g_VirtualVoxelSize + rayIncrement) // 1.74f > sqrt(3)
        {
            float3 currentPosWorld = worldCamPos + rayCurrent * worldDir;
            float dist; float3 color; float4 label_1; float4 label_2; float4 label_3;

            if (trilinearInterpolationSimpleFastFast(currentPosWorld, dist, color, label_1, label_2, label_3))
            {
                if (lastSample.weight > 0 && lastSample.sdf > 0.0f && dist < 0.0f) // current sample is always valid here
                {
                    float alpha; // = findIntersectionLinear(lastSample.alpha, rayCurrent, lastSample.sdf, dist);
                    findIntersectionBisection(worldCamPos, worldDir, lastSample.sdf, lastSample.alpha, dist, rayCurrent, alpha);
					
                    float3 currentIso = worldCamPos + alpha * worldDir;
                    if (abs(lastSample.sdf - dist) < g_thresSampleDist)
                    {
                        if (abs(dist) < g_thresDist)
                        {
                            g_output[dTid.xy] = alpha / depthToRayLength; // Convert ray length to depth depthToRayLength
                            g_outputColor[dTid.xy] = float4(color / 255.0f, 1.0f);
                            if (g_useGradients == 1)
                            {
                                float3 normal = gradientForPoint(currentIso);
                                g_outputNormals[dTid.xy] = float4(mul(float4(normal, 0.0f), g_ViewMat).xyz, 1.0f);
                            }

                            return;
                        }
                    }
                }

                lastSample.sdf = dist;
                lastSample.alpha = rayCurrent;
                lastSample.weight = 1;
            }
            else
            {
                lastSample.weight = 0;
            }
			
            rayCurrent += rayIncrement;
        }

        currentIndex++;
    }
}
*/

void traverseCoarseGridSimpleSampleAll(float3 worldCamPos, float3 worldDir, float3 camDir, int3 dTid)
{
    // Last Sample
    Sample lastSample; lastSample.sdf = 0.0f; lastSample.alpha = 0.0f; lastSample.weight = 0;
    const float depthToRayLength = 1.0f/camDir.z; // scale factor to convert from depth to ray length

    float rayCurrent = depthToRayLength * max(g_SensorDepthWorldMin, kinectProjZToCamera(g_RayIntervalMin[dTid.xy])); // Convert depth to raylength
    float rayEnd = depthToRayLength * min(g_SensorDepthWorldMax, kinectProjZToCamera(g_RayIntervalMax[dTid.xy])); // Convert depth to raylength
 
	[allow_uav_condition]
    while (rayCurrent < rayEnd)
    {
        float3 currentPosWorld = worldCamPos + rayCurrent * worldDir;
        HashEntry entry = getHashEntryForSDFBlockPos(g_Hash, worldToSDFBlock(currentPosWorld));
        
        float dist; float3 color; float4 label_1; float4 label_2; float4 label_3;

        if (trilinearInterpolationSimpleFastFast(currentPosWorld, dist, color, label_1, label_2, label_3))
        {
            if (lastSample.weight > 0 && lastSample.sdf > 0.0f && dist < 0.0f) // current sample is always valid here
            {
                float alpha; // = findIntersectionLinear(lastSample.alpha, rayCurrent, lastSample.sdf, dist);
                findIntersectionBisection(worldCamPos, worldDir, lastSample.sdf, lastSample.alpha, dist, rayCurrent, alpha);
					
                float3 currentIso = worldCamPos + alpha * worldDir;
                if (abs(lastSample.sdf - dist) < g_thresSampleDist)
                {
                    if (abs(dist) < g_thresDist)
                    {
                        g_output[dTid.xy] = alpha/depthToRayLength; // Convert ray length to depth depthToRayLength
                        
						int3 virtualVoxelPos = worldToVirtualVoxelPos(currentIso);
						int ptr = getHashEntryForSDFBlockPos(g_Hash, virtualVoxelPosToSDFBlock(virtualVoxelPos)).ptr;
						int linearMemoryIndex = ptr + virtualVoxelPosToLocalSDFBlockIndex(virtualVoxelPos);
						Voxel v = getVoxel(g_SDFBlocksSDF, g_SDFBlocksRGBW, g_SDFBlocksLabel_1, g_SDFBlocksLabel_2, g_SDFBlocksLabel_3, g_SDFBlocksLabelFrequency, linearMemoryIndex);
						color.x   = v.color.x;
						color.y   = v.color.y;
						color.z   = v.color.z;
						label_1.x = v.label[0];
						label_1.y = v.label[1];
						label_1.z = v.label[2];
						label_1.w = v.label[3];
						label_2.x = v.label[4];
						label_2.y = v.label[5];
						label_2.z = v.label[6];
						label_2.w = v.label[7];
						label_3.x = v.label[8];
						label_3.y = v.label[9];
						label_3.z = v.label[10];
						label_3.w = v.label[11];
						
						g_outputColor[dTid.xy]   = float4(color/255.0f, 1.0f);
                        g_outputLabel_1[dTid.xy] = float4(label_1/255.0f);
                        g_outputLabel_2[dTid.xy] = float4(label_2/255.0f);
                        g_outputLabel_3[dTid.xy] = float4(label_3/255.0f);

                        if (g_useGradients == 1)
                        {
                            float3 normal = gradientForPoint(currentIso);
                            g_outputNormals[dTid.xy] = float4(mul(float4(normal, 0.0f), g_ViewMat).xyz, 1.0f);
                        }

                        return;
                    }
                }
            }

            lastSample.sdf = dist;
            lastSample.alpha = rayCurrent;
            lastSample.weight = 1;
            rayCurrent += rayIncrement;
        }
        else
        {
            lastSample.weight = 0;
            rayCurrent += rayIncrement;
        }
    }
}

[numthreads(groupthreads, groupthreads, 1)]
void renderCS(int3 dTid : SV_DispatchThreadID)
{
	g_output[dTid.xy]        = MINF;
    g_outputColor[dTid.xy]   = float4(MINF, MINF, MINF, MINF);
    g_outputLabel_1[dTid.xy] = float4(MINF, MINF, MINF, MINF);
    g_outputLabel_2[dTid.xy] = float4(MINF, MINF, MINF, MINF);
    g_outputLabel_3[dTid.xy] = float4(MINF, MINF, MINF, MINF);
	
	if (dTid.x >= (int)g_RenderTargetWidth || dTid.y >= (int)g_RenderTargetHeight) return;

	float3 camDir = normalize(kinectProjToCamera(dTid.x, dTid.y, 1.0f));

	float3 worldCamPos = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), g_ViewMatInverse).xyz;
	float3 worldDir = normalize(mul(float4(camDir, 0.0f), g_ViewMatInverse).xyz);

	if (g_enableMultiLayerSplatting)
	{
		//traverseCoarseGridSimpleSampleAllMultiLayer(worldCamPos, worldDir, camDir, dTid);
	}
	else
	{
		traverseCoarseGridSimpleSampleAll(worldCamPos, worldDir, camDir, dTid);
	}
}