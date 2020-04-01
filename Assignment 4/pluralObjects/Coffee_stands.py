# Data Access Objects:
# All of these are meant to be singletons
class Coffee_stands:
    def __init__(self, conn):
        self._conn = conn

    def insert(self, coffee_stand):
        if coffee_stand.number_of_employees == None:
            self._conn.execute("INSERT INTO Coffee_stands VALUES (?, ?)", [coffee_stand.id, coffee_stand.location])
        else:
            self._conn.execute("INSERT INTO Coffee_stands VALUES (?, ?, ?)", [coffee_stand.id, coffee_stand.location, coffee_stand.number_of_employees])


    def print_coffee_stands(self):
        c = self._conn.cursor()
        c.execute("SELECT * FROM Coffee_stands ORDER BY id ASC")
        list = c.fetchall()

        return list