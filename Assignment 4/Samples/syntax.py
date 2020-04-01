import sys

# ----------------------------------------------
# STRINGS
# ----------------------------------------------
helloStr = 'hello.'
hello2Str = "hello."
longStr = """I'm soooooooooooooooooooo
             long that I need 2 rows
          """
print(longStr)

str = "AbCdE"
s = str[1]                  # b (starts at 0)
l = len(str)                # 5
s = str + 'fGhIj'           # AbCdEfGhIj
s = s.upper()               # ABCDEFGHIJ
s = s.lower()               # abcdefghij
s = s.replace('ef', '...'); # abcd...ghij
s = s.split('.')            # (a list):['abcd', '', '', 'ghij']
s = "  1 2 3 4\n".strip()   # "1 2 3 4" (Removes all the white chars including \n, \t, \r
                            #  from the begining and end of the string
s = "SPL %d is the greatest course ever. %s will teach it next year again!" % (201, 'Mary')




# ----------------------------------------------
#  If statement
# ----------------------------------------------
x=2
y=0
if x >= 0.5 and y >= 0.5:
  print ('All above half')
elif x >= 0.5 or y >= 0.5:
  print ('One of them is above half.')
else:
  print ('None of them is above half.')




# ----------------------------------------------
# Python Lists - each entry has its own type
# ----------------------------------------------
cars = ['Ford', 'Honda', 551]
s = cars[0]   # Ford
s = cars[1]   # Honda
l = cars[2]   # 551
l = len(cars) # 3

cars.append("bob")   # adds "Bob" to the end of the list.
cars.insert(3, 42)   # inserts the element at the given index
cars.insert(3, 9)    # inserts the element at the given index
cars.insert(3, "A")  # inserts the element at the given index
cars.remove("Honda") # remove the first instance elem
#cars.sort()         # sorts the list in place (does not return it), doesn't work on mixed-type list
s=cars.pop(2)        # removes and returns the element at the given index




# ----------------------------------------------
# For, in, while, if in
# ----------------------------------------------
for car in cars:
  print(car)

i = 0
while i < len(cars):
  print(cars[i])
  i += 1

if 'Ford' in cars:
  print('Yay!')

# slicing
# s[x:y] is the elements of s starting at index x until index y EXCLUDING element at index y.
str = "0123456789"
s = str[1:4]   # "123"
s = str[8:]    # "789"
s = str[8:100] # "789"
s = str[:]     # "0123456789"
s = str[-2]    # "8"




# ----------------------------------------------
# standard input - sys module's stdin in
# ----------------------------------------------
while True:
    print("Input: ")
    inputline = sys.stdin.readline() # read a line from the user
    inputline = inputline.strip('\n') # strip the '\n' from the line of the user
    if inputline == 'exit':
        break
    else:
        outline = inputline + ".." + inputline[-2:]
        print("Got echo: %s" % (outline))

# ----------------------------------------------
# input file - sys module's
# ----------------------------------------------
with open("syntax.py") as inputfile:    #Print this file to the screen
    for line in inputfile:
        print(line, end="")      #end="" so it won’t add \n to the print because line already has it




# ----------------------------------------------
# Dictionary
# ----------------------------------------------
myDict = {'Name': 'Yardena', 'Age': 20, 'Year': 'First'}
print("myDict['Name']: ", myDict['Name'])        # myDict['Name']:  Yardena
print("myDict['Age']: ", myDict['Age'])          # myDict['Age']:  20

myDict['Age'] = 25                       # Update existing entry
myDict['School'] = "BGU Univ"            # Add new entry
#print("myDict['abc']: ", myDict['abc'])   # KeyError: 'abc'

del myDict['Name'] # remove entry with key 'Name'
myDict.clear()     # remove all entries in dict
del myDict         # delete entire dictionary

myDict2 = {'K': '1', 'K': 2}             # No duplicate key is allowed
print("myDict2['K']: ", myDict2['K'])    # myDict['K']:  2


# ---------------------------------------------
#  Tuples - Like a list but immutable
# ---------------------------------------------
tup1 = ('physics', 'chemistry', 1997, 2000);
tup2 = (1, 2, 3, 4, 5 )
tup3 = ()                        # The empty tuple
tup4 = (50,)                     # Tuple containing a single value must have a comma
print(tup1[0])                   # physics
print(tup2[1:5])                 # [2, 3, 4, 5]
del tup4                         # Remove the tuple


len(tup1)         #Gives the total length of the tuple.
#max(tup1)         #Returns the item with the max value (not supported with mix types) .
#min(tup1)         #Returns the item with the min value. (not supported with mix types) .
tuple([1, "one"]) #Converts a list into a tuple

