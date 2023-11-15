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
writer = FFMpegWriter(fps=24, metadata=metadata)
fig = plt.figure()
skip_frames = 0

# towers = [15, 135, 180]
towers = [15]

iter = []
stdev = []
err = []
iter_ct = 0
count = skip_frames
writer = FFMpegWriter(fps=8, metadata=dict(title='Movie Test', artist='Matplotlib',comment='Movie support!'))
with writer.saving(fig, "writer_test.mp4", 100):
    for l in r:
        iter_ct += 1
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

            # towers
            for t in towers:
                x = math.cos(t * math.pi / 180) * 1.2
                y = math.sin(t * math.pi / 180) * 1.2
                plt.scatter([x], [y], c='#000000', s=30)

            plt.text(-1.75, 1.25, ("σ: {:0.3f}; err: {:3.3f}º".format(float(l[0][2]), min(abs(float(l[0][0])-float(l[0][1])), abs(360-(float(l[0][0])-float(l[0][1])))))), fontdict=None)
            # plt.legend(["Particles", "Actual position", "Position estimate", "Tower"])
            # iter.append(iter_ct)
            # stdev.append(l[0][2])
            # err.append(min(abs(float(l[0][0])-float(l[0][1])), abs(360-(float(l[0][0])-float(l[0][1])))))
            # plt.plot(iter, stdev)
            # plt.plot(iter, err)
            print(l[0])
            writer.grab_frame()
            count = skip_frames
        else:
            count -= 1
            continue
