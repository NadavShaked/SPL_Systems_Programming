# Data Access Objects:
# All of these are meant to be singletons
class Products:
    def __init__(self, cursor):
        self._conn = cursor

    def insert(self, product):
        self._conn.execute("INSERT INTO Products VALUES (?, ?, ?, ?)", [product.id, product.description, product.price, product.quantity])


    def update_product_quantity(self, product):
        self._conn.execute("UPDATE Products SET quantity = quantity + " + product.quantity + " WHERE id = " + product.id)


    def get_product_quantity_by_product_id(self, product):
        c = self._conn.cursor()
        c.execute("SELECT quantity FROM Products WHERE id = " + product.id)
        ret = c.fetchone()

       # ret = self._conn.fetchone()
        return ret[0]


    def print_products(self):
        c = self._conn.cursor()
        c.execute("SELECT * FROM Products ORDER BY id ASC")
        list = c.fetchall()

        return list