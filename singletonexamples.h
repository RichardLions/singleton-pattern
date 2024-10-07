#pragma once

#include <catch2/catch_test_macros.hpp>
#include <memory>

template<typename T>
class Singleton
{
public:
    Singleton() = default;
    virtual ~Singleton() = default;

    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& GetInstance()
    {
        REQUIRE(ms_Instance);
        return *(ms_Instance.get());
    }

    static void SetInstance(std::unique_ptr<T>&& instance)
    {
        ms_Instance = std::move(instance);
    }
private:
    static std::unique_ptr<T> ms_Instance;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::ms_Instance{};


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

class SingletonTypeB : public SingletonType
{
public:
    SingletonTypeB(const uint32_t value) : m_Value{value} {}
    uint32_t GetValue() const { return m_Value; }
private:
    const uint32_t m_Value{0};
};

TEST_CASE("Singleton - Unit Tests")
{
    SingletonType::SetInstance(std::make_unique<SingletonTypeA>());
    REQUIRE(SingletonType::GetInstance().GetValue() == 1);

    SingletonType::SetInstance(std::make_unique<SingletonTypeB>(2));
    REQUIRE(SingletonType::GetInstance().GetValue() == 2);
}
