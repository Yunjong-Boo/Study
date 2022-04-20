import random

data = []
a = list(range(4))

for i in range(0,3):    
    data += a
    random.shuffle(data)

print(data)
