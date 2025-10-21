#include <iostream>
#include <memory>
#include <vector>

class Child; // Forward declaration

class Parent {
public:
    std::string name;
    std::vector<std::shared_ptr<Child>> children; // Parent owns children

    Parent(const std::string& n) : name(n) {
        std::cout << "Parent " << name << " created." << std::endl;
    }

    ~Parent() {
        std::cout << "Parent " << name << " destroyed." << std::endl;
    }
};

class Child {
public:
    std::string name;
    // Using weak_ptr to avoid circular dependency with Parent
    std::weak_ptr<Parent> parent; 

    Child(const std::string& n) : name(n) {
        std::cout << "Child " << name << " created." << std::endl;
    }

    ~Child() {
        std::cout << "Child " << name << " destroyed." << std::endl;
    }

    void printParentName() {
        // Attempt to get a shared_ptr from the weak_ptr
        if (auto lockedParent = parent.lock()) { 
            std::cout << "Child " << name << "'s parent is " << lockedParent->name << "." << std::endl;
        } else {
            std::cout << "Child " << name << "'s parent no longer exists." << std::endl;
        }
    }
};

int main() {
    std::shared_ptr<Parent> myParent = std::make_shared<Parent>("Alice");
    
    { // Introduce a scope to control child lifetimes
        std::shared_ptr<Child> child1 = std::make_shared<Child>("Bob");
        std::shared_ptr<Child> child2 = std::make_shared<Child>("Charlie");

        // Parent owns children
        myParent->children.push_back(child1);
        myParent->children.push_back(child2);

        // Children have a weak reference to their parent
        child1->parent = myParent;
        child2->parent = myParent;

        child1->printParentName();
        child2->printParentName();
    } // child1 and child2 go out of scope here, their shared_ptr counts become 0, and they are destroyed.

    // Now, try to access parent from a child (which is now destroyed)
    // If child1 was still in scope and we tried to access parent after parent was destroyed,
    // the lock() would return a null shared_ptr.
    // In this example, child1 and child2 are already out of scope and destroyed.

    myParent.reset(); // Parent is explicitly destroyed here
    return 0;
}