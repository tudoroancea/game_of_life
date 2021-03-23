set(CMAKE_INCLUDE_CURRENT_DIR ON)


add_library(simulation
			STATIC
			GameOfLife.cpp GameOfLife.hpp
			Motif.cpp Motif.hpp
			EquivalenceTable.hpp EquivalenceTable.cpp
			rapidcsv.h
			termcolor.hpp
			Cell.cpp Cell.hpp)