#pragma once
#include <vector>
#include <utility>

#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
							return &mgr;\
						}\
					private:\
						type();\
						~type();\

#define DIRUP		0x01
#define DIRDOWN		0x02
#define DIRLEFT		0x04
#define DIRRIGHT	0x08

class MazeMaker
{
	SINGLE(MazeMaker);
public:
	void BackTracking(int row, int column);
	void CellRandomSelect(int range);
	void MazeProcess();
	int CellSearch(int x, int y);
	void MoveToMerge();
	void MoveToPreviousCell();

public:
	std::vector<std::vector<int>>& GetArr() { return arr; }
	const std::vector<std::vector<int>> GetDirArr() { return dirArr; }
	const std::pair<int, int> GetStartPos() { return startPos; }

public:
	void Init();

private:
	std::vector<std::vector<int>> arr;
	std::vector<std::vector<int>> dirArr;
	std::pair<int, int> startPos;
	std::pair<int, int> currentPos;
};

