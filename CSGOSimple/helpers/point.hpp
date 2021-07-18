class SPoint
{
public:
    float x, y;
    constexpr SPoint() : x(0), y(0) {}
    constexpr SPoint(float posX, float posY) : x(posX), y(posY) {}
    constexpr SPoint(const SPoint& pt) = default;


    constexpr bool operator==(const SPoint& rhs) const
    {
        if (this->x != rhs.x) return false;
        if (this->y != rhs.y) return false;
        return true;
    }

    constexpr bool operator!=(const SPoint& rhs) const
    {
        return !(rhs == *this);
    }

    constexpr SPoint& operator +=(const SPoint& p2)
    {
        this->x += p2.x;
        this->y += p2.y;
        return *this;
    }

    constexpr SPoint& operator -=(const SPoint& p2)
    {
        this->x -= p2.x;
        this->y -= p2.y;
        return *this;
    }

    constexpr SPoint operator +(const SPoint& point) const
    {
        auto tmp = *this;
        return tmp += point;
    }

    constexpr SPoint operator -(const SPoint& point) const
    {
        auto tmp = *this;
        return tmp -= point;
    }

    constexpr SPoint operator +(const float& val) const
    {
        SPoint tmp = *this;
        tmp.x += val;
        tmp.y += val;
        return tmp;
    }

    constexpr SPoint operator -(const float& val) const
    {
        SPoint tmp = *this;
        tmp.x -= val;
        tmp.y -= val;
        return tmp;
    }

    constexpr SPoint operator *(const float& val) const
    {
        SPoint tmp = *this;
        tmp.x *= val;
        tmp.y *= val;
        return tmp;
    }
};