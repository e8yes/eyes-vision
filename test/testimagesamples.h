#ifndef TEST_IMAGE_SAMPLES_H
#define TEST_IMAGE_SAMPLES_H

#include "test.h"

namespace test
{

class test_image_samples: public if_test
{
public:
        test_image_samples();
        ~test_image_samples() override;
        void    run() const override;
};

}

#endif // TEST_IMAGE_SAMPLES_H
