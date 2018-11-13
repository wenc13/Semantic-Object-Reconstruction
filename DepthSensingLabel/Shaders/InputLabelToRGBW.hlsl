cbuffer consts : register(b0)
{
    int4 color[12];
    int imageWidth;
    int imageHeigth;
    int labelNum;
    int dummy0;
};

Texture2D<float4> inputLabel00 : register(t0);
Texture2D<float4> inputLabel01 : register(t1);
Texture2D<float4> inputLabel02 : register(t2);
Texture2D<float4> inputLabel03 : register(t3);
Texture2D<float4> inputLabel04 : register(t4);
Texture2D<float4> inputLabel05 : register(t5);
Texture2D<float4> inputLabel06 : register(t6);
Texture2D<float4> inputLabel07 : register(t7);
Texture2D<float4> inputLabel08 : register(t8);
Texture2D<float4> inputLabel09 : register(t9);
Texture2D<float4> inputLabel10 : register(t10);
Texture2D<float4> inputLabel11 : register(t11);

RWTexture2D<float4> output : register(u0);

#ifndef groupthreads
#define groupthreads 1
#endif

#ifndef MINF
#define MINF asfloat(0xff800000)
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void RGB2HSB(int red, int green, int blue, out float hue, out float saturation, out float brightness)
{
    int cmax = (red > green) ? red : green;
    if (blue > cmax) cmax = blue;
    int cmin = (red < green) ? red : green;
    if (blue < cmin) cmin = blue;

    brightness = ((float) cmax) / 255.0f;
    if (cmax != 0)
        saturation = ((float) (cmax - cmin)) / ((float) cmax);
    else
        saturation = 0;

    if (saturation == 0)
        hue = 0;
    else
    {
        float redc = ((float) (cmax - red)) / ((float) (cmax - cmin));
        float greenc = ((float) (cmax - green)) / ((float) (cmax - cmin));
        float bluec = ((float) (cmax - blue)) / ((float) (cmax - cmin));

        if (red == cmax)
            hue = bluec - greenc;
        else if (green == cmax)
            hue = 2.0f + redc - bluec;
        else
            hue = 4.0f + greenc - redc;

        hue = hue / 6.0f;
        if (hue < 0.0f)
            hue = hue + 1.0f;
    }
}

void HSB2RGB(float hue, float saturation, float brightness, out int red, out int green, out int blue)
{
    if (saturation == 0)
        red = green = blue = (int) (brightness * 255.0f + 0.5f);
    else
    {
        float h = (hue - (float) (floor(hue))) * 6.0f;
        float f = h - (float) (floor(h));

        float p = brightness * (1.0f - saturation);
        float q = brightness * (1.0f - saturation * f);
        float t = brightness * (1.0f - saturation * (1.0f - f));

        switch ((int) h)
        {
            case 0:
                red   = (int) (brightness * 255.0f + 0.5f);
                green = (int) (t * 255.0f + 0.5f);
                blue  = (int) (p * 255.0f + 0.5f);
                break;
            case 1:
                red   = (int) (q * 255.0f + 0.5f);
                green = (int) (brightness * 255.0f + 0.5f);
                blue  = (int) (p * 255.0f + 0.5f);
                break;
            case 2:
                red   = (int) (p * 255.0f + 0.5f);
                green = (int) (brightness * 255.0f + 0.5f);
                blue  = (int) (t * 255.0f + 0.5f);
                break;
            case 3:
                red   = (int) (p * 255.0f + 0.5f);
                green = (int) (q * 255.0f + 0.5f);
                blue  = (int) (brightness * 255.0f + 0.5f);
                break;
            case 4:
                red   = (int) (t * 255.0f + 0.5f);
                green = (int) (p * 255.0f + 0.5f);
                blue  = (int) (brightness * 255.0f + 0.5f);
                break;
            case 5:
                red   = (int) (brightness * 255.0f + 0.5f);
                green = (int) (p * 255.0f + 0.5f);
                blue  = (int) (q * 255.0f + 0.5f);
                break;
        }
    }
}

//void RGB2HSI(float r, float g, float b, out float h, out float s, out float i)
//{
//    i = (r + g + b) / 3.0;

//    float rn = r / (r + g + b);
//    float gn = g / (r + g + b);
//    float bn = b / (r + g + b);

//    h = acos((0.5 * ((rn - gn) + (rn - bn))) / (sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn))));
//    if (b > g)
//    {
//        h = 2 * M_PI - h;
//    }

//    s = 1 - 3 * min(rn, min(gn, bn));
//}

//void HSI2RGB(float h, float s, float i, out float r, out float g, out float b)
//{
//    float x = i * (1 - s);
//    if (h < 2 * M_PI / 3)
//    {
//        float y = i * (1 + (s * cos(h)) / (cos(M_PI / 3 - h)));
//        float z = 3 * i - (x + y);
//        b = x; r = y; g = z;
//    }
//    else if (h < 4 * M_PI / 3)
//    {
//        float y = i * (1 + (s * cos(h - 2 * M_PI / 3)) / (cos(M_PI / 3 - (h - 2 * M_PI / 3))));
//        float z = 3 * i - (x + y);
//        r = x; g = y; b = z;
//    }
//    else
//    {
//        float y = i * (1 + (s * cos(h - 4 * M_PI / 3)) / (cos(M_PI / 3 - (h - 4 * M_PI / 3))));
//        float z = 3 * i - (x + y);
//        r = z; g = x; b = y;
//    }
//}

[numthreads(groupthreads, groupthreads, 1)]
void inputLabelToRGBWCS(int3 dTid : SV_DispatchThreadID)
{
    //if (dTid.x >= imageWidth || dTid.y >= imageHeigth)
    //    return;

    float inputLabel[12] =
    {
        inputLabel00[dTid.xy].x, inputLabel01[dTid.xy].x, inputLabel02[dTid.xy].x, inputLabel03[dTid.xy].x,
        inputLabel04[dTid.xy].x, inputLabel05[dTid.xy].x, inputLabel06[dTid.xy].x, inputLabel07[dTid.xy].x,
        inputLabel08[dTid.xy].x, inputLabel09[dTid.xy].x, inputLabel10[dTid.xy].x, inputLabel11[dTid.xy].x
    };
    
    float maxLabelProbability = 0.0f;
    int maxLabelIndex = 0;
    for (int i = 0; i < labelNum; i++)
    {
        if (inputLabel[i] > maxLabelProbability)
        {
            maxLabelProbability = inputLabel[i];
            maxLabelIndex = i;
        }
    }

    float hue, saturation, brightness;
    RGB2HSB(color[maxLabelIndex].x, color[maxLabelIndex].y, color[maxLabelIndex].z, hue, saturation, brightness);
    
    saturation = maxLabelProbability * saturation;
    //saturation = float(labelNum)/float(labelNum-1) * maxLabelProbability - 1.0f/float(labelNum-1);
    
    int red, green, blue;
    HSB2RGB(hue, saturation, brightness, red, green, blue);

    output[dTid.xy] = float4(red/255.0f, green/255.0f, blue/255.0f, 1.0f);

    //if (maxLabelIndex == 4)
    //{
    //    output[dTid.xy] = float4(red/255.0f, green/255.0f, blue/255.0f, 1.0f);
    //}
    //else
    //{
    //    output[dTid.xy] = float4(1.0f, 1.0f, 1.0f, 1.0f);
    //}
}