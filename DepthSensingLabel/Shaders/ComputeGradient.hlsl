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

bool getValueNearestNeighbourFloat(const float2 p, int imageWidth, int imageHeigth, out float pos)
{
    const int u = (int)(p.x + 0.5f);
    const int v = (int)(p.y + 0.5f);

    if (u < 0 || u > imageWidth || v < 0 || v > imageHeigth)
        return false;
    
    pos = input[int2(u, v)].x;

    if (pos != MINF)
        return true;
    else
        return false;
}

[numthreads(groupthreads, groupthreads, 1)]
void computeGradientCS(int3 dTid : SV_DispatchThreadID)
{
	/*
    int x = dTid.x;
    int y = dTid.y;

    if (x >= 0 && x < imageWidth && y >= 0 && y <imageHeigth)
    {
        output[int2(x, y)] = float4(MINF, MINF, MINF, MINF);
        
        if (x > 0 && x < imageWidth -1 && y > 0 && y < imageHeigth -1)
        {
            float pos00; bool valid00 = getValueNearestNeighbourFloat(float2(x-1, y-1), imageWidth, imageHeigth, pos00); if(!valid00) return;
            float pos01; bool valid01 = getValueNearestNeighbourFloat(float2(x-1, y-0), imageWidth, imageHeigth, pos01); if(!valid01) return;
            float pos02; bool valid02 = getValueNearestNeighbourFloat(float2(x-1, y+1), imageWidth, imageHeigth, pos02); if(!valid02) return;

            float pos10; bool valid10 = getValueNearestNeighbourFloat(float2(x-0, y-1), imageWidth, imageHeigth, pos10); if(!valid10) return;
            float pos11; bool valid11 = getValueNearestNeighbourFloat(float2(x-0, y-0), imageWidth, imageHeigth, pos11); if(!valid11) return;
            float pos12; bool valid12 = getValueNearestNeighbourFloat(float2(x-0, y+1), imageWidth, imageHeigth, pos12); if(!valid12) return;

            float pos20; bool valid20 = getValueNearestNeighbourFloat(float2(x+1, y-1), imageWidth, imageHeigth, pos20); if(!valid20) return;
            float pos21; bool valid21 = getValueNearestNeighbourFloat(float2(x+1, y-0), imageWidth, imageHeigth, pos21); if(!valid21) return;
            float pos22; bool valid22 = getValueNearestNeighbourFloat(float2(x+1, y+1), imageWidth, imageHeigth, pos22); if(!valid22) return;

            float resU = (-1.0f) * pos00 + (1.0f) * pos20 + (-2.0f) * pos01 + (2.0f) * pos21 + (-1.0f) * pos02 + (1.0f) * pos22;
            resU /= 8.0f;
			
            float resV = (-1.0f) * pos00 + (-2.0f) * pos10 + (-1.0f) * pos20 + (1.0f) * pos02 + (2.0f) * pos12 + (1.0f) * pos22;
            resV /= 8.0f;

            output[int2(x, y)] = float4(resU, resV, MINF, MINF);
        }
    }
    */

    int x = dTid.x;
    int y = dTid.y;

    if (x >= 0 && x < imageWidth && y >= 0 && y < imageHeigth)
    {
        output[dTid.xy] = float4(0, 0, MINF, MINF);

        if (x > 0 && x < imageWidth - 1 && y > 0 && y < imageHeigth - 1)
        {
            float dx1, dx2, dx3, dx_out;

            dx1 = input[int2(x + 1, y - 1)] - input[int2(x - 1, y - 1)];
            dx2 = input[int2(x + 1, y)] - input[int2(x - 1, y)];
            dx3 = input[int2(x + 1, y + 1)] - input[int2(x - 1, y + 1)];

            dx_out = (dx1 + 2 * dx2 + dx3) / 8;

            float dy1, dy2, dy3, dy_out;

            dy1 = input[int2(x - 1, y + 1)] - input[int2(x - 1, y - 1)];
            dy2 = input[int2(x, y + 1)] - input[int2(x, y - 1)];
            dy3 = input[int2(x + 1, y + 1)] - input[int2(x + 1, y - 1)];

            dy_out = (dy1 + 2 * dy2 + dy3) / 8;

            output[dTid.xy] = float4(dx_out, dy_out, MINF, MINF);
        }
    }
}