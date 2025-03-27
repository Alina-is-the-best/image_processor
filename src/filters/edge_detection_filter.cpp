#include "filter.h"
#include <algorithm>
#include <cmath>

namespace constants {
constexpr float KRedLuminance = 0.299f;
constexpr float KGreenLuminance = 0.587f;
constexpr float KBlueLuminance = 0.114f;
}  // namespace constants

class EdgeDetectionFilter : public IFilter {
public:
    explicit EdgeDetectionFilter(float threshold) : threshold_(threshold) {
    }

    void Apply(Image& image) const override {
        Image grayscale(image.GetWidth(), image.GetHeight());

        for (size_t y = 0; y < image.GetHeight(); ++y) {
            for (size_t x = 0; x < image.GetWidth(); ++x) {
                Pixel p = image.GetPixel(x, y);
                float gray =
                    constants::KRedLuminance * p.r + constants::KGreenLuminance * p.g + constants::KBlueLuminance * p.b;
                grayscale.SetPixel(x, y, {gray, gray, gray});
            }
        }

        Image result(image.GetWidth(), image.GetHeight());
        const float kernel[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

        for (size_t y = 0; y < grayscale.GetHeight(); ++y) {
            for (size_t x = 0; x < grayscale.GetWidth(); ++x) {
                float sum = 0.0f;

                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = std::clamp(static_cast<int>(x) + dx, 0, static_cast<int>(grayscale.GetWidth()) - 1);
                        int ny = std::clamp(static_cast<int>(y) + dy, 0, static_cast<int>(grayscale.GetHeight()) - 1);

                        Pixel p = grayscale.GetPixel(nx, ny);
                        sum += p.r * kernel[dy + 1][dx + 1];
                    }
                }

                float value = std::clamp(sum, 0.0f, 1.0f);
                value = (value > threshold_) ? 1.0f : 0.0f;
                result.SetPixel(x, y, {value, value, value});
            }
        }

        image = result;
    }

private:
    float threshold_;
};

FilterPtr CreateEdgeDetectionFilter(float threshold) {
    return std::make_shared<EdgeDetectionFilter>(threshold);
}