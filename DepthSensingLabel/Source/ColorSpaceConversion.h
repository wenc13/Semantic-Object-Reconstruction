#pragma once

#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline void HSI2RGB(float h, float s, float i, float* r, float* g, float* b)
{
	float x = i * (1 - s);
	if (h < 2 * M_PI / 3)
	{
		float y = i * (1 + (s * cos(h)) / (cos(M_PI / 3 - h)));
		float z = 3 * i - (x + y);
		*b = x; *r = y; *g = z;
	}
	else if (h < 4 * M_PI / 3)
	{
		float y = i * (1 + (s * cos(h - 2 * M_PI / 3)) / (cos(M_PI / 3 - (h - 2 * M_PI / 3))));
		float z = 3 * i - (x + y);
		*r = x; *g = y; *b = z;
	}
	else
	{
		float y = i * (1 + (s * cos(h - 4 * M_PI / 3)) / (cos(M_PI / 3 - (h - 4 * M_PI / 3))));
		float z = 3 * i - (x + y);
		*r = z; *g = x; *b = y;
	}
}

inline void RGB2HSI(float r, float g, float b, float* h, float* s, float* i)
{
	*i = (r + g + b) / 3.0;

	float rn = r / (r + g + b);
	float gn = g / (r + g + b);
	float bn = b / (r + g + b);

	*h = acos((0.5 * ((rn - gn) + (rn - bn))) / (sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn))));
	if (b > g)
	{
		*h = 2 * M_PI - *h;
	}

	*s = 1 - 3 * std::min(rn, std::min(gn, bn));
}