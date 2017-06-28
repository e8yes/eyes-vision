#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <opencv2/core.hpp>

namespace e8
{

class camera
{
public:
        camera();
        camera(float f, cv::Vec2f const& c, cv::Vec3f const& t, cv::Matx33f const& rot);

        float                   f() const;
        cv::Vec2f               c() const;
        cv::Vec3f               t() const;
        cv::Matx33f             r() const;
        cv::Matx33f             inv_r() const;
        std::vector<cv::Vec2f>  proj(std::vector<cv::Vec3f> const& pts) const;
private:
        cv::Matx33f     m_inv_r;
        float           m_f;
        cv::Vec2f       m_c;
        cv::Vec3f       m_t;
        cv::Matx33f     m_r;
};

std::ostream&    operator<<(std::ostream& os, camera const& cam);

}

#endif // CAMERA_H
