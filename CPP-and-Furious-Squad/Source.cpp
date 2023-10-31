#include "PrintGameBoard.h"
#include <thread>
#include <chrono>

int main()
{

	std::vector<Pillar> pillars;
	Point p1{ 3, 2 };
	pillars.emplace_back(p1, Color::RED);
	PrintGameBoard g{24};
	g.print(pillars);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	p1= { 5, 3 };
	pillars.emplace_back(p1, Color::RED);
	g.print(pillars);
	std::this_thread::sleep_for(std::chrono::seconds(1));
p1 = { 12, 9 };
	pillars.emplace_back(p1, Color::BLACK);
g.print(pillars);
	return 0;
}