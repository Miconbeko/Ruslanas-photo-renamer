add_library(removeIdsFromDirs removeIdsFromDirs.cxx)

target_link_libraries(removeIdsFromDirs PRIVATE
    database
    macros
    utils
    compiler_flags
)
