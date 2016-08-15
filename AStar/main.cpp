#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Astar.h"
using namespace std;

bool read_Map2(string str, int *** map,int *sX, int *sY);

int main(void)
{
	
	//맵 필드, 사이즈 	
	int sX,sY,eX,eY;
	int ** map;
	int xSize, ySize;
 	read_Map2("map4.txt",&map, &xSize, &ySize);

	//시작, 끝
	//map1 	
	//sX = 0;  	sY = 0;		eX = 0; 	eY = 19;
	//map3
	sX = 0;  	sY = 0;		eX = 4; 	eY = 4;
	//map4
	sX = 0;  	sY = 0;		eX = 49; 	eY = 99;


	//A* 쿨래스
	CAstar Astar;
	Astar.MapCopy(xSize, ySize, &map);
	Astar.SetSEpoint(sX, sY, eX, eY);
	vector< pair<int, int > > path = Astar.FindPath();
	printf("탐색 끝\n");



	int i,j;
	int c=3;
	for(i=0; i<path.size(); ++i)
	{
		map[path[i].second][path[i].first] = c++;	
		if(c>9)
			c=3;
	}

	FILE * fp2;
	fp2 = fopen("out1.txt","w");
	for(i=0; i<ySize; ++i)
	{
		for(j=0; j<xSize; ++j)
		{
			if(map[i][j] == 0 )
				fprintf(fp2,"  ");
			else
				fprintf(fp2,"%d ", map[i][j]);

		}
		fprintf(fp2,"\n");
	}
	fclose(fp2);
	
	for(i=0; i<ySize; ++i)
		delete[] map[i];
	delete[] map;
	
	return 0;
}


bool read_Map2(string str, int *** map,int *sX, int *sY)
{
	ifstream fin(str.c_str());
	
	if( fin == NULL )
	{
		cout<<"Found not Map file!"<<endl;
		return false;
	}
	
	int xSize, ySize;
	fin >> xSize >> ySize;
	(*sX) = xSize; 
	(*sY) = ySize; 
	
	
	*map = new int*[ySize];
	for(int i=0; i<ySize; ++i)
	{
		(*map)[i] = new int[xSize];
	}
	
	for(int i=0; i<ySize ; ++i)
	{
		for(int j=0; j<xSize ; ++j)
		{
			fin >> (*map)[i][j];
		}
		
	}	
	
	return true;
}
