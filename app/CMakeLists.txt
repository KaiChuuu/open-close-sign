# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.cpp")
add_executable(cool_sign ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(cool_sign LINK_PRIVATE hal)

# Copy executable to final location (change `hello_world` to project name as needed)
add_custom_command(TARGET cool_sign POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:cool_sign>"
     "~/cmpt433/public/project/cool_sign" 
  COMMENT "Copying ARM executable to public NFS directory")

add_custom_command(TARGET cool_sign POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_SOURCE_DIR}/fonts"
    "~/cmpt433/public/project/fonts"
    COMMENT "Copying fonts to public NFS directory")

add_custom_command(TARGET cool_sign POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_SOURCE_DIR}/server-copy"
    "~/cmpt433/public/project/server-copy"
  COMMENT "Copying SERVER files to public NFS directory")    