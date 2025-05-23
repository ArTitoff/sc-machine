set(SC_CORE_SRC "${SC_MACHINE_ROOT}/sc-memory/sc-core/src")

file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*.cpp" "src/*.hpp"
    "src/*/*.cpp" "src/*/*.hpp"
    "src/*/*/*.cpp" "src/*/*/*.hpp"
    "src/*/*/*/*.cpp" "src/*/*/*/*.hpp"
)

find_package(websocketpp REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_library(sc-server-lib SHARED ${SOURCES})
target_link_libraries(sc-server-lib
    LINK_PUBLIC sc-config
    LINK_PUBLIC sc-memory
    LINK_PUBLIC websocketpp::websocketpp
    LINK_PUBLIC nlohmann_json::nlohmann_json
)
target_compile_definitions(sc-server-lib
    PUBLIC ASIO_STANDALONE
)
target_include_directories(sc-server-lib
    PRIVATE ${SC_CORE_SRC}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    PUBLIC $<INSTALL_INTERFACE:src>
)
set_target_properties(sc-server-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-server-lib)
endif()

if(${SC_BUILD_TESTS})
    add_subdirectory(tests)
endif()
