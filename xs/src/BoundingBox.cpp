#include "BoundingBox.hpp"
#include <algorithm>

namespace Slic3r {

template <class PointClass>
BoundingBoxBase<PointClass>::BoundingBoxBase(const std::vector<PointClass> &points)
{
    typename std::vector<PointClass>::const_iterator it = points.begin();
    this->min.x = this->max.x = it->x;
    this->min.y = this->max.y = it->y;
    for (++it; it != points.end(); ++it) {
        this->min.x = std::min(it->x, this->min.x);
        this->min.y = std::min(it->y, this->min.y);
        this->max.x = std::max(it->x, this->max.x);
        this->max.y = std::max(it->y, this->max.y);
    }
}
template BoundingBoxBase<Point>::BoundingBoxBase(const std::vector<Point> &points);

template <class PointClass>
BoundingBox3Base<PointClass>::BoundingBox3Base(const std::vector<PointClass> &points)
    : BoundingBoxBase<PointClass>(points)
{
    typename std::vector<PointClass>::const_iterator it = points.begin();
    this->min.z = this->max.z = it->z;
    for (++it; it != points.end(); ++it) {
        this->min.z = std::min(it->z, this->min.z);
        this->max.z = std::max(it->z, this->max.z);
    }
}
template BoundingBox3Base<Pointf3>::BoundingBox3Base(const std::vector<Pointf3> &points);

BoundingBox::BoundingBox(const Lines &lines)
{
    Points points;
    for (Lines::const_iterator line = lines.begin(); line != lines.end(); ++line) {
        points.push_back(line->a);
        points.push_back(line->b);
    }
    *this = BoundingBox(points);
}

void
BoundingBox::polygon(Polygon* polygon) const
{
    polygon->points.clear();
    polygon->points.resize(4);
    polygon->points[0].x = this->min.x;
    polygon->points[0].y = this->min.y;
    polygon->points[1].x = this->max.x;
    polygon->points[1].y = this->min.y;
    polygon->points[2].x = this->max.x;
    polygon->points[2].y = this->max.y;
    polygon->points[3].x = this->min.x;
    polygon->points[3].y = this->max.y;
}

template <class PointClass> void
BoundingBoxBase<PointClass>::scale(double factor)
{
    this->min.scale(factor);
    this->max.scale(factor);
}
template void BoundingBoxBase<Point>::scale(double factor);
template void BoundingBoxBase<Pointf3>::scale(double factor);

template <class PointClass> void
BoundingBoxBase<PointClass>::merge(const PointClass &point)
{
    this->min.x = std::min(point.x, this->min.x);
    this->min.y = std::min(point.y, this->min.y);
    this->max.x = std::max(point.x, this->max.x);
    this->max.y = std::max(point.y, this->max.y);
}
template void BoundingBoxBase<Point>::merge(const Point &point);

template <class PointClass> void
BoundingBoxBase<PointClass>::merge(const BoundingBoxBase<PointClass> &bb)
{
    this->min.x = std::min(bb.min.x, this->min.x);
    this->min.y = std::min(bb.min.y, this->min.y);
    this->max.x = std::max(bb.max.x, this->max.x);
    this->max.y = std::max(bb.max.y, this->max.y);
}
template void BoundingBoxBase<Point>::merge(const BoundingBoxBase<Point> &bb);

template <class PointClass> void
BoundingBox3Base<PointClass>::merge(const PointClass &point)
{
    BoundingBoxBase<PointClass>::merge(point);
    this->min.z = std::min(point.z, this->min.z);
    this->max.z = std::max(point.z, this->max.z);
}
template void BoundingBox3Base<Pointf3>::merge(const Pointf3 &point);

template <class PointClass> void
BoundingBox3Base<PointClass>::merge(const BoundingBox3Base<PointClass> &bb)
{
    BoundingBoxBase<PointClass>::merge(bb);
    this->min.z = std::min(bb.min.z, this->min.z);
    this->max.z = std::max(bb.max.z, this->max.z);
}
template void BoundingBox3Base<Pointf3>::merge(const BoundingBox3Base<Pointf3> &bb);

template <class PointClass> PointClass
BoundingBoxBase<PointClass>::size() const
{
    return PointClass(this->max.x - this->min.x, this->max.y - this->min.y);
}
template Point BoundingBoxBase<Point>::size() const;

template <class PointClass> PointClass
BoundingBox3Base<PointClass>::size() const
{
    return PointClass(this->max.x - this->min.x, this->max.y - this->min.y, this->max.z - this->min.z);
}
template Pointf3 BoundingBox3Base<Pointf3>::size() const;

template <class PointClass> void
BoundingBoxBase<PointClass>::translate(coordf_t x, coordf_t y)
{
    this->min.translate(x, y);
    this->max.translate(x, y);
}
template void BoundingBoxBase<Point>::translate(coordf_t x, coordf_t y);

template <class PointClass> void
BoundingBox3Base<PointClass>::translate(coordf_t x, coordf_t y, coordf_t z)
{
    this->min.translate(x, y, z);
    this->max.translate(x, y, z);
}
template void BoundingBox3Base<Pointf3>::translate(coordf_t x, coordf_t y, coordf_t z);

template <class PointClass> PointClass
BoundingBoxBase<PointClass>::center() const
{
    return PointClass(
        (this->max.x + this->min.x)/2,
        (this->max.y + this->min.y)/2
    );
}
template Point BoundingBoxBase<Point>::center() const;

template <class PointClass> PointClass
BoundingBox3Base<PointClass>::center() const
{
    return PointClass(
        (this->max.x + this->min.x)/2,
        (this->max.y + this->min.y)/2,
        (this->max.z + this->min.z)/2
    );
}
template Pointf3 BoundingBox3Base<Pointf3>::center() const;

}
