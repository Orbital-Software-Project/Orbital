using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
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


        public Setup()
        {

            this.workDir = Path.Combine(Directory.GetCurrentDirectory(), "tmp");
            this.orbitalRootDir = "C:\\Users\\rphil\\Desktop\\Orbital";
            this.orbitalThirdPartyDir = Path.Combine(orbitalRootDir, "ThirdParty");

            if (Directory.Exists(workDir))
            {
                //Directory.Delete(workDir, true);
            } 

            Directory.CreateDirectory(workDir);
        }


        public void Start()
        {

            this.setupVCPKG();
            this.setupImGui();
            this.setupImGuiNodeEditor();
            this.setupDBoW2(this.vcpkgToolchainFile);
            this.setupG2O();
            this.setupOpenvslam();

            //this.setupAssimp();

            this.setupOrbital();
        }


        private void setupOrbital()
        {
            StringBuilder cmakeArgs = new StringBuilder();

            cmakeArgs.Append("-DCMAKE_TOOLCHAIN_FILE=" + this.vcpkgToolchainFile);
            cmakeArgs.Append(" ");
            cmakeArgs.Append("-DVCPKG_TARGET_TRIPLET=x64-windows");
            cmakeArgs.Append(" ");
            cmakeArgs.Append("-DCMAKE_BUILD_TYPE=Debug");
            cmakeArgs.Append(" ");
            cmakeArgs.Append("-Dg2o_DIR=" + Path.Combine(this.g2oInstallDir, "lib", "cmake", "g2o"));
            cmakeArgs.Append(" ");
            cmakeArgs.Append("-DDBoW2_DIR=" + Path.Combine(this.dbow2InstallDir, "lib", "cmake", "DBoW2"));
            cmakeArgs.Append(" "); 
            cmakeArgs.Append("..");

            // cmake config
            ProcessStartInfo cmakeConfig = new ProcessStartInfo("cmake", cmakeArgs.ToString());
            cmakeConfig.WorkingDirectory = Path.Combine(this.orbitalRootDir, "Build");
            Process.Start(cmakeConfig).WaitForExit();


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

            if(!File.Exists(Path.Combine(vcpkgDir, "vcpkg.exe")))
            {
                ProcessStartInfo vcpkgBootstrap = new ProcessStartInfo("cmd", "/C bootstrap-vcpkg.bat");
                vcpkgBootstrap.WorkingDirectory = vcpkgDir;
                Process.Start(vcpkgBootstrap).WaitForExit();
            }
            

            ProcessStartInfo vcpkgInstall = new ProcessStartInfo("cmd", "/C vcpkg.exe install assimp opencv eigen3 yaml-cpp glog suitesparse glfw3 glew nativefiledialog glm --triplet x64-windows");
            vcpkgInstall.WorkingDirectory = vcpkgDir;
            Process.Start(vcpkgInstall).WaitForExit();

        }

        private void setupDBoW2(string vcpkgToolchainFile)
        {
            this.dbow2InstallDir = Path.Combine(this.orbitalThirdPartyDir, "DBoW2");


            if (!Directory.Exists(this.dbow2InstallDir))
            {
                ProcessStartInfo gitClone = new ProcessStartInfo("git", "clone https://github.com/shinsumicco/DBoW2.git");
                gitClone.WorkingDirectory = this.workDir;
                Process.Start(gitClone).WaitForExit();

                // Configure Project
                DirectoryInfo dirInfo = Directory.CreateDirectory(Path.Combine(this.workDir, "DBoW2", "Build"));



                StringBuilder cmakeArgs = new StringBuilder();
                cmakeArgs.Append("-DCMAKE_TOOLCHAIN_FILE=" + this.vcpkgToolchainFile);
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DVCPKG_TARGET_TRIPLET=x64-windows");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DBUILD_SHARED_LIBS=ON");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DCMAKE_BUILD_TYPE=Debug");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DCMAKE_INSTALL_PREFIX=" + this.dbow2InstallDir);
                cmakeArgs.Append(" ");
                cmakeArgs.Append("..");

                ProcessStartInfo cmakeConfig = new ProcessStartInfo("cmake", cmakeArgs.ToString());
                cmakeConfig.WorkingDirectory = dirInfo.FullName;

                Process.Start(cmakeConfig).WaitForExit();

                // Build and Install
                ProcessStartInfo cmakeBuild = new ProcessStartInfo("cmake", "--build . --target install --config Debug");
                cmakeBuild.WorkingDirectory = dirInfo.FullName;
                Process.Start(cmakeBuild).WaitForExit();

            }

        }

        private void setupOpenvslam()
        {
            this.openvslamInstallDir = Path.Combine(this.orbitalThirdPartyDir, "openvslam");



            ProcessStartInfo openvslam = new ProcessStartInfo("git", "clone -b develop https://github.com/Cambloid/openvslam.git");
            openvslam.WorkingDirectory = this.orbitalThirdPartyDir;

            Process.Start(openvslam).WaitForExit();

         }

        private void setupImGui()
        {
            string zipFile = Path.Combine(this.workDir, "v1.77.zip");
            if(!File.Exists(zipFile))
            {
                System.Net.WebClient client = new System.Net.WebClient();
                client.DownloadFile("https://github.com/ocornut/imgui/archive/v1.77.zip", zipFile);

                ZipFile.ExtractToDirectory(zipFile, this.orbitalThirdPartyDir);
            }
        }

        private void setupImGuiNodeEditor()
        {
            this.imguiNodeEditorInstallDir = Path.Combine(this.orbitalThirdPartyDir, "imgui-node-editor");

            if (!File.Exists(this.imguiNodeEditorInstallDir))
            {
                ProcessStartInfo openvslam = new ProcessStartInfo("git", "clone https://github.com/thedmd/imgui-node-editor.git");
                openvslam.WorkingDirectory = this.orbitalThirdPartyDir;
                Process.Start(openvslam).WaitForExit();



            }
        }

        private void setupG2O()
        {

            string g2oSrcDir   = Path.Combine(this.workDir, "g2o");
            string g2oBuildDir = Path.Combine(g2oSrcDir, "Build");
            this.g2oInstallDir = Path.Combine(orbitalThirdPartyDir, "g2o");

            if(!Directory.Exists(g2oSrcDir))
            {
                ProcessStartInfo g2o = new ProcessStartInfo("git", "clone https://github.com/RainerKuemmerle/g2o.git");
                g2o.WorkingDirectory = this.workDir;
                Process.Start(g2o).WaitForExit();
            }

            if (!Directory.Exists(this.g2oInstallDir))
            {

                StringBuilder cmakeArgs = new StringBuilder();
                cmakeArgs.Append("-DCMAKE_TOOLCHAIN_FILE=" + this.vcpkgToolchainFile);
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DVCPKG_TARGET_TRIPLET=x64-windows");
                cmakeArgs.Append(" ");
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

                // cmake config
                ProcessStartInfo cmakeConfig = new ProcessStartInfo("cmake", cmakeArgs.ToString());
                cmakeConfig.WorkingDirectory = g2oBuildDir;
                Process.Start(cmakeConfig).WaitForExit();


                // Build and Install
                ProcessStartInfo cmakeBuild = new ProcessStartInfo("cmake", "--build . --target install --config Debug");
                cmakeBuild.WorkingDirectory = g2oBuildDir;
                Process.Start(cmakeBuild).WaitForExit();

            }
        }

        private void setupAssimp()
        {
            this.assimpInstallDir = Path.Combine(this.orbitalThirdPartyDir, "assimp");

            if (!Directory.Exists(this.assimpInstallDir))
            {

                string zipFile        = Path.Combine(this.workDir, "v5.0.1.zip");
                string assimpSrcDir   = Path.Combine(this.workDir, "assimp-5.0.1");
                string assimpBuildDir = Path.Combine(assimpSrcDir, "Build");

                if (!File.Exists(zipFile))
                {
                    System.Net.WebClient client = new System.Net.WebClient();
                    client.DownloadFile("https://github.com/assimp/assimp/archive/v5.0.1.zip", zipFile);

                    ZipFile.ExtractToDirectory(zipFile, this.workDir);
                }

                StringBuilder cmakeArgs = new StringBuilder();
                cmakeArgs.Append("-DASSIMP_BUILD_TESTS=OFF");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DCMAKE_BUILD_TYPE=Debug");
                cmakeArgs.Append(" ");
                cmakeArgs.Append("-DCMAKE_INSTALL_PREFIX=" + this.assimpInstallDir);
                cmakeArgs.Append(" ");
                cmakeArgs.Append("..");

                Directory.CreateDirectory(assimpBuildDir);

                // cmake config
                ProcessStartInfo cmakeConfig = new ProcessStartInfo("cmake", cmakeArgs.ToString());
                cmakeConfig.WorkingDirectory = assimpBuildDir;
                Process.Start(cmakeConfig).WaitForExit();

                // Build and Install
                ProcessStartInfo cmakeBuild = new ProcessStartInfo("cmake", "--build . --target install --config Debug");
                cmakeBuild.WorkingDirectory = assimpBuildDir;
                Process.Start(cmakeBuild).WaitForExit();


            }
        }

    }
}
