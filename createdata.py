import random

c = open("data.txt", "w+")
c.truncate(0)
for i in range(0,250000):
    c.write(str(random.randrange(0,500000))+"\n")
c.close()   