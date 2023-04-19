cmake_minimum_required(VERSION 3.24)
project(UDS)

set(CMAKE_CXX_STANDARD 23)

add_executable(server MainServer.cpp "SourceFiles/UdsServer.cpp" "HeaderFiles/UdsServer.h")
add_executable(client MainClient.cpp "SourceFiles/UdsClient.cpp" "HeaderFiles/UdsClient.h" "HeaderFiles/Uds.h" "ScriptReader/Handler.h" "ScriptReader/Handler.cpp" "ScriptReader/parser.tab.cpp" "ScriptReader/lexer.flex.cpp" "ScriptReader/parser.tab.h")
#add_executable(scriptReader "ScriptReader/parser.tab.cpp" "ScriptReader/lexer.flex.cpp" "ScriptReader/parser.tab.h" ScriptReader/Handler.h ScriptReader/Handler.cpp)
target_include_directories(client PRIVATE "C:/Users/Public/Documents/Vector/XL Driver Library 20.30.14/bin")
target_include_directories(server PRIVATE "C:/Users/Public/Documents/Vector/XL Driver Library 20.30.14/bin")
target_include_directories(server PRIVATE "C:/Users/Public/Documents/Peak/Include")
target_include_directories(client PRIVATE "C:/Users/Public/Documents/Peak/Include")

find_library(VXLAPI64_LIBRARY NAMES vxlapi64 PATHS "C:/Users/Public/Documents/Vector/XL Driver Library 20.30.14/bin")
find_library(PCAN_BASIC_LIBRARY NAMES PCANBasic PATHS "C:/Users/Public/Documents/Peak/x64/VC_LIB")
target_link_libraries(client PRIVATE ${VXLAPI64_LIBRARY})
target_link_libraries(server PRIVATE ${VXLAPI64_LIBRARY})
target_link_libraries(client PRIVATE ${PCAN_BASIC_LIBRARY})
target_link_libraries(server PRIVATE ${PCAN_BASIC_LIBRARY})


