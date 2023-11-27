/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake --build /Users/neiman/Documents/School/CPE416/code/lab4/cmake-build-debug --target lab4 -j 6
cd ~/Documents/School/CPE416/code/lab4/data_visualization || exit
/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake --build /Users/neiman/Documents/School/CPE416/code/lab4/cmake-build-debug --target lab4 -j 6
rm OUTPUT.txt
rm writer_test.mp4
../cmake-build-debug/lab4 > OUTPUT.txt
source venv/bin/activate
python3 main.py