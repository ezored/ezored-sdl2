# Header search paths
set(PROJECT_HEADER_SEARCH_PATHS
    CACHE INTERNAL ""
)

# Library search paths
set(PROJECT_LIBRARY_SEARCH_PATHS "" CACHE INTERNAL "")

# Header files
file(GLOB H_FILES_ENGINE "${PROJECT_ROOT_PATH}/projects/samples/sdl2/engine/*.hpp")

# Source files
file(GLOB S_FILES_ENGINE "${PROJECT_ROOT_PATH}/projects/samples/sdl2/engine/*.cpp")
file(GLOB S_FILES_PROJECT "${PROJECT_ROOT_PATH}/projects/samples/sdl2/*.cpp")

# Merge lists
set(
    PROJECT_HEADER_FILES
    ${H_FILES_ENGINE}
    CACHE INTERNAL ""
)

set(
    PROJECT_SOURCE_FILES
    ${S_FILES_ENGINE}
    ${S_FILES_PROJECT}
    CACHE INTERNAL ""
)

set(
    PROJECT_SOURCE_FILES
    ${PROJECT_HEADER_FILES}
    ${PROJECT_SOURCE_FILES} 
    CACHE INTERNAL ""
)

# Framework links
set(PROJECT_FRAMEWORK_LINKS "" CACHE INTERNAL "")

# Library links
set(PROJECT_LIBRARY_LINKS "" CACHE INTERNAL "")

# CFlags
set(PROJECT_CMAKE_C_FLAGS "${PROJECT_CMAKE_C_FLAGS} -Wall" CACHE INTERNAL "")

# CXXFlags
set(PROJECT_CMAKE_CXX_FLAGS "${PROJECT_CMAKE_CXX_FLAGS} -std=c++${PROJECT_CONFIG_CPP_STANDARD} -Wall" CACHE INTERNAL "")

# Compile options
set(PROJECT_COMPILE_OPTIONS "" CACHE INTERNAL "")

# Flags
set(CMAKE_C_FLAGS "${PROJECT_CMAKE_C_FLAGS}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS "${PROJECT_CMAKE_CXX_FLAGS}" CACHE INTERNAL "")
