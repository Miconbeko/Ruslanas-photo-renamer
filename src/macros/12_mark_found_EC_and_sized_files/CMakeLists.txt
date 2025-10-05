add_library(markFoundECAndSizedFiles markFoundECAndSizedFiles.cxx)

target_link_libraries(markFoundECAndSizedFiles PRIVATE
    database
    macros
    utils
    compiler_flags
)
