cbuffer consts : register(b0)
{
	int      g_ImageWidth;
	int      g_ImageHeight;
	
	int      labelNum;
	int      level;
	
	float    distWeight;
	float    normalWeight;
	float    colorWeight;
	float    labelWeight;
	
	float    distThres;
	float    normalThres;
	float    colorThres;
	float    labelThres;
	
	float4x4 transform; // dx style transformation matrix !!!
	
	float    g_levelFactor;
	int      labelNo;
	float    dummy01;
	float    dummy02;
};

Texture2D<float4> target                       : register(t0);
Texture2D<float4> targetNormals                : register(t1);
Texture2D<float4> targetIntensities            : register(t2);
Texture2D<float4> input                        : register(t3);
Texture2D<float4> inputNormals                 : register(t4);
Texture2D<float4> inputIntensities             : register(t5);

Texture2D<float4> targetBackGroundLabel00      : register(t6);
Texture2D<float4> targetBackGroundLabel01      : register(t7);
Texture2D<float4> targetBackGroundLabel02      : register(t8);
Texture2D<float4> targetBackGroundLabel03      : register(t9);
Texture2D<float4> targetBackGroundLabel04      : register(t10);
Texture2D<float4> targetBackGroundLabel05      : register(t11);
Texture2D<float4> targetBackGroundLabel06      : register(t12);
Texture2D<float4> targetBackGroundLabel07      : register(t13);
Texture2D<float4> targetBackGroundLabel08      : register(t14);
Texture2D<float4> targetBackGroundLabel09      : register(t15);
Texture2D<float4> targetBackGroundLabel10      : register(t16);
Texture2D<float4> targetBackGroundLabel11      : register(t17);

Texture2D<float4> inputBackGroundLabel00       : register(t18);
Texture2D<float4> inputBackGroundLabel01       : register(t19);
Texture2D<float4> inputBackGroundLabel02       : register(t20);
Texture2D<float4> inputBackGroundLabel03       : register(t21);
Texture2D<float4> inputBackGroundLabel04       : register(t22);
Texture2D<float4> inputBackGroundLabel05       : register(t23);
Texture2D<float4> inputBackGroundLabel06       : register(t24);
Texture2D<float4> inputBackGroundLabel07       : register(t25);
Texture2D<float4> inputBackGroundLabel08       : register(t26);
Texture2D<float4> inputBackGroundLabel09       : register(t27);
Texture2D<float4> inputBackGroundLabel10       : register(t28);
Texture2D<float4> inputBackGroundLabel11       : register(t29);

RWTexture2D<float4> output                     : register(u0);
RWTexture2D<float4> outputNormals              : register(u1);

// should be set by application
#ifndef groupthreads
#define groupthreads 1
#endif

//#ifndef NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240
//#define NUI_CAMERA_SKELETON_TO_DEPTH_IMAGE_MULTIPLIER_320x240 (285.63f)
//#endif

//#ifndef FLT_EPSILON
//#define FLT_EPSILON 1.192092896e-07f
//#endif

#define MINF asfloat(0xff800000)
#define MAXF asfloat(0x7F7FFFFF)

#include "GlobalAppStateShaderBuffer.h.hlsl"
#include "KinectCameraUtil.h.hlsl"
#include "Util.h.hlsl"

void getBestCorrespondence(uint2 screenPos, out float4 pTarget, out float4 nTarget)
{
	pTarget = target[int2(screenPos.x, screenPos.y)];
	nTarget = targetNormals[int2(screenPos.x, screenPos.y)];
}

[numthreads(groupthreads, groupthreads, 1)]
void projectiveCorrespondencesCS(int3 dTid : SV_DispatchThreadID)
{
	output[dTid.xy]                     = float4(MINF, MINF, MINF, MINF);
	outputNormals[dTid.xy]              = float4(MINF, MINF, MINF, MINF);

	float4 pInput = input[dTid.xy];
	float4 nInput = inputNormals[dTid.xy];
	float4 iInput = inputIntensities[dTid.xy];

    if (isValid(pInput) && isValid(nInput) && isValid(iInput))
	{
		pInput.w = 1.0f;    // assert it is a point
		float4 pTransInput = mul(pInput, transform);

		nInput.w = 0.0f;    // assert it is a vector
		float4 nTransInput = mul(nInput, transform); // transformation is a rotation M^(-1)^T = M, translation is ignored because it is a vector
		
		int2 screenPos = cameraToKinectScreenInt(pTransInput.xyz) / g_levelFactor;

        float4 pTarget = float4(MINF, MINF, MINF, MINF);
        float4 nTarget = float4(MINF, MINF, MINF, MINF);
            
        if (screenPos.x >= 0 && screenPos.y >= 0 && screenPos.x < g_ImageWidth && screenPos.y < g_ImageHeight)
        {
            getBestCorrespondence(screenPos, pTarget, nTarget);
        }
            
        if (isValid(pTarget) && isValid(nTarget))
        {
            float distance = length(pTransInput.xyz - pTarget.xyz);
            float dNormal = dot(nTransInput.xyz, nTarget.xyz);

            if ((distWeight > 0.0f ? distance <= distThres : 1) &&
				(normalWeight > 0.0f ? dNormal >= normalThres : 1))
            {
                if (labelNo == labelNum)
                {
                    output[dTid.xy] = pTarget;
				    //nTarget.w = max(0.0, 0.5f*((1.0f-d/distThres)+(1.0f-cameraToKinectProjZ(pTransInput.z)))); // for weighted ICP;
                    nTarget.w = 1.0f;
                    outputNormals[dTid.xy] = nTarget;
                }
                else
                {
                    float labeltarget[12] =
                    {
                        targetBackGroundLabel00[dTid.xy].x, targetBackGroundLabel01[dTid.xy].x, targetBackGroundLabel02[dTid.xy].x, targetBackGroundLabel03[dTid.xy].x,
                        targetBackGroundLabel04[dTid.xy].x, targetBackGroundLabel05[dTid.xy].x, targetBackGroundLabel06[dTid.xy].x, targetBackGroundLabel07[dTid.xy].x,
                        targetBackGroundLabel08[dTid.xy].x, targetBackGroundLabel09[dTid.xy].x, targetBackGroundLabel10[dTid.xy].x, targetBackGroundLabel11[dTid.xy].x
                    };

                    float labelinput[12] =
                    {
                        inputBackGroundLabel00[dTid.xy].x, inputBackGroundLabel01[dTid.xy].x, inputBackGroundLabel02[dTid.xy].x, inputBackGroundLabel03[dTid.xy].x,
                        inputBackGroundLabel04[dTid.xy].x, inputBackGroundLabel05[dTid.xy].x, inputBackGroundLabel06[dTid.xy].x, inputBackGroundLabel07[dTid.xy].x,
                        inputBackGroundLabel08[dTid.xy].x, inputBackGroundLabel09[dTid.xy].x, inputBackGroundLabel10[dTid.xy].x, inputBackGroundLabel11[dTid.xy].x
                    };

                    float targetEntropy = 0.0f;
                    float inputEntropy = 0.0f;

                    for (uint m = 0; m < labelNum; m++)
                    {
                        float pt = labeltarget[m];
                        float pi = labelinput[m];

                        if (pt != 0)
                            targetEntropy -= pt * log(pt);
                        if (pi != 0)
                            inputEntropy -= pi * log(pi);
                    }

                    float targetConfidence = 1 - targetEntropy / log(labelNum);
                    float inputConfidence = 1 - inputEntropy / log(labelNum);

                    uint indextarget = 0;
                    uint indexinput = 0;

                    float maxtarget = 0.0f;
                    float maxinput = 0.0f;

                    for (uint n = 0; n < labelNum; n++)
                    {
                        if (labeltarget[n] > maxtarget)
                        {
                            indextarget = n;
                            maxtarget = labeltarget[n];
                        }

                        if (labelinput[n] > maxinput)
                        {
                            indexinput = n;
                            maxinput = labelinput[n];
                        }
                    }
                    
                    if (labelNo == 0)
                    {
                        if (indexinput != labelNo)
                        {
                            output[dTid.xy] = pTarget;
                            //nTarget.w = max(0.0, 0.5f*((1.0f-d/distThres)+(1.0f-cameraToKinectProjZ(pTransInput.z)))); // for weighted ICP;
                            nTarget.w = 1.0f;
                            outputNormals[dTid.xy] = nTarget;
                        }
                    }
                    else
                    {
                        //if (indexinput == labelNo)
                        if ((indexinput != labelNo) && (indexinput != 0))
                        {
                            output[dTid.xy] = pTarget;
                            //nTarget.w = max(0.0, 0.5f*((1.0f-d/distThres)+(1.0f-cameraToKinectProjZ(pTransInput.z)))); // for weighted ICP;
                            nTarget.w = 1.0f;
                            outputNormals[dTid.xy] = nTarget;
                        }
                    }
                }
            }
        }
	}
}