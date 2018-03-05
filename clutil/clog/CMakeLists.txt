cmake_minimum_required(VERSION 3.5)

set(SOURCES
        cclog.h
        clog.h
        cclog.cc
        clog.c
        ansi.h
        )

add_library(clog ${SOURCES})

install(TARGETS clog DESTINATION lib)
install(FILES ansi.h clog.h cclog.h DESTINATION include/clog)
