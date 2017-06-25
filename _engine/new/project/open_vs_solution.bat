@echo off
pushd %~dp0

IF NOT EXIST "./project/visual_studio_2015/game.sln" (
    call "./update_vs_solution.bat"
)

call "./project/visual_studio_2015/game.sln"
popd