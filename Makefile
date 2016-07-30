a.out : main.cpp utilities.cpp field.cpp guigame.cpp connection.cpp
	g++ main.cpp utilities.cpp field.cpp guigame.cpp connection.cpp -std=c++11 -lsfml-graphics -lsfml-system -lsfml-window -lsfml-network
