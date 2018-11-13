cbuffer consts : register(b0)
{
	int imageWidth;
	int imageHeigth;
    int initial;
    int dummy;
};

Texture2D<float4> input : register(t0);
RWTexture2D<float4> output : register(u0);

#ifndef groupthreads
#define groupthreads 1
#endif

#define MINF asfloat(0xff800000)

[numthreads(groupthreads, groupthreads, 1)]
void convertLabelCS(int3 dTid : SV_DispatchThreadID)
{
    //if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;

    output[dTid.xy] = input[dTid.xy];
}