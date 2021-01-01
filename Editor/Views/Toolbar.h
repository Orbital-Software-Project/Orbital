#pragma once

#include "Editor/Views/IView.hpp"

namespace Orb {


class Toolbar : public IView {

public:
    Toolbar();

    ~Toolbar();

    void OnRender();

};

}
