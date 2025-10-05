add_library(macros macros.cxx)

add_subdirectory(01_name_files_by_date)
add_subdirectory(02_append_EC_to_file)
add_subdirectory(03_copy_file_set_prefix_with_AB)
add_subdirectory(04_group_files_by_EC)
add_subdirectory(05_group_dirs_by_brand)
add_subdirectory(06_group_dirs_by_model)
add_subdirectory(07_convert_to_jpg)
add_subdirectory(08_remove_prefixes_in_files)
add_subdirectory(09_delete_files_with_EC)
add_subdirectory(10_rename_AB_to_EC_files)
add_subdirectory(11_check_EC_dir_file_match)

target_link_libraries(macros PUBLIC
    database
    config
)

target_link_libraries(macros PRIVATE
    utils
    compiler_flags
)
