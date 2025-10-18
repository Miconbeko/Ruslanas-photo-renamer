add_library(correlateABWithOldEC correlateABWithOldEC.cxx)

target_link_libraries(correlateABWithOldEC PRIVATE
    database
    macros
    utils
    compiler_flags
)
