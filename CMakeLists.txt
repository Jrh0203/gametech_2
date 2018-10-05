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

include_directories(/lusr/opt/bullet-2.86/include/bullet
)

ADD_EXECUTABLE(proj2
  Bootstrap.cpp
  Ball.cpp
  Object.cpp
  Paddle.cpp
)
target_link_libraries(proj2 ${OGRE_LIBRARIES}
BulletDynamics BulletCollision LinearMath )



