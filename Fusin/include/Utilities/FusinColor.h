#ifndef _FUSIN_COLOR_VALUE_H
#define _FUSIN_COLOR_VALUE_H

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
		extern const ColorRGB Red;
		extern const ColorRGB Green;
		extern const ColorRGB Blue;
		extern const ColorRGB Black;
		extern const ColorRGB White;

		extern const ColorRGB Yellow;
		extern const ColorRGB Cyan;
		extern const ColorRGB Magenta;

		extern const ColorRGB Orange;
		extern const ColorRGB Lime;
		extern const ColorRGB Spring;
		extern const ColorRGB Azure;
		extern const ColorRGB Purple;
		extern const ColorRGB Spring;
		extern const ColorRGB Rose;
	}

}

#endif