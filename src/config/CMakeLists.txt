add_library(config config.cxx)

target_link_libraries(config PRIVATE
    compiler_flags
)
