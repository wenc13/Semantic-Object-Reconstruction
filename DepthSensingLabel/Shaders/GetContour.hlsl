cbuffer consts : register(b0)
{
    int imageWidth;
    int imageHeigth;

    float2 dummy;
};

Texture2D<float> input : register(t0);
RWTexture2D<float4> output : register(u0);

#ifndef groupthreads
#define groupthreads 1
#endif

#define MINF asfloat(0xff800000)

[numthreads(groupthreads, groupthreads, 1)]
void getContourCS(int3 dTid : SV_DispatchThreadID)
{
	//if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;
    
    if (input[dTid.xy] > 0.0f)
    {
        output[dTid.xy] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        output[dTid.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}