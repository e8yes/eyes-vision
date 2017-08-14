#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace e8util
{

cv::Mat3f       im2float(cv::Mat const& I);
cv::Mat         float2im(cv::Mat3f const& I);
cv::Mat1f       im2grayfloat(cv::Mat const& I);
void            imshow(cv::Mat const& I, std::string const& name = "default");


}

#endif // UTIL_H
