#!/bin/bash
choice () {
    local choice=$1
    if [[ ${opts[choice]} ]] # toggle
    then
        opts[choice]=
    else
        opts[choice]=+
    fi
}

kernName=`uname -s`
case "${kernName}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=macOS;;
    CYGWIN*)    machine=Win;;
    MINGW*)     machine=Win;;
    *)          echo "Unsupported Host System"&&exit 1;;
esac

# if [ "${machine}" == "macOS" ]; then
#     platforms=( "${machine}" "iOS" "Android")
#     s=({1..3})
# else
#     platforms=( "${machine}" "Android")
#     s=({1..2})
# fi

# while :
# do
#     clear
#     options=()
#     for x in "${s[@]}"; do
#         m+=("${platforms[$x]} ${opts[$x]}")
#     done
#     options+=("Done")
#     select opt in "${options[@]}"
#     do

# done

if [ "${machine}" == "macOS" ]; then
    mkdir build-macOS
    mkdir -p products/macOS
    pushd build-macOS
    cmake -GXcode ..
    cmake --build . --config Release
    cp Release/Unity_ZBar.dylib ../products/macOS
    popd
    rm -rf build-macOS

    mkdir build-iOS
    mkdir -p products/iOS
    pushd build-iOS 
    cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=../platforms/iOS/ios.toolchain.cmake -DPLATFORM=OS -DARCHS="armv7 armv7s arm64 x86_64 armv7k arm64_32" -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=$1
    # open *.xcodeproj
    # read -p "iOS requires signing. Please set valid development team. Press [Enter] to continue".
    mkdir Debug-iphoneos
    mkdir Release-iphoneos
    cmake --build . --config Release
    cp Release-iphoneos/Unity_ZBar.dylib ../products/iOS
    popd
    rm -rf build-iOS
elif [ "${machine}" == "Linux" ]; then
    mkdir build-linux
    mkdir -p products/linux
    pushd build-linux
    cmake -GNinja ..
    cmake --build . --config Release
    cp Unity_ZBar.so ../products/linux
    popd
    rm -rf build-linux
else
    mkdir build-win
    mkdir -p products/Windows
    pushd build-win
    cmake -A x64 ..
    cmake --build . --config Release
    cp Release/Unity_ZBar.dll ../products/Windows
    popd
    rm -rf build-win
fi

cp ./libzbar/android.sh .
patch ./android.sh < android.patch
source android.sh
rm android.sh