add_library(copyTests copyTests.cxx)

target_link_libraries(copyTests PRIVATE
    config
    macros
    utils
    compiler_flags
)
