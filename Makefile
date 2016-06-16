a.out : main.cpp utilities.cpp field.cpp guigame.cpp
	g++ main.cpp utilities.cpp field.cpp guigame.cpp -std=c++11 -lsfml-graphics -lsfml-system -lsfml-window
