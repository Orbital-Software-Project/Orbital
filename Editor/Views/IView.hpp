#pragma once

namespace Orb {
    class IView {

    public:
        enum class ViewWindowRequest {
            None,
            Close,
        };

    public:
        virtual ~IView() {}

        virtual void OnRender() = 0;


    };
}