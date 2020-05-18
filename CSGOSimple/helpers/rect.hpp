#pragma once
#include "point.hpp"

class SRect
{
public:
    float left, top, right, bottom;

    constexpr SRect() : left(0), top(0), right(0), bottom(0) { }
    constexpr SRect(float lft, float tp, float rght, float bttm) : left(lft), top(tp), right(rght), bottom(bttm) { }
    constexpr SRect(const SPoint& pt1, const SPoint& pt2) : left(pt1.x), top(pt1.y), right(pt2.x), bottom(pt2.y) { }

    constexpr SRect& operator +=(const SPoint& pt)
    {
        this->top += pt.y;
        this->bottom += pt.y;
        this->left += pt.x;
        this->right += pt.x;
        return *this;
    }
    constexpr SRect& operator -=(const SPoint& pt)
    {
        this->top -= pt.y;
        this->bottom -= pt.y;
        this->left -= pt.x;
        this->right -= pt.x;
        return *this;
    }

    constexpr float    Height() const { return this->bottom - this->top; }
    constexpr float    Width()  const { return this->right - this->left; }
    constexpr SPoint   Pos()    const { return SPoint(left, top); }
    constexpr SPoint   Mid()    const { return SPoint((left + right) / 2, (top + bottom) / 2); }
};