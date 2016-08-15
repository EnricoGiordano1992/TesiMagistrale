// Astar.cpp: implementation of the CAstar class.
//
//////////////////////////////////////////////////////////////////////

#include "Astar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool compare(const node &a, const node &b )
{
	return a.F < b.F; //오름차순
	//return a.value > b.value; //내림차순
}

bool compare2(const nodePath &a, const nodePath &b )
{
	return a.index > b.index; //내림차순
}

CAstar::CAstar()
{
	printf("//////////////////////////////////////////////////////////////////////////\n");

    printf("// Made by J.H.KIM, 2009 / feelmare@daum.net, feelmare@gmail.com        //\n");

    printf("// blog : http://feelmare.blogspot.com                                  //\n");

    printf("// My Lab : VISLAB(http://me.pusan.ac.kr)                               //\n");

    printf("//////////////////////////////////////////////////////////////////////////\n");

	MapInfo=0;
}

CAstar::~CAstar()
{
	if(MapInfo!=0)
	{
		for(int i=0; i<ySize; ++i)
			delete[] MapInfo[i];
		delete[] MapInfo;
		MapInfo=0;
	}
}

void CAstar::MapCopy(int xSize, int ySize, int ***map)
{
	this->xSize = xSize;
	this->ySize = ySize;

	MapInfo = new int*[ySize];
	for(int i=0; i<ySize; ++i)
	{
		MapInfo[i] = new int[xSize];
		memcpy(MapInfo[i], (*map)[i], sizeof(int)*xSize);
	}

	/*
	for(i=0; i<ySize; ++i)
	{
		for(int j=0; j<xSize; ++j)
		{
			printf("%d ", MapInfo[i][j]);
		}
		printf("\n");
	}
	*/
}

void CAstar::SetSEpoint(int sX, int sY, int eX, int eY)
{
	Sx = sX;
	Sy = sY;
	Ex = eX;
	Ey = eY;
}



vector< pair<int, int> > CAstar::FindPath()
{
	printf("탐색 시작\n");
	int i,j,k;
	int rX, rY;
	int cX, cY;
	while(1)
	{
		//제일 처음에는 시작 점
		if(closeList.size()==0)
		{	
			int g = 0;
			int h = Get_H(Sx,Sy,Ex,Ey);
			int f = g+h;
			closeList.push_back(EasyNodeMake(g,h,f,0,0,Sx,Sy));

		}else{
			//경로 검색을 마치는 시점1 
			if(openList.size() == 0 )
			{
				printf("경로 탐색 실패\n");
				break;		
			}

			//OpenList에서 제일 작은 값의 노드를 빼서 CloseList에 넣는다.
			sort(openList.begin(), openList.end(),compare);
			closeList.push_back( openList[0] );
			openList.erase(openList.begin());

			//경로 검색을 마치는 시점2 			
			if(closeList[closeList.size()-1].Mx == Ex && closeList[closeList.size()-1].My==Ey)
			{
				printf("경로 탐색 끝\n");
				break;		
			}
		}

		//현재 위치 
		cX = closeList[closeList.size()-1].Mx;
		cY = closeList[closeList.size()-1].My;		

		//8방향 조사 OpenList 만들기 및 부모 방향 갱신
		for(i=-1; i<2; ++i)
		{
			for(j=-1; j<2; ++j)
			{
				//조사할 위치
				rX = cX + j;
				rY = cY + i;

				//예외 처리 1//////////////////////
				//맵 사이즈를 벗어 났을때 
				if( rX < 0 || rX >=xSize || rY < 0 || rY >= ySize)
					continue;				
				//장애물 일때
				if( MapInfo[rY][rX] == 1 )
					continue;
				//현재 위치를 가르킬때
				if( rX==cX && rY==cY)
					continue;
				///////////////////////////////////

				//예외 처리 2//////////////////////
				//openList일때 단 부모 방향및 F,G값 업데이트
				int key=0;
				for(k=0; k<openList.size(); ++k)
				{
					if(openList[k].Mx == rX && openList[k].My == rY)
					{
						//업데이트
						//현재 포인트에서 오픈리스트 포인드까지의 g값 계산
						int g = Get_G(cX,cY,rX,rY) + closeList[closeList.size()-1].G;
						//현재 포이트에서의 g값이 작으면 노드 갱신
						if(g < openList[k].G)
						{
							openList[k].G = g;
							openList[k].F = openList[k].G+openList[k].H;
							openList[k].Px = cX;
							openList[k].Py = cY;
						}
						key=1;
						break;
					}
				}
				if(key)
					continue;

				//closeList일때
				key=0;
				for(k=0; k<closeList.size(); ++k)
				{
					if(closeList[k].Mx == rX && closeList[k].My == rY)
					{
						key=1;
						break;
					}
				}
				if(key)
					continue;

				//////////////////////////////////////////////////////////////////////////
				//make OpenList
				
				int g = Get_G(cX,cY,rX,rY) + closeList[closeList.size()-1].G;
				int h = Get_H(rX,rY,Ex,Ey);
				int f = g+h;
				openList.push_back(EasyNodeMake(g,h,f,cX,cY,rX,rY));
			}
		}
	}

	printf("탐색 끝 정렬\n");

	//패스를 역으로 찾아서 벡터에 넣는다. 
	int index=0;
	nodePath tempPath;
	tempPath.index=index++;
	tempPath.x = Ex;
	tempPath.y = Ey;
	gFindPath.push_back(tempPath);	
	int fX = closeList[closeList.size()-1].Px;
	int fY = closeList[closeList.size()-1].Py;
	tempPath.index=index++;
	tempPath.x = fX;
	tempPath.y = fY;
	gFindPath.push_back(tempPath);
	while(1)
	{
		
		for(k=0; k<openList.size(); ++k)
		{
			if( (openList[k].Mx==fX) && (openList[k].My==fY))
			{
				fX = openList[k].Px;
				fY = openList[k].Py;
				tempPath.index=index++;
				tempPath.x = fX;
				tempPath.y = fY;
				gFindPath.push_back(tempPath);
				break;
			}
		}
		
		if(fX==0 && fY==0)
			break;

		for(k=0; k<closeList.size(); ++k)
		{
			if( (closeList[k].Mx==fX) && (closeList[k].My==fY))
			{
				fX = closeList[k].Px;
				fY = closeList[k].Py;
				tempPath.index=index++;
				tempPath.x = fX;
				tempPath.y = fY;				
				gFindPath.push_back(tempPath);
				break;
			}
		}

		if(fX==0 && fY==0)
			break;

	}

	sort(gFindPath.begin(), gFindPath.end(),compare2);
	for(i=1; i<gFindPath.size(); ++i)
	{
		path.push_back( make_pair(gFindPath[i].x,  gFindPath[i].y ) );
	}

	return path;
}

node CAstar::EasyNodeMake(int g, int h, int f, int pX, int pY, int mX, int mY)
{
	node temp;
	temp.G = g;
	temp.H = h;
	temp.F = f;
	temp.Px = pX;
	temp.Py = pY;
	temp.Mx = mX;
	temp.My = mY;

	return temp;
}


double CAstar::Get_G( int start_X, int start_Y, int cX, int cY )
{

	int cost_gX = abs(cX - start_X);
	int cost_gY = abs(cY - start_Y);	

	if( (cost_gX+cost_gY) == 1)
		return 10;
	else 
		return 14;

//	double cost_G = sqrt( pow( float(cost_gX), 2 ) + pow( cost_gY, 2 ));	
//	return cost_G;
}

double CAstar::Get_H( int cX, int cY, int end_X, int end_Y )
{
	int cost_hX = abs(end_X - cX);
	int cost_hY = abs(end_Y - cY);
	
	double cost_H = cost_hX + cost_hY;
	
	return cost_H;
}

double CAstar::Get_F( int start_X, int start_Y, int cX, int cY, int end_X, int end_Y )
{
	
	double cost_G = Get_G( start_X, start_Y, cX, cY );	
	double cost_H = Get_G( cX, cY, end_X, end_Y );//Get_H( cX, cY, end_X, end_Y );
	printf("%d %d- %d %d = %lf + %lf = %lf\n",start_X, start_Y, cX, cY,cost_G,cost_H,cost_G + cost_H);
	
	return cost_G + cost_H;
}
