#pragma once
#include <vector>
#include <stdexcept>

struct Pixel {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

class Image {
public:
    Image(size_t width, size_t height);
    Image(size_t width, size_t height, const std::vector<std::vector<Pixel>>& data);

    size_t GetWidth() const {
        return width_;
    }
    size_t GetHeight() const {
        return height_;
    }

    Pixel GetPixel(size_t x, size_t y) const;
    void SetPixel(size_t x, size_t y, const Pixel& pixel);

private:
    size_t width_;
    size_t height_;
    std::vector<Pixel> data_;
};