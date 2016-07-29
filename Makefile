a.out : main.cpp utilities.cpp field.cpp guigame.cpp hostorclient.cpp server.cpp client.cpp
	g++ main.cpp utilities.cpp field.cpp guigame.cpp hostorclient.cpp server.cpp client.cpp -std=c++11 -lsfml-graphics -lsfml-system -lsfml-window -lsfml-network
