cmake_minimum_required(VERSION 3.5)

project(c_sat)

############################################################
# Create a library
############################################################

# Generate the static library from the library sources
add_library(sat_lib STATIC 
    checksum.c
    tc_decoder.c
    tm_encoder.c
    task_manager.c
)

# Set lib includes
target_include_directories(sat_lib
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)


############################################################
# Create an executable
############################################################

# Add an executable with the above sources
add_executable(main 
    main.c
)

# link the new sat_lib target with the main binary target
target_link_libraries( main
    PRIVATE 
        sat_lib
)