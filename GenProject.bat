set proyecto=FractureViewer



cd build
cmake -G "Visual Studio 12" -D PROJECTNAME:STRING=%proyecto% ../
mkdir Debug
mkdir Release
copy ..\lib\_bin\ Debug
copy ..\lib\_bin\ Release
move ..\res res

cd ..
ChangeStartup.exe .\build\%proyecto%.sln %proyecto%
pause
