using System;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Text;

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
            
            this.orbitalRootDir = Path.GetFullPath("../../../../../../"); // Orbital root dir
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
             commit 70703da7829ba74a92a08fe3c6fe0664c39c97c9 (HEAD -> docking, origin/docking)                                       
             Merge: b11518d7 e18abe36                                                                                                
             Author: ocornut <omarcornut@gmail.com>                                                                                  
             Date:   Fri Jan 8 18:55:16 2021 +0100
            
             Merge branch 'master' into docking
             */
            this.execCommand(
                "git",
                "checkout 70703da7829ba74a92a08fe3c6fe0664c39c97c9",
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
                commit ac4bd3506296a808675334eeb569146ec8f63dd6 (HEAD -> develop, origin/develop)
                Author: thedmd <michcic@gmail.com>
                Date:   Tue Dec 29 16:10:00 2020 +0100

                Examples: Correct case of `data` directory (#97)
                 */
                this.execCommand(
                    "git",
                    "checkout ac4bd3506296a808675334eeb569146ec8f63dd6", 
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
             commit 3765b5ab9c7f83db0065b554068ad8eff57cb3a0 (HEAD -> master, origin/master, origin/HEAD)                            
             Author: Cedric Guillemet <ceguille@microsoft.com>                                                                      
             Date:   Sat Jan 2 18:28:22 2021 +0100            
            
             over filter for operation

             */
            this.execCommand(
                "git",
                "checkout 3765b5ab9c7f83db0065b554068ad8eff57cb3a0",
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
            var psi = new ProcessStartInfo
            {
                FileName = command.Trim(),
                Arguments = arguments,
                WorkingDirectory = workdir,
                WindowStyle = ProcessWindowStyle.Hidden,
                UseShellExecute = false,
                CreateNoWindow = true,
            };

            Process proc = Process.Start(psi);
            proc.WaitForExit();

        }

    }
}
