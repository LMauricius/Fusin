mkdir build
cd ./build

echo "--- Building Fusin Debug ---"
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

echo "--- Building Fusin Release ---"
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

echo "Done!"