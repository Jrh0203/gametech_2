cmake_minimum_required (VERSION 3.3.0)
set(CMAKE_PREFIX_PATH "/lusr/opt/ogre-1.11/lib/;${CMAKE_PREFIX_PATH}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(OgreTutorialBootstrap)



## [discover_ogre]
# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
## [discover_ogre]


ADD_EXECUTABLE(proj1
  Bootstrap.cpp
  Ball.cpp
)
target_link_libraries(proj1 ${OGRE_LIBRARIES}
/lusr/opt/bullet-2.86)



