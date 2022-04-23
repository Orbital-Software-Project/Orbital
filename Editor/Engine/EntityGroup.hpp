#pragma once

#include "IEntity.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <memory>

namespace Orb {

class EntityGroup : public IEntity {

public:
    EntityGroup() { 
        this->SetName("Entity group"); 
    }

    ~EntityGroup() {}

    void Draw(std::shared_ptr<Shader> shader) {
        for (std::shared_ptr<IEntity> ent : this->subEntities) {
            ent->Draw(shader);
        }
    }

    IEntity::EntityType GetEntityType() { 
        return  IEntity::EntityType::Group; 
    }

    void AddEntity(std::shared_ptr<IEntity> entity) {
        this->subEntities.push_back(entity);
    }

    std::vector<std::shared_ptr<IEntity>> GetChildEntites() {
        return this->subEntities;
    }

    void SetVisibility(bool visible) {
        this->Visible = visible;
        for (std::shared_ptr<IEntity> ent : this->subEntities) {
            ent->Visible = visible;
        }
    }

private:
    std::vector<std::shared_ptr<IEntity>> subEntities;

};

}
