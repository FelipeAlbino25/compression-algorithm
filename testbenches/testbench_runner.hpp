#ifndef TESTBENCH_RUNNER_HPP
#define TESTBENCH_RUNNER_HPP

#include <string>

class TestbenchRunner {
public:
    //receives the option for testbench and the file
    void run(const std::string& option, const std::string& fileName);
};

#endif