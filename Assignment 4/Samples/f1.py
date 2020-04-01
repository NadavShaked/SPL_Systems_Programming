# file f1.py

print("f1.py - level 0")

def main():
    print("Executing main() of f1.py")

if __name__ == "__main__":
    print("f1.py is being run directly")
    main()
else:
    print("f1.py is being imported into another module")
