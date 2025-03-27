#include "filter.h"
#include "image.h"

namespace constants {
constexpr float KSepiaRedR = 0.393f;
constexpr float KSepiaRedG = 0.769f;
constexpr float KSepiaRedB = 0.189f;
constexpr float KSepiaGreenR = 0.349f;
constexpr float KSepiaGreenG = 0.686f;
constexpr float KSepiaGreenB = 0.168f;
constexpr float KSepiaBlueR = 0.272f;
constexpr float KSepiaBlueG = 0.534f;
constexpr float KSepiaBlueB = 0.131f;

constexpr float KPixelMaxFloat = 1.0f;
constexpr float KPixelMinFloat = 0.0f;
}  // namespace constants

class SepiaFilter : public IFilter {
public:
    void Apply(Image& image) const override {
        for (size_t y = 0; y < image.GetHeight(); ++y) {
            for (size_t x = 0; x < image.GetWidth(); ++x) {
                Pixel p = image.GetPixel(x, y);
                Pixel sepia_p = ApplySepia(p);
                image.SetPixel(x, y, sepia_p);
            }
        }
    }

private:
    Pixel ApplySepia(const Pixel& p) const {
        float r = p.r;
        float g = p.g;
        float b = p.b;

        float new_r = constants::KSepiaRedR * r + constants::KSepiaRedG * g + constants::KSepiaRedB * b;
        float new_g = constants::KSepiaGreenR * r + constants::KSepiaGreenG * g + constants::KSepiaGreenB * b;
        float new_b = constants::KSepiaBlueR * r + constants::KSepiaBlueG * g + constants::KSepiaBlueB * b;

        new_r = std::min(constants::KPixelMaxFloat, std::max(constants::KPixelMinFloat, new_r));
        new_g = std::min(constants::KPixelMaxFloat, std::max(constants::KPixelMinFloat, new_g));
        new_b = std::min(constants::KPixelMaxFloat, std::max(constants::KPixelMinFloat, new_b));

        return {new_r, new_g, new_b};
    }
};

FilterPtr CreateSepiaFilter() {
    return std::make_shared<SepiaFilter>();
}