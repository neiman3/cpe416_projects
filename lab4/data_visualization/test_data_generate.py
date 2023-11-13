# generate angles in C struct format
start = 87
stop = 91
num = 10

l = ["{" + str(int(x * (1 / num * (stop - start) / 360 * 50000) + start / 360 * 50000)) + ",1}" for x in range(0, num)]
print("particle test_data["+str(num)+"] = {" + ", ".join(l) + "};")