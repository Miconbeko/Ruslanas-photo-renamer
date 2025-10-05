add_library(groupPhotosByEC groupPhotosByEC.cxx)

target_link_libraries(groupPhotosByEC PRIVATE
    database
    macros
    utils
    compiler_flags
)
