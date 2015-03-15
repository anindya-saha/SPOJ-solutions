#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stack>
#define MAX 2001
#define INF 15001
#define NEG_INF -1
using namespace std;

struct Point
{
	int x;
	int y;
};

bool onSegment(Point p1, Point p2, Point p3);
Point p0;

Point nextToTop(stack<Point> &S)
{
	Point p = S.top();
	S.pop();
	Point res = S.top();
	S.push(p);
	return res;
}

void swap(Point &p1, Point &p2)
{
	Point temp = p1;
	p1 = p2;
	p2 = temp;
}

int dist(Point p1, Point p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear
	return (val > 0)? 1: 2; // clock or counterclock wise
}

int compare(const void *vp1, const void *vp2)
{
	Point *p1 = (Point *)vp1;
	Point *p2 = (Point *)vp2;

	int o = orientation(p0, *p1, *p2);
	if (o == 0)
		return (dist(p0, *p2) >= dist(p0, *p1))? -1 : 1;

	return (o == 2)? -1: 1;
}

void convexHull(Point points[], int n,stack<Point> &S)
{
	int ymin = points[0].y, min = 0;
	for (int i = 1; i < n; i++)
	{
		int y = points[i].y;

		if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
			ymin = points[i].y, min = i;
	}

	swap(points[0], points[min]);

	p0 = points[0];

	if(n>1)
		qsort(&points[1], n-1, sizeof(Point), compare);



	S.push(points[0]);
	if(n>2)
	{
		int i=0;
		while((!orientation(points[i],points[i+1],points[i+2]))&&(i<n-2))
		{
			i++;
		}
		if(i<(n-2))
		{
			S.push(points[++i]);
			S.push(points[++i]);
			for (; i < n; i++)
			{
				while (orientation(nextToTop(S), S.top(), points[i]) != 2)
					S.pop();
				S.push(points[i]);
			}
		}
		else
		{
			S.push(points[i+1]);
		}
	}
	else if(n==2)
	{
		S.push(points[1]);
	}

}
bool doInstersect(Point p1,Point p2,Point p3,Point p4)
{
	int d1,d2,d3,d4;
	d1=orientation(p3,p4,p1);
	d2=orientation(p3,p4,p2);
	d3=orientation(p1,p2,p3);
	d4=orientation(p1,p2,p4);
	if(((d1==1&&d2==2)||(d1==2&&d2==1))&&((d3==1&&d4==2)||(d3==2&&d4==1)))
		return true;
	else if(d1==0 && onSegment(p3,p4,p1))
		return true;
	else if(d2==0 && onSegment(p3,p4,p2))
		return true;
	else if(d3==0 && onSegment(p1,p2,p3))
		return true;
	else if(d4==0 && onSegment(p1,p2,p4))
		return true;
	else 
		return false;
}
bool onSegment(Point p1, Point p2, Point p3)
{
	if(p3.x>=(min(p1.x,p2.x))&&(p3.x<=max(p1.x,p2.x))&&(p3.y>=min(p1.y,p2.y))&&(p3.y<=max(p1.y,p2.y)))
	{
		return true;
	}
	return false;
}
bool isInside(Point inner[MAX], int n, Point outer[MAX], int m)
{
	bool isInside=true;
	int prev_orientation,curr_orientation;
	for(int i=0;i<n;i++)
	{
		prev_orientation=orientation(inner[i],outer[0],outer[1]);
		for(int j=0;j<m;j++)
		{
			curr_orientation=orientation(inner[i],outer[j],outer[(j+1)%m]);
			if(curr_orientation!=prev_orientation)
				return false;
		}
	}
	return true;
}
int main()
{
	int n,m,test_case=1;
	Point whitePoints[MAX],blackPoints[MAX],whiteConvexHull[MAX],blackConvexHull[MAX];
	int minWhiteX,maxWhiteX,minWhiteY,maxWhiteY;
	int minBlackX,maxBlackX,minBlackY,maxBlackY;
	int whiteHullPointCount,blackHullPointCount,whitePointCount,blackPointCount;
	stack<Point> whiteStack,blackStack;
	while(1)
	{
		scanf("%d",&n);
		scanf("%d",&m);
		if(n==0&&m==0)
			break;

		minBlackX=INF;
		maxBlackX=NEG_INF;
		minBlackY=INF;
		maxBlackY=NEG_INF;
		blackPointCount=0;
		int x1,y1,x2,y2;
		for(int i=0;i<n;i++)
		{
			scanf("%d",&x1);
			scanf("%d",&y1);

			scanf("%d",&x2);
			scanf("%d",&y2);

			blackPoints[blackPointCount].x=x1;
			blackPoints[blackPointCount++].y=y1;

			blackPoints[blackPointCount].x=x2;
			blackPoints[blackPointCount++].y=y1;
			
			blackPoints[blackPointCount].x=x1;
			blackPoints[blackPointCount++].y=y2;
			
			blackPoints[blackPointCount].x=x2;
			blackPoints[blackPointCount++].y=y2;

			if(x1<minBlackX)
				minBlackX=x1;

			if(x2>maxBlackX)
				maxBlackX=x2;

			if(y1<minBlackY)
				minBlackY=y1;

			if(y2>maxBlackY)
				maxBlackY=y2;
		}
		minWhiteX=INF;
		maxWhiteX=NEG_INF;
		minWhiteY=INF;
		maxWhiteY=NEG_INF;
		whitePointCount=0;
		for(int i=0;i<m;i++)
		{
			scanf("%d",&x1);
			scanf("%d",&y1);

			scanf("%d",&x2);
			scanf("%d",&y2);


			whitePoints[whitePointCount].x=x1;
			whitePoints[whitePointCount++].y=y1;

			whitePoints[whitePointCount].x=x2;
			whitePoints[whitePointCount++].y=y1;
			
			whitePoints[whitePointCount].x=x1;
			whitePoints[whitePointCount++].y=y2;
			
			whitePoints[whitePointCount].x=x2;
			whitePoints[whitePointCount++].y=y2;

			if(x1<minWhiteX)
				minWhiteX=x1;

			if(x2>maxWhiteX)
				maxWhiteX=x2;

			if(y1<minWhiteY)
				minWhiteY=y1;

			if(y2>maxWhiteY)
				maxWhiteY=y2;
		}
		if((minBlackX>maxWhiteX)||(maxBlackX<minWhiteX)||(minBlackY>maxWhiteY)||(maxBlackY<minWhiteY))
		{
			printf("Case %d: It is possible to separate the two groups of vendors.\n",test_case++);
		}
		else
		{

			convexHull(whitePoints,whitePointCount,whiteStack);
			convexHull(blackPoints,blackPointCount,blackStack);
			whiteHullPointCount=blackHullPointCount=0;
			while (!blackStack.empty())
			{
				Point p = blackStack.top();
				blackStack.pop();
				blackConvexHull[blackHullPointCount++]=p;
			}
			while (!whiteStack.empty())
			{
				Point p = whiteStack.top();
				whiteStack.pop();
				whiteConvexHull[whiteHullPointCount++]=p;
			}

			if(isInside(blackConvexHull,blackHullPointCount,whiteConvexHull,whiteHullPointCount))
			{
					printf("Case %d: It is not possible to separate the two groups of vendors.\n",test_case++);
			}
			else if(isInside(whiteConvexHull,whiteHullPointCount,blackConvexHull,blackHullPointCount))
			{
					printf("Case %d: It is not possible to separate the two groups of vendors.\n",test_case++);
			}
			else
			{
				int flag=1;

				for(int i=0;i<blackHullPointCount;i++)
				{
					for(int j=0;j<whiteHullPointCount;j++)
					{
						if(doInstersect(blackConvexHull[i],blackConvexHull[(i+1)%
							blackHullPointCount],whiteConvexHull[j],whiteConvexHull[(j+1)%whiteHullPointCount]))
						{
							flag=0;
							break;
						}
					}
					if(!flag)
						break;
				}
				if(flag)
						printf("Case %d: It is possible to separate the two groups of vendors.\n",test_case++);
				else
						printf("Case %d: It is not possible to separate the two groups of vendors.\n",test_case++);
			}
		}

	}

	return 0;
}