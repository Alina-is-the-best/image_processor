#include "image.h"
#include "reader.h"
#include "writer.h"
#include "filter.h"
#include <iostream>
#include <vector>
#include <stdexcept>

namespace constants {
constexpr int MinRequiredArgs = 3;
constexpr int InputFileArgPos = 1;
constexpr int OutputFileArgPos = 2;
constexpr int FirstFilterArgPos = 3;
}  // namespace constants

int main(int argc, char** argv) {
    try {
        using constants::FirstFilterArgPos;
        using constants::InputFileArgPos;
        using constants::MinRequiredArgs;
        using constants::OutputFileArgPos;

        if (argc < MinRequiredArgs) {
            std::cout << "Usage: " << argv[0]
                      << " <input.bmp> <output.bmp> [-фильтр1 [параметры]] [-фильтр2 [параметры]]...\n"
                      << "Available filters:\n"
                      << "  -neg          Negative\n"
                      << "  -gs           Grayscale\n"
                      << "  -crop W H     Crop\n"
                      << "  -sharp        Sharpening\n"
                      << "  -edge T       Edge detection\n"
                      << "  -blur S       Gaussian blur\n"
                      << "  -sepia        Sepia\n";
            return 1;
        }

        auto reader = reader::GetBMPReader(argv[InputFileArgPos]);
        Image image = reader->GetImage();

        std::vector<FilterPtr> filters;
        int current_arg = FirstFilterArgPos;

        while (current_arg < argc) {
            std::string filter_type = argv[current_arg];

            if (filter_type == "-neg") {
                filters.push_back(CreateNegativeFilter());
                current_arg += 1;
            } else if (filter_type == "-gs") {
                filters.push_back(CreateGrayscaleFilter());
                current_arg += 1;
            } else if (filter_type == "-crop") {
                if (current_arg + 2 >= argc) {
                    throw std::runtime_error("Not enough arguments for -crop filter");
                }
                size_t width = std::stoul(argv[current_arg + 1]);
                size_t height = std::stoul(argv[current_arg + 2]);
                filters.push_back(CreateCropFilter(width, height));
                current_arg += 3;
            } else if (filter_type == "-sharp") {
                filters.push_back(CreateSharpeningFilter());
                current_arg += 1;
            } else if (filter_type == "-edge") {
                if (current_arg + 1 >= argc) {
                    throw std::runtime_error("Not enough arguments for -edge filter");
                }
                float threshold = std::stof(argv[current_arg + 1]);
                filters.push_back(CreateEdgeDetectionFilter(threshold));
                current_arg += 2;
            } else if (filter_type == "-blur") {
                if (current_arg + 1 >= argc) {
                    throw std::runtime_error("Not enough arguments for -blur filter");
                }
                float sigma = std::stof(argv[current_arg + 1]);
                filters.push_back(CreateGaussianBlurFilter(sigma));
                current_arg += 2;
            } else if (filter_type == "-sepia") {
                filters.push_back(CreateSepiaFilter());
                current_arg += 1;
            } else {
                throw std::runtime_error("Unknown filter type: " + filter_type);
            }
        }

        for (auto& filter : filters) {
            filter->Apply(image);
        }

        auto writer = writer::GetBMPWriter(argv[OutputFileArgPos]);
        writer->Write(image);

        std::cout << "Success! Output saved to: " << argv[OutputFileArgPos] << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}