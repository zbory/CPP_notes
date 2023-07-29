
class Singleton {

    private:

        Singleton() {}

        Singleton(const Singleton&) = delete; 
        Singleton(Singleton&&) = delete;
        Singleton operator=(const Singleton&) = delete;
        Singleton operator=(Singleton&&) = delete;

    public:

        Singleton& instance() {
            static Singleton s;
            return s;
        }

        //

};