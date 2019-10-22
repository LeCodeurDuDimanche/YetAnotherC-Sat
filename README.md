# Yet another SAT C++ implementation

Custom C++ implementation of the Separated Axis Theorem (SAT, explained [here](http://www.dyn4j.org/2010/01/sat/))

## Features :

* Simple point, rectangle, circle and convex polygon collision
* Support for any polygons (including concaves polygons)
* Can load and save any shape, using a custom file format
* A graphical shape editor is included

### Dependencies

The graphical editor was only tested under Linux but should work on Windows.

The graphical editor depends on SDL2 and SDL2_image (`sdl2` and `sdl2_image` package on Archlinux, `libsdl2-dev` and `libsdl2-image-dev` on Ubuntu)

### Build steps

* To build both the library and the editor :

 `make`

* To build the library only :

`make release`


* To build the editor only :

`make editor`


### Usage

You can create a Circle, a Rectangle, a Point or a Polygon like so :
```c++
    Circle circle(x, y, radius);
    Rectangle rect(x, y, w, h), rotatedRect(x, y, w, h, angle, rotationCenter);
    Point point(x, y);
    Polygon polygon(vector_of_points);
```

Now you can test collision bewteen shapes :
```c++
    Shape circle = new Circle(10, 20, 5), rect = new Rectangle(15, 5, 10, 15);
    Point point(10, 25);

    circle->isColliding(point); //true
    rect->isColliding(point); // false
    circle->isColliding(rect); //true
    //Same as
    rect->isColliding(circle); // true
```

You can specify an additional reference point (refX, refY) to the constructors thath defines the origin of the shape, used in rotations and displacements of the shape :
```c++
    Circle circle(refX, refY, centerX, centerY, radius);
```
*Note : rectangles always take the supplied (x, y) point as origin*

You can also load and save any shape from/to file :
```c++
    Polygon polygon(refX, refY, "path/to/file");
    polygon->save(filename, deltaX, deltaY);
```

Finally, you can also move and rotate the shapes, and get information about rotation and position:
```c++    
    Shape rect = new Rectangle(15, 5, 10, 15);
    rect->move(2, 2);
    rect->getCoordinates(x, y); //x = 2 and y = 2
    rect->add(5, 5); // 7, 7
    rect->rotate(180); //Rotate 180 degrees around rect origin (upper left corner)
    rect->rotate(-90, rectCenter); // Rotate 270 degrees around a point
    rect->setRotation(0); //Set rotation to an absolute angle value
    rect->getRotation(); // = 0
```

### Editor

The editor allows to draw shapes by hand and to save them to use in your application.

Commands:

    Left click : add point, drag point if a point was near the click position
    Right click : delete last point
    Midddle click (mouse wheel button) : Move last point
    Mouse wheel : delete points, leaving at least 4 points
    K : save shape to file
    O: open background image,
    L : load shape from file
    Left/right arrows : adjust line bright
    W/A/S/D : move shape
    Space/Enter : Not entirely implemented (does nothing)


### Misc

The majority of variables names, comments and messages are in French.

The polygon decomposition algorithm was written by Mark Bayazit and published at https://mpen.ca/406/bayazit
