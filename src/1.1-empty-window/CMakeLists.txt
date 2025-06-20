add_executable(1.1-hello-window)

target_sources(1.1-hello-window PRIVATE 1.1-empty-window.cpp)
target_link_libraries(1.1-hello-window PRIVATE common)
