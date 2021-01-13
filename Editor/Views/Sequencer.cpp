
#include "Editor/Views/Sequencer.h"
#include "Editor/Utils.hpp"

#include "Editor/Tasks/MeshImportTask.h"
#include "Editor/Tasks/TaskWorker.h"
#include "Editor/Global.h"

#include "Engine/Video.h"

#include <string>
#include <imgui.h>


namespace Orb {

    Sequencer::Sequencer() {

        // sequence with default values
        sequence.mFrameMin = 0;
        sequence.mFrameMax = 1000;

        /**/
    }

    Sequencer::~Sequencer() {

    }

    void Sequencer::OnRender() {

        ImGui::Begin("Sequencer");

        ImGui::Separator();

        // Fill sequencer
        {
            //if (this->sequence.myItems.size() == 1) {

            this->sequence.myItems.clear();
            auto& videosEnts = Global::GetInstance().Renderer->EntitiesByType(IEntity::EntityType::Video);

            // Get all video entities and fill sequencer
            for (int i = 0; i < videosEnts.size(); i++) {
                std::shared_ptr<Video> ent = std::dynamic_pointer_cast<Video, IEntity>(videosEnts[i]);
                if (ent->SyncCurrFrameIdxSequencer()) {
                    this->VideoSliderValue = ent->GetFrameIdx();
                }
                this->sequence.myItems.push_back(
                    TrackSequence::TrackSequenceItem{
                        TrackSequenceItemType::Clip,
                        0,
                        ent->GetNumFrames(),
                        true,
                        ent->GetName()
                    }
                );
            }
            //}
        }



        ImGui::PushItemWidth(100);
        ImGui::InputInt("Current Frame", &this->VideoSliderValue);
        ImGui::PopItemWidth();
        ImGui::Separator();

        // ------------------------------
        // Draw the sequencer
        // ------------------------------

        ImSequencer::Sequencer(
            &this->sequence,
            &this->VideoSliderValue,
            &this->SequencerExpandet,
            &this->SequencerSelectedEntry,
            &this->FirstFrame,
            ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME
        );

        ImGui::End();
    }

}
