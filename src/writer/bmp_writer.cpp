#include "writer.h"
#include "image.h"
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <algorithm>

namespace {
constexpr int KBmpBitsPerPixel = 24;
constexpr int KBmpCompressionNone = 0;
constexpr float KColorNormalizationFactor = 255.0f;
constexpr int KBytesPerPixel = 3;
constexpr int KPaddingAlignment = 4;
constexpr int KBmpHeaderSize = 54;
constexpr float KRoundingOffset = 0.5f;
constexpr int KPixelsPerMeter = 2835;

inline uint8_t SafeColorConvert(float value) {
    float normalized = value * KColorNormalizationFactor;
    float clamped = std::clamp(normalized, 0.0f, KColorNormalizationFactor);
    return static_cast<uint8_t>(clamped + KRoundingOffset);
}
}  // namespace

#pragma pack(push, 1)
struct BmpHeader {
    uint16_t type = 0x4D42;
    uint32_t size = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t offset = KBmpHeaderSize;
    uint32_t info_size = 40;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t planes = 1;
    uint16_t bpp = KBmpBitsPerPixel;
    uint32_t compression = KBmpCompressionNone;
    uint32_t image_size = 0;
    int32_t x_pixels_per_meter = KPixelsPerMeter;
    int32_t y_pixels_per_meter = KPixelsPerMeter;
    uint32_t colors_used = 0;
    uint32_t colors_important = 0;
};
#pragma pack(pop)

namespace writer {

class BMPWriter : public IWriter {
public:
    explicit BMPWriter(const std::string& path) : path_(path) {
    }

    void Write(const Image& image) const override {
        const size_t width = image.GetWidth();
        const size_t height = image.GetHeight();

        if (width == 0 || height == 0) {
            throw std::runtime_error("Image dimensions cannot be zero");
        }

        const size_t row_size = (width * KBytesPerPixel + KPaddingAlignment - 1) & ~(KPaddingAlignment - 1);
        const size_t image_size = row_size * height;
        const size_t file_size = KBmpHeaderSize + image_size;

        BmpHeader header;
        header.width = static_cast<int32_t>(width);
        header.height = static_cast<int32_t>(height);
        header.size = static_cast<uint32_t>(file_size);
        header.image_size = static_cast<uint32_t>(image_size);

        std::ofstream file(path_, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create output file: " + path_);
        }

        if (!file.write(reinterpret_cast<const char*>(&header), sizeof(header))) {
            throw std::runtime_error("Failed to write BMP header");
        }

        std::vector<uint8_t> row(row_size, 0);
        for (size_t y = 0; y < height; ++y) {
            const size_t target_y = height - 1 - y;
            for (size_t x = 0; x < width; ++x) {
                const Pixel& p = image.GetPixel(x, target_y);
                const size_t offset = x * KBytesPerPixel;
                row[offset + 0] = SafeColorConvert(p.b);  // B
                row[offset + 1] = SafeColorConvert(p.g);  // G
                row[offset + 2] = SafeColorConvert(p.r);  // R
            }

            if (!file.write(reinterpret_cast<const char*>(row.data()), static_cast<std::streamsize>(row_size))) {
                throw std::runtime_error("Failed to write BMP row data");
            }
        }

        if (!file.good()) {
            throw std::runtime_error("Error occurred during BMP file writing");
        }
    }

private:
    std::string path_;
};

std::shared_ptr<IWriter> GetBMPWriter(const std::string& path) {
    return std::make_shared<BMPWriter>(path);
}
}  // namespace writer