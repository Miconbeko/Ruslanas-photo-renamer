add_library(groupDirsByBrand groupDirsByBrand.cxx)

target_link_libraries(groupDirsByBrand PRIVATE
    database
    macros
    utils
    compiler_flags
)
