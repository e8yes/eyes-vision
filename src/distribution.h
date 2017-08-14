#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <random>

namespace e8util
{

class rng
{
public:
        rng(unsigned int seed);

        rng();

        float   draw();
        float   inv_G(float t) const;
        float   standard_normal_quantile(float p) const;
        float   normal_quantile(float p, float sigma, float mu) const;
        float   draw_normal(float mu, float sigma);

private:
        std::mt19937                            engine;
        std::uniform_real_distribution<float>   ur;
};

}

#endif // DISTRIBUTION_H
