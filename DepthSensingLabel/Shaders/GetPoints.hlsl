cbuffer consts : register(b0)
{
	int imageWidth;
	int imageHeigth;

	float2 dummy;
};

Texture2D<float4> input : register(t0);
RWBuffer<float4> output : register(u0);

#ifndef groupthreads
#define groupthreads 1
#endif

#define MINF asfloat(0xff800000)

[numthreads(groupthreads, groupthreads, 1)]
void getPointsCS(int3 dTid : SV_DispatchThreadID)
{
	//if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;
    
    output[dTid.x + dTid.y * imageWidth] = float4(input[dTid.xy].x, input[dTid.xy].y, -input[dTid.xy].z, input[dTid.xy].w);
}