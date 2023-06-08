#pragma once
#include <cstdint>

class Color
{
public:
	float r, g, b, a;

	constexpr Color() : r(255), g(255), b(255), a(255) {}
	constexpr Color(float r, float g, float b) : r(r), g(g), b(b), a(255) {}
	constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
	constexpr Color(const uint32_t colorCodo)
	{
		r = (float)((colorCodo >> 16) & 0xff);
		g = (float)((colorCodo >> 8) & 0xff);
		b = (float)(colorCodo & 0xff);
		a = 255;
	}

	const static Color sRed;
	const static Color sGreen;
	const static Color sBlue;
	const static Color sYellow;
	const static Color sWhite;
	const static Color sBlack;

	Color GetColorTo01() { return { r / 255,g / 255,b / 255 ,a / 255 }; }

	Color operator=(const Color other);

	// 比較演算子のオーバーロード
	bool operator!=(const Color other) const;
	Color& operator++();

	Color operator+(const float num) const;		 // 一つの値との足し算
	Color operator-(const float num) const;		 // 一つの値との引き算
	Color operator*(const float num) const;         // 一つの値との掛け算
	Color operator/(const float num) const;         // 一つの値との割り算

};

