import numpy as np
import matplotlib.pyplot as plt


f, (ax1) = plt.subplots(1,1)
hoek = []
waarde = []
with open('MUSIC_spectrum.txt') as myfile:
    for line in myfile:
        rij = line.split()
        hoek.append(float(rij[0]))
        waarde.append(float(rij[1]))
ax1.semilogy(hoek,waarde)

ax1.semilogy([-18,-18],[0.01,1e5], linestyle='--', color='grey')
ax1.semilogy([25,25],[0.01,1e5], linestyle='--', color='grey')
plt.show()
