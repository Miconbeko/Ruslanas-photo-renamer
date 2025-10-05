add_library(copyFileSetPrefixWithAB copyFileSetPrefixWithAB.cxx)

target_link_libraries(copyFileSetPrefixWithAB PRIVATE
    appendECToFile
    database
    macros
    utils
    compiler_flags
)
