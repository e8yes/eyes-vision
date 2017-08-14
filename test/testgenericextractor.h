#ifndef TEST_GENERIC_EXTRACTOR_H
#define TEST_GENERIC_EXTRACTOR_H

#include "test.h"

namespace test
{

class test_generic_extractor: public if_test
{
public:
        test_generic_extractor();
        ~test_generic_extractor() override;
        void    run() const override;
};

}


#endif // TEST_GENERIC_EXTRACTOR_H
