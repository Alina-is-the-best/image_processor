#include "filter.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr float KMaxColorValue = 255.0f;
constexpr float KMinNormalizedValue = 0.0f;
constexpr float KMaxNormalizedValue = 1.0f;
}  // namespace

class NegativeFilter : public IFilter {
public:
    void Apply(Image& image) const override {
        for (size_t y = 0; y < image.GetHeight(); ++y) {
            for (size_t x = 0; x < image.GetWidth(); ++x) {
                Pixel p = image.GetPixel(x, y);

                p.r = std::round((KMaxNormalizedValue - p.r) * KMaxColorValue) / KMaxColorValue;
                p.g = std::round((KMaxNormalizedValue - p.g) * KMaxColorValue) / KMaxColorValue;
                p.b = std::round((KMaxNormalizedValue - p.b) * KMaxColorValue) / KMaxColorValue;

                p.r = std::clamp(p.r, KMinNormalizedValue, KMaxNormalizedValue);
                p.g = std::clamp(p.g, KMinNormalizedValue, KMaxNormalizedValue);
                p.b = std::clamp(p.b, KMinNormalizedValue, KMaxNormalizedValue);

                image.SetPixel(x, y, p);
            }
        }
    }
};

FilterPtr CreateNegativeFilter() {
    return std::make_shared<NegativeFilter>();
}