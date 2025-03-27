#include "image.h"
#include <algorithm>

Image::Image(size_t width, size_t height) : width_(width), height_(height), data_(width * height) {
    if (width == 0 || height == 0) {
        throw std::invalid_argument("Image dimensions cannot be zero");
    }
}

Image::Image(size_t width, size_t height, const std::vector<std::vector<Pixel>>& data) : Image(width, height) {
    if (data.size() != height || (height > 0 && data[0].size() != width)) {
        throw std::invalid_argument("Input data dimensions mismatch");
    }

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            data_[y * width_ + x] = data[y][x];
        }
    }
}

Pixel Image::GetPixel(size_t x, size_t y) const {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    return data_[y * width_ + x];
}

void Image::SetPixel(size_t x, size_t y, const Pixel& pixel) {
    if (x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    data_[y * width_ + x] = pixel;
}