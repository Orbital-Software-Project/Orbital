#pragma once

namespace Orb {
    class IView {

    public:
        enum class ViewWindowRequest {
            None,
            Close,

            // Toolbar
            Open_MapViewer,
            Open_Outliner,
            Open_Sequencer,
            Open_Taskpanel,
            Open_VideoPrev,
            Open_PropertyEd,

        };

    public:
        virtual ~IView() {}

        virtual void OnRender() = 0;


        bool HasRequest(ViewWindowRequest& outRequest) {
            if (!this->Open) {
                outRequest = ViewWindowRequest::Close;

                // Request handled
                this->Request = ViewWindowRequest::None;
                return true;
            }

            outRequest = this->Request;

            // Request handled
            this->Request = ViewWindowRequest::None;

            // return true if request was set
            return outRequest != ViewWindowRequest::None;
        }

    protected:
        ViewWindowRequest Request = ViewWindowRequest::None;
	
        bool Open = true;

    };
}