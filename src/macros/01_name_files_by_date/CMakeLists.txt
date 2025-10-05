add_library(nameFilesByDate nameFilesByDate.cxx)

target_link_libraries(nameFilesByDate PRIVATE
    macros
    utils
    compiler_flags
)
