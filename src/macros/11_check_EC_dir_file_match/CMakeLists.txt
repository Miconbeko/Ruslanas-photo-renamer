add_library(checkECDirFileMatch checkECDirFileMatch.cxx)

target_link_libraries(checkECDirFileMatch PRIVATE
    database
    macros
    utils
    compiler_flags
)
