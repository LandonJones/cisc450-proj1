with open('test2.txt') as f: 
    text = f.readlines() 
bad = [] 
for c in text: 
    if (len(c) == 81): 
        bad.append(c)
print(bad) 
