#ifndef TEST_RECONSTRUCTION_H
#define TEST_RECONSTRUCTION_H

#include "test.h"

namespace test
{

class test_scanner: public if_test
{
public:
        test_scanner();
        void run() const override;
};

}


#endif // TEST_RECONSTRUCTION_H
