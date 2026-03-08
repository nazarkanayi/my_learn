

#include <iostream>
#include <vector>
#include <memory>
#include <string>  
#include <list>
#include <algorithm>
 
// Observer interface
class Observer {
public:
    virtual ~Observer() {}
    virtual void update(const std::string& message) = 0;
};  

// Subject class
class Subject {
private:
    std::list<std::weak_ptr<Observer>> observers; // Use weak_ptr to avoid circular dependency
public:
    void attach(const std::shared_ptr<Observer>& observer) {
        observers.push_back(observer);
    }   
    void detach(const std::shared_ptr<Observer>& observer) {

        observers.erase(std::remove_if(observers.begin(), observers.end(),
            [&observer](const std::weak_ptr<Observer>& o) {
                return o.lock() == observer;
            }), observers.end());
    }
    void notify(const std::string& message) {
        for (auto it = observers.begin(); it != observers.end(); ) {
            if (auto obs = it->lock()) { // Try to get a shared_ptr
                obs->update(message);
                ++it;

            } else {
                it = observers.erase(it); // Remove expired weak_ptr
            }
        }
    }
};  

// Concrete Observer
class ConcreteObserver : public Observer {
private:
    std::string name;
public:
    ConcreteObserver(const std::string& n) : name(n) {}
    void update(const std::string& message) override {  
        std::cout << "Observer " << name << " received message: " << message << std::endl;
    }
};  

int main() {
    std::shared_ptr<Subject> subject = std::make_shared<Subject>();

    std::shared_ptr<Observer> observer1 = std::make_shared<ConcreteObserver>("Observer1");
    std::shared_ptr<Observer> observer2 = std::make_shared<ConcreteObserver>("Observer2");

    subject->attach(observer1);
    subject->attach(observer2);

    subject->notify("First Notification");

    //subject->detach(observer1);


   observer2.reset(); // Simulate observer2 going out of scope
    subject->notify("Second Notification");

    return 0;
}   