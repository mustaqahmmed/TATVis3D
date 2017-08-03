#ifndef __PEDESTRIAN_H__
#define __PEDESTRIAN_H__

#include "tatmath.h"
#include <list>

using namespace std;

class TATPedesPoint
{
public:
	TATVector2 Position;
	TATVector2 Velocity;
	float Density;
    TATVector Color;

public:
	TATPedesPoint();
	void GetDensityColor(TATVector& color);
	void GetPlace(TATHomoMatrix& place);
};

class TATPedesCollection
{
private:
	list<TATPedesPoint*> PedesPoints;

public:
	TATPedesCollection();
	~TATPedesCollection();
	void AddPoint(TATPedesPoint* point);
	void DeletePoint(TATPedesPoint* point);
	void DeleteAllPoints();
	list<TATPedesPoint*>::iterator GetFirstPointPos();
	list<TATPedesPoint*>::iterator GetLastPointPos();
	TATPedesPoint* GetFirstPoint();
	TATPedesPoint* GetLastPoint();
	TATPedesPoint* GetNextPoint(list<TATPedesPoint*>::iterator& pos);
};

class TATPedestrian
{
private:
	list<TATPedesPoint*> PedesPoints;
	list<TATPedesPoint*>::iterator CurPointPos;

public:
	bool Ended;

public:
	TATPedestrian();
	~TATPedestrian();
	list<TATPedesPoint*>::iterator GetCurPointPos();
	void AddPoint(TATPedesPoint* point);
	void DeletePoint(TATPedesPoint* point);
	void DeleteAllPoints();
	list<TATPedesPoint*>::iterator GetFirstPointPos();
	list<TATPedesPoint*>::iterator GetLastPointPos();
	TATPedesPoint* GetFirstPoint();
	TATPedesPoint* GetLastPoint();
	TATPedesPoint* GetNextPoint(list<TATPedesPoint*>::iterator& pos);
	void MovePointFirst();
	void MovePointNext();
	void GetDensityColor(float density, TATVector& color);
	bool IsEnded();

};

#endif
