using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Text;

using System.Runtime.InteropServices;

namespace Build
{
    public class Setup
    {

        private string workDir = string.Empty;
        private string orbitalRootDir = string.Empty;
        private string orbitalThirdPartyDir = string.Empty;
        private string vcpkgToolchainFile = string.Empty;
        private string g2oInstallDir = string.Empty;
        private string dbow2InstallDir = string.Empty;
        private string openvslamInstallDir = string.Empty;
        private string assimpInstallDir = string.Empty;
        private string imguiNodeEditorInstallDir = string.Empty;

        private bool isWindows = false;

        public Setup()
        {

            this.workDir = Path.Combine(Directory.GetCurrentDirectory(), "tmp");
            this.orbitalRootDir = "/home/celvin/Documents/Projects/Orbital";
            this.orbitalThirdPartyDir = Path.Combine(this.orbitalRootDir, "ThirdParty");

            if (Directory.Exists(workDir))
            {
                //Directory.Delete(workDir, true);
            }

            Directory.CreateDirectory(workDir);
        }

        public void Start()
        {
            this.isWindows = RuntimeInformation.IsOSPlatform(OSPlatform.Windows);

            this.setupVCPKG();
            this.setupImGui();
            this.setupImGuiNodeEditor();
            this.setupImGuizmoSequencer();
            this.setupDBoW2();
            this.setupG2O();
            this.setupOpenvslam();
            this.setupOrbital();
        }

        private void setupOrbital()
        {
            if(!this.isWindows) {
                return;
            }
            

            string orbitalBuildDir = Path.Combine(this.orbitalRootDir, "Build");
            if (!Directory.Exists(orbitalBuildDir))
            {
                Directory.CreateDirectory(orbitalBuildDir);
            }

            StringBuilder cmakeArgs = new StringBuilder();
            cmakeArgs.Append("-DCMAKE_TOOLCHAIN_FILE=" + this.vcpkgToolchainFile);
            cmakeArgs.Append(" ");
           
            if(this.isWindows) {
                cmakeArgs.Append("-DVCPKG_TARGET_TRIPLET=x64-windows");
                cmakeArgs.Append(" ");
            }
            
            cmakeArgs.Append("-DCMAKE_BUILD_TYPE=Debug");
            cmakeArgs.Append(" ");
            cmakeArgs.Append("-Dg2o_DIR=" + Path.Combine(this.g2oInstallDir, "lib", "cmake", "g2o"));
            cmakeArgs.Append(" ");
            cmakeArgs.Append("-DDBoW2_DIR=" + Path.Combine(this.dbow2InstallDir, "lib", "cmake", "DBoW2"));
            cmakeArgs.Append(" "); 
            cmakeArgs.Append("..");

            // CMake config
            this.execCommand(
                "cmake", 
                cmakeArgs.ToString(),
                orbitalBuildDir
                );

            // Build
            this.execCommand(
                "cmake",
                "--build . --config Debug",
                orbitalBuildDir
                );

        }

        private void setupVCPKG()
        {

            string zipFile = Path.Combine(this.workDir, "2020.06.zip");
            string vcpkgDir = Path.Combine(this.orbitalThirdPartyDir, "vcpkg-2020.06");
            this.vcpkgToolchainFile = Path.Combine(vcpkgDir, "scripts", "buildsystems", "vcpkg.cmake");


            if (!File.Exists(zipFile)) {
                System.Net.WebClient client = new System.Net.WebClient();
                client.DownloadFile("https://github.com/microsoft/vcpkg/archive/2020.06.zip", zipFile);
            }
            
            if (!Directory.Exists(vcpkgDir))
            {
                ZipFile.ExtractToDirectory(zipFile, this.orbitalThirdPartyDir);
            }

            


            if(!File.Exists(Path.Combine(vcpkgDir, "vcpkg" + (this.isWindows ? ".exe" : ""))))
            {
                if(this.isWindows) {
                    this.execCommand(
                        "cmd", 
                        "/C bootstrap-vcpkg.bat", 
                        vcpkgDir
                    );
                } else {
                    this.execCommand(
                        "sh", 
                        "bootstrap-vcpkg.sh", 
                        vcpkgDir
                    );
                }
                
            }



            // HACK
            if(this.isWindows)  {
                this.execCommand(
                    "vcpkg.exe", 
                    "install assimp opencv eigen3 yaml-cpp glog suitesparse glfw3 glew nativefiledialog glm ceres --triplet x64-windows", 
                    vcpkgDir
                );
            } else {
                this.execCommand(
                    "bash",
                    "-c \"./vcpkg install assimp opencv eigen3 yaml-cpp glog suitesparse glfw3 glew nativefiledialog glm ceres\"", 
                    vcpkgDir
                );
            }
            

            
        }

        private void setupDBoW2()
        {
            this.dbow2InstallDir = Path.Combine(this.orbitalThirdPartyDir, "DBoW2");

            if (!Directory.Exists(this.dbow2InstallDir))
            {
                this.execCommand(
                    "git",
                    "clone https://github.com/shinsumicco/DBoW2.git",
                    this.workDir
                    );


                /*
                 commit e8cc74d24705d0ad61b68df4e1086d1deff67136 (HEAD -> master, origin/master, origin/HEAD)
                 Author: Paul Lesur <12513150+lesurp@users.noreply.github.com>
                 Date:   Fri Mar 13 02:38:12 2020 +0100
                 
                     match library name with the exported cmake name (#6)
                 
                     cmake's `find_library` is case sensitive, therefore we need to look for
                     `dbow2` since we install `libdbow2.so`
                 
                     Co-authored-by: Paul Lesur <gitconfig@lesurpaul.fr>
                 */
                this.execCommand(
                "git",
                "checkout e8cc74d24705d0ad61b68df4e1086d1deff67136",
                Path.Combine(this.workDir, "DBoW2")
                );

                // Configure Project
                DirectoryInfo dirInfo = Directory.CreateDirectory(Path.Combine(this.workDir, "DBoW2", "Build"));

                StringBuilder cmakeArgs = new StringBuilder();
                cmakeArgs.Append("-DCMAKE_TOOLCHAIN_FILE=" + this.vcpkgToolchainFile);
                
                if(this.isWindows) {
                    cmakeArgs.Append(" ");
                    cmakeArgs.Append("-DVCPKG_TARGET_TRIPLET=x64-windows");
                }
                
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DBUILD_SHARED_LIBS=ON");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DCMAKE_BUILD_TYPE=Debug");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DCMAKE_INSTALL_PREFIX=" + this.dbow2InstallDir);
                cmakeArgs.Append(" ");
                cmakeArgs.Append("..");


                // Config
                this.execCommand(
                    "cmake",
                    cmakeArgs.ToString(),
                    dirInfo.FullName
                    );


                // Build and Install
                this.execCommand(
                    "cmake",
                    "--build . --target install --config Debug",
                    dirInfo.FullName
                    );
            }

        }

        private void setupOpenvslam()
        {
            this.openvslamInstallDir = Path.Combine(this.orbitalThirdPartyDir, "openvslam");

            this.execCommand("git",
                "clone -b develop https://github.com/xdspacelab/openvslam.git",
                this.orbitalThirdPartyDir
                );


            /*
             commit 29435e14513a437719952ba916e5994e71ea6e55 (HEAD -> develop, origin/develop)
             Author: shinsumicco <shinsumicco@gmail.com>
             Date:   Wed Aug 19 01:42:00 2020 +0900
             
             Remove duplicated configuration in CMakeLists.txt
            */
            this.execCommand(
                "git", 
                "checkout 29435e14513a437719952ba916e5994e71ea6e55", 
                Path.Combine(this.orbitalThirdPartyDir, "openvslam")
                );

        }

        private void setupImGui()
        {
            this.execCommand(
                "git",
                "clone -b docking https://github.com/ocornut/imgui.git",
                this.orbitalThirdPartyDir
                );


            /*
             commit fa004ae79a287ad744772fcb6fa9d9c7dad7aeb4 (HEAD -> docking, origin/docking)
             Merge: 42575d4a 1ec464eb
             Author: ocornut <omarcornut@gmail.com>
             Date:   Tue Sep 22 17:00:57 2020 +0200
             
                 Merge branch 'master' into docking
             
                 # Conflicts:
                 #       examples/imgui_examples.sln
                 #       examples/imgui_impl_dx12.cpp
                 #       examples/imgui_impl_dx12.h
                 #       examples/imgui_impl_opengl3.cpp
                 #       imgui.cpp
                 #       imgui_demo.cpp
                 #       imgui_internal.h
                 #       imgui_widgets.cpp
             */
            this.execCommand(
                "git",
                "checkout fa004ae79a287ad744772fcb6fa9d9c7dad7aeb4",
                Path.Combine(this.orbitalThirdPartyDir, "imgui")
                );
        }

        private void setupImGuiNodeEditor()
        {
            this.imguiNodeEditorInstallDir = Path.Combine(this.orbitalThirdPartyDir, "imgui-node-editor");

            if (!File.Exists(this.imguiNodeEditorInstallDir))
            {
                this.execCommand(
                    "git", 
                    "clone https://github.com/thedmd/imgui-node-editor.git", 
                    this.orbitalThirdPartyDir
                    );

                /*
                 commit 687a72f940c76cf5064e13fe55fa0408c18fcbe4 (HEAD -> master, origin/master, origin/HEAD)
                 Author: thedmd <michcic@gmail.com>
                 Date:   Fri Sep 4 11:56:12 2020 +0200
                 
                 Convert GLuint to void* using intptr_t #62
                 */
                this.execCommand(
                    "git", 
                    "checkout 687a72f940c76cf5064e13fe55fa0408c18fcbe4", 
                    Path.Combine(this.orbitalThirdPartyDir, "imgui-node-editor")
                    );

            }
        }

        private void setupImGuizmoSequencer()
        {
            this.execCommand(
                    "git",
                    "clone https://github.com/CedricGuillemet/ImGuizmo.git",
                    this.orbitalThirdPartyDir
                    );


            /*
             commit 28c67f9f586aa83212ebbc057e6dc138b093fa35 (HEAD -> develop, origin/master, origin/HEAD, master)
             Author: None <none@none.ch>
             Date:   Wed Sep 23 10:59:46 2020 +0200
             
                 Add Docking WIP
             
                 + Checkout Git repo with specific commit id (latest)
             */
            this.execCommand(
                "git",
                "checkout 28c67f9f586aa83212ebbc057e6dc138b093fa35",
                Path.Combine(this.orbitalThirdPartyDir, "ImGuizmo")
                );

        }

        private void setupG2O()
        {

            string g2oSrcDir   = Path.Combine(this.workDir, "g2o");
            string g2oBuildDir = Path.Combine(g2oSrcDir, "Build");
            this.g2oInstallDir = Path.Combine(orbitalThirdPartyDir, "g2o");

            if (!Directory.Exists(g2oSrcDir))
            {
                this.execCommand(
                    "git",
                    "clone https://github.com/RainerKuemmerle/g2o.git",
                    this.workDir
                    );

                /*
                 commit 31ea135f7d374c72568807520f522d2ab4543d39 (HEAD -> master, origin/master, origin/HEAD)
                 Author: Rainer Kuemmerle <rainer.kuemmerle@gmail.com>
                 Date:   Mon Sep 7 17:29:02 2020 +0200
                 
                 Implement IO test for slam3d_addons
                 */
                this.execCommand(
                "git",
                "checkout 31ea135f7d374c72568807520f522d2ab4543d39",
                Path.Combine(this.workDir, "g2o")
                );
            }

            if (!Directory.Exists(this.g2oInstallDir))
            {

                StringBuilder cmakeArgs = new StringBuilder();
                cmakeArgs.Append("-DCMAKE_TOOLCHAIN_FILE=" + this.vcpkgToolchainFile);
                cmakeArgs.Append(" ");

                if(this.isWindows)  {
                    cmakeArgs.Append("-DVCPKG_TARGET_TRIPLET=x64-windows");
                    cmakeArgs.Append(" ");
                }
                
                cmakeArgs.Append("-DCMAKE_BUILD_TYPE=Debug");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DCMAKE_INSTALL_PREFIX=" + this.g2oInstallDir);
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DBUILD_SHARED_LIBS=ON");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DBUILD_UNITTESTS=OFF");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DG2O_USE_CHOLMOD=OFF");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DG2O_USE_CSPARSE=ON");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DG2O_USE_OPENGL=OFF");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DG2O_USE_OPENMP=ON");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("..");

                Directory.CreateDirectory(g2oBuildDir);

                // CMake config
                this.execCommand(
                    "cmake", 
                    cmakeArgs.ToString(), 
                    g2oBuildDir
                    );


                // Build and Install
                this.execCommand(
                    "cmake", 
                    "--build . --target install --config Debug", 
                    g2oBuildDir
                    );

            }
        }

        private void execCommand(string command, string arguments, string workdir)
        {
            ProcessStartInfo process = new ProcessStartInfo(command.Trim(), arguments);
            process.WorkingDirectory = workdir;
            Process.Start(process).WaitForExit();
        }
    
    }
}
