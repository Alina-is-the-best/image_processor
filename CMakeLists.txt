cmake_minimum_required(VERSION 3.10)
project(image_processor VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(SRC
        src/image.cpp
        src/reader/bmp_reader.cpp
        src/filters/grayscale_filter.cpp
        src/filters/negative_filter.cpp
        src/filters/crop_filter.cpp
        src/filters/sharpening_filter.cpp
        src/filters/edge_detection_filter.cpp
        src/filters/gaussian_blur_filter.cpp
        src/filters/sepia_filter.cpp
        src/writer/bmp_writer.cpp
)

add_library(reader ${SRC})

add_executable(image_processor image_processor.cpp)
target_link_libraries(image_processor PRIVATE reader)

add_custom_command(TARGET image_processor POST_BUILD
        COMMAND sh -c "echo '=== TEST START ===';
                   cd /opt/shad/tasks/image_processor &&
                   /usr/bin/python3 test_script/test_image_processor.py $<TARGET_FILE:image_processor> ||
                   (echo 'Tests failed with code $?' && exit 1)"
        COMMENT "Running image processor tests"
        VERBATIM
)