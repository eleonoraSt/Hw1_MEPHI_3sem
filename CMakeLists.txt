cmake_minimum_required(VERSION 3.30)
project(Sem_3_Hw_1 LANGUAGES CXX)

find_package(Qt6 6.11 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_executable(Sem_3_Hw_1
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    include/UnqPtr.h
    include/ShrdPtr.h
    include/Sequence.h
    include/DynamicArray.h
    include/Option.h
    include/LinkedList.h
    include/ArraySequence.h
    include/ListSequence.h
    include/deleters.h
)

target_link_libraries(Sem_3_Hw_1
    PRIVATE
        Qt6::Core
        Qt6::Widgets
)

enable_testing()

find_package(Qt6 6.11 REQUIRED COMPONENTS Test)

qt_add_executable(testUnqPtr
    tests/TestUnqPtr.cpp
)

qt_add_executable(testShrdPtr
    tests/TestShrdPtr.cpp
)

target_link_libraries(testUnqPtr
    PRIVATE "-Wl,--stack,17000000"
        Qt6::Core
        Qt6::Test
)

target_link_libraries(testShrdPtr
    PRIVATE "-Wl,--stack,25000000"
        Qt6::Core
        Qt6::Test
)

add_test(NAME testUnqPtr COMMAND testUnqPtr)

add_test(NAME testShrdPtr COMMAND testShrdPtr)

install(TARGETS Sem_3_Hw_1
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Sem_3_Hw_1
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
