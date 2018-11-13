cbuffer consts : register(b0)
{
    int imageWidth;
    int imageHeigth;
    int labelNum;
    int dummy;
};

Texture2D<float4> input           : register(t0);
Texture2D<float4> inputNormal     : register(t1);
Texture2D<float4> label01         : register(t2);
Texture2D<float4> label02         : register(t3);
Texture2D<float4> label03         : register(t4);
Texture2D<float4> label04         : register(t5);
Texture2D<float4> label05         : register(t6);
Texture2D<float4> label06         : register(t7);
Texture2D<float4> label07         : register(t8);
Texture2D<float4> label08         : register(t9);
Texture2D<float4> label09         : register(t10);
Texture2D<float4> label10         : register(t11);
Texture2D<float4> label11         : register(t12);
Texture2D<float4> label12         : register(t13);

RWBuffer<float>  outputConfidence : register(u0);
RWBuffer<float4> outputNormal     : register(u1);
RWBuffer<float4> outputPoint      : register(u2);
RWBuffer<int>    outputIndex      : register(u3);

#ifndef groupthreads
#define groupthreads 1
#endif

#define MINF asfloat(0xff800000)

#include "Util.h.hlsl"

[numthreads(groupthreads, groupthreads, 1)]
void getConfidenceNormalPointWithoutBGCS(int3 dTid : SV_DispatchThreadID)
{
	//if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;
    
    outputConfidence[dTid.x+dTid.y*imageWidth] = 0.0f;
    outputNormal[dTid.x+dTid.y*imageWidth] = float4(MINF, MINF, MINF, MINF);
    outputPoint[dTid.x+dTid.y*imageWidth] = float4(MINF, MINF, MINF, MINF);
    outputIndex[dTid.x+dTid.y*imageWidth] = 0;

    float4 pInput = input[dTid.xy];
    float4 nInput = inputNormal[dTid.xy];

    if (isValid(pInput) && isValid(nInput))
    {
        float label[12] =
        {
            label01[dTid.xy].x, label02[dTid.xy].x, label03[dTid.xy].x, label04[dTid.xy].x,
            label05[dTid.xy].x, label06[dTid.xy].x, label07[dTid.xy].x, label08[dTid.xy].x,
            label09[dTid.xy].x, label10[dTid.xy].x, label11[dTid.xy].x, label12[dTid.xy].x
        };

        float labelEntropy = 0.0f;
        for (uint m = 0; m < labelNum; m++)
        {
            float pt = label[m];
            if (pt != 0)
                labelEntropy -= pt * log(pt);
        }

        uint maxIndex = 0; float maxLabel = 0.0f;
        for (uint n = 0; n < labelNum; n++)
        {
            if (label[n] > maxLabel)
            {
                maxIndex = n;
                maxLabel = label[n];
            }
        }

        if (maxIndex != 0)
        {
            outputConfidence[dTid.x+dTid.y*imageWidth] = 1-labelEntropy/log(labelNum);
            outputNormal[dTid.x+dTid.y*imageWidth] = nInput;
            outputPoint[dTid.x+dTid.y*imageWidth] = float4(pInput.x, pInput.y, -pInput.z, pInput.w);
            outputIndex[dTid.x+dTid.y*imageWidth] = maxIndex;
        }
        else
        {
            outputConfidence[dTid.x+dTid.y*imageWidth] = 0.0f;
            outputNormal[dTid.x+dTid.y*imageWidth] = float4(MINF, MINF, MINF, MINF);
            outputPoint[dTid.x+dTid.y*imageWidth] = float4(MINF, MINF, MINF, MINF);
            outputIndex[dTid.x+dTid.y*imageWidth] = 0;
        }
    }
}