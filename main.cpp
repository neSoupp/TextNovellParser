#include <iostream>
#include <fstream>
#include <string>


class Parser {
public:
    void parseFile(const std::string& filename);
};

void Parser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл: " << filename << std::endl;
        return;
    }
    
    std::string name;
    std::string text;
    std::string line;

    while (std::getline(file, line)) {
        if (line == "name") {
            if (std::getline(file, line)) {
                name = line;
            } else {
                std::cout << "Ошибка: Отсутствует имя после ключевого слова 'name'." << std::endl;
                return;
            }
        } else if (line == "txt") {
            if (std::getline(file, line)) {
                text = line;
            } else {
                std::cout << "Ошибка: Отсутствует текст после ключевого слова 'txt'." << std::endl;
                return;
            }

            std::cout << "\033[93m" << name << "\033[0m" << ":" << "\033[92m" << text << "\033[0m" << std::endl;

            std::string input;
            std::cout << "\033[1m" << "next " << "\033[0m" << "для продолжения, " << "\033[1m" << "exit " << "\033[0m" << "для выхода: ";
            std::getline(std::cin, input);

            while (input != "next" && input != "exit") {
                std::cout << "Неверная команда. Попробуйте снова: ";
                std::getline(std::cin, input);
            }

            if (input == "exit") {
                break;
            }
        }
    }

    file.close();
}

int main() {
    Parser parser;
    parser.parseFile("parserscript.txt");

    return 0;
}
