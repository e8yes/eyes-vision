#ifndef TEST_CNNFEATURE_H
#define TEST_CNNFEATURE_H

#include "test.h"

namespace test
{

class test_cnnfeature: public if_test
{
public:
        test_cnnfeature();
        ~test_cnnfeature() override;
        void    run() const override;
};

}

#endif // TEST_CNNFEATURE_H
