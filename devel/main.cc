
#include <iostream>
#include <memory>  // Include the memory header for shared_ptr
#include <string>
#include <vector>

class Animal {
private:
    std::string m_name;

public:
    Animal(const std::string& name) : m_name(name) {}

    const std::string& getName() const { return m_name; }

    virtual std::string speak() const {
        return "???";  // Default sound for an unknown animal
    }
};

class Dog : public Animal {
private:
    std::string m_breed;

public:
    Dog(const std::string& name, const std::string& breed)
        : Animal(name), m_breed(breed) {}

    std::string speak() const override {
        return "Woof!";  // Dogs say woof
    }

    const std::string& getBreed() const { return m_breed; }
};

class Cat : public Animal {
private:
    std::string m_color;

public:
    Cat(const std::string& name, const std::string& color)
        : Animal(name), m_color(color) {}

    std::string speak() const override {
        return "Meow!";  // Cats say meow
    }

    const std::string& getColor() const { return m_color; }
};

int main() {
    // Create shared pointers for Dog and Cat
    std::shared_ptr<Animal> myDog = std::make_shared<Dog>("Rover", "Golden Retriever");
    std::shared_ptr<Animal> myCat = std::make_shared<Cat>("Whiskers", "Gray");

    // Create a vector of shared pointers to Animal
    std::vector<std::shared_ptr<Animal>> animals;
    animals.push_back(myDog);
    animals.push_back(myCat);

    // Iterate over the vector and make each animal speak
    for (const auto& animal : animals) {
        std::cout << "My " << animal->getName() << " says: " << animal->speak()
                  << std::endl;
    }

    // Clean up (not necessary in this simple example)

    return 0;
}
