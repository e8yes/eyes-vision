#ifndef TEST_EXTRACTOR_H
#define TEST_EXTRACTOR_H

#include "test.h"

namespace test
{

class test_extractor: public if_test
{
public:
        test_extractor();
        ~test_extractor();

        void    run() const;
};

}


#endif // TEST_EXTRACTOR_H
