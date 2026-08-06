add_library(groupPhotosByAB groupPhotosByAB.cxx)

target_link_libraries(groupPhotosByAB PRIVATE
    database
    macros
    utils
    compiler_flags
)
