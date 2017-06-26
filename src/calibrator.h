#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <map>
#include <set>
#include <opencv2/core.hpp>
#include "camera.h"

namespace e8
{

class if_calibrator
{
public:
        if_calibrator();
        virtual ~if_calibrator();

        virtual bool    detect(cv::Mat& detect_map) = 0;
        virtual bool    calibrate(camera& cam,  cv::Mat& project_map) const = 0;
};

class epipolar_calibrator: public if_calibrator
{
public:
        epipolar_calibrator();
        ~epipolar_calibrator();

        bool    detect(cv::Mat& detect_map) override;
        bool    calibrate(camera& cam, cv::Mat& project_map) const override;
};


class checker_calibrator: public if_calibrator
{
public:
        checker_calibrator(cv::Mat1b const& checker, float width, unsigned num_grids, camera const& init);
        ~checker_calibrator();

        bool            detect(cv::Mat& detect_map) override;
        bool            calibrate(camera& cam, cv::Mat& project_map) const override;
protected:
        cv::Mat1b const&        m_checker;
        float                   m_width;
        unsigned                m_grids;
        camera const&           m_init;

        float                   m_thickness;
        cv::Vec2i               m_scale;

        std::vector<cv::Vec2i>  m_planes[3];
        cv::Vec4i               m_axes[3];
};

}

#endif // CALIBRATOR_H
