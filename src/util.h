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

        float
        draw()
        {
                return ur(engine);
        }

        float
        inv_G(float t)
        {
                float const c[] = {2.515517, 0.802853, 0.010328};
                float const d[] = {1.432788, 0.189269, 0.001308};
                return t - ((c[2]*t + c[1])*t + c[0]) / (((d[2]*t + d[1])*t + d[0])*t + 1.0);
        }

        float
        standard_normal_quantile(double p)
        {
                if (p < 0.5)
                        // F^-1(p) ~ -G^-1(p)
                        return -inv_G(std::sqrt(-2.0*std::log(p)));
                else
                        // F^-1(p) ~ G^-1(1-p)
                        return inv_G(std::sqrt(-2.0*std::log(1.0 - p)));
        }

        float
        normal_quantile(float p, float sigma, float mu)
        {
                float x = standard_normal_quantile(p);
                return x*sigma + mu;
        }

        float
        draw_normal(float mu, float sigma)
        {
                return normal_quantile(draw(), sigma, mu);
        }

private:
        std::mt19937                            engine;
        std::uniform_real_distribution<float>   ur;
};

}

#endif // UTIL_H
