#include "filter.h"
#include "image.h"

class CropFilter : public IFilter {
public:
    CropFilter(size_t width, size_t height) : width_(width), height_(height) {
    }

    void Apply(Image& image) const override {
        size_t new_width = std::min(width_, image.GetWidth());
        size_t new_height = std::min(height_, image.GetHeight());
        Image result(new_width, new_height);

        for (size_t y = 0; y < new_height; ++y) {
            for (size_t x = 0; x < new_width; ++x) {
                result.SetPixel(x, y, image.GetPixel(x, y));
            }
        }
        image = result;
    }

private:
    size_t width_;
    size_t height_;
};

FilterPtr CreateCropFilter(size_t width, size_t height) {
    return std::make_shared<CropFilter>(width, height);
}