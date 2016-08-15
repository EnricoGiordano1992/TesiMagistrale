// Astar.cpp: implementation of the CAstar class.
//
//////////////////////////////////////////////////////////////////////

#include "Astar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool compare(const node &a, const node &b )
{
	return a.F < b.F; //��������
	//return a.value > b.value; //��������
}

bool compare2(const nodePath &a, const nodePath &b )
{
	return a.index > b.index; //��������
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
	printf("Ž�� ����\n");
	int i,j,k;
	int rX, rY;
	int cX, cY;
	while(1)
	{
		//���� ó������ ���� ��
		if(closeList.size()==0)
		{	
			int g = 0;
			int h = Get_H(Sx,Sy,Ex,Ey);
			int f = g+h;
			closeList.push_back(EasyNodeMake(g,h,f,0,0,Sx,Sy));

		}else{
			//��� �˻��� ��ġ�� ����1 
			if(openList.size() == 0 )
			{
				printf("��� Ž�� ����\n");
				break;		
			}

			//OpenList���� ���� ���� ���� ��带 ���� CloseList�� �ִ´�.
			sort(openList.begin(), openList.end(),compare);
			closeList.push_back( openList[0] );
			openList.erase(openList.begin());

			//��� �˻��� ��ġ�� ����2 			
			if(closeList[closeList.size()-1].Mx == Ex && closeList[closeList.size()-1].My==Ey)
			{
				printf("��� Ž�� ��\n");
				break;		
			}
		}

		//���� ��ġ 
		cX = closeList[closeList.size()-1].Mx;
		cY = closeList[closeList.size()-1].My;		

		//8���� ���� OpenList ����� �� �θ� ���� ����
		for(i=-1; i<2; ++i)
		{
			for(j=-1; j<2; ++j)
			{
				//������ ��ġ
				rX = cX + j;
				rY = cY + i;

				//���� ó�� 1//////////////////////
				//�� ����� ���� ������ 
				if( rX < 0 || rX >=xSize || rY < 0 || rY >= ySize)
					continue;				
				//��ֹ� �϶�
				if( MapInfo[rY][rX] == 1 )
					continue;
				//���� ��ġ�� ����ų��
				if( rX==cX && rY==cY)
					continue;
				///////////////////////////////////

				//���� ó�� 2//////////////////////
				//openList�϶� �� �θ� ����� F,G�� ������Ʈ
				int key=0;
				for(k=0; k<openList.size(); ++k)
				{
					if(openList[k].Mx == rX && openList[k].My == rY)
					{
						//������Ʈ
						//���� ����Ʈ���� ���¸���Ʈ ���ε������ g�� ���
						int g = Get_G(cX,cY,rX,rY) + closeList[closeList.size()-1].G;
						//���� ����Ʈ������ g���� ������ ��� ����
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

				//closeList�϶�
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

	printf("Ž�� �� ����\n");

	//�н��� ������ ã�Ƽ� ���Ϳ� �ִ´�. 
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
