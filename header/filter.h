#pragma once
#include "image.h"
#include <memory>

class IFilter {
public:
    virtual ~IFilter() = default;
    virtual void Apply(Image& image) const = 0;
};

using FilterPtr = std::shared_ptr<IFilter>;

FilterPtr CreateGrayscaleFilter();
FilterPtr CreateNegativeFilter();
FilterPtr CreateCropFilter(size_t width, size_t height);
FilterPtr CreateSharpeningFilter();
FilterPtr CreateEdgeDetectionFilter(float threshold);
FilterPtr CreateGaussianBlurFilter(float sigma);
FilterPtr CreateSepiaFilter();