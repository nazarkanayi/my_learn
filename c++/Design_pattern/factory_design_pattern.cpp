
#include <iostream>
#include <memory>
#include <string>   
// Product interface
class Product {
public:
    virtual ~Product() {}
    virtual void info() const = 0;
};              

// Concrete Product A
class ConcreteProductA : public Product {
public:

    void info() const override {
        std::cout << "I am Concrete Product A" << std::endl;            

    }
};
// Concrete Product B
class ConcreteProductB : public Product {
public:
    void info() const override {
        std::cout << "I am Concrete Product B" << std::endl;
    }
};
// Factory class
class Factory {
public:
    enum ProductType { PRODUCT_A, PRODUCT_B };      

    static std::shared_ptr<Product> createProduct(ProductType type) {
        switch (type) {
            case PRODUCT_A:
                return std::make_shared<ConcreteProductA>();
            case PRODUCT_B:
                return std::make_shared<ConcreteProductB>();
            default:
                return nullptr;
        }
    }
};
int main() {
    std::shared_ptr<Product> productA = Factory::createProduct(Factory::PRODUCT_A);
    std::shared_ptr<Product> productB = Factory::createProduct(Factory::PRODUCT_B); 
    productA->info();
    productB->info();
    return 0;
}






