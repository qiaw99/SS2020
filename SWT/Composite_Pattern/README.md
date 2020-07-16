# When to use it:
- you want to represent part-whole hierachies of objects.
- you want client to be able to ignore difference between compositions of objects and individual objects.Clients will treat all objects in the composite structure uniformly.

# Dis: 
- Once tree structure is defined,comosite design makes tree overly general.
- Leaf class have to create some methods which has to empty.
