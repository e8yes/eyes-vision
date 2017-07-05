#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <random>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace e8util
{

cv::Mat3f       im2float(cv::Mat const& I);
cv::Mat         float2im(cv::Mat3f const& I);
cv::Mat1f       im2grayfloat(cv::Mat const& I);
void            imshow(cv::Mat const& I, std::string const& name = "default");

struct island
{
        island(unsigned id):
                id(id), area(0)
        {
        }

        unsigned char                   id;
        std::vector<cv::Point2i>        pixel_locs;
        unsigned                        area;
};

std::vector<island>     islands(cv::Mat1b const& bw, cv::Mat1b& id_map, unsigned target = 0xFF);

class rng
{
public:
        rng(unsigned int seed):
                engine(seed), ur(0,1)
        {
        }

        rng():
                ur(0,1)
        {
                std::random_device rd;
                engine.seed(rd());
        }

        float draw()
        {
                return ur(engine);
        }
private:
        std::mt19937                            engine;
        std::uniform_real_distribution<float>   ur;
};

}

#endif // UTIL_H
