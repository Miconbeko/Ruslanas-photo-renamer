add_library(removePrefixesInFiles removePrefixesInFiles.cxx)

target_link_libraries(removePrefixesInFiles PRIVATE
    macros
    utils
    compiler_flags
)
