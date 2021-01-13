#pragma once

#include "Shader.h"

#include <memory>
#include <glm/glm.hpp>

namespace Orb {

    
class IEntity {

public:
    enum class EntityType {
        Any,
        Video,
        Camera,
        Group,
        Mesh
    };

public:
    IEntity() {};

    virtual ~IEntity() {}

public:
    virtual void Draw(std::shared_ptr<Shader> shader) = 0;

    virtual EntityType GetEntityType() = 0;

    std::string GetName() { return this->name; }

    void SetName(std::string name) { this->name = name; }

    bool Selected = false;

    bool Visible = true;

    bool VisibleInOutliner = true;

    glm::mat4 Matrix = glm::mat4(1.0f);

    glm::vec3 Position = glm::vec4(0.0f);

    glm::vec3 Rotation = glm::vec4(0.0f);

    glm::vec3 Scale = glm::vec4(1.0f);

    

    std::shared_ptr<void> Data;

private:
    std::string name = "IEntity";

};

}
