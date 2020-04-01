# The Repository
import atexit
import os
import _sqlite3

from pluralObjects.Activities import Activities
from pluralObjects.Coffee_stands import Coffee_stands
from pluralObjects.Employees import Employees
from pluralObjects.Products import Products
from pluralObjects.Suppliers import Suppliers


class Repository:
    def __init__(self):
        self._conn = _sqlite3.connect('moncafe.db')
        self.employees = Employees(self._conn)
        self.coffee_stands = Coffee_stands(self._conn)
        self.products = Products(self._conn)
        self.suppliers = Suppliers(self._conn)
        self.activities = Activities(self._conn)

    def _close(self):
        self._conn.commit()
        self._conn.close()

    def create_tables(self):
        self._conn.executescript("""
        CREATE TABLE Suppliers(
                id INTEGER PRIMARY KEY ,
                name TEXT NOT NULL ,
                contact_information TEXT);
        CREATE TABLE Products(
                id INTEGER PRIMARY KEY ,
                description TEXT NOT NULL ,
                price REAL NOT NULL,
                quantity INTEGER NOT NULL );        
        CREATE TABLE Activities(
                product_id INTEGER ,
                quantity INTEGER NOT NULL ,
                activator_id INTEGER ,
                date DATE NOT NULL ,
                FOREIGN KEY(product_id) REFERENCES Products(id));
        CREATE TABLE Coffee_stands(
                id INTEGER PRIMARY KEY ,
                location TEXT NOT NULL ,
                number_of_employees INTEGER );
        CREATE TABLE Employees(id INTEGER PRIMARY KEY ,
                name TEXT NOT NULL ,
                salary REAL NOT NULL ,
                coffee_stand INTEGER ,
                FOREIGN KEY(coffee_stand) REFERENCES Coffee_stands(id));
    """)


# the repository singleton
repo = Repository()
atexit.register(repo._close)