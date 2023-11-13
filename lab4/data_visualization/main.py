import matplotlib.pyplot as plt
from matplotlib.animation import FFMpegWriter
import math
r = []
with open("OUTPUT.txt", 'r') as f:
    for line in f:
        t = line.split("\t")
        if t[-1:] == ['\n']:
            t = t[:-1]
        r.append([t[:4], t[4:]])
r.pop(0)

metadata = dict(title='Movie Test', artist='Matplotlib',
                comment='Movie support!')
writer = FFMpegWriter(fps=15, metadata=metadata)
for l in r:
    plt.close(1)
    fig = plt.figure(1)
    for i in range(0,int(len(l[1])/2)):
        # particle, weight
        p = l[1][i*2:i*2+2]
        x = math.cos(float(p[0]) * math.pi / 180)
        y = math.sin(float(p[0]) * math.pi / 180)
        plt.scatter([x],[y], s=int(100*float(p[1])), c=0)

    # Robot est and real
    x = math.cos(float(l[0]) * math.pi / 180)
    y = math.sin(float(l[0]) * math.pi / 180)
    plt.scatter([x], [y], s=int(100 * float(p[1])), c='')
    x = math.cos(float(l[1]) * math.pi / 180)
    y = math.sin(float(l[1]) * math.pi / 180)
    plt.xlim(-1.5,1.5)
    plt.ylim(-1.5,1.5)
    plt.show()
    print(l[0])
    input("")