add_library(logger logger.cxx)

target_link_libraries(logger PUBLIC
    config
    compiler_flags
)
