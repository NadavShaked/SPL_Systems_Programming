# Data Access Objects:
# All of these are meant to be singletons
class Activities:
    def __init__(self, conn):
        self._conn = conn

    def insert(self, activity):
        if activity.activator_id == None:
            self._conn.execute("INSERT INTO Activities VALUES (?, ?, ?)", [activity.product_id, activity.quantity, activity.date])
        else:
            self._conn.execute("INSERT INTO Activities VALUES (?, ?, ?, ?)", [activity.product_id, activity.quantity, activity.activator_id, activity.date])

    def print_activities(self):
        c = self._conn.cursor()
        c.execute("SELECT * FROM Activities ORDER BY date ASC")
        list = c.fetchall()

        return list

    def print_activities_report(self):
        c = self._conn.cursor()
        c.execute(
            "SELECT activity.date, product.description, activity.quantity, employee.name, supplier.name FROM Activities activity LEFT JOIN Products as product ON activity.product_id = product.id LEFT JOIN Employees as employee ON activity.activator_id = employee.id LEFT JOIN Suppliers as supplier ON activity.activator_id = supplier.id ORDER BY activity.date ASC")
        activities_info_list = c.fetchall()
        if activities_info_list.__len__() > 0:
            print("\nActivities")
            # activities_info_list[0] - activity_date
            # activities_info_list[1] - activity_product_name
            # activities_info_list[1] - activity_product_quantity_sold
            # activities_info_list[3] - activity_employee_name
            # activities_info_list[4] - activity_supplier_name
            for activity_info in activities_info_list:
                print(activity_info)