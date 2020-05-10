cookbook = {
    'sandwich': {
        'ingredients': ['ham', 'bread', 'cheese', 'tomatoes'],
        'meal': 'lunch',
        'prep_time': 10,
        },
    'cake': {
        'ingredients': ['flour', 'sugar', 'eggs'],
        'meal': 'dessert',
        'prep_time': 60,
    },
    'salad': {
        'ingredients': ['avocado', 'arugula', 'tomatoes', 'spinach'],
        'meal': 'lunch',
        'prep_time': 15,
    }
}

def add_recipe(name, ingredients, meal, prep_time):
    cookbook[name] = {
        'ingredients': ingredients,
        'meal': meal,
        'prep_time': prep_time,
    }
    print('\nRecipe added\n')

def delete_recipe(name):
    if name in cookbook:
        del cookbook[name]
    else:
        print('The recipe does not exist\n')

def print_recipe(name):
    if (name in cookbook):
        print('\nRecipe for:', name)
        print('Ingredients:', cookbook[name]['ingredients'])
        print('To be eaten for', cookbook[name]['meal'], end='.')
        print('\nTakes', cookbook[name]['prep_time'], 'minutes of cooking.')
        print('\n')
    else:
        print('The recipe does not exist\n\n')

def print_book():
    print('Cookbook: ', end = "")
    print(*cookbook, sep=', ')
    print('\n')


number = None
while number != '5':
    print('Please select an option by typing the corresponding number:')
    print('1: Add a recipe')
    print('2: Delete a recipe')
    print('3: Print a recipe')
    print('4: Print the cookbook')
    print('5: Quit')


    number = input('>>')
    if number == '1':
        name = input('Enter the recipe name:\n>>')
        ingredients = input('Enter the ingredients:\n>>')
        meal = input('Type of meal:\n>>')
        prep_time = input('Enter preparation time:\n>>')
        while not prep_time.isdigit():
            prep_time =  input('Enter preparation time:\n>>')
            if not prep_time.isdigit():
                print('Please, enter  valid time.')
        add_recipe(name, ingredients, meal, prep_time)
    elif number == '2':
        name = input('Enter the name of the recipe to delete:\n>>')
        delete_recipe(name)
    elif number == '3':
        name = input('Enter the name of the recipe to print it:\n>>')
        print_recipe(name)
    elif number == '4':
        print_book()
    elif number == '5':
        print('Cookbook closed')
        exit()
    else:
        print('\nThis option does not exist, please type the corresponding number.')
        print('To exit, enter 5\n\n')
