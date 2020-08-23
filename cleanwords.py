words = []
new_list = []

with open("words.txt", 'r') as f:
    words = f.read().split()

for word in words:
    if word.isalpha():
        new_list.append(word.lower())

with open('words.txt', 'w') as f:
    for word in new_list:
        f.write("%s\n" % word)