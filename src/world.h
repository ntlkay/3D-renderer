#pragma once
#include <functional>
#include <vector>

#include "camera.h"
#include "object.h"

namespace renderer {

class World {
private:
    std::vector<Object> objects_;
    std::vector<Camera> cameras_;

public:
    void addObject(Object&& object);
    void addCamera(Camera&& camera);

    const std::vector<Object>& getObjects() const;
    const std::vector<Camera>& getCameras() const;

    class PrimitiveIterator {
    private:
        std::reference_wrapper<const World> world_;
        unsigned int object_index_;
        unsigned int triangle_index_;

    public:
        PrimitiveIterator(const World& world, unsigned int object_index,
                          unsigned int triangle_index);

        const Triangle& operator*() const;
        PrimitiveIterator& operator++();
        bool operator==(const PrimitiveIterator& other) const;
        bool operator!=(const PrimitiveIterator& other) const;
    };

    PrimitiveIterator begin() const;
    PrimitiveIterator end() const;
};

}  // namespace renderer
