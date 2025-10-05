add_library(macrosExecutor macrosExecutor.cxx)

target_link_libraries(macrosExecutor PUBLIC
    macros
)

target_link_libraries(macrosExecutor PRIVATE
    utils
    compiler_flags
)
