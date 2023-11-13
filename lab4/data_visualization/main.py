import matplotlib.pyplot as plt
import random
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
fig = plt.figure(0)
default = 20
count = default
for l in r:
    if count==0:
        fig.clf()
        for i in range(0,int(len(l[1])/2)):
            # particle, weight
            p = l[1][i*2:i*2+2]
            x = math.cos(float(p[0]) * math.pi / 180) + random.randrange(-100,100)/5000
            y = math.sin(float(p[0]) * math.pi / 180) + random.randrange(-100,100)/5000
            plt.scatter([x],[y], s=int(100*float(p[1])), c=0)

        # Robot est and real
        x = math.cos(float(l[0][0]) * math.pi / 180)
        y = math.sin(float(l[0][0]) * math.pi / 180)
        plt.scatter([x], [y], c='#22FF22',s=16)
        x = math.cos(float(l[0][1]) * math.pi / 180)
        y = math.sin(float(l[0][1]) * math.pi / 180)
        plt.scatter([x], [y], c='#FF2222',s=16)
        plt.xlim(-2,2)
        plt.ylim(-1.5,1.5)
        plt.show()
        print(l[0])
        input("")
        count = default
    else:
        count -= 1
        continue