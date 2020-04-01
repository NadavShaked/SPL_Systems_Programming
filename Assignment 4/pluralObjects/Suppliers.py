# Data Access Objects:
# All of these are meant to be singletons
class Suppliers:
    def __init__(self, conn):
        self._conn = conn

    def insert(self, supplier):
        if supplier.contact_information == None:
            self._conn.execute("INSERT INTO Suppliers VALUES (?, ?)", [supplier.id, supplier.name])
        else:
            self._conn.execute("INSERT INTO Suppliers VALUES (?, ?, ?)", [supplier.id, supplier.name, supplier.contact_information])


    def print_suppliers(self):
        c = self._conn.cursor()
        c.execute("SELECT * FROM Suppliers ORDER BY id ASC")
        list = c.fetchall()

        return list