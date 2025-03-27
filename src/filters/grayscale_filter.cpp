#include "filter.h"
#include "image.h"
#include <algorithm>  // для std::clamp

namespace constants {
constexpr float KRedLuminance = 0.299f;
constexpr float KGreenLuminance = 0.587f;
constexpr float KBlueLuminance = 0.114f;
}  // namespace constants

class GrayscaleFilter : public IFilter {
public:
    void Apply(Image& image) const override {
        const size_t width = image.GetWidth();
        const size_t height = image.GetHeight();

        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                const Pixel p = image.GetPixel(x, y);
                float gray =
                    constants::KRedLuminance * p.r + constants::KGreenLuminance * p.g + constants::KBlueLuminance * p.b;

                gray = std::clamp(gray, 0.0f, 1.0f);

                image.SetPixel(x, y, {gray, gray, gray});
            }
        }
    }
};

FilterPtr CreateGrayscaleFilter() {
    return std::make_shared<GrayscaleFilter>();
}