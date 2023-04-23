	# minimum CMake version required
    cmake_minimum_required(VERSION 3.15)
    # Project name, version and description
    project(discord-bot VERSION 1.0 DESCRIPTION "MeeXP")

    include(FetchContent)

    Set(FETCHCONTENT_QUIET FALSE)
     
    # Add DPP as dependency
    add_subdirectory(lib/dpp)
     
    # Create an executable
    add_executable(${PROJECT_NAME}
        src/main.cpp
        src/fileIO.cpp
        src/event.cpp
    )
     
    # Linking libraries
    target_link_libraries(${PROJECT_NAME}
        dpp
    )
     
    # Specify includes
    target_include_directories(${PROJECT_NAME} PRIVATE
        lib/dpp/include
    )
     
    # Set C++ version
    set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17 # or 20 if you want something more recent
        CXX_STANDARD_REQUIRED ON
    )