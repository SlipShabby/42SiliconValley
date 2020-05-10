import string

def text_analyzer(text=None):
    """This function prints the number of upper characters, lower characters,
    punctuation and spaces in a given text."""
    
    if text is None:
        text = input("What is the text to analyse?\n >>")
    if len(text) == 0:
        print("Error")
        return

    upper = len(list(filter(lambda x: x.isupper(), text)))
    lower = len(list(filter(lambda x: x.islower(), text)))
    punct = len(list(filter(lambda x: x in string.punctuation, text)))
    spaces = len(list(filter(lambda x: x.isspace(), text)))

    print('The text contains %d characters:' %len(text))
    print('- %d upper letters' %upper)
    print('- %d lower letters' %lower)
    print('- %d punctuation marks' %punct)
    print('- %d spaces' %spaces)
