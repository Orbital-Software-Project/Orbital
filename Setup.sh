#!/bin/sh -e

# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"

mkdir ThirdParty

cd Scripts

# Install vcpkg
bash SetupVCPKG.sh

# Install Eigen3
# Atm vcpkg version is used
#bash SetupEigen3.sh

# Install OpenCV
bash SetupOpenCV.sh

# Install DBoW2
bash SetupDBoW2.sh

# Install g2o
bash SetupG2O.sh

# Install OpenVSLAM
bash SetupOpenVSLAM.sh

# Install ImGui
bash SetupImGui.sh

# Setup Orbital
bash SetupOrbital.sh
