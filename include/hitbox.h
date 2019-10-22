#ifndef HITBOX_H_INCLUDED
#define HITBOX_H_INCLUDED

#include <vector>
#include <fstream>

class Vector{
public:
    double x,y;

    Vector(double x=0, double y=0);


    bool operator==(const Vector& test) const;
    bool isCollinear(const Vector &test) const;

    bool operator!() const;
};

class Point{
public:
    double x,y;
    Point(double x=0, double y=0);

    int load(std::ifstream& file);
    int save(std::ofstream& file);

    bool operator==(const Point& test) const { return (x==test.x&&y==test.y);};

    void projectPointToAxis(const Vector& axis, Point& projection) const;
};

class Shape{
    public:
    virtual bool isColliding(const Shape* poly) const=0;
    virtual bool isColliding(const Point& point) const=0;

    virtual void getCoordinates(double &x, double &y) const=0;
    virtual void getCoordinates(int &x, int &y) const=0;

    virtual bool getAxis(std::vector<Vector>& pAxis, const Shape* collider) const=0;

    virtual void move(double x, double y)=0;
    virtual void add(double x, double y)=0;

    virtual void rotate(double angle=0, Point* rotPoint=nullptr)=0;
    virtual void setRotation(double angle=0, Point* rotPoint=nullptr)=0;
    virtual double getRotation() const=0;

    virtual int load(double x,double y,const std::string& filename)=0;
    //dX,dY: decalage du point coords avec le coin superieur gauche de l'image
    virtual int save(const std::string& filename, double dX=0, double dY=0)=0;

    virtual ~Shape(){};

    virtual void projectShapeOnAxis(const Vector& axis,std::vector<Point>& projectedPoints) const=0;

};


class Circle: public Shape{
public:
    Circle(double refX, double refY, double x,double y, double radius);
    Circle(double x,double y, double radius);
    Circle(double x,double y,const std::string& filename);


    virtual bool isColliding(const Shape* poly) const;
    virtual bool isColliding(const Point& point) const;

    virtual void getCoordinates(double &x, double &y) const;
    virtual void getCoordinates(int &x, int &y) const;

    virtual void move(double x, double y);
    virtual void add(double x, double y);

    virtual void rotate(double angle=0, Point* rotPoint=nullptr);
    virtual void setRotation(double angle=0, Point* rotPoint=nullptr);
    virtual double getRotation() const;

    virtual int load(double x,double y,const std::string& filename);
    virtual int save(const std::string& filename, double dX=0, double dY=0);

    double getRadius() const;
    Point getCenter() const;
    virtual bool getAxis(std::vector<Vector>& pAxis, const Shape* collider) const;

    virtual ~Circle(){};

    virtual void projectShapeOnAxis(const Vector& axis,std::vector<Point>& projectedPoints) const;

private:
    Point center, coords;
    double radius;
};

class ConvexPolygon: public Shape {
public:
    ConvexPolygon(double refX, double refY,const std::vector<Point>& points);
    ConvexPolygon(const std::vector<Point>& points);
    ConvexPolygon(double x,double y,const std::string& filename);

    virtual bool isColliding(const Shape* poly) const;
    virtual bool isColliding(const Point& point) const;

    virtual void getCoordinates(double &x, double &y) const;
    virtual void getCoordinates(int &x, int &y) const;

    virtual void move(double x, double y);
    virtual void add(double x, double y);

    virtual void rotate(double angle=0, Point* rotPoint=nullptr);
    virtual void setRotation(double angle=0, Point* rotPoint=nullptr);
    virtual double getRotation() const;


    virtual int load(double x,double y,const std::string& filename);
    virtual int save(const std::string& filename, double dX=0, double dY=0);

    virtual const std::vector<Point>& getPoints() const;
    virtual bool getAxis(std::vector<Vector>& pAxis, const Shape* collider) const;

    virtual ~ConvexPolygon(){};
    virtual void projectShapeOnAxis(const Vector& axis,std::vector<Point>& projectedPoints) const;
protected:
    ConvexPolygon(){}; //Pour les constructeurs de formes regulieres
    Point coords;
    std::vector<Point> vertices;
    std::vector<Vector> axis;
    double rotAngle;

    void construct();
    };
class Rectangle: public ConvexPolygon{
public:
    Rectangle(double x, double y, double w, double h, double angle=0, Point* rotPoint=nullptr);
    Rectangle(const ConvexPolygon& poly);
    virtual ~Rectangle(){};
};
class Polygon: public Shape{
public:
    Polygon(const std::vector<Point>& points);
    Polygon(int refX, int refY,const std::vector<Point>& points);
    Polygon(double x,double y,const std::string& filename);

    virtual bool isColliding(const Shape* poly) const;
    virtual bool isColliding(const Point& point) const;

    virtual void move(double x, double y);
    virtual void add(double x, double y);

    virtual void rotate(double angle=0, Point* rotPoint=nullptr);
    virtual void setRotation(double angle=0, Point* rotPoint=nullptr);
    virtual double getRotation() const;

    virtual int load(double x,double y,const std::string& filename);
    virtual int save(const std::string& filename, double dX=0, double dY=0);

    virtual bool getAxis(std::vector<Vector>& pAxis, const Shape* collider) const;

    virtual void getCoordinates(double &x, double &y) const;
    virtual void getCoordinates(int &x, int &y) const;

    virtual const std::vector<Point>& getPoints() const;
    const std::vector<ConvexPolygon>& getUnderlayingPolygons() const;

    virtual ~Polygon(){};

    virtual void projectShapeOnAxis(const Vector& axis,std::vector<Point>& projectedPoints) const;

private:
    std::vector<ConvexPolygon> convexPolys;
    std::vector<Point> points;
    Point coords;
    double rotAngle;

    void construct();
};

Shape* loadShape(int refX, int refY, const std::string& filename);


#endif // HITBOX_H_INCLUDED
