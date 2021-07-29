#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <stack>
#include <string>
#include <vector>

const int PLAYFIELD_X = 512;
const int PLAYFIELD_Y = 384;
int OFFSET = 0; // Map audio
double TIMESTEP = 300; // ms between circles
int STEP = 64; // Step between points in osu! units

struct Point
{
	int x;
	int y;
};

struct Path
{
	Point p1;
	Point p2;
};

void PrintCircles(std::vector<Path> &paths, std::ofstream &file)
{
	int circles = 0;
	for(auto &p : paths)
	{
		file << p.p1.x << "," << p.p1.y << "," << (int)(OFFSET + circles * TIMESTEP) << ",1,0" << std::endl;
		circles++;
	}
}

int main(int argc, char** argv)
{
	std::ofstream filepath;
	std::string filename = "hitobjects.txt";
	filepath.open(filename);
	filepath << "[HitObjects]" << std::endl;

	std::cout << "Offset pls" << std::endl;
	std::cin >> OFFSET;
	std::cout << "Bpm pls" << std::endl;
	std::cin >> TIMESTEP;
	TIMESTEP = 60000 / TIMESTEP / 2;
	std::cout << "Precision step pls. For best results use 128 or 64 or 32" << std::endl;
	std::cin >> STEP;

	// Find all points on the borders of the playfield where the circles can be placed
	// Depends on the precision step
	std::vector<Point> points;

	for(int x = 0; x <= PLAYFIELD_X; x += STEP)
	{
		int y = 0;
		points.push_back({ x, y });
		y = PLAYFIELD_Y;
		points.push_back({ x, y });
	}

	for(int y = 0; y <= PLAYFIELD_Y; y += STEP)
	{
		if(y == 0 || y == PLAYFIELD_Y) // Avoid duplicates
			continue;
		int x = 0;
		points.push_back({ x, y });
		x = PLAYFIELD_X;
		points.push_back({ x, y });
	}

	// print points
	//for(auto c : points) {
	//	filepath << c.x << ":" << c.y << std::endl;
	//}
	//filepath << std::endl;	

	// Find all movement paths between each point
	std::vector<Path> paths;

	for(int i = 0; i < points.size(); i++)
	{
		for(int j = 0; j < points.size(); j++)
		{
			if(i == j)
				continue;
			int x1, x2, y1, y2;
			x1 = points[i].x;
			y1 = points[i].y;
			x2 = points[j].x;
			y2 = points[j].y;
			// Don't do paths exactly on borders for now to avoid paths on the same line
			// E.g. 0:32 to 0:64 and 0:0 to 0:384 are on the same line but we only care about moving on this path once 
			if(x1 == 0 && x2 == 0)
				continue;
			if(y1 == 0 && y2 == 0)
				continue;
			if(x1 == PLAYFIELD_X && x2 == PLAYFIELD_X)
				continue;
			if(y1 == PLAYFIELD_Y && y2 == PLAYFIELD_Y)
				continue;

			Path path;
			path.p1 = points[i];
			path.p2 = points[j];
			paths.push_back(path);

		}
	}

	// Add paths exactly on borders
	Point p1, p2, p3, p4;
	p1 = { 0,0 };
	p2 = { 0, PLAYFIELD_Y };
	p3 = { PLAYFIELD_X, 0 };
	p4 = { PLAYFIELD_X, PLAYFIELD_Y };
	paths.push_back({ p1,p2 });
	paths.push_back({ p2,p1 });
	paths.push_back({ p1,p3 });
	paths.push_back({ p3,p1 });
	paths.push_back({ p2,p4 });
	paths.push_back({ p4,p2 });
	paths.push_back({ p3,p4 });
	paths.push_back({ p4,p3 });

	std::random_device rd;
	std::mt19937 mt(rd());
	std::shuffle(std::begin(paths), std::end(paths), mt);

	//PrintCircles(paths, filepath);
	//filepath << std::endl;

	// And now we try to make a single sequence between circles that uses all possible paths we've found
	// Store the sequence in a stack to allow rewinding back the process if the sequence goes into the loop without using all the paths
	std::stack<Path> history;
	Path currentPath = paths[0];
	Point neededStart = currentPath.p2;
	history.push(currentPath);
	paths.erase(paths.begin());

	int rewinds = 0;
	while(paths.size())
	{
		bool found = false;
		//std::cout << "paths " << paths.size() << " history " << history.size() << std::endl;
		for(int c = 0; c < paths.size(); c++)
		{
			Path nextPath = paths[c];
			if(nextPath.p1.x == neededStart.x && nextPath.p1.y == neededStart.y)
			{
				history.push(nextPath);
				paths.erase(paths.begin() + c); // Slow but works
				currentPath = nextPath;
				neededStart = currentPath.p2;

				found = true;
				break;
			}
		}

		if(!found)
		{
			//std::cout << "Not found " << neededStart.x << ":" << neededStart.y << std::endl;
			for(int k = 0; k < rewinds; k++)
			{
				Path toReturn = history.top();
				history.pop();
				currentPath = history.top();
				neededStart = currentPath.p2;
				paths.push_back(toReturn);
			}
			// Rewind back more times at once because the sequence can still go into the same loop after a single rewind
			rewinds++;
		}
	}

	std::cout << "Done with " << rewinds << " rewinds (that's ok)" << std::endl;
	std::cout << "Saving " << history.size() << " circles to file... ";

	while(history.size())
	{
		paths.push_back(history.top());
		history.pop();
	}

	std::reverse(paths.begin(), paths.end());

	PrintCircles(paths, filepath);
	filepath.close();

	std::cout << "Done" << std::endl;
	std::cout << "Check " << filename << std::endl;
	system("pause");
	return 0;
}
