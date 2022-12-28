#ifndef PMATH_H
#define PMATH_H

#include <math.h>
#include <algorithm>

const double PI = 3.141592741;
const double EPSILON = 0.0001;

struct Vec
{
    double x, y;

    Vec(){};

    Vec(double X, double Y)
    {
        x = X, y = Y;
    }

    void Set(double X, double Y)
    {
        x = X, y = Y;
    }

    Vec operator-(void) const
    {
        return Vec(-x, -y);
    }

    Vec operator*(double s) const
    {
        return Vec(x * s, y * s);
    }

    Vec operator/(double s) const
    {
        return Vec(x / s, y / s);
    }

    void operator*=(double s)
    {
        x *= s;
        y *= s;
    }

    Vec operator+(const Vec &rhs) const
    {
        return Vec(x + rhs.x, y + rhs.y);
    }

    Vec operator+(double s) const
    {
        return Vec(x + s, y + s);
    }

    void operator+=(const Vec &rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    Vec operator-(const Vec &rhs) const
    {
        return Vec(x - rhs.x, y - rhs.y);
    }

    void operator-=(const Vec &rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    double squared_vec_length(void) const
    {
        return x * x + y * y;
    }

    double vect_length(void) const
    {
        return std::sqrt(x * x + y * y);
    }

    void Rotate(double radians)
    {
        double c = std::cos(radians);
        double s = std::sin(radians);

        double xp = x * c - y * s;
        double yp = x * s + y * c;

        x = xp;
        y = yp;
    }

    void Normalize(void)
    {
        double len = vect_length();

        if (len > EPSILON)
        {
            double invLen = 1.0f / len;
            x *= invLen;
            y *= invLen;
        }
    }
};

inline double Random(double l, double h)
{
    double a = (double)rand();
    a /= RAND_MAX;
    a = (h - l) * a + l;
    return a;
}

inline Vec operator*(double s, const Vec &v)
{
    return Vec(s * v.x, s * v.y);
}

inline double Cross(const Vec &a, const Vec &b)
{
    return a.x * b.y - a.y * b.x;
}

inline Vec Cross(const Vec &v, double a)
{
    return Vec(a * v.y, -a * v.x);
}

inline Vec Cross(double a, const Vec &v)
{
    return Vec(-a * v.y, a * v.x);
}

inline double Dot(const Vec &a, const Vec &b)
{
    return a.x * b.x + a.y * b.y;
}

inline double Sqr(double a)
{
    return a * a;
}

inline bool Equal(double a, double b)
{
    return std::abs(a - b) <= EPSILON;
}

inline double Clamp(double min, double max, double a)
{
    if (a < min)
        return min;
    if (a > max)
        return max;
    return a;
}

const double gravityScale = 5.0;
const Vec gravity(0, 10.0 * gravityScale);
const double dt = 1.0 / 60.0;

struct Mat2
{
    union
    {
        struct
        {
            double m00, m01;
            double m10, m11;
        };

        double m[2][2];
        double v[4];
    };

    Mat2() {}
    Mat2(double radians)
    {
        double c = std::cos(radians);
        double s = std::sin(radians);

        m00 = c;
        m01 = -s;
        m10 = s;
        m11 = c;
    }

    Mat2(double a, double b, double c, double d)
        : m00(a), m01(b), m10(c), m11(d)
    {
    }

    void Set(double radians)
    {
        double c = std::cos(radians);
        double s = std::sin(radians);

        m00 = c;
        m01 = -s;
        m10 = s;
        m11 = c;
    }

    Mat2 Abs(void) const
    {
        return Mat2(std::abs(m00), std::abs(m01), std::abs(m10), std::abs(m11));
    }

    Vec AxisX(void) const
    {
        return Vec(m00, m10);
    }

    Vec AxisY(void) const
    {
        return Vec(m01, m11);
    }

    Mat2 Transpose(void) const
    {
        return Mat2(m00, m10, m01, m11);
    }

    const Vec operator*(const Vec &rhs) const
    {
        return Vec(m00 * rhs.x + m01 * rhs.y, m10 * rhs.x + m11 * rhs.y);
    }

    const Mat2 operator*(const Mat2 &rhs) const
    {
        // [00 01]  [00 01]
        // [10 11]  [10 11]

        return Mat2(
            m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
            m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1],
            m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
            m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]);
    }
};

#endif // PMATH_H
