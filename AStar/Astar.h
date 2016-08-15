// Astar.h: interface for the CAstar class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Made by J.H.KIM, 2009 / feelmare@daum.net, feelmare@gmail.com        //

// blog : http://feelmare.blogspot.com                                  //

// My Lab : VISLAB(http://me.pusan.ac.kr)                               //

//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASTAR_H__990846FC_6452_4900_AE03_135F7F0DE6A0__INCLUDED_)
#define AFX_ASTAR_H__990846FC_6452_4900_AE03_135F7F0DE6A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <vector>
#include <algorithm>
#include <math.h>
#include <cstring>
using namespace std;

struct node{
	int G;
	int H;
	int F;
	int Px;
	int Py;
	int Mx;
	int My;
};

struct nodePath{
	int x;
	int y;
	int index;
};

class CAstar  
{
public:

	double Get_G( int start_X, int start_Y, int cX, int cY );
	double Get_H( int cX, int cY, int end_X, int end_Y );
	double Get_F( int start_X, int start_Y, int cX, int cY, int end_X, int end_Y );

	node EasyNodeMake(int g, int h, int f, int pX, int pY, int mX, int mY);
	vector< pair<int, int> > FindPath();
	void SetSEpoint(int sX, int sY, int eX, int eY);
	void MapCopy(int xSize, int ySize, int *** map);
	CAstar();
	virtual ~CAstar();

	//Map information
	int ** MapInfo;
	int xSize;
	int ySize;

	//start, end point
	int Sx;
	int Sy;
	int Ex;
	int Ey;

	vector< node > openList;
	vector< node > closeList;
	vector< pair<int, int> > path;
	vector< nodePath > gFindPath;
};

#endif // !defined(AFX_ASTAR_H__990846FC_6452_4900_AE03_135F7F0DE6A0__INCLUDED_)
