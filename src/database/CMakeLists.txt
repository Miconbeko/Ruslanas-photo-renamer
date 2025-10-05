add_library(database database.cxx)

target_link_libraries(database PUBLIC OpenXLSX::OpenXLSX)

target_link_libraries(database PRIVATE
    config
    compiler_flags
)
