add_library(copyMissingABFiles copyMissingABFiles.cxx)

target_link_libraries(copyMissingABFiles PRIVATE
    database
    macros
    utils
    compiler_flags
)
