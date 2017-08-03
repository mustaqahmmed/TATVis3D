#include "pedestrian.h"

// TATPedesPoint
TATPedesPoint::TATPedesPoint()
{
	Density = 0.0f;
    Color.X = (rand() % 255) / 255.0;
    Color.Y = (rand() % 255) / 255.0;
    Color.Z = (rand() % 255) / 255.0;
}
void TATPedesPoint::GetDensityColor(TATVector& color)
{
	color.X = Density;
	color.Y = 0.0f;
	color.Z = 1.0f - Density;
}
void TATPedesPoint::GetPlace(TATHomoMatrix& place)
{
	TATVector n(Velocity.X, Velocity.Y, 0.0f);
	//n = TATMatrixRotZ(90.0f) * n;
	place.SetN(n);
	place.SetS(TATMatrixRotZ(90.0f) * n);
	place.SetA(0.0f, 0.0f, 1.0f);
	place.SetP(Position.X, Position.Y, 3.0f);
    TAT::RotateXO(place, 90.0f);
}

//class TATPedesCollection
TATPedesCollection::TATPedesCollection()
{

}
TATPedesCollection::~TATPedesCollection()
{
	DeleteAllPoints();
}
void TATPedesCollection::AddPoint(TATPedesPoint* point)
{
	PedesPoints.push_back(point);
}
void TATPedesCollection::DeletePoint(TATPedesPoint* point)
{
	PedesPoints.remove(point);
	delete point;
}
void TATPedesCollection::DeleteAllPoints()
{
	list<TATPedesPoint*>::iterator ite = PedesPoints.begin();
	while (ite != PedesPoints.end()) {
		TATPedesPoint* point = *ite;
		delete point;
		ite++;
	}
	PedesPoints.clear();
}
list<TATPedesPoint*>::iterator TATPedesCollection::GetFirstPointPos()
{
	return PedesPoints.begin();
}
list<TATPedesPoint*>::iterator TATPedesCollection::GetLastPointPos()
{
	return PedesPoints.end();
}
TATPedesPoint* TATPedesCollection::GetFirstPoint()
{
	if (PedesPoints.size() == 0) {
		return NULL;
	}
	return *(PedesPoints.begin());
}
TATPedesPoint* TATPedesCollection::GetLastPoint()
{
	if (PedesPoints.size() == 0) {
		return NULL;
	}
	list<TATPedesPoint*>::iterator iteLast = PedesPoints.end();
	iteLast--;
	return *iteLast;
}
TATPedesPoint* TATPedesCollection::GetNextPoint(list<TATPedesPoint*>::iterator& pos)
{
	if (PedesPoints.size() == 0) {
		return NULL;
	}
	if (pos == PedesPoints.end()) {
		return NULL;
	}
	TATPedesPoint* point = *pos;
	return point;
}

//class TATPedestrian
TATPedestrian::TATPedestrian()
{
	CurPointPos = PedesPoints.end();
	Ended = true;
}
TATPedestrian::~TATPedestrian()
{
	DeleteAllPoints();
}
list<TATPedesPoint*>::iterator TATPedestrian::GetCurPointPos()
{
	return CurPointPos;
}
void TATPedestrian::AddPoint(TATPedesPoint* point)
{
	PedesPoints.push_back(point);
}
void TATPedestrian::DeletePoint(TATPedesPoint* point)
{
	PedesPoints.remove(point);
	delete point;
}
void TATPedestrian::DeleteAllPoints()
{
	list<TATPedesPoint*>::iterator ite = PedesPoints.begin();
	while (ite != PedesPoints.end()) {
		TATPedesPoint* point = *ite;
		delete point;
		ite++;
	}
	PedesPoints.clear();
}
list<TATPedesPoint*>::iterator TATPedestrian::GetFirstPointPos()
{
	return PedesPoints.begin();
}
list<TATPedesPoint*>::iterator TATPedestrian::GetLastPointPos()
{
	return PedesPoints.end();
}
TATPedesPoint* TATPedestrian::GetFirstPoint()
{
	if (PedesPoints.size() == 0) {
		return NULL;
	}
	return *(PedesPoints.begin());
}
TATPedesPoint* TATPedestrian::GetLastPoint()
{
	if (PedesPoints.size() == 0) {
		return NULL;
	}
	list<TATPedesPoint*>::iterator iteLast = PedesPoints.end();
	iteLast--;
	return *iteLast;
}
TATPedesPoint* TATPedestrian::GetNextPoint(list<TATPedesPoint*>::iterator& pos)
{
	if (PedesPoints.size() == 0) {
		return NULL;
	}
	if (pos == PedesPoints.end()) {
		return NULL;
	}
	TATPedesPoint* point = *pos;
	return point;
}
void TATPedestrian::MovePointFirst()
{
	CurPointPos = PedesPoints.begin();
}
void TATPedestrian::MovePointNext()
{
	if(CurPointPos != PedesPoints.end())
		CurPointPos++;
	//if (CurPointPos == PedesPoints.end()) {
	//	CurPointPos--;
	//}
	
}
void TATPedestrian::GetDensityColor(float density, TATVector& color)
{
	color.X = density;
	color.Y = 1.0f - density;
	color.Z = 0.0f;
}
bool TATPedestrian::IsEnded()
{
	return (CurPointPos == PedesPoints.end());
}

