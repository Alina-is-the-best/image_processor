#pragma once

#include <string>
#include <vector>
#include "filter.h"

class ArgParser {
public:
    ArgParser(int argc, char** argv);

    std::string GetInputPath() const {
        return input_path_;
    }
    std::string GetOutputPath() const {
        return output_path_;
    }
    std::vector<FilterPtr> GetFilters() const {
        return filters_;
    }

private:
    void ParseFilters(int& i, int argc, char** argv);

    std::string input_path_;
    std::string output_path_;
    std::vector<FilterPtr> filters_;
};