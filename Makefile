a.out : main.cpp utilities.cpp field.cpp 
	g++ main.cpp utilities.cpp field.cpp -std=c++11 -lsfml-graphics -lsfml-system -lsfml-window
