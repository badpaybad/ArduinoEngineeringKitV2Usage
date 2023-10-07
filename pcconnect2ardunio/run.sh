cd build
cmake ..
cmake --build .
echo "Start program"
./pcconnect2ardunio

# stty -f "/dev/ttyACM0"