#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "hitbox.h"
#include "convex.cpp"
typedef enum {CIRCLE, CONVEX_POLYGON, GENERIC_POLYGON} SHAPE_TYPE;

//Shape generic loading

Shape* loadShape(int refX, int refY, const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios_base::binary);
    if (!file)
        return nullptr;
    unsigned char type;
    file>>type;
    if (!file)
        return nullptr;
    file.close();
    switch(SHAPE_TYPE(type))
    {
        case CIRCLE:
            return new Circle(refX, refY,filename);
        case CONVEX_POLYGON:
            return new ConvexPolygon(refX, refY, filename);
        case GENERIC_POLYGON:
            return new Polygon(refX, refY, filename);
    }
    return nullptr;
}

void checkRepet(std::vector<Vector>& v)
{
    for (unsigned i=0; i<v.size();i++)
    {
        for (std::vector<Vector>::iterator toDel=v.begin()+i+1; toDel!=v.end();++toDel)
        {
            if (toDel->isCollinear(v[i]))
            {
                toDel=v.erase(toDel);
                toDel--;
            }
        }
    }
}
bool pointIntersection(const std::vector<Point>& aPoints,const Point& bPoint)
{
    if (aPoints.size()<1)
        return false;
    bool vertical=true;
    Point aMax(aPoints[0].x,aPoints[0].y), aMin(aPoints[0].x,aPoints[0].y);
    double x=bPoint.x;
    for (unsigned i=1; i<aPoints.size();i++)
    {
        if (aPoints[i].x>aMax.x)
            aMax.x=aPoints[i].x;
        else if (aPoints[i].x<aMin.x)
            aMin.x=aPoints[i].x;
        if (aPoints[i].y>aMax.y)
            aMax.y=aPoints[i].y;
        else if (aPoints[i].y<aMin.y)
            aMin.y=aPoints[i].y;

        if (aPoints[i].x!=x)
        {
            vertical=false;
        }
    }

    if (vertical)
    {
        return (aMax.y>=bPoint.y&&aMin.y<=bPoint.y);
    }
    else
    {
        return (aMax.x>=bPoint.x&&aMin.x<=bPoint.x);
    }
}
bool pointIntersection(const std::vector<Point>& aPoints,const std::vector<Point>& bPoints)
{
    if (aPoints.size()<1||bPoints.size()<1)
        return false;
    bool vertical=true;
    Point aMax(aPoints[0].x,aPoints[0].y), aMin(aPoints[0].x,aPoints[0].y), bMax(bPoints[0].x,bPoints[0].y),bMin(bPoints[0].x,bPoints[0].y);
    double x=aPoints[0].x;
    for (unsigned i=1; i<aPoints.size();i++)
    {
        if (aPoints[i].x>aMax.x)
            aMax.x=aPoints[i].x;
        else if (aPoints[i].x<aMin.x)
            aMin.x=aPoints[i].x;
        if (aPoints[i].y>aMax.y)
            aMax.y=aPoints[i].y;
        else if (aPoints[i].y<aMin.y)
            aMin.y=aPoints[i].y;

        if (aPoints[i].x!=x)
        {
            vertical=false;
        }
    }
    for (unsigned i=1; i<bPoints.size();i++)
    {
        if (bPoints[i].x>bMax.x)
            bMax.x=bPoints[i].x;
        else if (bPoints[i].x<bMin.x)
            bMin.x=bPoints[i].x;
        if (bPoints[i].y>bMax.y)
            bMax.y=bPoints[i].y;
        else if (bPoints[i].y<bMin.y)
            bMin.y=bPoints[i].y;
        if (bPoints[i].x!=x)
        {
            vertical=false;
        }
    }
  /*  std::cout<<"A:"<<aPoints.size()<<"; B:"<<bPoints.size()<<std::endl;
    if (vertical) std::cout<<"VERTICALE"<<std::endl;
    std::cout<<"A: Min: ("<<aMin.x<<";"<<aMin.y<<"), Max: ("<<aMax.x<<";"<<aMax.y<<")"<<std::endl;
    std::cout<<"B: Min: ("<<bMin.x<<";"<<bMin.y<<"), Max: ("<<bMax.x<<";"<<bMax.y<<")"<<std::endl;*/

    if (vertical)
    {
        return ((aMax.y>=bMin.y&&aMax.y<=bMax.y)||(bMax.y>=aMin.y&&bMax.y<=aMax.y));
    }
    else
    {
        return ((aMax.x>=bMin.x&&aMax.x<=bMax.x)||(bMax.x>=aMin.x&&bMax.x<=aMax.x));
    }
}


Vector::Vector(double x, double y)
{
    this->x=x;
    this->y=y;
}
bool Vector::operator==(const Vector& test) const
{
    return (x==test.x&&y==test.y);
}

bool Vector::isCollinear(const Vector& test) const
{
    double x2=test.x, y2=test.y;
    return !((y*x2)-(x*y2));
}

void Circle::getCoordinates(double &x, double &y) const
{
    x=center.x;
    y=center.y;
}

void Circle::getCoordinates(int &x, int &y) const
{
    x=center.x;
    y=center.y;
}

bool Vector::operator!() const
{
    return (!x)&&(!y);
}


Point::Point(double x, double y)
{
    this->x=x;
    this->y=y;
}

int Point::load(std::ifstream& file)
{
   file.read((char*)&x,sizeof(double));
   if (!file)
        return -1;
   file.read((char*)&y,sizeof(double));
   if (!file)
        return -1;
   return 0;
}

int Point::save(std::ofstream& file)
{
    file.write((char*)&x,sizeof(double));
    if (!file)
        return -1;
    file.write((char*)&y,sizeof(double));
    if (!file)
        return -1;
    return 0;
}



void Point::projectPointToAxis(const Vector& axis, Point& projection) const
{
    if (!axis)
        return;

    Point point;

    if (!axis.x)
    {
        point.x=0;
        point.y=y;
    }
    else if (!axis.y)
    {
        point.x=x;
        point.y=0;

    }
    else
    {
        point.y=(axis.y * y + axis.x * x) / (axis.x * axis.x / axis.y + axis.y);
        point.x=axis.x*point.y/axis.y;
    }
    projection=point;
}

Circle::Circle(double x,double y, const std::string& filename)
{
   load(x,y,filename);
}

Circle::Circle(double x,double y, double radius)
{
   *this=Circle(x,y,x,y,radius);
}

Circle::Circle(double refX, double refY,double x, double y, double radius):
    center(x,y),coords(refX,refY)
{
    this->radius=radius;
}

int Circle::load(double x,double y,const std::string& filename)
{
    std::ifstream file(filename.c_str(),std::ios_base::binary|std::ios_base::in);
    if (!file)
        return -1;
    unsigned char type;
    file>>type;
    if (!file)
        return -1;
    if (SHAPE_TYPE(type)!=CIRCLE)
        return -1;
    coords=Point(x,y);
    if (center.load(file)<0)
        return -1;
    center.x+=coords.x;
    center.y+=coords.y;
    file.read((char*)&radius,sizeof(double));
    if (!file)
        return -1;
    return 0;
}

int Circle::save(const std::string& filename,double dX,double dY)
{
    std::ofstream file(filename.c_str(),std::ios_base::binary|std::ios_base::out);
    if (!file)
        return -1;

    //Relative coordinates to absolute ones
    coords.x-=dX;
    coords.y-=dY;
    move(0,0);
    //Saving shape type
    unsigned char type=CIRCLE;
    file<<type;
    if (!file)
        return -1;

    if (center.save(file)<0)
        return -1;
    file.write((char*)&radius,sizeof(radius));
    if (!file)
        return -1;
    return 0;
}

double Circle::getRadius() const
{
    return radius;
}
Point Circle::getCenter() const
{
    return center;
}
void Circle::move(double x, double y)
{
    double prevX=coords.x,prevY=coords.y;
    coords=Point(x,y);
    center=Point(center.x+x-prevX,center.y+y-prevY);
}

void Circle::add(double x, double y)
{
    move(coords.x+x,coords.y+y);
}

void Circle::rotate(double angle, Point* rotPoint)
{
    //Juste pour ne pas avoir d'erreur, mais rien besoin de faire
}

void Circle::setRotation(double angle, Point* rotPoint)
{
    //Juste pour ne pas avoir d'erreur, mais rien besoin de faire
}

double Circle::getRotation() const
{
    return 0;
}

bool Circle::getAxis(std::vector<Vector>& pAxis, const Shape* collider) const
{
    const ConvexPolygon* colliderP=dynamic_cast<const ConvexPolygon*>(collider);
    if (!colliderP)
        return false;

    std::vector<Point> colliderPoints=colliderP->getPoints();
    double minimum=(center.x-colliderPoints[0].x)*(center.x-colliderPoints[0].x)+(center.y-colliderPoints[0].y)*(center.y-colliderPoints[0].y);
    double minPoint=0;

    for (unsigned i=1;i<colliderPoints.size();i++)
    {
        double dist=(center.x-colliderPoints[i].x)*(center.x-colliderPoints[i].x)+(center.y-colliderPoints[i].y)*(center.y-colliderPoints[i].y);
        if (dist<minimum)
        {
            minimum=dist;
            minPoint=i;
        }
    }
    if (pAxis.size()<1)
        pAxis.resize(1);
    pAxis[0]=Vector(colliderPoints[minPoint].x-center.x,colliderPoints[minPoint].y-center.y);
    return true;
}
bool Circle::isColliding(const Shape* collider) const
{
    Point colliderCircumcenter;
    const Circle* circleCollider;
    if (dynamic_cast<const Polygon*>(collider))
        return collider->isColliding(this);
    else if ((circleCollider=dynamic_cast<const Circle*>(collider)))
    {
        double r=circleCollider->getRadius();
        Point colliderCenter=circleCollider->getCenter();
        return ((center.x-colliderCenter.x)*(center.x-colliderCenter.x)+(center.y-colliderCenter.y)*(center.y-colliderCenter.y)<=(r+radius)*(radius+r));
    }
    std::vector<Vector> axis;
    if (!getAxis(axis,collider))
        return false;

    Vector projectAxis=axis[0];
    std::vector<Point> projections;
    std::vector<Point> colliderPoints;

    collider->projectShapeOnAxis(projectAxis,colliderPoints);
    projectShapeOnAxis(projectAxis,projections);
    if (!pointIntersection(projections,colliderPoints))
        return false;

    collider->getAxis(axis,nullptr);
    for (unsigned i=0; i<axis.size();i++)
    {
        colliderPoints.clear();

        collider->projectShapeOnAxis(axis[i],colliderPoints);
        projectShapeOnAxis(axis[i],projections);

        if (!pointIntersection(projections,colliderPoints))
            return false;
    }
    return true;
}
bool Circle::isColliding(const Point& point) const
{
    return ((center.x-point.x)*(center.x-point.x)+(center.y-point.y)*(center.y-point.y)<=radius*radius);
}


void Circle::projectShapeOnAxis(const Vector& axis, std::vector<Point>& projectedPoints) const
{
    Point point1,point2;

    if (projectedPoints.size()<2)
        projectedPoints.resize(2);
   #if 0
    double coeff=axis.y/axis.x;
    point1=point2=center;
    if (!axis.x) //verticale
    {
        point1.y+=radius;
        point2.y-=radius;
    }
    else if (!axis.y) //horizontale
    {
        point1.x+=radius;
        point2.x-=radius;
    }
    else
    {
        double dist=0;
        double difX=0,difY=0;
        while(dist<radius*radius)
        {
            difX+=0.01;
            difY+=0.01*coeff;
            dist=difX*difX+difY*difY;
        }
        point1=Point(center.x+difX,center.y+difY);
        point2=Point(center.x-difX,center.y-difY);
    }
    point1.projectPointToAxis(axis,projectedPoints[0]);
    point2.projectPointToAxis(axis,projectedPoints[1]);
    #else
    point1=point2=center;
    if (!axis.x) //verticale
    {
        point1.y+=radius;
        point2.y-=radius;
    }
    else if (!axis.y) //horizontale
    {
        point1.x+=radius;
        point2.x-=radius;
    }
    else
    {
        double dist=sqrt(axis.x*axis.x+axis.y*axis.y);
        Vector vRadius((radius*axis.x)/dist,(radius*axis.y)/dist);
        point1=Point(center.x+vRadius.x,center.y+vRadius.y);
        point2=Point(center.x-vRadius.x,center.y-vRadius.y);
    }
    point1.projectPointToAxis(axis,projectedPoints[0]);
    point2.projectPointToAxis(axis,projectedPoints[1]);

   #endif
}

void ConvexPolygon::construct()
{
    unsigned i;
    for (i=0; i<vertices.size()-1;i++)
        axis.push_back(Vector(vertices[i+1].y-vertices[i].y,-(vertices[i+1].x-vertices[i].x)));

    axis.push_back(Vector(vertices[0].y-vertices[i].y,-(vertices[0].x-vertices[i].x)));
    checkRepet(axis);
    rotAngle = 0;
}

ConvexPolygon::ConvexPolygon(const std::vector<Point>& points)
{
    if (points.size())
        *this=ConvexPolygon(points[0].x,points[0].y,points);
}

ConvexPolygon::ConvexPolygon(double refX, double refY,const std::vector<Point>& points):
    coords(refX, refY)
{
    vertices=points;
    construct();
}

ConvexPolygon::ConvexPolygon(double x, double y,const std::string& filename)
{
    load(x,y,filename);
}

int ConvexPolygon::load(double x,double y,const std::string& filename)
{
    std::ifstream file(filename.c_str(),std::ios_base::binary|std::ios_base::in);
    if (!file)
        return -1;

    unsigned char type;
    file>>type;
    if (!file)
        return -1;
    if (SHAPE_TYPE(type)!=CONVEX_POLYGON)
        return -1;

    coords=Point(x,y);
    short nb;
    file.read((char*)&nb, sizeof(nb));
    if (!file)
        return -1;
    for (int i=0;i<nb;i++)
    {
        Point temp;
        if (temp.load(file)<0)
            return -1;
        temp.x+=coords.x;
        temp.y+=coords.y;
        vertices.push_back(temp);
    }
    construct();
    return 0;
}

int ConvexPolygon::save(const std::string& filename,double dX,double dY)
{
    std::ofstream file(filename.c_str(),std::ios_base::binary|std::ios_base::out);
    if (!file)
        return -1;

    //Relative coordinates to absolute ones
    coords.x-=dX;
    coords.y-=dY;
    move(0,0);
    //Saving shape type
    unsigned char type=CONVEX_POLYGON;
    file<<type;
    if (!file)
        return -1;

    short nb=vertices.size();
    file.write((char*)&nb, sizeof(nb));
    if (!file)
        return -1;
    for (int i=0;i<nb;i++)
        if (vertices[i].save(file)<0)
            return -1;
    return 0;
}

bool ConvexPolygon::isColliding(const Shape* poly) const
{
    if (dynamic_cast<const Circle*>(poly)||dynamic_cast<const Polygon*>(poly))
        return poly->isColliding(this);

    const ConvexPolygon* collider=dynamic_cast<const ConvexPolygon*>(poly);
    if (!collider)
        return false;

    std::vector<Vector> axis;
    collider->getAxis(axis,nullptr);
    axis.insert(axis.begin(),this->axis.begin(), this->axis.end());
    checkRepet(axis);

    for (unsigned i=0; i<axis.size();i++)
    {
        std::vector<Point> colliderProj, proj;


        collider->projectShapeOnAxis(axis[i],colliderProj);
        projectShapeOnAxis(axis[i],proj);
        if (!pointIntersection(proj,colliderProj))
            return false;
    }
    return true;
}
bool ConvexPolygon::isColliding(const Point& point) const
{
    for (unsigned i=0; i<axis.size();i++)
    {
        std::vector<Point> proj;
        Point pointProj;


        point.projectPointToAxis(axis[i],pointProj);
        projectShapeOnAxis(axis[i],proj);
        if (!pointIntersection(proj,pointProj))
            return false;
    }
    return true;
}

void ConvexPolygon::getCoordinates(double &x, double &y) const
{
    x=coords.x;
    y=coords.y;
}
void ConvexPolygon::getCoordinates(int &x, int &y) const
{
    x=coords.x;
    y=coords.y;
}

void ConvexPolygon::move(double x, double y)
{
    Vector moveV(coords.x,coords.y);
    coords=Point(x,y);
    moveV.x=x-moveV.x;
    moveV.y=y-moveV.y;
    for (unsigned i=0; i<vertices.size();i++)
    {
        vertices[i].x+=moveV.x;
        vertices[i].y+=moveV.y;
    }
}
void ConvexPolygon::add(double x, double y)
{
    move(coords.x+x,coords.y+y);
}

void ConvexPolygon::rotate(double angle, Point* rotPoint)
{
    if (!angle)
        return;

    rotAngle += angle;

    Point center;
    if (rotPoint!=nullptr)
        center={rotPoint->x + coords.x, rotPoint->y + coords.y};
    else
        center=coords;
    //std::cout << "coords ("<<coords.x<<";"<<coords.y<<") rotPoint ("<<rotPoint->x<<";"<<rotPoint->y<<")";
    //std::cout << "Rotating with center point ("<<center.x<<";"<<center.y<<std::endl;
    double radAngle=(angle/180)*M_PI;
    double cosAngle=cos(radAngle);
    double sinAngle=sin(radAngle);

    for (auto &vertex : vertices)
    {
       // std::cout << "vertex("<<vertex.x<<";"<<vertex.y<<") center("<<center.x<<";"<<center.y<<")" <<std::endl;
        //Avant la modification de vertex.x
        double dY=sinAngle*(vertex.x-center.x)+cosAngle*(vertex.y-center.y);
        vertex.x=center.x+cosAngle*(vertex.x-center.x)-sinAngle*(vertex.y-center.y);
        vertex.y=center.y+dY;
    }
}

void ConvexPolygon::setRotation(double angle, Point* rotPoint)
{
    if (angle == rotAngle)
        return;

    rotate(angle - rotAngle, rotPoint);
}

double ConvexPolygon::getRotation() const
{
    return rotAngle;
}


const std::vector<Point>& ConvexPolygon::getPoints() const
{
    return vertices;
}

bool ConvexPolygon::getAxis(std::vector<Vector>& pAxis, const Shape* collider) const
{
    pAxis=axis;
    return true;
}

void ConvexPolygon::projectShapeOnAxis(const Vector& axis, std::vector<Point>& projectedPoints) const
{
    if (projectedPoints.size()<vertices.size())
        projectedPoints.resize(vertices.size());

    for (unsigned i=0;i<vertices.size();i++)
        vertices[i].projectPointToAxis(axis,projectedPoints[i]);
}

Rectangle::Rectangle(double x, double y, double w, double h, double angle, Point* rotPoint)
{
    coords=Point(x,y);
    vertices.push_back(Point(x,y));
    vertices.push_back(Point(x,y+h));
    vertices.push_back(Point(x+w,y+h));
    vertices.push_back(Point(x+w,y));

    if (angle)
    {
        Point center;

        if (rotPoint!=nullptr)
        {
            center=*rotPoint;
        }
        else
            center=Point(x+w/2,y-h/2);

        double radAngle=(angle/360)*2*M_PI;
        double cosAngle=cos(radAngle);
        double sinAngle=sin(radAngle);

        for (auto &vertex : vertices)
        {
            //Avant la modification de vertex.x
            double dY=sinAngle*(vertex.x-center.x)+cosAngle*(vertex.y-center.y);
            vertex.x=center.x+cosAngle*(vertex.x-center.x)-sinAngle*(vertex.y-center.y);
            vertex.y=center.y+dY;
        }
    }
    unsigned i;
    for (i=0; i<vertices.size()-1;i++)
        axis.push_back(Vector(vertices[i+1].x-vertices[i].x,vertices[i+1].y-vertices[i].y));

    axis.push_back(Vector(vertices[0].x-vertices[i].x,vertices[0].y-vertices[i].y));
    checkRepet(axis);
}

void Polygon::construct()
{
    std::vector<std::vector<Point> > polys;
    decomposePoly(this->points,polys);
    for (auto i : polys)
        convexPolys.push_back(ConvexPolygon(coords.x, coords.y, i));
    rotAngle = 0;
}

Polygon::Polygon(const std::vector<Point>& points)
{
    if (points.size())
        *this=Polygon(points[0].x,points[0].y,points);
}

Polygon::Polygon(int refX, int refY,const std::vector<Point>& points):
    coords(refX,refY)
{
    this->points=points;
    construct();
}

Polygon::Polygon(double x, double y,const std::string& filename)
{
    load(x,y,filename);
}

int Polygon::load(double x,double y,const std::string& filename)
{
    std::ifstream file(filename,std::ios_base::binary|std::ios_base::in);
    if (!file)
        return -1;

    unsigned char type;
    file>>type;
    if (!file)
        return -1;
    if (SHAPE_TYPE(type)!=GENERIC_POLYGON)
        return -1;

    coords=Point(x,y);
    short nb;
    file.read((char*)&nb, sizeof(nb));
    if (!file)
        return -1;
    for (int i=0;i<nb;i++)
    {
        Point temp;
        if (temp.load(file)<0)
            return -1;
        temp.x+=coords.x;
        temp.y+=coords.y;
        points.push_back(temp);
    }
    construct();
    return 0;
}

int Polygon::save(const std::string& filename,double dX,double dY)
{
    std::ofstream file(filename,std::ios_base::binary|std::ios_base::out);
    if (!file)
        return -1;

    //Relative coordinates to absolute ones
    coords.x-=dX;
    coords.y-=dY;
    move(0,0);
    //Saving shape type
    unsigned char type=GENERIC_POLYGON;
    file<<type;
    if (!file)
        return -1;

    short nb=points.size();
    file.write((char*)&nb, sizeof(nb));
    if (!file)
        return -1;
    for (int i=0;i<nb;i++)
        if (points[i].save(file)<0)
            return -1;
    return 0;
}

bool Polygon::isColliding(const Shape* poly) const
{
    for (auto i : convexPolys)
        if (i.isColliding(poly))
            return true;
    return false;
}
bool Polygon::isColliding(const Point& point) const
{
    for (auto i : convexPolys)
        if (i.isColliding(point))
            return true;
    return false;
}

void Polygon::move(double x, double y)
{
    Vector moveV(x-coords.x,y-coords.y);

    for (unsigned i=0; i<convexPolys.size();i++)
        convexPolys[i].add(moveV.x,moveV.y);

    if (convexPolys.size()>1)
    {
        for (unsigned i=0;i<points.size();i++)
        {
            points[i].x+=moveV.x;
            points[i].y+=moveV.y;
        }
    }

    coords=Point(x,y);
}
void Polygon::add(double x, double y)
{
    move(coords.x+x,coords.y+y);
}

void Polygon::rotate(double angle, Point* rotPoint)
{
    if (!angle)
        return;

    Point center;
    if (rotPoint!=nullptr)
        center={rotPoint->x + coords.x, rotPoint->y + coords.y};
    else
        center=coords;
    if (convexPolys.size() > 1)
    {

        double radAngle=(angle/180)*M_PI;
        double cosAngle=cos(radAngle);
        double sinAngle=sin(radAngle);

        for (auto& point : points)
        {
            //Avant la modification de point.x
            double dY=sinAngle*(point.x-center.x)+cosAngle*(point.y-center.y);
            point.x=center.x+cosAngle*(point.x-center.x)-sinAngle*(point.y-center.y);
            point.y=center.y+dY;
        }
    }

    for (auto& p : convexPolys)
        p.rotate(angle, rotPoint);

    rotAngle += angle;
}

void Polygon::setRotation(double angle, Point* rotPoint)
{
    if (angle == rotAngle)
        return;

    rotate(angle - rotAngle, rotPoint);
}

double Polygon::getRotation() const
{
    return rotAngle;
}


bool Polygon::getAxis(std::vector<Vector>& pAxis, const Shape* collider) const
{
    for (auto i : convexPolys)
        i.getAxis(pAxis,collider);
    return true;
}

void Polygon::getCoordinates(double &x, double &y) const
{
    x=coords.x;
    y=coords.y;
}

void Polygon::getCoordinates(int &x, int &y) const
{
    x=coords.x;
    y=coords.y;
}

const std::vector<Point>& Polygon::getPoints() const
{
    if (convexPolys.size()>1)
    {
        return points;
    }
    return convexPolys[0].getPoints();
}

const std::vector<ConvexPolygon>& Polygon::getUnderlayingPolygons() const
{
    return convexPolys;
}

void Polygon::projectShapeOnAxis(const Vector& axis,std::vector<Point>& projectedPoints) const
{
    for (auto i : convexPolys)
        i.projectShapeOnAxis(axis,projectedPoints);
}
