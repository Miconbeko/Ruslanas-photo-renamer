add_library(convertToJpg convertToJpg.cxx)

target_link_libraries(convertToJpg PRIVATE
    macros
    utils
    compiler_flags
)
