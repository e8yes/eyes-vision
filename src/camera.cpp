#include <cmath>
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

static cv::Vec2f
proj(cv::Vec3f const& p, cv::Vec2f const& c, cv::Matx33f const& inv_r, cv::Vec3f const& t, float f)
{
        cv::Vec3f const& proj_p = inv_r*(p - t);
        return -f*cv::Vec2f(proj_p[0]/proj_p[2], proj_p[1]/proj_p[2]) + c;
}

std::vector<cv::Vec2f>
e8::camera::proj(std::vector<cv::Vec3f> const& pts) const
{
        std::vector<cv::Vec2f> proj_pts(pts.size());
        for (unsigned i = 0; i < pts.size(); i ++)
                proj_pts[i] = ::proj(pts[i], m_c, m_inv_r, m_t, m_f);
        return proj_pts;
}


void
e8::camera::proj_sqerr(double* error, std::vector<e8::point_corr> const& corrs) const
{
        for (unsigned i = 0; i < corrs.size(); i ++) {
                cv::Vec2f const& p = ::proj(corrs[i].b, m_c, m_inv_r, m_t, m_f);
                cv::Vec2f d = p - cv::Vec2f(corrs[i].a[0], corrs[i].a[1]);
                error[i] = d.dot(d);
        }
}

std::ostream&
e8::operator<<(std::ostream& os, camera const& cam)
{
        os << "camera=[f=" << cam.f() << ",T=" << cam.t() << ",R=" << cam.r() << "]";
        return os;
}
