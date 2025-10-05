add_library(deleteFilesWithEC deleteFilesWithEC.cxx)

target_link_libraries(deleteFilesWithEC PRIVATE
    database
    macros
    utils
    compiler_flags
)
