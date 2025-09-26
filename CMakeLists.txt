cmake_minimum_required(VERSION 3.10) #i dont know what im doing, i expect this to fail spectacularly

project(QShell LANGUAGES C)
add_executable(QShell main.c)
target_link_libraries(QShell ws2_32)
