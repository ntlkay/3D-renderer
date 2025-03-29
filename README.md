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

Клавиши для перемещения камеры - W, A, S, D. Для поворота камеры - Q, E