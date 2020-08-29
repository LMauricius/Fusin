#include "Utilities/FusinColor.h"
#include <math.h>

namespace Fusin
{
	ColorRGB::ColorRGB()
		: r(0.0f)
		, g(0.0f)
		, b(0.0f)
	{

	}

	ColorRGB::ColorRGB(const ColorRGB& c)
		: r(c.r)
		, g(c.g)
		, b(c.b)
	{

	}

	ColorRGB::ColorRGB(float r, float g, float b)
		: r(r)
		, g(g)
		, b(b)
	{

	}

	ColorRGB::ColorRGB(unsigned int c)
	{
		r = (((float)(c << (sizeof(unsigned int) * 8 - 24))) / 255);
		g = (((float)(c << (sizeof(unsigned int) * 8 - 16))) / 255);
		b = (((float)(c << (sizeof(unsigned int) * 8 - 8))) / 255);
	}

	ColorRGB::ColorRGB(const ColorHSV& c)
	{
		double      hh, p, q, t, ff;
		long        i;

		if (c.s <= 0.0)
		{
			r = c.v;
			g = c.v;
			b = c.v;
		}
		hh = c.h;
		if (hh >= 360.0) hh = hh - floor(hh/360.0)*360.0;
		else if (hh < 0.0) hh = 360.0 - (-hh - floor(hh / 360.0)*360.0);
		hh /= 60.0;
		i = (long)hh;
		ff = hh - i;
		p = c.v * (1.0 - c.s);
		q = c.v * (1.0 - (c.s * ff));
		t = c.v * (1.0 - (c.s * (1.0 - ff)));

		switch (i)
		{
		case 0:
			r = c.v;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = c.v;
			b = p;
			break;
		case 2:
			r = p;
			g = c.v;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = c.v;
			break;
		case 4:
			r = t;
			g = p;
			b = c.v;
			break;
		case 5:
		default:
			r = c.v;
			g = p;
			b = q;
			break;
		}
	}


	ColorRGB& ColorRGB::operator=(const ColorRGB& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		return *this;
	}

	ColorRGB& ColorRGB::operator+=(const ColorRGB& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		return *this;
	}

	ColorRGB& ColorRGB::operator-=(const ColorRGB& c)
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		return *this;
	}

	ColorRGB& ColorRGB::operator*=(const ColorRGB& c)
	{
		r *= c.r;
		g *= c.g;
		b *= c.b;
		return *this;
	}

	ColorRGB& ColorRGB::operator*=(float f)
	{
		r *= f;
		g *= f;
		b *= f;
		return *this;
	}

	ColorRGB ColorRGB::operator+(const ColorRGB& c)
	{
		return ColorRGB(r + c.r, g + c.g, b + c.b);
	}

	ColorRGB ColorRGB::operator-(const ColorRGB& c)
	{
		return ColorRGB(r - c.r, g - c.g, b - c.b);
	}

	ColorRGB ColorRGB::operator*(const ColorRGB& c)
	{
		return ColorRGB(r * c.r, g * c.g, b * c.b);
	}

	ColorRGB ColorRGB::operator*(float f)
	{
		return ColorRGB(r * f, g * f, b * f);
	}

	bool ColorRGB::operator==(const ColorRGB& c) const
	{
		return r == c.r && g == c.g && b == c.b;
	}

	bool ColorRGB::operator!=(const ColorRGB& c) const
	{
		return r != c.r || g != c.g || b != c.b;
	}

	bool ColorRGB::operator>(const ColorRGB& c) const
	{
		if (r > c.r) return true;
		if (g > c.g) return true;
		if (b > c.b) return true;
		return false;
	}

	bool ColorRGB::operator<(const ColorRGB& c) const
	{
		if (r < c.r) return true;
		if (g < c.g) return true;
		if (b < c.b) return true;
		return false;
	}

	bool ColorRGB::operator>=(const ColorRGB& c) const
	{
		if (r >= c.r) return true;
		if (g >= c.g) return true;
		if (b >= c.b) return true;
		return false;
	}

	bool ColorRGB::operator<=(const ColorRGB& c) const
	{
		if (r <= c.r) return true;
		if (g <= c.g) return true;
		if (b <= c.b) return true;
		return false;
	}

	ColorRGB::operator unsigned int() const
	{
		unsigned int c;
		c = r * 255;
		c << 8;
		c = g * 255;
		c << 8;
		c = b * 255;
		c << 8;
		return c;
	}

	ColorRGB::operator bool() const
	{
		return (r != 0.0f || g != 0.0f || b != 0.0f);
	}

	ColorHSV::ColorHSV()
		: h(0.0f)
		, s(0.0f)
		, v(0.0f)
	{

	}

	ColorHSV::ColorHSV(const ColorHSV& c)
		: h(c.h)
		, s(c.s)
		, v(c.v)
	{

	}

	ColorHSV::ColorHSV(float h, float s, float v)
		: h(h)
		, s(s)
		, v(v)
	{

	}

	ColorHSV::ColorHSV(const ColorRGB& c)
	{
		double      min, max, delta;

		min = c.r < c.g ? c.r : c.g;
		min = min  < c.b ? min : c.b;

		max = c.r > c.g ? c.r : c.g;
		max = max  > c.b ? max : c.b;

		v = max;                                // v
		delta = max - min;
		if (delta < 0.00001)
		{
			s = 0;
			h = 0; // undefined, maybe nan?
		}
		if (max > 0.0) // NOTE: if Max == 0, this divide would cause a crash
		{
			s = (delta / max);                  // s
		}
		else
		{
			// if max is 0, then r = g = b = 0              
			// s = 0, h is undefined
			s = 0.0;
			h = 0.0;                            // its now undefined
		}
		if (c.r >= max)                           // > is bogus, just keeps compilor happy
			h = (c.g - c.b) / delta;        // between yellow & magenta
		else
			if (c.g >= max)
				h = 2.0 + (c.b - c.r) / delta;  // between cyan & yellow
			else
				h = 4.0 + (c.r - c.g) / delta;  // between magenta & cyan

		h *= 60.0;                              // degrees

		if (h < 0.0)
			h += 360.0;
	}


	ColorHSV& ColorHSV::operator=(const ColorHSV& c)
	{
		h = c.h;
		s = c.s;
		v = c.v;
		return *this;
	}

	ColorHSV& ColorHSV::operator+=(const ColorHSV& c)
	{
		*this = (ColorRGB)(*this) + c;
		return *this;
	}

	ColorHSV& ColorHSV::operator-=(const ColorHSV& c)
	{
		*this = (ColorRGB)(*this) - c;
		return *this;
	}

	ColorHSV& ColorHSV::operator*=(const ColorHSV& c)
	{
		*this = (ColorRGB)(*this) * c;
		return *this;
	}

	ColorHSV& ColorHSV::operator*=(float f)
	{
		v *= f;
		return *this;
	}

	ColorHSV ColorHSV::operator+(const ColorHSV& c)
	{
		return ColorHSV((ColorRGB)(*this) + c);
	}

	ColorHSV ColorHSV::operator-(const ColorHSV& c)
	{
		return ColorHSV((ColorRGB)(*this) - c);
	}

	ColorHSV ColorHSV::operator*(const ColorHSV& c)
	{
		return ColorHSV((ColorRGB)(*this) * c);
	}

	ColorHSV ColorHSV::operator*(float f)
	{
		return ColorHSV(h, s, v*f);
	}

	bool ColorHSV::operator==(const ColorHSV& c) const
	{
		return h == c.h && s == c.s && v == c.v;
	}

	bool ColorHSV::operator!=(const ColorHSV& c) const
	{
		return h != c.h || s != c.s || v != c.v;
	}

	bool ColorHSV::operator>(const ColorHSV& c) const
	{
		return (ColorRGB)*this > (ColorRGB)c;
	}

	bool ColorHSV::operator<(const ColorHSV& c) const
	{
		return (ColorRGB)*this < (ColorRGB)c;
	}

	bool ColorHSV::operator>=(const ColorHSV& c) const
	{
		return (ColorRGB)*this >= (ColorRGB)c;
	}

	bool ColorHSV::operator<=(const ColorHSV& c) const
	{
		return (ColorRGB)*this <= (ColorRGB)c;
	}


	namespace Color
	{
		const ColorRGB Red(1, 0, 0);
		const ColorRGB Green(0, 1, 0);
		const ColorRGB Blue(0, 0, 1);
		const ColorRGB Black(0, 0, 0);
		const ColorRGB White(1, 1, 1);

		const ColorRGB Yellow(1, 1, 0);
		const ColorRGB Cyan(0, 1, 1);
		const ColorRGB Magenta(1, 0, 1);

		const ColorRGB Orange(1, 0.5, 0);
		const ColorRGB Lime(0.5, 1, 0);
		const ColorRGB Spring(0, 1, 0.5);
		const ColorRGB Azure(0, 0.5, 1);
		const ColorRGB Purple(0.5, 0, 1);
		const ColorRGB Rose(1, 0, 0.5);
	}

}