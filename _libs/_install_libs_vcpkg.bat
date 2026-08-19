@set BIN_CACHE=--no-binarycaching

@rem --clean-after-build

vcpkg install ixwebsocket:x64-mingw-dynamic       --host-triplet=x64-mingw-dynamic     --clean-buildtrees-after-build --clean-packages-after-build    %BIN_CACHE%
vcpkg install ixwebsocket:x64-mingw-static        --host-triplet=x64-mingw-dynamic     --clean-buildtrees-after-build --clean-packages-after-build    %BIN_CACHE%

vcpkg install ixwebsocket:x64-windows             --host-triplet=x64-windows           --clean-buildtrees-after-build --clean-packages-after-build    %BIN_CACHE%
vcpkg install ixwebsocket:x64-windows-static      --host-triplet=x64-windows           --clean-buildtrees-after-build --clean-packages-after-build    %BIN_CACHE%
vcpkg install ixwebsocket:x64-windows-static-md   --host-triplet=x64-windows           --clean-buildtrees-after-build --clean-packages-after-build    %BIN_CACHE%

