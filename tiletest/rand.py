import random

totes = int(input("Enter size: "))
m = int(input("Enter number of tiles: "))

print("const unsigned short test_Map[" + str(totes) + "] = {")
print("    ", end='')

t = 0
for i in range(totes - 1):
    if t > 8:
        t = 0
        print()
        print("    ", end='')
    print('0x' + format(random.randrange(0, m), '04x') + ', ', end='')
    t = t + 1

print('0x' + format(random.randrange(0, m), '04x'))
print("};")


