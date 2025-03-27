#include "filter.h"
#include <algorithm>

class SharpeningFilter : public IFilter {
public:
    SharpeningFilter() = default;

    void Apply(Image& image) const override {
        const Image original = image;
        const int kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

        for (size_t y = 0; y < image.GetHeight(); ++y) {
            for (size_t x = 0; x < image.GetWidth(); ++x) {
                float r = 0.0f;
                float g = 0.0f;
                float b = 0.0f;

                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        const int nx = std::clamp(static_cast<int>(x) + dx, 0, static_cast<int>(image.GetWidth()) - 1);
                        const int ny = std::clamp(static_cast<int>(y) + dy, 0, static_cast<int>(image.GetHeight()) - 1);

                        const Pixel p = original.GetPixel(nx, ny);
                        const float weight = static_cast<float>(kernel[dy + 1][dx + 1]);

                        r += p.r * weight;
                        g += p.g * weight;
                        b += p.b * weight;
                    }
                }

                r = std::clamp(r, 0.0f, 1.0f);
                g = std::clamp(g, 0.0f, 1.0f);
                b = std::clamp(b, 0.0f, 1.0f);

                image.SetPixel(x, y, {r, g, b});
            }
        }
    }
};

FilterPtr CreateSharpeningFilter() {
    return std::make_shared<SharpeningFilter>();
}