#ifndef CAMERA_H
#define CAMERA_H

#include "opencv2/core.hpp"

namespace e8
{

class camera
{
public:
        camera(float f, cv::Vec3f const& t, cv::Matx33f const& rot);

        float                   f() const;
        cv::Vec3f               t() const;
        cv::Matx33f             r() const;
        std::vector<cv::Vec2f>  proj(std::vector<cv::Vec3f> const& pts) const;
private:
        float           m_f;
        cv::Vec3f       m_t;
        cv::Matx33f     m_r;
};

}

#endif // CAMERA_H
