#include "world.h"

namespace renderer {

void World::addObject(Object&& object) {
    objects_.push_back(std::move(object));
}

void World::addCamera(Camera&& camera) {
    cameras_.push_back(std::move(camera));
}

const std::vector<Object>& World::getObjects() const {
    return objects_;
}

const std::vector<Camera>& World::getCameras() const {
    return cameras_;
}

World::PrimitiveIterator::PrimitiveIterator(const World& world, unsigned int object_index,
                                            unsigned int triangle_index)
    : world_(std::cref(world)), object_index_(object_index), triangle_index_(triangle_index) {
}

const Triangle& World::PrimitiveIterator::operator*() const {
    return world_.get().objects_[object_index_].getTriangles()[triangle_index_];
}

World::PrimitiveIterator& World::PrimitiveIterator::operator++() {
    ++triangle_index_;
    if (triangle_index_ >= world_.get().objects_[object_index_].getTriangles().size()) {
        triangle_index_ = 0;
        ++object_index_;
    }
    return *this;
}

bool World::PrimitiveIterator::operator==(const PrimitiveIterator& other) const {
    return object_index_ == other.object_index_ && triangle_index_ == other.triangle_index_;
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
