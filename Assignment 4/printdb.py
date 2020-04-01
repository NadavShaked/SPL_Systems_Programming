import sys

import Repository
from singleObjects.Activity import Activity
from singleObjects.Product import Product


def main(args):
    repo = Repository.repo
    repo.__init__()
    printdb(repo)


def printdb(repo):
    activities_list = repo.activities.print_activities()
    print("Activities")
    for activity in activities_list:
        print(activity)
    coffee_stands_list = repo.coffee_stands.print_coffee_stands()
    print("Coffee stands")
    for coffee_stand in coffee_stands_list:
        print(coffee_stand)
    employees_list = repo.employees.print_employees()
    print("Employees")
    for employee in employees_list:
        print(employee)
    products_list = repo.products.print_products()
    print("Products")
    for product in products_list:
        print(product)
    suppliers_list = repo.suppliers.print_suppliers()
    print("Suplliers")
    for supplier in suppliers_list:
        print(supplier)
    repo.employees.print_employees_report()
    repo.activities.print_activities_report()


if __name__ == '__main__':
    main(sys.argv)
