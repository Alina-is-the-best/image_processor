#include "reader.h"
#include "image.h"
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace {
constexpr int KBmpBitsPerPixel = 24;
constexpr int KBmpCompressionNone = 0;
constexpr float KColorNormalizationFactor = 255.0f;
constexpr int KBytesPerPixel = 3;
constexpr int KPaddingAlignment = 4;
constexpr uint16_t KBmpSignature = 0x4D42;  // 'BM'
}  // namespace

#pragma pack(push, 1)
struct BmpHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t info_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t colors_important;
};
#pragma pack(pop)

namespace reader {

class BMPReader : public IReader {
public:
    explicit BMPReader(const std::string& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + path);
        }

        file.seekg(0, std::ios::end);
        const size_t file_size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        if (file_size < sizeof(BmpHeader)) {
            throw std::runtime_error("File is too small to be a valid BMP");
        }

        BmpHeader header;
        if (!file.read(reinterpret_cast<char*>(&header), sizeof(header))) {
            throw std::runtime_error("Failed to read BMP header");
        }

        if (header.type != KBmpSignature) {
            throw std::runtime_error("Not a valid BMP file (invalid signature)");
        }

        if (header.bpp != KBmpBitsPerPixel || header.compression != KBmpCompressionNone) {
            throw std::runtime_error("Only 24-bit uncompressed BMP files are supported");
        }

        if (header.offset > file_size) {
            throw std::runtime_error("Invalid BMP data offset");
        }

        width_ = static_cast<size_t>(std::abs(header.width));
        height_ = static_cast<size_t>(std::abs(header.height));
        const bool is_top_down = header.height < 0;

        const size_t row_size = (width_ * KBytesPerPixel + KPaddingAlignment - 1) & ~(KPaddingAlignment - 1);
        const size_t required_size = header.offset + height_ * row_size;
        if (file_size < required_size) {
            throw std::runtime_error("BMP file is truncated");
        }

        file.seekg(static_cast<std::streamoff>(header.offset), std::ios::beg);
        data_.resize(height_);
        std::vector<uint8_t> row(row_size);

        for (size_t y = 0; y < height_; ++y) {
            if (!file.read(reinterpret_cast<char*>(row.data()), static_cast<std::streamsize>(row_size))) {
                throw std::runtime_error("Failed to read BMP row data");
            }

            const size_t target_y = is_top_down ? y : height_ - 1 - y;
            data_[target_y].resize(width_);

            for (size_t x = 0; x < width_; ++x) {
                const size_t offset = x * KBytesPerPixel;
                data_[target_y][x] = {
                    static_cast<float>(row[offset + 2]) / KColorNormalizationFactor,  // R
                    static_cast<float>(row[offset + 1]) / KColorNormalizationFactor,  // G
                    static_cast<float>(row[offset + 0]) / KColorNormalizationFactor   // B
                };
            }
        }
    }

    Image GetImage() const override {
        return Image(width_, height_, data_);
    }

private:
    size_t width_;
    size_t height_;
    std::vector<std::vector<Pixel>> data_;
};

std::shared_ptr<IReader> GetBMPReader(const std::string& path) {
    return std::make_shared<BMPReader>(path);
}
}  // namespace reader