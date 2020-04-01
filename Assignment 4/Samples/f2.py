# file f2.py

print("f2.py - level 0(1)")

import f1

print("f2.py - level 0(2)")

if __name__ == "__main__":
    print("f2.py is being run directly")
else:
    print("f2.py is being imported into another module")


