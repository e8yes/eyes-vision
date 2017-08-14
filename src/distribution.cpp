#include "distribution.h"


e8util::rng::rng(unsigned int seed):
        engine(seed), ur(0,1)
{
}

e8util::rng::rng():
        ur(0,1)
{
        std::random_device rd;
        engine.seed(rd());
}

float
e8util::rng::draw()
{
        return ur(engine);
}

float
e8util::rng::inv_G(float t) const
{
        float const c[] = {2.515517, 0.802853, 0.010328};
        float const d[] = {1.432788, 0.189269, 0.001308};
        return t - ((c[2]*t + c[1])*t + c[0]) / (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}

float
e8util::rng::standard_normal_quantile(float p) const
{
        if (p < 0.5)
                // F^-1(p) ~ -G^-1(p)
                return -inv_G(std::sqrt(-2.0*std::log(p)));
        else
                // F^-1(p) ~ G^-1(1-p)
                return inv_G(std::sqrt(-2.0*std::log(1.0 - p)));
}

float
e8util::rng::normal_quantile(float p, float sigma, float mu) const
{
        float x = standard_normal_quantile(p);
        return x*sigma + mu;
}

float
e8util::rng::draw_normal(float mu, float sigma)
{
        return normal_quantile(draw(), sigma, mu);
}
