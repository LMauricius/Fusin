#ifndef _FUSIN_COLOR_VALUE_H
#define _FUSIN_COLOR_VALUE_H

#include "FusinPrerequisites.h"

namespace Fusin
{
	class ColorHSV;

	class _FUSIN_EXPORT ColorRGB
	{
	public:
		ColorRGB();
		ColorRGB(const ColorRGB& c);
		ColorRGB(float r, float g, float b);
		ColorRGB(const ColorHSV& c);
		ColorRGB(unsigned int c);

		ColorRGB& operator=(const ColorRGB& c);
		ColorRGB& operator+=(const ColorRGB& c);
		ColorRGB& operator-=(const ColorRGB& c);
		ColorRGB& operator*=(const ColorRGB& c);
		ColorRGB& operator*=(float f);
		ColorRGB operator+(const ColorRGB& c);
		ColorRGB operator-(const ColorRGB& c);
		ColorRGB operator*(const ColorRGB& c);
		ColorRGB operator*(float f);
		bool operator==(const ColorRGB& c) const;
		bool operator!=(const ColorRGB& c) const;
		bool operator>(const ColorRGB& c) const;
		bool operator<(const ColorRGB& c) const;
		bool operator>=(const ColorRGB& c) const;
		bool operator<=(const ColorRGB& c) const;
		operator unsigned int() const;
		operator bool() const;

		float r, g, b;
	};

	class _FUSIN_EXPORT ColorHSV
	{
	public:
		ColorHSV();
		ColorHSV(const ColorHSV& c);
		ColorHSV(float h, float s, float v);
		ColorHSV(const ColorRGB& c);

		ColorHSV& operator=(const ColorHSV& c);
		ColorHSV& operator+=(const ColorHSV& c);
		ColorHSV& operator-=(const ColorHSV& c);
		ColorHSV& operator*=(const ColorHSV& c);
		ColorHSV& operator*=(float f);
		ColorHSV operator+(const ColorHSV& c);
		ColorHSV operator-(const ColorHSV& c);
		ColorHSV operator*(const ColorHSV& c);
		ColorHSV operator*(float f);

		bool operator==(const ColorHSV& c) const;
		bool operator!=(const ColorHSV& c) const;
		bool operator>(const ColorHSV& c) const;
		bool operator<(const ColorHSV& c) const;
		bool operator>=(const ColorHSV& c) const;
		bool operator<=(const ColorHSV& c) const;

		float h, s, v;
	};

	namespace Color
	{
		_FUSIN_EXPORT extern const ColorRGB Red;
		_FUSIN_EXPORT extern const ColorRGB Green;
		_FUSIN_EXPORT extern const ColorRGB Blue;
		_FUSIN_EXPORT extern const ColorRGB Black;
		_FUSIN_EXPORT extern const ColorRGB White;

		_FUSIN_EXPORT extern const ColorRGB Yellow;
		_FUSIN_EXPORT extern const ColorRGB Cyan;
		_FUSIN_EXPORT extern const ColorRGB Magenta;

		_FUSIN_EXPORT extern const ColorRGB Orange;
		_FUSIN_EXPORT extern const ColorRGB Lime;
		_FUSIN_EXPORT extern const ColorRGB Spring;
		_FUSIN_EXPORT extern const ColorRGB Azure;
		_FUSIN_EXPORT extern const ColorRGB Purple;
		_FUSIN_EXPORT extern const ColorRGB Spring;
		_FUSIN_EXPORT extern const ColorRGB Rose;
	}

}

#endif