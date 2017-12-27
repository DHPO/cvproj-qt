#include "./color_colorspace.h"

Vec<uchar, 3> map(Vec<uchar, 1> data)
{
    if (data[0] < 128)
        return Vec<uchar, 3>(255 - 2 * data[0], 2 * data[0], 0);
    else
        return Vec<uchar, 3>(0, 2 * data[0], 255 - 2 * data[0]);
}

Vec<float, 3> RGBToHSV(Vec<uchar, 3> rgb)
{
    float r = rgb[2], g = rgb[1], b = rgb[0];

    if (r == g && r == b)
        return Vec<float, 3>(0, 0, r);
    if (r > g && g >= b)
        /* max: r, min: b */
        return Vec<float, 3>(60.0 * (g - b) / (r - b), (1 - b / r), r);
    if (r > b && b > g)
        /* max: r, min: g */
        return Vec<float, 3>(60.0 * (g - b) / (r - g) + 360, (1 - g / r), r);
    if (g > r && r >= b)
        /* max: g, min: b */
        return Vec<float, 3>(60.0 * (b - r) / (g - b) + 120, (1 - b / g), g);
    if (g > b && b > r)
        /* max: g, min: r */
        return Vec<float, 3>(60.0 * (b - r) / (g - r) + 120, (1 - r / g), g);
    if (b > r && r >= g)
        /* max: b, min: g */
        return Vec<float, 3>(60.0 * (r - g) / (b - g) + 240, (1 - g / b), b);
    if (b > g && g > r)
        /* max: b, min: r */
        return Vec<float, 3>(60.0 * (r - g) / (b - r) + 240, (1 - r / b), b);
}

Vec<uchar, 3> HSVToRGB(Vec<float, 3> hsv)
{
    float h = hsv[0], s = hsv[1], v = hsv[2];

    int hi = int(h / 60) % 6;
    float f = h / 60 - hi;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch(hi) {
        case 0:
            return Vec<uchar, 3> (p, t, v);
        case 1:
            return Vec<uchar, 3> (p, v, q);
        case 2:
            return Vec<uchar, 3> (t, v, p);
        case 3:
            return Vec<uchar, 3> (v, q, p);
        case 4:
            return Vec<uchar, 3> (v, p, t);
        case 5:
            return Vec<uchar, 3> (q, p, v);
    }
}

Vec<float, 3> RGBToHSVConverter::map(Vec<uchar, 3> rgb)
{
    return RGBToHSV(rgb);
}

Vec<uchar, 3> HSVToRGBConverter::map(Vec<float, 3> hsv)
{
    return HSVToRGB(hsv);
}