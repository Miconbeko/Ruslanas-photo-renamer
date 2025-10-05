add_library(renameABToECFiles renameABToECFiles.cxx)

target_link_libraries(renameABToECFiles PRIVATE
    database
    macros
    utils
    compiler_flags
)
