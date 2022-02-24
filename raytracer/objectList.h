#pragma once
#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "object.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class ObjectList{
public:
    ObjectList() {}
    // shared_pt allows multiple geometries to share a common instance
    /*
    std::shared_ptr is a smart pointer that retains shared ownership of an object through a pointer. Several shared_ptr objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens:

    the last remaining shared_ptr owning the object is destroyed;
    the last remaining shared_ptr owning the object is assigned another pointer via operator= or reset().
    */
    ObjectList(shared_ptr<Object> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Object> object) { objects.push_back(object); }

    //reading of a class variables is okay inside of the function, but writing inside of this function will generate a compiler error.
    bool getIntersect(const Ray& r, double t_min, double t_max, intersection& intersect) const;

public:
    std::vector<shared_ptr<Object>> objects;
};

#endif