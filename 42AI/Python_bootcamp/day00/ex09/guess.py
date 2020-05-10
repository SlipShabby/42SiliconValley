import random

print('This is an interactive guessing game!')
print('You have to enter a number between 1 and 99 to find out the' +
      'secret number.')
print('Type \'exit\' to end the game.')
print('Good luck!')

number = random.randint(1,99)
attempts = 0

while (1):
    print('What\`s your guess between 1 and 99?')
    n = input()
    if n == 'exit':
        print('Goodbye!')
        exit()
    attempts += 1
    if not n.isdigit():
        print('That\`s not a number.')
    elif int(n) == 42:
        print('The answer to the ultimate question of life, '
				+ 'the universe and everything is 42.')
        print('Congratulations\! You got it on your first try\!')
        exit()
    elif int(n) == number:
        print('Congratulations, you\'ve got it!')
        print('You won in', attempts, 'attempts!')
        exit()
    elif int(n) != number:
        print('Too high!' if int(n) > number else 'Too low!')
