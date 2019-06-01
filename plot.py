import simplejson as json

print ('Tracks')
with open(r'gps.json', 'r') as infile:
    data=infile.read()
print ('  json')
tracks=json.loads(data)


print ('Plot')
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

plt.figure(figsize=(10,10), dpi=800)
plt.grid(True)
plt.axis('equal')
#plt.axis([5, 16, 47, 56])


print ('   Track(s)')
for i in tracks:
    for j in tracks[i]:
        lat=[]
        lon=[]
        
        # für jedes Segment die Koordinaten in das richtige Format bringen
        for punkt in j:
            lon.append(float(punkt[0]))
            lat.append(float(punkt[1]))
        plt.plot(lon, lat, 'b,')
#        plt.plot(lon, lat, 'b' ,lw = 0.2)

print ('   save')
plt.savefig('karte.png')