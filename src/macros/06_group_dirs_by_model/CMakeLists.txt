add_library(groupDirsByModel groupDirsByModel.cxx)

target_link_libraries(groupDirsByModel PRIVATE
    database
    macros
    utils
    compiler_flags
)
