import random

maxv = 63
cols = 8
rows = 1024 // cols

for i in range(rows):
    for j in range(cols):
        print(str(random.randint(0, maxv)) + ', ', sep = '', end = '')
    print()

