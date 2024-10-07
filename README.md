# Singleton Pattern

This pattern was inspired by [C++ Software Design - Guideline 38](https://www.oreilly.com/library/view/c-software-design/9781098113155/).

## When To Use

This pattern is used to ensure only one instance of an object exists and provides global access to the instance. Ensuring only one instance of an object exists can be a limitation. To be able to control initialisation, I have relaxed this requirement in the example implementation. 

Singletons have a bad reputation, and for good reason. Storing globally accessible state can create all sorts of hard to fix and hard to reproduce bugs. However because the pattern is very convenient, it is overused, which reinforces it's bad reputation.

Some examples when a Singleton could be used are memory allocators, logging, file system access and database access.

### Implementation Considerations

Lifetime management can be tricky for Singletons. Because they are globally accessible, they technically need to be always available. Making them static solves this problem. However the initialisation order for static variables is unpredictable. Also Singletons may rely on each other for initialisation. The examples in this repository use smart pointers to solve this problem.

It is common to access a Singleton using a concrete type. However this reduces testability and removes the opportunity to create different implementations. Using abstraction removes this problem by calling code only depending on the abstraction allowing it to be mocked or have different platform implementations.

## Example

Initialising and calling a Singleton:

```cpp
template<typename T>
class Singleton
{
public:
    ...
    static T& GetInstance();
    static void SetInstance(std::unique_ptr<T>&& instance);
private:
    static std::unique_ptr<T> ms_Instance;
};

class SingletonType : public Singleton<SingletonType>
{
public:
    virtual uint32_t GetValue() const = 0;
};

class SingletonTypeA final : public SingletonType
{
public:
    [[nodiscard]] uint32_t GetValue() const override { return 1; }
};

SingletonType::SetInstance(std::make_unique<SingletonTypeA>());
REQUIRE(SingletonType::GetInstance().GetValue() == 1);
```

## Setup

This repository uses the .sln/.proj files created by Visual Studio 2022 Community Edition.
Using MSVC compiler, Preview version(C++23 Preview). 

### Catch2
The examples for how to use the pattern are written as Unit Tests.

Launching the program in Debug or Release will run the Unit Tests.

Alternative:
Installing the Test Adapter for Catch2 Visual Studio extension enables running the Unit Tests via the Test Explorer Window. Setup the Test Explorer to use the project's .runsettings file.

### vcpkg
This repository uses vcpkg in manifest mode for it's dependencies. To interact with vcpkg, open a Developer PowerShell (View -> Terminal).

To setup vcpkg, install it via the Visual Studio installer. To enable/disable it run these commands from the Developer PowerShell:
```
vcpkg integrate install
vcpkg integrate remove
```

To add additional dependencies run:
```
vcpkg add port [dependency name]
```

To update the version of a dependency modify the overrides section of vcpkg.json. 

To create a clean vcpkg.json and vcpkg-configuration.json file run:
```
vcpkg new --application
```

### TODO
- [x] Implementation
- [x] Unit Tests
