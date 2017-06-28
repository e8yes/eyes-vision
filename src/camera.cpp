#include <opencv2/core.hpp>
#include "camera.h"


e8::camera::camera():
        m_f(0.0f), m_t(0.0f), m_r(cv::Matx33f::eye())
{
}

e8::camera::camera(float f, cv::Vec2f const& c, cv::Vec3f const& t, cv::Matx33f const& rot):
        m_f(f), m_c(c), m_t(t), m_r(rot), m_inv_r(rot.t())
{
}

float
e8::camera::f() const
{
        return m_f;
}

cv::Vec2f
e8::camera::c() const
{
        return m_c;
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

cv::Matx33f
e8::camera::inv_r() const
{
        return m_inv_r;
}

std::vector<cv::Vec2f>
e8::camera::proj(std::vector<cv::Vec3f> const& pts) const
{
        std::vector<cv::Vec2f> proj_pts(pts.size());
        for (unsigned i = 0; i < pts.size(); i ++) {
                cv::Vec3f const& p = m_inv_r*(pts[i] - m_t);
                proj_pts[i] = m_f*cv::Vec2f(p[0]/p[2], p[1]/p[2]);
        }
        return proj_pts;
}

std::ostream&
e8::operator<<(std::ostream& os, camera const& cam)
{
        os << "camera=[f=" << cam.f() << ",T=" << cam.t() << ",R=" << cam.r() << "]";
        return os;
}
