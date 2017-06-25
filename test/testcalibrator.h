#ifndef TEST_CALIBRATOR_H
#define TEST_CALIBRATOR_H

#include "test.h"

namespace test
{

class test_calibrator: public if_test
{
public:
        test_calibrator();
        ~test_calibrator();

        void    run() const override;
};

}

#endif // TEST_CALIBRATOR_H
