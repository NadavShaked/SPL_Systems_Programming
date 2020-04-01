# In order to run the code from a:
# 1. Python's console insert the following "import <your-python-file>"
# 2. Unix/Windows(cmd) terminal type in "python <your-python-file>"

import sqlite3    #importing the sqlite3 library will result in a sqlite3 variable
import os         #Just for os.path.isfile(...
import atexit     #To define "at exit" behavior (Like finally in Java)

# Check if DB already exists BEFORE calling the connect, since connect will create it
DBExist = os.path.isfile('cow.db')
# Will create a connection to 'cow.db' file. If the file does not exist, it will create it
dbcon = sqlite3.connect('cow.db')
cursor = dbcon.cursor()

#Define a function to be called when the interpreter terminates
def close_db():
    dbcon.commit()
    dbcon.close()
    os.remove('cow.db')   # !!!! Just for debug do not forget to remove it !!!!

#register close_db to be called when the interpreter terminates
atexit.register(close_db)

def create_tables():
        # DBExist and cursor were set preior to calling this function
        if not DBExist: # First time creating the database tables.
            cursor.execute(""" CREATE TABLE category_tab(category_id  INT PRIMARY KEY,
                                                         name         TEXT NOT NULL,
                                                         description TEXT )
                           """)
            cursor.execute(""" CREATE TABLE region_tab( region_id    INT PRIMARY KEY,
                                                        name         TEXT,
                                                        temp         INTEGER,
                                                        description  TEXT )
                           """)
            cursor.execute(""" CREATE TABLE cow_tab ( cow_id          INT PRIMARY KEY,
                                                      name            TEXT,
                                                      sex             TEXT NOT NULL,
                                                      DOB             DATE,
                                                      region_id       INT NOT NULL,
                                                      category_id     INT,

                                                      FOREIGN KEY(region_id)   REFERENCES region_tab(region_id),
                                                      FOREIGN KEY(category_id) REFERENCES category_tab(category_id) )
                           """)


#Insert a record into cow_tab table
def insert_cow(id, name, sex, dob, regId, catId ):
    cursor.execute("INSERT INTO cow_tab VALUES (?, ?, ?, ?, ?, ?)", (id, name, sex, dob, regId, catId))


#Insert a record into region_tab table
def insert_region(regId, name, temp, desc):
    cursor.execute("INSERT INTO region_tab VALUES (?, ?, ?, ?)", [regId, name, temp, desc])

#Insert a record into category_tab table
def insert_category(catId, name, desc):
    cursor.execute("INSERT INTO category_tab(category_id, name, description) VALUES (?, ?, ?)", [catId, name, desc])


def insert_data():
    insert_region(10, "Arava", 35, "South of Israel, 175Km length")
    insert_region(20, "Negev", 27, "Best place ever")
    insert_region(30, "Sfela", 26, "Attitude: 120-460m")
    insert_region(40, "Sharon",25, "Part of the coastal plain of Israel")
    insert_region(50, "Galil", 24, "North west")
    insert_region(60, "Golan", 28, "North east")

    insert_category(600, 'Reg', 'Just a Cow')
    insert_category(601, 'Red', 'Holy Cow')
    insert_category(602, 'Wild', 'Wild, Happy & Free')

    insert_cow(1000, "Dina", "F", 300303, 40, 601)
    insert_cow(2000, "Rina", "F", 120815, 20, 602)
    insert_cow(3000, "Gila", "F", 230115, 50, 600)
    insert_cow(4000, "Bob",  "M", 200214, 50, 600)

# Print a table as a list
def print_table_as_a_list(table):
    cursor.execute('SELECT * FROM ' + table);
    list = cursor.fetchall()
    print("\nAll " + table +  " as list:\n"
          "-------------------------")
    print(list)

# Print a table as a table
def print_table_as_a_table(table):
    cursor.execute('SELECT * FROM ' + table);
    list = cursor.fetchall()
    print("\nAll {} as a table:\n"
          "-------------------------".format(table))
    i=0
    for item in list:
        i=i+1
        print("item{}: {}".format(i, str(item)))

# Print the name of a cow
def print_cow_name(id):
    print("\nCow's name\n"
          "------------------------")
    cursor.execute(
        "SELECT name FROM cow_tab WHERE cow_id=({})".format(id))
    cow = cursor.fetchone()
    print("Cow {}: {}".format(id, str(cow[0])))

# Return a string with all the information of the requested region
def getRegInfo(regId):
    cursor.execute("SELECT * FROM region_tab WHERE region_id=({})".format(regId))
    reg = cursor.fetchone()
    return "Region={}({}, Temp={})".format(reg[1], reg[3], reg[2])

# Return a string with all the information of the requested category
def getCatInfo(catId):
    cursor.execute("SELECT * FROM category_tab WHERE category_id=({})".format(catId))
    cat = cursor.fetchone()
    return "Category={}({})".format(cat[1], cat[2])

def formatDate(date):
    return "%02d/%02d/%02d" % (date/10000, (date-date/1000*1000)/100, (date-date/100*100))


def all_about(cowName):
    print("\nAll about {}{} :\n------------------------".format(cowName, "(s)"))
    cursor.execute("SELECT * FROM cow_tab WHERE name = '{}' ".format(cowName))

    list = cursor.fetchall()
    for cow in list:
        regInfo = getRegInfo(cow[4])
        catInfo = getCatInfo(cow[5])
        cow_id=cow[0]
        sex=cow[2]
        DOB=formatDate(cow[3])
 #       print("id={}, sex={}, DOB={},{}, {}".format(cow_id, sex, DOB, regInfo, catInfo))
        print("Cow {}: id={}, sex={}, DOB={}, {}, {}".format(cowName, cow_id, sex, DOB, regInfo, catInfo))


# Join example
# The function will produce a report by joining all three tables

def selectRegJoin():
    return """
        SELECT cow.name, cow.DOB, reg.name, cat.name, cat.description FROM cow_tab cow
        JOIN region_tab reg ON cow.region_id = reg.region_id
        JOIN category_tab cat ON cat.category_id = cow.category_id
        ORDER BY cow.name
        """

def selectLeftJoin():
    return """
        SELECT cow.name, cow.DOB, reg.name, cat.name, cat.description FROM cow_tab cow
        JOIN region_tab reg ON cow.region_id = reg.region_id
        LEFT JOIN category_tab cat ON cat.category_id = cow.category_id
        ORDER BY cow.name
        """

def cow_report(left):
    if left:
        print("\nCow report: Left Join")
    else:
        print("\nCow report: Join")

    print("%-7s %-9s %-7s %-4s" % ("Name", "DOB", "Region", "Category"))
    print("---------------------------------------")

    if left:
       cursor.execute(selectLeftJoin())
    else:
       cursor.execute(selectRegJoin())

    list = cursor.fetchall()
    for item in list:
        DOB=formatDate(item[1])
        print("%-7s %-9s %-7s %-4s(%-s)" % (item[0], DOB, item[2], item[3], item[4]))



def main():
    if not DBExist:
        create_tables()
        insert_data()

    print_table_as_a_list("category_tab")
    print_table_as_a_table("cow_tab")
    print_cow_name(4000)
    if not DBExist:
        insert_cow(5000, "Rina", "F", 111111, 40, 601 )
    all_about("Rina")
    cow_report(0)


    #Join vs Left Join
    insert_cow(9991, "Ziva1",  "F", 101010, 50, "NULL")
    insert_cow(9992, "Ziva2",  "F", 111111, 50, "NULL")
    insert_cow(9993, "Ziva3",  "F", 121212, 50, "NULL")

    cow_report(0)
    cow_report(1)

# Call main() from the command line that runs the rest of the code
# Be aware that main() function is NOT required and that the entire
# main body could be writen "flat" at first level.
# You may also use the if __name__....
main()


