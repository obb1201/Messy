#pragma once

#define PI 				3.1415926535897932f
#define HALF_PI			1.57079632679f

namespace fm
{
	// Converts radians to degrees.
	static float ToDegrees(const float radian)
	{
		return radian * (180.f / PI);
	}

	// Converts degrees to radians.
	static float ToRadians(const float degree)
	{
		return degree * (PI / 180.f);
	}

	static float Clamp(float value, float min, float max)
	{
		value = (value > max) ? max : value;
		value = (value < min) ? min : value;
		return value;
	}
}