# When to use it:
- you want to represent part-whole hierachies of objects.
- you want client to be able to ignore difference between compositions of objects and individual objects.Clients will treat all objects in the composite structure uniformly.

# Disadvantages: 
- Once tree structure is defined,comosite design makes tree overly general.
- Leaf class have to create some methods which has to empty.

![](http://2.bp.blogspot.com/-t88k0zLYMwA/UGMuUovjwzI/AAAAAAAAAaU/OA8Qoov2hZA/s1600/CompositeDesignPatternGeneric.gif)

![](http://4.bp.blogspot.com/-hxzn_ztVv5g/UGNJlLm1HPI/AAAAAAAAAbI/gkr2g8P-oP0/s1600/CompositeDesignPatternExample.gif)
# Elements:
## Component
- declares interface for objects in composition.
- implements deafault behaviour for the interface common to all classes as appropriate.
- declares an interface for accessing and managing its child components.

## Leaf
- represents leaf objects in the composition.A leaf has no children.
- defines behaviour for primitive objects in the composition.

## Composite
- defines behaviour for components having children.
- stores child components.
- implements child related operations in the component interface.

## Client
- manipulates objects in the composition through the component interface.
