#include <iostream>
#include <string>
#include <memory>

class Payload {
    public:
        virtual ~Payload() = default;
        virtual std::string getText() const = 0;
};

class TextPayload : public Payload {
    private:
        std::string text;
    public:

        TextPayload(std::string text)
            : text{ std::move(text) }
        {}

        std::string getText() const final {
            return text;
        }
};

class PayloadDecorator : public Payload {
    private:
        std::shared_ptr<Payload> payload;
    public:
        PayloadDecorator(const std::shared_ptr<Payload> &payload)
            : payload{ payload }
        {}
        const auto& getSource() const {
            return *payload;
        }
};

class GZipDecorator : public PayloadDecorator {
    public:
        using PayloadDecorator::PayloadDecorator;
        std::string getText() const final {
            return "gzip(" + getSource().getText() + ")";
        }
};

class OpenSSLDecorator : public PayloadDecorator {
    public:
        using PayloadDecorator::PayloadDecorator;
        std::string getText() const final {
            return "openssl(" + getSource().getText() + ")";
        }
};


int main() {

    auto text  = std::make_shared<TextPayload>("almafa");
    auto gzip  = std::make_shared<GZipDecorator>(text);
    auto crypt = std::make_shared<OpenSSLDecorator>(gzip);

    std::cout << "text:\t\t"  << text->getText()  << '\n';
    std::cout << "gzip:\t\t"  << gzip->getText()  << '\n';
    std::cout << "crypt:\t\t" << crypt->getText() << '\n';
}
