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

Texture2D<float4> target                    : register(t0);
Texture2D<float4> targetNormals             : register(t1);
Texture2D<float4> targetLabels              : register(t2);
Texture2D<float4> input                     : register(t3);
Texture2D<float4> inputNormals              : register(t4);
Texture2D<float4> inputLabels               : register(t5);
Texture2D<float4> targetLabelGradients      : register(t6);

RWTexture2D<float4> outputLabels            : register(u0);
RWTexture2D<float4> outputLabelGradients    : register(u1);

// test
//RWBuffer<float> outputLabelFloat0 : register(u2);
//RWBuffer<float> outputLabelFloat1 : register(u3);
//RWBuffer<float> outputLabelFloat2 : register(u4);
//RWBuffer<float> outputLabelFloat3 : register(u5);
// test

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

float4 bilinearInterpolation(float x, float y, uint imageWidth, uint imageHeight)
{
    /*
    const int2 p00 = int2(floor(x), floor(y));
    const int2 p01 = p00 + int2(0, 1);
    const int2 p10 = p00 + int2(1, 0);
    const int2 p11 = p00 + int2(1, 1);

    const float alpha = x - p00.x;
    const float beta = y - p00.y;

    float4 s0 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float w0 = 0.0f;
    if (p00.x < imageWidth && p00.y < imageHeight)
    {
        //float4 v00 = d_input[p00.y * imageWidth + p00.x];
        float4 v00 = float4(targetIntensities[p00].x, targetIntensityGradients[p00].x, targetIntensityGradients[p00].y, 1.0f);
        if (v00.x != MINF && v00.y != MINF && v00.z != MINF)
        {
            s0 += (1.0f - alpha) * v00;
            w0 += (1.0f - alpha);
        }
    }
    if (p10.x < imageWidth && p10.y < imageHeight)
    {
        //float4 v10 = d_input[p10.y * imageWidth + p10.x];
        float4 v10 = float4(targetIntensities[p10].x, targetIntensityGradients[p10].x, targetIntensityGradients[p10].y, 1.0f);
        if (v10.x != MINF && v10.y != MINF && v10.z != MINF)
        {
            s0 += alpha * v10;
            w0 += alpha;
        }
    }

    float4 s1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float w1 = 0.0f;
    if (p01.x < imageWidth && p01.y < imageHeight)
    {
        //float4 v01 = d_input[p01.y * imageWidth + p01.x];
        float4 v01 = float4(targetIntensities[p01].x, targetIntensityGradients[p01].x, targetIntensityGradients[p01].y, 1.0f);
        if (v01.x != MINF && v01.y != MINF && v01.z != MINF)
        {
            s1 += (1.0f - alpha) * v01;
            w1 += (1.0f - alpha);
        }
    }
    if (p11.x < imageWidth && p11.y < imageHeight)
    {
        //float4 v11 = d_input[p11.y * imageWidth + p11.x];
        float4 v11 = float4(targetIntensities[p11].x, targetIntensityGradients[p11].x, targetIntensityGradients[p11].y, 1.0f);
        if (v11.x != MINF && v11.y != MINF && v11.z != MINF)
        {
            s1 += alpha * v11;
            w1 += alpha;
        }
    }

    const float4 p0 = s0 / w0;
    const float4 p1 = s1 / w1;

    float4 ss = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float ww = 0.0f;
    if (w0 > 0.0f)
    {
        ss += (1.0f - beta) * p0;
        ww += (1.0f - beta);
    }
    if (w1 > 0.0f)
    {
        ss += beta * p1;
        ww += beta;
    }

    if (ww > 0.0f)
        return ss / ww;
    else
        return float4(MINF, MINF, MINF, MINF);
    */

    float aIntensity = 0.0f, bIntensity = 0.0f, cIntensity = 0.0f, dIntensity = 0.0f;
    float aGradientx = 0.0f, bGradientx = 0.0f, cGradientx = 0.0f, dGradientx = 0.0f;
    float aGradienty = 0.0f, bGradienty = 0.0f, cGradienty = 0.0f, dGradienty = 0.0f;

    float4 result = float4(MINF, MINF, MINF, MINF);

    int2 p; float2 delta;

    p.x = (int) floor(x);
    p.y = (int) floor(y);

    delta.x = x - (float) p.x;
    delta.y = y - (float) p.y;

    aIntensity = targetLabels[int2(p.x, p.y)].x;
    aGradientx = targetLabelGradients[int2(p.x, p.y)].x;
    aGradienty = targetLabelGradients[int2(p.x, p.y)].y;

    if (delta.x != 0)
    {
        bIntensity = targetLabels[int2(p.x+1, p.y)].x;
        bGradientx = targetLabelGradients[int2(p.x+1, p.y)].x;
        bGradienty = targetLabelGradients[int2(p.x+1, p.y)].y;
    }

    if (delta.y != 0)
    {
        cIntensity = targetLabels[int2(p.x, p.y+1)].x;
        cGradientx = targetLabelGradients[int2(p.x, p.y+1)].x;
        cGradienty = targetLabelGradients[int2(p.x, p.y+1)].y;
    }
    
    if (delta.x != 0 && delta.y != 0)
    {
        dIntensity = targetLabels[int2(p.x+1, p.y+1)].x;
        dGradientx = targetLabelGradients[int2(p.x+1, p.y+1)].x;
        dGradienty = targetLabelGradients[int2(p.x+1, p.y+1)].y;
    }

    //if (aIntensity < 0 || bIntensity < 0 || cIntensity < 0 || dIntensity < 0)
    //    //result.x = -1 / 255.0f;
    //    result.x = MINF;
    //else
    //    result.x = ((float) aIntensity * (1.0f - delta.x) * (1.0f - delta.y) + (float) bIntensity * delta.x * (1.0f - delta.y) + (float) cIntensity * (1.0f - delta.x) * delta.y + (float) dIntensity * delta.x * delta.y);

    //if (aGradientx < 0 || bGradientx < 0 || cGradientx < 0 || dGradientx < 0)
    //    //result.y =  -1 / 255.0f;
    //    result.y = MINF;
    //else
    //    result.y = ((float) aGradientx * (1.0f - delta.x) * (1.0f - delta.y) + (float) bGradientx * delta.x * (1.0f - delta.y) + (float) cGradientx * (1.0f - delta.x) * delta.y + (float) dGradientx * delta.x * delta.y);
    
    //if (aGradienty < 0 || bGradienty < 0 || cGradienty < 0 || dGradienty < 0)
    //    //result.z = -1 / 255.0f;
    //    result.z = MINF;
    //else
    //    result.z = ((float) aGradienty * (1.0f - delta.x) * (1.0f - delta.y) + (float) bGradienty * delta.x * (1.0f - delta.y) + (float) cGradienty * (1.0f - delta.x) * delta.y + (float) dGradienty * delta.x * delta.y);

    result.x = ((float) aIntensity * (1.0f - delta.x) * (1.0f - delta.y) + (float) bIntensity * delta.x * (1.0f - delta.y) + (float) cIntensity * (1.0f - delta.x) * delta.y + (float) dIntensity * delta.x * delta.y);
    result.y = ((float) aGradientx * (1.0f - delta.x) * (1.0f - delta.y) + (float) bGradientx * delta.x * (1.0f - delta.y) + (float) cGradientx * (1.0f - delta.x) * delta.y + (float) dGradientx * delta.x * delta.y);
    result.z = ((float) aGradienty * (1.0f - delta.x) * (1.0f - delta.y) + (float) bGradienty * delta.x * (1.0f - delta.y) + (float) cGradienty * (1.0f - delta.x) * delta.y + (float) dGradienty * delta.x * delta.y);

    return result;
}

//void getBestCorrespondence1x1(uint2 screenPos, float4 pInput, float4 nInput, float4 cInput, float4 lInput, out float4 pTarget, out float4 nTarget, out float4 cTarget, out float4 lTarget)
//{
//	pTarget = target[int2(screenPos.x, screenPos.y)];
//	nTarget = targetNormals[int2(screenPos.x, screenPos.y)];
//	cTarget = targetColors[int2(screenPos.x, screenPos.y)];
//	lTarget = targetLabels[int2(screenPos.x, screenPos.y)];
//}

//void getBestCorrespondence5x5(int2 screenPos, float4 pInput, float4 nInput, float4 cInput, out float4 pTarget, out float4 nTarget)
//{
//	float dMin = MAXF;
//	pTarget = float4(MINF, MINF, MINF, MINF);
//	nTarget = float4(MINF, MINF, MINF, MINF);
//
//	for (int i = -2; i<=2; i++)
//	{
//		for (int j = -2; j<=2; j++)
//		{
//			int2 screenPosNew = screenPos + int2(i, j);
//
//			if (screenPosNew.x >= 0 && screenPosNew.y >= 0 && screenPosNew.x < g_ImageWidth && screenPosNew.y < g_ImageHeight)
//			{
//				float4 p = target[int2(screenPosNew.x, screenPosNew.y)];
//				float4 n = targetNormals[int2(screenPosNew.x, screenPosNew.y)];
//				
//				if (isValid(p) && isValid(n))
//				{
//					float d = length(pInput-p);
//
//					if (d < dMin)
//					{
//						pTarget = p;
//						nTarget = n;
//
//						dMin = d;
//					}
//				}
//			}
//		}
//	}
//}

//void getBestCorrespondence5x5Color(int2 screenPos, float4 pInput, float4 nInput, float4 cInput, out float4 pTarget, out float4 nTarget)
//{
//	float dMin = MAXF;
//	pTarget = float4(MINF, MINF, MINF, MINF);
//	nTarget = float4(MINF, MINF, MINF, MINF);
//
//	for (int i = -2; i<=2; i++)
//	{
//		for (int j = -2; j<=2; j++)
//		{
//			int2 screenPosNew = screenPos + int2(i, j);
//
//			if (screenPosNew.x >= 0 && screenPosNew.y >= 0 && screenPosNew.x < g_ImageWidth && screenPosNew.y < g_ImageHeight)
//			{
//				float4 p = target[int2(screenPosNew.x, screenPosNew.y)];
//				float4 n = targetNormals[int2(screenPosNew.x, screenPosNew.y)];
//				float4 c = targetColors[int2(screenPosNew.x, screenPosNew.y)];
//
//				if (isValid(p) && isValid(n) && isValidColor(c)) // isValid(c)
//				{
//					float d = length(pInput.xyz-p.xyz);
//					float cd = length(cInput.xyz-c.xyz);
//
//					float dist = 4.0*d+cd;
//					if (dist < dMin)
//					{
//						pTarget = p;
//						nTarget = n;
//
//						dMin = dist;
//					}
//				}
//			}
//		}
//	}
//}

//float lengthLabel(float4 inputLabel, float4 targetLabel, int labelNum)
//{
//	float labeldist = 0.0f;
//	
//	float ilabel[16], tlable[16];
//
//	unsigned char *iilabel;
//	unsigned char *ttlabel;
//
//	iilabel = (unsigned char*)&inputLabel.x;
//	ttlabel = (unsigned char*)&targetLabel.x;
//	for (int i = 0; i < 4; i++)
//	{
//		ilabel[i] = (int)iilabel[i] / 100.0f;
//		tlabel[i] = (int)ttlabel[i] / 100.0f;
//	}
//
//	iilabel = (unsigned char*)&inputLabel.y;
//	ttlabel = (unsigned char*)&targetLabel.y;
//	for (int i = 4; i < 8; i++)
//	{
//		ilabel[i] = (int)iilabel[i] / 100.0f;
//		tlabel[i] = (int)tilabel[i] / 100.0f;
//	}
//
//	iilabel = (unsigned char*)&inputLabel.z;
//	ttlabel = (unsigned char*)&targetLabel.z;
//	for (int i = 8; i < 12; i++)
//	{
//		ilabel[i] = (int)iilabel[i] / 100.0f;
//		tlabel[i] = (int)ttlabel[i] / 100.0f;
//	}
//
//	iilabel4 = (unsigned char*)&inputLabel.w;
//	ttlabel4 = (unsigned char*)&targetLabel.w;
//	for (int i = 12; i < 16; i++)
//	{
//		ilabel[i] = (int)iilabel[i] / 100.0f;
//		tlabel[i] = (int)ttlabel[i] / 100.0f;
//	}
//
//	for (int i = 0; i < labelNum; i++)
//	{
//		labeldist += (ilabel[i]-tlabel[i]) * (ilabel[i]-tlabel[i]);
//	}
//
//	return labeldist;
//}

[numthreads(groupthreads, groupthreads, 1)]
void projectiveLabelCorrespondencesCS(int3 dTid : SV_DispatchThreadID)
{
	//output[dTid.xy]                     = float4(MINF, MINF, MINF, MINF);
	//outputNormals[dTid.xy]              = float4(MINF, MINF, MINF, MINF);
    //outputIntensities[dTid.xy]          = float4(MINF, MINF, MINF, MINF);
    //outputIntensityGradients[dTid.xy]   = float4(MINF, MINF, MINF, MINF);
    outputLabels[dTid.xy]               = float4(MINF, MINF, MINF, MINF);
    outputLabelGradients[dTid.xy]       = float4(MINF, MINF, MINF, MINF);

	float4 pInput = input[dTid.xy];
	float4 nInput = inputNormals[dTid.xy];
	float4 lInput = inputLabels[dTid.xy];

    if (isValid(pInput) && isValid(nInput) && isValid(lInput) /*&& isValid(lInput)*/)
	{
		pInput.w = 1.0f;    // assert it is a point
		float4 pTransInput = mul(pInput, transform);

		nInput.w = 0.0f;    // assert it is a vector
		float4 nTransInput = mul(nInput, transform); // transformation is a rotation M^(-1)^T = M, translation is ignored because it is a vector
		
		//if(pTransInput.z > FLT_EPSILON) // really necessary
		//{
		int2 screenPos = cameraToKinectScreenInt(pTransInput.xyz) / g_levelFactor;

        float4 pTarget = float4(MINF, MINF, MINF, MINF);
        float4 nTarget = float4(MINF, MINF, MINF, MINF);
            
        if (screenPos.x >= 0 && screenPos.y >= 0 && screenPos.x < g_ImageWidth && screenPos.y < g_ImageHeight)
        {
            getBestCorrespondence(screenPos, pTarget, nTarget);
        }
            
        float2 imagePos = cameraToKinectScreenFloat(pTransInput.xyz) / g_levelFactor;

        float4 lTarget = float4(MINF, MINF, MINF, MINF);
        float4 lGradientTargetX = float4(MINF, MINF, MINF, MINF);
        float4 lGradientTargetY = float4(MINF, MINF, MINF, MINF);

        if (imagePos.x >= 1.0f && imagePos.y >= 1.0f && imagePos.x <= (float)(g_ImageWidth-1) && imagePos.y <= (float)(g_ImageHeight-1))
        {
            float4 labelAndGradient = bilinearInterpolation(imagePos.x, imagePos.y, g_ImageWidth, g_ImageHeight);
            lTarget.x = labelAndGradient.x;
            lGradientTargetX.x = labelAndGradient.y;
            lGradientTargetY.x = labelAndGradient.z;
        }
            
        if (isValid(pTarget) && isValid(nTarget) /*&& isValid(iTarget)*/ /*&& isValid(lTarget)*/ /*&& isValid(iGradientTargetX) && isValid(iGradientTargetY)*/ /*&& isValid(lGradientTarget)*/)
        {
            float distance = length(pTransInput.xyz - pTarget.xyz);
            float dNormal = dot(nTransInput.xyz, nTarget.xyz);
            //float dIntensity = length(iInput.x - iTarget.x);
            //float dProbability = length(lInput - lTarget);

            if ((distWeight   > 0.0f ? distance <= distThres      : 1) &&
				(normalWeight > 0.0f ? dNormal >= normalThres     : 1) /*&&
				(colorWeight  > 0.0f ? dIntensity <= colorThres   : 1) *//*&&
                (labelWeight  > 0.0f ? dProbability <= labelThres : 1)*/)
            {
                //output[dTid.xy] = pTarget;
				//nTarget.w = max(0.0, 0.5f*((1.0f-d/distThres)+(1.0f-cameraToKinectProjZ(pTransInput.z)))); // for weighted ICP;
                //nTarget.w = 1.0f;
                //outputNormals[dTid.xy] = nTarget;
                //outputIntensities[dTid.xy] = iTarget;
				//outputProbabilities[dTid.xy] = lTarget;
                //outputIntensityGradients[dTid.xy] = float4(iGradientTargetX.x, iGradientTargetY.x, MINF, MINF);
                //outputProbabilityGradients[dTid.xy] = lGradientTarget;
            }
        }

        if (/*isValid(pTarget) && isValid(nTarget) && */isValid(lTarget) /*&& isValid(lTarget)*/ && isValid(lGradientTargetX) && isValid(lGradientTargetY) /*&& isValid(lGradientTarget)*/)
        {
            //float distance = length(pTransInput.xyz - pTarget.xyz);
            //float dNormal = dot(nTransInput.xyz, nTarget.xyz);
            //float dLabel = length(lInput.x - lTarget.x);
            //float dProbability = length(lInput - lTarget);

            //if ((distWeight   > 0.0f ? distance <= distThres      : 1) &&
			//	  (normalWeight > 0.0f ? dNormal >= normalThres     : 1) &&
			//	  (colorWeight  > 0.0f ? dIntensity <= colorThres   : 1) &&
            //    (labelWeight  > 0.0f ? dLabel <= labelThres       : 1))
            //{
                //output[dTid.xy] = pTarget;
				//nTarget.w = max(0.0, 0.5f*((1.0f-d/distThres)+(1.0f-cameraToKinectProjZ(pTransInput.z)))); // for weighted ICP;
                //nTarget.w = 1.0f;
                //outputNormals[dTid.xy] = nTarget;
                outputLabels[dTid.xy] = lTarget;
				//outputProbabilities[dTid.xy] = lTarget;
                outputLabelGradients[dTid.xy] = float4(lGradientTargetX.x, lGradientTargetY.x, MINF, MINF);
                //outputProbabilityGradients[dTid.xy] = lGradientTarget;
            //}
        }
                   
        //if (screenPos.x >= 0 && screenPos.y >= 0 && screenPos.x < g_ImageWidth && screenPos.y < g_ImageHeight)
		//{
		//	float4 pTarget;
		//	float4 nTarget;
		//	//float4 iTarget;
        //  //float4 lTarget;
        //
		//	getBestCorrespondence1x1(screenPos, pTarget, nTarget);
		//	//getBestCorrespondence1x1(screenPos, pTransInput, nTransInput, cInput, lInput, pTarget, nTarget, cTarget, lTarget);
		//	//getBestCorrespondence5x5(screenPos, pTransInput, nTransInput, cInput, pTarget, nTarget);
		//	//getBestCorrespondence5x5Color(screenPos, pTransInput, nTransInput, cInput, pTarget, nTarget);
        //
		//	if (isValid(pTarget) && isValid(nTarget) && isValidColor(cTarget) /*&& isValidLabel(lTarget)*/)
		//	{
		//		float distance = length(pTransInput.xyz - pTarget.xyz);
		//		float dNormal = dot(nTransInput.xyz, nTarget.xyz);
        //       
        //      float intensityInput = 0.1140f * cInput.x + 0.5870f * cInput.y + 0.2989f * cInput.z;
		//		float intensityTarget = 0.1140f * cTarget.x + 0.5870f * cTarget.y + 0.2989f * cTarget.z;
		//		
		//		float dColor = length(intensityInput - intensityTarget);
		//		//float dLabel = lengthLabel(lInput, lTarget, labelNum);
		//			
		//		/*if ((  distWeight > 0.0f ? distance <= distThres  : 1) && 
		//		    (normalWeight > 0.0f ? dNormal >= normalThres : 1) && 
		//		    ( colorWeight > 0.0f ? dColor <= colorThres   : 1) &&
		//		    ( labelWeight > 0.0f ? dlabel <= labelThres   : 1))*/
		//			
		//		if ((  distWeight > 0.0f ? distance <= distThres  : 1) &&
		//			(normalWeight > 0.0f ? dNormal >= normalThres : 1) &&
		//			( colorWeight > 0.0f ? dColor <= colorThres   : 1))
		//		{
		//			output[dTid.xy]        = pTarget;
		//			//nTarget.w = max(0.0, 0.5f*((1.0f-d/distThres)+(1.0f-cameraToKinectProjZ(pTransInput.z)))); // for weighted ICP;
		//			nTarget.w = 1.0f;
		//			outputNormals[dTid.xy] = nTarget;
		//			outputColors[dTid.xy]  = cTarget;
		//			//outputLabels[dTid.xy]  = lTarget;
		//		}
		//	}
		//}
	    //}
	}
}