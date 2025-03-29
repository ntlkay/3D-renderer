#pragma once
#include <vector>

#include "camera.h"
#include "object.h"

namespace renderer {

class World {

private:
    std::vector<Object> objects_;
    std::vector<Camera> cameras_;

public:
    void addObject(Object obj);
    void addCamera(Camera cam);

    std::vector<Object> getObjects() const;
    std::vector<Camera> getCameras() const;

    class PrimitiveIterator {
    private:
        const World& world_;
        size_t objIndex_;
        size_t triIndex_;

    public:
        PrimitiveIterator(const World& w, size_t oi, size_t ti);

        const Triangle& operator*() const;
        PrimitiveIterator& operator++();
        bool operator==(const PrimitiveIterator& other) const;
        bool operator!=(const PrimitiveIterator& other) const;
    };

    PrimitiveIterator begin() const;
    PrimitiveIterator end() const;
};

}  // namespace renderer
