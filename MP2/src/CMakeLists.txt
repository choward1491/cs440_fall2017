cmake_minimum_required(VERSION 3.8)
project(src)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        commandline_parser.cpp
        commandline_parser.hpp
        custom_exception.hpp
        FileWrap.cpp
        FileWrap.hpp
        gif.h
        gif_wrapper.cpp
        gif_wrapper.hpp
        graph.hpp
        graph_impl.hxx
        image.cpp
        image.hpp
        image_pixel.cpp
        image_pixel.hpp
        main.cpp
        MessageException.hpp
        text_color.cpp
        text_color.hpp csp_solver.h test_csp.h game_agent.hpp breakthrough_state.hpp
        breakthrough_actions.hpp breakthrough_rules.hpp breakthrough_transition_func.hpp
        breakthrough_piece_type.hpp breakthrough_minimax_agent.hpp breakthrough_provided_heuristics.hpp
        game_instance.hpp breakthrough_alphabeta_agent.hpp breakthrough_utility_ab.hpp
        breakthrough_cone_heuristics.hpp breakthrough_extended_rules.hpp breakthrough_learned_heuristics.hpp
        bt_heuristic_learning_costfunc.hpp
        opt_solver.hpp opt_callbacks.hpp opt_pso.hpp opt_test_rosenbrock2d.hpp breakthrough_costfunc.hpp.hpp breakthrough_handcraft_heuristics.hpp)


include_directories(/usr/local/include /usr/include /usr/local/lib)
add_library(png STATIC IMPORTED)
set_property(TARGET png PROPERTY IMPORTED_LOCATION /usr/local/lib/libpng.a)
add_library(z SHARED IMPORTED)
set_property(TARGET z PROPERTY IMPORTED_LOCATION /usr/lib/libz.dylib)
if ( CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(EXENAME mp_exec_d)
else()
    set(EXENAME mp_exec)
endif ()
add_executable(${EXENAME} ${SOURCE_FILES})
target_link_libraries(${EXENAME} png)
target_link_libraries(${EXENAME} z)

