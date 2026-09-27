cmake_minimum_required(VERSION 3.19)
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
        Qt::Core
        Qt::Widgets
)

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
