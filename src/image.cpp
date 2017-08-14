#include <algorithm>
#include "image.h"

cv::Mat3f
e8util::im2float(cv::Mat const& I)
{
        cv::Mat3f J(I.size());
        I.convertTo(J, CV_32FC3, 1/255.0f);
        return J;
}

cv::Mat1f
e8util::im2grayfloat(cv::Mat const& I)
{
        cv::Mat1f J(I.size());
        cv::Mat1b gray;
        cv::cvtColor(I, gray, CV_BGR2GRAY);
        gray.convertTo(J, CV_32F, 1/255.0f);
        return J;
}

cv::Mat
e8util::float2im(cv::Mat3f const& I)
{
        cv::Mat J;
        I.convertTo(J, CV_8UC3, 255.0f);
        return J;
}

void
e8util::imshow(cv::Mat const& I, std::string const& name)
{
        cv::namedWindow(name, cv::WINDOW_NORMAL);
        cv::imshow(name, I);
        cv::waitKey(0);
}
