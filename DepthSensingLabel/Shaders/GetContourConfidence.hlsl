cbuffer consts : register(b0)
{
    int imageWidth;
    int imageHeigth;
    int labelNum;
    int dummy;
};

Texture2D<float>  input   : register(t0);
Texture2D<float4> label01 : register(t1);
Texture2D<float4> label02 : register(t2);
Texture2D<float4> label03 : register(t3);
Texture2D<float4> label04 : register(t4);
Texture2D<float4> label05 : register(t5);
Texture2D<float4> label06 : register(t6);
Texture2D<float4> label07 : register(t7);
Texture2D<float4> label08 : register(t8);
Texture2D<float4> label09 : register(t9);
Texture2D<float4> label10 : register(t10);
Texture2D<float4> label11 : register(t11);
Texture2D<float4> label12 : register(t12);

RWBuffer<float> output    : register(u0);
RWBuffer<int> outputIndex : register(u1);

#ifndef groupthreads
#define groupthreads 1
#endif

#define MINF asfloat(0xff800000)

[numthreads(groupthreads, groupthreads, 1)]
void getContourConfidenceCS(int3 dTid : SV_DispatchThreadID)
{
	//if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;

    output[dTid.x + dTid.y * imageWidth] = 0.0f;

    float label[12] =
    {
        label01[dTid.xy].x, label02[dTid.xy].x, label03[dTid.xy].x, label04[dTid.xy].x,
        label05[dTid.xy].x, label06[dTid.xy].x, label07[dTid.xy].x, label08[dTid.xy].x,
        label09[dTid.xy].x, label10[dTid.xy].x, label11[dTid.xy].x, label12[dTid.xy].x 
    };

    if (input[dTid.xy] > 0.0f)
    {
        float labelEntropy = 0.0f;
        for (uint m = 0; m < labelNum; m++)
        {
            float pt = label[m];
            if (pt != 0)
                labelEntropy -= pt * log(pt);
        }

        output[dTid.x + dTid.y * imageWidth] = 1-labelEntropy/log(labelNum);
    }
}