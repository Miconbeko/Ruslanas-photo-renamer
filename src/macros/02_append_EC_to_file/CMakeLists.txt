add_library(appendECToFile appendECToFile.cxx)

target_link_libraries(appendECToFile PRIVATE
    copyFileSetPrefixWithAB
    database
    macros
    utils
    compiler_flags
)
