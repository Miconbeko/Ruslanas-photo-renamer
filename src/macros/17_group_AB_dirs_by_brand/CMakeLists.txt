add_library(groupABDirsByBrand groupABDirsByBrand.cxx)

target_link_libraries(groupABDirsByBrand PRIVATE
    database
    macros
    utils
    compiler_flags
)
