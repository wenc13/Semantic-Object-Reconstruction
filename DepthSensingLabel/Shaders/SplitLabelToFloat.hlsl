cbuffer consts : register(b0)
{
    int imageWidth;
    int imageHeigth;
    int labelNum;
    int currentLabel;
};

Texture2D<float4> input0   : register(t0);
Texture2D<float4> input1   : register(t1);
Texture2D<float4> input2   : register(t2);

RWTexture2D<float4> output : register(u0);

#ifndef groupthreads
#define groupthreads 1
#endif

#define MINF asfloat(0xff800000)

[numthreads(groupthreads, groupthreads, 1)]
void splitLabelToFloatCS(int3 dTid : SV_DispatchThreadID)
{
    //if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;

    //float inputLabel[4] = { input[dTid.xy].x, input[dTid.xy].y, input[dTid.xy].z, input[dTid.xy].w };

    //if (currentLabel < labelNum)
    //{
    //    output[dTid.xy] = float4(inputLabel[currentLabel%4], inputLabel[currentLabel%4], inputLabel[currentLabel%4], 1.0f);
    //}
    //else
    //{
    //    output[dTid.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    //}

    //float3 colors[12];
    //if (labelNum == 3)
    //{
    //    // Table
    //    colors[0] = float3(225.0f, 225.0f, 225.0f); colors[1] = float3(255.0f, 219.0f, 100.0f); colors[2]  = float3(157.0f, 197.0f, 233.0f); colors[3]  = float3(255.0f, 255.0f, 255.0f);
	   // colors[4] = float3(255.0f, 255.0f, 255.0f); colors[5] = float3(255.0f, 255.0f, 255.0f); colors[6]  = float3(255.0f, 255.0f, 255.0f); colors[7]  = float3(255.0f, 255.0f, 255.0f);
	   // colors[8] = float3(255.0f, 255.0f, 255.0f); colors[9] = float3(255.0f, 255.0f, 255.0f); colors[10] = float3(255.0f, 255.0f, 255.0f); colors[11] = float3(255.0f, 255.0f, 255.0f);
    //}
    //else if (labelNum == 5)
    //{
    //    // Chair
	   // colors[0] = float3(225.0f, 225.0f, 225.0f); colors[1] = float3(243.0f, 177.0f, 133.0f); colors[2]  = float3(255.0f, 219.0f, 100.0f); colors[3]  = float3(157.0f, 233.0f, 225.0f);
	   // colors[4] = float3(157.0f, 197.0f, 233.0f); colors[5] = float3(255.0f, 255.0f, 255.0f); colors[6]  = float3(255.0f, 255.0f, 255.0f); colors[7]  = float3(255.0f, 255.0f, 255.0f);
	   // colors[8] = float3(255.0f, 255.0f, 255.0f); colors[9] = float3(255.0f, 255.0f, 255.0f); colors[10] = float3(255.0f, 255.0f, 255.0f); colors[11] = float3(255.0f, 255.0f, 255.0f);
    //}
    //else if (labelNum == 7)
    //{
    //    // Motorcycle
    //    colors[0] = float3(225.0f, 225.0f, 225.0f); colors[1] = float3(157.0f, 197.0f, 233.0f); colors[2]  = float3(243.0f, 177.0f, 133.0f); colors[3]  = float3(233.0f, 157.0f, 165.0f);
    //    colors[4] = float3(255.0f, 219.0f, 100.0f); colors[5] = float3(157.0f, 233.0f, 225.0f); colors[6]  = float3(167.0f, 211.0f, 143.0f); colors[7]  = float3(255.0f, 255.0f, 255.0f);
    //    colors[8] = float3(255.0f, 255.0f, 255.0f); colors[9] = float3(255.0f, 255.0f, 255.0f); colors[10] = float3(255.0f, 255.0f, 255.0f); colors[11] = float3(255.0f, 255.0f, 255.0f);
    //}

    float inputLabel[12] =
    {
        input0[dTid.xy].x, input0[dTid.xy].y, input0[dTid.xy].z, input0[dTid.xy].w,
        input1[dTid.xy].x, input1[dTid.xy].y, input1[dTid.xy].z, input1[dTid.xy].w,
        input2[dTid.xy].x, input2[dTid.xy].y, input2[dTid.xy].z, input2[dTid.xy].w
    };

    float labelSum = 0.0f;
    for (uint i = 0; i < labelNum; i++)
    {
        labelSum = labelSum + inputLabel[i];
    }
    if(labelSum != 0.0f)
    {
        for (uint j = 0; j < labelNum; j++)
        {
            inputLabel[j] = inputLabel[j] / labelSum;
        }
    }

    if (currentLabel < labelNum)
    {
        output[dTid.xy] = float4(inputLabel[currentLabel], inputLabel[currentLabel], inputLabel[currentLabel], 1.0f);
    }
    else
    {
        output[dTid.xy] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
}