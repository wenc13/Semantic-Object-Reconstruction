cbuffer consts : register(b0)
{
	int imageWidth;
	int imageHeigth;

	float2 dummy;
};

Texture2D<float4> input : register(t0);
RWTexture2D<float4> output : register(u0);

#ifndef groupthreads
#define groupthreads 1
#endif

#define MINF asfloat(0xff800000)

[numthreads(groupthreads, groupthreads, 1)]
void convertColorToIntensityFloatCS(int3 dTid : SV_DispatchThreadID)
{
    //if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;

    float intensity = 0.1140f * input[dTid.xy].x + 0.5870f * input[dTid.xy].y + 0.2989f * input[dTid.xy].z;
    output[dTid.xy] = float4(intensity, MINF, MINF, MINF);
}