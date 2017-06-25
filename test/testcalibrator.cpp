#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "src/calibrator.h"
#include "testcalibrator.h"


test::test_calibrator::test_calibrator()
{
}

test::test_calibrator::~test_calibrator()
{
}

void
test::test_calibrator::run() const
{
        cv::Mat1b checker = cv::imread("res/checker/checker3.png", cv::IMREAD_GRAYSCALE);

        float const f = 1000.0f;
        cv::Vec3f t(0, 0, 0);
        cv::Matx33f r(1, 0, 0, 0, 1, 0, 0, 0, 1);

        e8::camera init(f, t, r);
        e8::checker_calibrator calib(checker, 25, 2*2, init);
        cv::Mat detect_map;
        if (calib.detect(detect_map)) {
                cv::imwrite("detect_map.png", detect_map);
                std::cout << "detection phrase passed" << std::endl;
        } else {
                cv::imwrite("detect_map.png", detect_map);
                throw std::string("detection phrase failed");
        }
        // e8::camera calibrated = calib.calibrate();
}
