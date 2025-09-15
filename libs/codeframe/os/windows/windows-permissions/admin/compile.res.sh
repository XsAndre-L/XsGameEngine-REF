# Copy and run in terminal to compile the admin.rc file to a admin.res file
# Available at ./libs/codeframe/platform-util/windows/admin.res

windres ./libs/codeframe/platform-util/windows/admin/admin.rc ./libs/codeframe/platform-util/windows/admin.res

# llvm-cvtres "libs\codeframe\platform-util\windows\admin.res" -out:libs\codeframe\platform-util\windows\admin.obj

# clang++ -I"$CODE_FRAME_SDK/depencencies/cpp/clang/include" "libs/codeframe/platform-util/windows/admin.cpp" "libs\codeframe\platform-util\windows\admin.obj" -L"$CODE_FRAME_SDK%/depencencies/cpp/clang/lib" -o my_admin_app.exe
