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
        struct keypoint
        {
                keypoint(cv::Vec2i const& kp, std::set<unsigned> const& lines):
                        kp(kp), lines(lines)
                {
                }

                cv::Vec2i               kp;
                std::set<unsigned>      lines;
        };

        bool                    is_equivalent_lines(cv::Vec4i const& a, cv::Vec4i const& b, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const;
        std::vector<cv::Vec4i>  find_principle_axes(std::vector<cv::Vec4i> const& lines, std::vector<keypoint> const& kps) const;
        std::vector<cv::Vec4i>  partition(std::vector<cv::Vec2i> planes[3], std::vector<cv::Vec4i> const& lines, std::vector<keypoint> const& kps) const;
        cv::Matx33f             homography(std::vector<cv::Vec2i> const& kps) const;
        std::vector<cv::Vec2f>  correspondences(std::vector<cv::Vec2i> const& kps) const;
        std::vector<cv::Vec4i>  hough_lines(cv::Mat1b const& edges, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const;
        std::vector<cv::Vec4i>  merge(std::vector<cv::Vec4i> const& lines, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const;
        std::vector<cv::Vec4i>  vectorize(cv::Mat1b const& edges, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const;
        std::vector<keypoint>   keypoints(std::vector<cv::Vec4i> const& lines, cv::Vec2i const& pmin, cv::Vec2i const& pmax) const;
        cv::Mat1b               binarize(cv::Mat1b const& image, cv::Vec2i& pmin, cv::Vec2i& pmax) const;

        cv::Mat1b const&        m_checker;
        float                   m_width;
        unsigned                m_grids;
        camera const&           m_init;

        std::vector<cv::Vec2i>  m_planes[3];
};

}

#endif // CALIBRATOR_H
