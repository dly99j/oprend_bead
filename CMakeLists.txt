cmake_minimum_required(VERSION 3.21)
project(oprend_bead C)

set(CMAKE_C_STANDARD 99)


add_executable(oprend_bead main.c util.h util.c)
