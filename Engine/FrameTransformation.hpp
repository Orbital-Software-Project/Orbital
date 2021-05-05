#pragma once

#include "Engine/IEntity.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include <unordered_map>

namespace Orb {

class FrameTransformation : public IEntity {

public:
    FrameTransformation() {
        this->SetName("Entity frame transformation"); 
    }

    ~FrameTransformation() {
        this->frame_mat.clear();
    }

    void Draw(std::shared_ptr<Shader> shader) {
        //for (std::shared_ptr<IEntity> ent : this->subEntities) {
            
            if (this->frame_mat.find(this->currFrameIdx) != this->frame_mat.end()) {
                auto backup_mat = ent->Matrix;
                ent->Matrix = this->frame_mat.at(this->currFrameIdx);
                ent->Draw(shader);
                ent->Matrix = backup_mat;
            }
            else {
                ent->Draw(shader);
            }
            
        //}
    }

    void SetFrameTransformation(int frame, glm::mat4 transformation) {
        if (this->frame_mat.find(frame) != this->frame_mat.end()) {
            this->frame_mat[frame] = transformation;
        
        } else {
            this->frame_mat.insert(
                std::pair<int, glm::mat4>(frame, transformation)
            );
        }
    }

    IEntity::EntityType GetEntityType() { 
        return  IEntity::EntityType::FrameTransformation;
    }

    void SetEntity(std::shared_ptr<IEntity> entity) {
        this->ent  = entity;
    }

    void SetVisibility(bool visible) {
        this->Visible = visible;
        this->ent->Visible = visible;
        /*for (std::shared_ptr<IEntity> ent : this->subEntities) {
            ent->Visible = visible;
        }*/
    }

private:
    std::shared_ptr<IEntity> ent;
    int currFrameIdx = 0;
    std::unordered_map<int, glm::mat4> frame_mat;
};

}
