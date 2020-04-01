import sys

import Repository
from printdb import printdb
from singleObjects.Activity import Activity
from singleObjects.Product import Product


def add_activity_to_Activities(filePath, repo):
    with open(filePath) as file:
        for line in file:
            line = line.strip()
            splitted_line = line.split(', ')
            product = Product(splitted_line[0], '', '', splitted_line[1])
            if int(splitted_line[1]) > 0:
                repo.products.update_product_quantity(product)
                activity = Activity(splitted_line[0], splitted_line[1], splitted_line[2], splitted_line[3])
                repo.activities.insert(activity)
            elif int(splitted_line[1]) < 0:
                product_quantity = repo.products.get_product_quantity_by_product_id(product)
                if product_quantity >= -int(splitted_line[1]):
                    repo.products.update_product_quantity(product)
                    activity = Activity(splitted_line[0], splitted_line[1], splitted_line[2], splitted_line[3])
                    repo.activities.insert(activity)


def main(args):
    inputfilename = args[1]
    repo = Repository.repo
    repo.__init__()
    add_activity_to_Activities(inputfilename, repo)
    printdb(repo)


if __name__ == '__main__':
    main(sys.argv)