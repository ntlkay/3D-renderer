#include "world.h"

namespace renderer {

void World::addObject(Object obj) {
    objects_.push_back(std::move(obj));
}

void World::addCamera(const Camera cam) {
    cameras_.push_back(cam);
}

std::vector<Object> World::getObjects() const {
    return objects_;
}

std::vector<Camera> World::getCameras() const {
    return cameras_;
}

World::PrimitiveIterator::PrimitiveIterator(const World& w, size_t oi, size_t ti)
    : world_(w), objIndex_(oi), triIndex_(ti) {
}

const Triangle& World::PrimitiveIterator::operator*() const {
    return world_.objects_[objIndex_].getTriangles()[triIndex_];
}

World::PrimitiveIterator& World::PrimitiveIterator::operator++() {
    ++triIndex_;
    if (triIndex_ >= world_.objects_[objIndex_].getTriangles().size()) {
        triIndex_ = 0;
        ++objIndex_;
    }
    return *this;
}

bool World::PrimitiveIterator::operator==(const PrimitiveIterator& other) const {
    return objIndex_ == other.objIndex_ && triIndex_ == other.triIndex_;
}

bool World::PrimitiveIterator::operator!=(const PrimitiveIterator& other) const {
    return !(*this == other);
}

World::PrimitiveIterator World::begin() const {
    return PrimitiveIterator(*this, 0, 0);
}

World::PrimitiveIterator World::end() const {
    return PrimitiveIterator(*this, objects_.size(), 0);
}

}  // namespace renderer
