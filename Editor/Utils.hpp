#pragma once


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <GL/glew.h>
#include <iostream>

#include <Eigen/Core>
#include <glm/glm.hpp>
#include <string>
#include <nfd.h>

namespace Orb {

class Utils {

public:
    static glm::mat4 ToGLM_Mat4f(Eigen::Matrix4f mat4) {

        glm::mat4 res = glm::mat4(0.0f);

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                res[j][i] = mat4(i, j);
            }
        }
        return res;
    }



    static glm::vec3 ToGLM_Vec3(Eigen::Vector3f vec3) {
        return glm::vec3(vec3(0), vec3(1), vec3(2));
    }

    static std::string PickFile() {
        std::string file = "";
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );

        if ( result == NFD_OKAY ) {
            puts("Success!");
            file = outPath;
            puts(outPath);
            free(outPath);
        }
        else if ( result == NFD_CANCEL ) {
            puts("User pressed cancel.");
        }
        else {
            printf("Error: %s\n", NFD_GetError() );
        }
        return file;
    }

    static std::string SaveFile() {
        std::string file = "";
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_SaveDialog(NULL, NULL, &outPath );

        if ( result == NFD_OKAY ) {
            puts("Success!");
            file = outPath;
            puts(outPath);
            free(outPath);
        }
        else if ( result == NFD_CANCEL ) {
            puts("User pressed cancel.");
        }
        else {
            printf("Error: %s\n", NFD_GetError() );
        }
        return file;
    }

};

}
