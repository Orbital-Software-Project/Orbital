#pragma once

#include "IEntity.h"

#include "Mesh.h"
#include "Camera.h"
#include "EntityGroup.hpp"

#include <GL/glew.h>
#include <vector>
#include <memory>

namespace Orb {

class SceneRenderer {

public:
    SceneRenderer();

    ~SceneRenderer();

    GLuint RenderToTexture(std::shared_ptr<Shader> shader,int width, int height);

    void Render(std::shared_ptr<Shader> shader, int width, int height);

    void RemoveEntity(std::shared_ptr<IEntity> entity);

    void AddEntity(std::shared_ptr<IEntity> entity);

    std::vector<std::shared_ptr<IEntity>> GetEntities();

    std::vector<std::shared_ptr<IEntity>> EntitiesByType(IEntity::EntityType entityType) {
        std::vector<std::shared_ptr<IEntity>> entsToReturn;
        auto ents = this->GetEntities();
        this->getEntitiesByType(ents, entsToReturn, entityType);
        return entsToReturn;
    }

private:
    void setFBOSize(int widht, int height);

    // template class parameter MUST be derived from IEntity
    void getEntitiesByType(std::vector<std::shared_ptr<IEntity>> &entities, std::vector<std::shared_ptr<IEntity>> &entsToReturn, IEntity::EntityType entityType) {
        for (std::shared_ptr<IEntity>& ent : entities) {
            if (ent->GetEntityType() == IEntity::EntityType::Group) {
                std::shared_ptr<EntityGroup> grp = std::dynamic_pointer_cast<EntityGroup, IEntity>(ent);
                auto children = grp->GetChildEntites();
                this->getEntitiesByType(children, entsToReturn, entityType);
            }
            if (ent->GetEntityType() == entityType || entityType == IEntity::EntityType::Any) {
                entsToReturn.push_back(ent);
            }
        }
    }


private:
    GLuint fbo = 0;
    GLuint rbo = 0;
    GLuint mapTexture = 0;
    GLuint vbo = 0, vao = 0, ebo = 0;
    int width = 800, height = 600;

    std::vector<std::shared_ptr<IEntity>> entities;

};

}
