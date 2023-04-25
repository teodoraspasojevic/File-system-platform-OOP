# File-system-platform-oop
Implementation of a file system platform, using object oriented programming and a tree as a data structure.

**Implementation**

Firstly, we created an abstract class SystemElement, from which we derived the Folder and File classes. The abstract class was used to create a TreeNode class that contains pointers which can refer to both Folder and File objects. We created two different classes, Folder and File, to allow us to add additional functionalities to the classes.

Next, we implemented the TreeNode class, which contains pointers to the SystemElement object it represents, as well as pointers to its son, brother, and father TreeNodes. We use this class when creating the Tree class.

We then implemented the Tree class, which has a pointer to the root TreeNode. This class is used to store all of the SystemElements. We implemented all of the necessary methods for searching through the Tree.

The FileSystem class was then created, which has a pointer to the Tree object.

Finally, we implemented the Command class, which contains all of the commands that can be used on the Tree object.

One specific algorithm implemented in this code is the algorithm that converts a tree with m sons into a binary tree.
