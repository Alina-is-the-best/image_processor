#include "filter.h"
#include "image.h"
#include <algorithm>
#include <vector>
#include <cmath>

namespace {
constexpr float KSigmaMultiplier = 3.0f;
constexpr float KTwoValue = 2.0f;
}  // namespace

class GaussianBlurFilter : public IFilter {
public:
    explicit GaussianBlurFilter(float sigma) : sigma_(sigma) {
        radius_ = static_cast<int>(std::ceil(KSigmaMultiplier * sigma_));
        kernel_.resize(2 * radius_ + 1);
        float sum = 0.0f;
        for (int i = -radius_; i <= radius_; ++i) {
            const float exponent = -static_cast<float>(i * i) / (KTwoValue * sigma_ * sigma_);
            kernel_[i + radius_] = std::exp(exponent);
            sum += kernel_[i + radius_];
        }
        for (float& k : kernel_) {
            k /= sum;
        }
    }

    void Apply(Image& image) const override {
        Image temp(image.GetWidth(), image.GetHeight());
        Apply1D(image, temp, true);
        Apply1D(temp, image, false);
    }

private:
    void Apply1D(const Image& src, Image& dst, bool horizontal) const {
        for (size_t y = 0; y < src.GetHeight(); ++y) {
            for (size_t x = 0; x < src.GetWidth(); ++x) {
                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;

                for (int i = -radius_; i <= radius_; ++i) {
                    int nx = 0;
                    int ny = 0;

                    if (horizontal) {
                        nx = std::clamp(static_cast<int>(x) + i, 0, static_cast<int>(src.GetWidth()) - 1);
                        ny = static_cast<int>(y);
                    } else {
                        nx = static_cast<int>(x);
                        ny = std::clamp(static_cast<int>(y) + i, 0, static_cast<int>(src.GetHeight()) - 1);
                    }

                    const Pixel p = src.GetPixel(nx, ny);
                    const float weight = kernel_[i + radius_];
                    r += p.r * weight;
                    g += p.g * weight;
                    b += p.b * weight;
                }

                dst.SetPixel(x, y, {r, g, b});
            }
        }
    }

    float sigma_;
    int radius_;
    std::vector<float> kernel_;
};

FilterPtr CreateGaussianBlurFilter(float sigma) {
    return std::make_shared<GaussianBlurFilter>(sigma);
}