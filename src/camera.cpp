#include "camera.h"


e8::camera::camera(float f, cv::Vec3f const& t, cv::Matx33f const& rot):
        m_f(f), m_t(t), m_r(rot)
{
}

float
e8::camera::f() const
{
        return m_f;
}

cv::Vec3f
e8::camera::t() const
{
        return m_t;
}

cv::Matx33f
e8::camera::r() const
{
        return m_r;
}

std::vector<cv::Vec2f>
e8::camera::proj(std::vector<cv::Vec3f> const& pts) const
{
}
