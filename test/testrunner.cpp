#include <vector>
#include <cstring>
#include <iostream>
#include "testrunner.h"


test::test_runner::test_runner()
{
}

test::test_runner::~test_runner()
{
        for (std::pair<std::string, if_test*> const& e: m_tests) {
                delete e.second;
        }
}

void
test::test_runner::run_all() const
{
        for (std::pair<std::string, if_test*> const& e: m_tests) {
                run(e.first);
        }
}

void
test::test_runner::run(std::string const& t) const
{
        if (m_status.at(t)) {
                m_tests.at(t)->run();
                std::cout << "Test case " << t << " has passed." << std::endl;
        }
}

void
test::test_runner::add(std::string const& t, if_test* inst, bool status)
{
        m_tests[t] = inst;
        m_status[t] = status;
}

void
test::test_runner::enable(std::string const& t)
{
        m_status[t] = true;
}

void
test::test_runner::disable(std::string const& t)
{
        m_status[t] = false;
}


test::test_runner
test::load(int argc, char** argv)
{
        test_runner runner;

        for (int i = 1; i < argc; i ++) {
                if ((!std::strcmp(argv[i], "--test") || !std::strcmp(argv[i], "-t")) && (i + 1 < argc)) {
                        runner.enable(argv[i + 1]);
                }
        }
        return runner;
}
