Как собрать и запустить проект:

```
git clone git@github.com:ntlkay/3D-renderer.git
cd 3D-renderer
git checkout dev
mkdir build; cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./3D-renderer
```

Клавиши для перемещения камеры - W/S, A/D, LShift/Backspace. Для поворота камеры вокруг вертикальной оси - Q/E; вокруг горизонтальной - стрелками вверх/вниз на клавиатуре