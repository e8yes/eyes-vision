#include <random>
#include "test.h"


std::mt19937 engine;
std::uniform_real_distribution<float>   ur(0,1);

test::if_test::if_test()
{
}

test::if_test::~if_test()
{
}

float
test::if_test::draw_rand() const
{
        return ur(engine);
}
