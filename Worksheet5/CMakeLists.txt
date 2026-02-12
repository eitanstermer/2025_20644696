cmake_minimum_required(VERSION 3.10)
project(Worksheet5)

add_executable(calc calc.cpp adder.cpp)

find_package(Doxygen REQUIRED)

set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

add_custom_target(doc
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
)

install(TARGETS calc
        RUNTIME DESTINATION bin)

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "Worksheet5")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VENDOR "Eitan Stermer")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Worksheet5 Calculator Installer")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Worksheet5")

include(CPack)
