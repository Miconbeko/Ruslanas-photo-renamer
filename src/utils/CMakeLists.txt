add_library(utils utils.cxx)

target_link_libraries(utils PUBLIC
    color
    compiler_flags
)
