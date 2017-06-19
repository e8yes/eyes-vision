#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <string>
#include <map>
#include "test.h"

namespace test
{

class test_runner
{
public:
        test_runner();
        ~test_runner();

        void run_all() const;
        void run(std::string const& t) const;

        void add(std::string const& t, if_test* inst, bool status = true);
        void enable(std::string const& t);
        void disable(std::string const& t);
private:
        std::map<std::string, if_test*>         m_tests;
        std::map<std::string, bool>             m_status;
};

test_runner    load(int argc, char** argv);

}


#endif // TEST_RUNNER_H
