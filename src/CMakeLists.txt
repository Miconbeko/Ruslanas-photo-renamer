add_executable(ruslanas-photo-account main.cxx)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS_OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# target_link_options(ruslanas-photo-account PRIVATE -Wl,--allow-multiple-definition)

add_subdirectory(color)
add_subdirectory(database)
add_subdirectory(config)
add_subdirectory(utils)
add_subdirectory(macrosExecutor)
add_subdirectory(macros)

target_link_libraries(ruslanas-photo-account PRIVATE
    database
    config
    macrosExecutor

    nameFilesByDate
    appendECToFile
    copyFileSetPrefixWithAB
    groupPhotosByEC
    groupDirsByBrand
    groupDirsByModel
    convertToJpg
    removePrefixesInFiles
    deleteFilesWithEC
    renameABToECFiles
    checkECDirFileMatch

    compiler_flags
)

add_library(compiler_flags INTERFACE)
target_include_directories(compiler_flags INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
