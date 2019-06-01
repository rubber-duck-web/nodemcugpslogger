import simplejson as json
import datetime
import os

#dir=r'.\sammel\stand'
dir=r'.\sammel\fahrt'

t_alt=0
d_alt=''
segment=[]
x={}
for dirpath, dirs, files in os.walk(dir): 
    for filename in files:
        fname = os.path.join(dirpath,filename)
        if fname.endswith('.TXT'):
            print (fname)
            infile= open (fname,'r')

            for zeile in infile:
                a=json.loads(zeile)
                
                item=a['date']+'T'+a['time']
                if d_alt=='':d_alt=a['date']
                if len(item)==19:
                    if 'lon' in a:
                        punkt=[a['lon'],a['lat']]
                    else:
                        punkt=[a['longitude'],a['latitude']]
                    if punkt != ['', '']:segment.append(punkt)
                    t_neu=datetime.datetime.strptime(item, '%Y-%m-%dT%H:%M:%S')
                    if t_alt!=0:
                        diff=t_neu-t_alt
                        if diff > datetime.timedelta(seconds=5):
                            if d_alt in x:
                                if segment!=[]:x[d_alt].append(segment)
                            else:
                                if segment!=[]:x[d_alt]=[segment]
                            d_alt=a['date']
#                            print ('bruch',len(segment))
                            segment=[]
                    t_alt=t_neu
#print ('bruch',len(segment))
if d_alt in x:
    if segment!=[]:x[d_alt].append(segment)
else:
    if segment!=[]:x[d_alt]=[segment]

for i in x:
    for j in x[i]:
        print (i,len(x[i]),len(j),int(len(j)*5/60))

with open('gps.json', 'w') as outfile:
    json.dump(x, outfile)