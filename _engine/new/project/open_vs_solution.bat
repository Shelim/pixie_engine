@echo off
pushd %~dp0

IF NOT EXIST "./project/visual_studio_2017/$EMPTY_UNIX_NAME$.sln" (
    call "./update_vs_solution.bat"
)

call "./project/visual_studio_2017/$EMPTY_UNIX_NAME$.sln"
popd