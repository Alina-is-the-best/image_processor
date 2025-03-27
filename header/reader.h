#pragma once

#include <memory>
#include <string>
#include "image.h"

namespace reader {

class IReader {
public:
    virtual ~IReader() = default;
    virtual Image GetImage() const = 0;
};

std::shared_ptr<IReader> GetFileReader();
std::shared_ptr<IReader> GetConsoleReader();
std::shared_ptr<IReader> GetBMPReader(const std::string& path);

}  // namespace reader