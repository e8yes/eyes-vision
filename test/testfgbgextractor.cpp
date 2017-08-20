#include <opencv2/imgcodecs.hpp>
#include "src/objectextractor.h"
#include "testfgbgextractor.h"


test::test_fgbg_extractor::test_fgbg_extractor()
{
}

test::test_fgbg_extractor::~test_fgbg_extractor()
{
}


void
test::test_fgbg_extractor::run() const
{
        cv::Mat I0 = cv::imread("res/fgbg/calib_fg.jpg", cv::IMREAD_COLOR);
        cv::Mat I1 = cv::imread("res/fgbg/calib_bg.jpg", cv::IMREAD_COLOR);

        cv::Mat3f fg(I0.size());
        cv::Mat3f bg(I1.size());
        I0.convertTo(fg, CV_32FC3, 1/255.0f);
        I1.convertTo(bg, CV_32FC3, 1/255.0f);

        e8::fgbg_object_extractor extractor(bg, 1, 0.1f, 0.1f);
        void* intermediate = extractor.extract_intermediate(fg);
        cv::Mat1b const& mask = extractor.extract_mapping(intermediate);

        cv::imwrite("mask.png", mask);

        delete intermediate;
}
