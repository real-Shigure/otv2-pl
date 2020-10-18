#pragma once

//Singleton pattern by Scott Meyers.
template <typename T> class Singleton {

protected:

    Singleton               () = default;
    ~Singleton              () = default;

    Singleton               (const Singleton&) = delete;
    Singleton& operator=    (const Singleton&) = delete;

    Singleton               (Singleton&&) = delete;
    Singleton& operator=    (Singleton&&) = delete;

public:

    /**
     * @return Non-dynamic allocated (static) instance.
     **/

    static T& GetInstance () {

        static T instance {};

        return instance;
    }

};