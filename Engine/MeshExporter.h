#pragma once

#include "Mesh.h"
#include <string>

namespace Orb {

class MeshExporter {

public:
    MeshExporter(std::shared_ptr<MeshData> mesh);

    ~MeshExporter();

    void Export(std::string file);

    static std::shared_ptr<MeshData> Import(std::string file);

private:
    std::string file = "";
    std::shared_ptr<MeshData> mesh;
};

}
