#pragma once
#include "image.h"
#include <memory>
#include <string>

namespace writer {

class IWriter {
public:
    virtual ~IWriter() = default;
    virtual void Write(const Image& image) const = 0;
};

std::shared_ptr<IWriter> GetBMPWriter(const std::string& path);
}  // namespace writer