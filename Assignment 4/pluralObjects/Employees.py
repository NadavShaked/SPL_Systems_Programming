# Data Access Objects:
# All of these are meant to be singletons
class Employees:
    def __init__(self, conn):
        self._conn = conn

    def insert(self, employee):
        self._conn.execute("INSERT INTO Employees VALUES (?, ?, ?, ?)", [employee.id, employee.name, employee.salary, employee.coffee_stand])


    def print_employees(self):
        c = self._conn.cursor()
        c.execute("SELECT * FROM Employees ORDER BY id ASC")
        list = c.fetchall()

        return list

    def print_employees_report(self):
        c = self._conn.cursor()
        print("\nEmployees report")
        c.execute(
            "SELECT employee.id, activity.quantity, product.price FROM Activities activity INNER JOIN Employees as employee ON activity.activator_id = employee.id LEFT JOIN Products as product ON activity.product_id = product.id")
        employees_sales_list = c.fetchall()
        # employees_sales_list[0] - employee_id
        # employees_sales_list[1] - employee_product_quantity_sold
        # employees_sales_list[2] - employee_product_sold_price
        c.execute(
            "SELECT employee.id, employee.name, employee.salary, coffee_stand.location FROM Employees employee LEFT JOIN Coffee_stands as coffee_stand ON employee.coffee_stand = coffee_stand.id ORDER BY employee.name ASC")
        list = c.fetchall()
        # list[0] - employee_id
        # list[1] - employee_name
        # list[2] - employee_salary
        # list[3] - employee_coffee_stand_location
        for employee in list:
            income = 0
            for sale in employees_sales_list:
                if employee[0] == sale[0]:
                    income = income + float(sale[1]) * (-1) * float(sale[2])
            print(employee[1] + " " + str(employee[2]) + " " + employee[3] + " " + str(income))