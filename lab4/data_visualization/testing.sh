cd ~/Documents/School/CPE416/code/lab4/data_visualization || exit
rm OUTPUT.txt
rm writer_test.mp4
../cmake-build-debug/lab4 > OUTPUT.txt
source venv/bin/activate
python3 main.py