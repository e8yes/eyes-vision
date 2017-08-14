#ifndef TEST_FGBGEXTRACTOR_H
#define TEST_FGBGEXTRACTOR_H

#include "test.h"

namespace test
{

class test_fgbg_extractor: public if_test
{
public:
        test_fgbg_extractor();
        ~test_fgbg_extractor();

        void    run() const;
};

}


#endif // TEST_FGBGEXTRACTOR_H
