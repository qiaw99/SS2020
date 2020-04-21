import sys

ls = sys.argv[1:]
for index in range(len(ls)):
    ls[index] = int(ls[index])
print(min(ls))
