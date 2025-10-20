add_library(renameOldECFoldersToNew renameOldECFoldersToNew.cxx)

target_link_libraries(renameOldECFoldersToNew PRIVATE
    database
    macros
    utils
    compiler_flags
)
