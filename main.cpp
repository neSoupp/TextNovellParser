#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Variable {
    std::string name;
    std::string value;
};

bool signs(const std::string& condition, const std::vector<Variable>& variables) {
    // Разбиваем условие на операнды и оператор сравнения
    std::istringstream fin(condition);
    std::string leftOperand, signOperand, rightOperand;
    fin >> leftOperand >> signOperand >> rightOperand;

    // Проходимся по списку переменных и ищем совпадение с левым операндом
    for (const Variable& variable : variables) {
        if (variable.name == leftOperand) {
            std::string leftValue = variable.value;
            // Внутренний цикл для поиска совпадения с правым операндом
            for (const Variable& variable : variables) {
                if (variable.name == rightOperand) {
                    std::string rightValue = variable.value;

                    // Проверяем оператор сравнения и возвращаем результат сравнения
                    if (signOperand == "==") {
                        return leftValue == rightValue;
                    } else if (signOperand == "!=") {
                        return leftValue != rightValue;
                    } else if (signOperand == ">") {
                        return std::stoi(leftValue) > std::stoi(rightValue);
                    } else if (signOperand == ">=") {
                        return std::stoi(leftValue) >= std::stoi(rightValue);
                    } else if (signOperand == "<") {
                        return std::stoi(leftValue) < std::stoi(rightValue);
                    } else if (signOperand == "<=") {
                        return std::stoi(leftValue) <= std::stoi(rightValue);
                    }
                }
            }
        }
    }

    // Если не найдено совпадение или неправильный оператор сравнения, возвращаем false
    return false;
}
void parserScript(const std::string& scriptFilename) {
    std::ifstream finScript(scriptFilename);
    std::string line; // Переменная для хранения текущей строки из файла
    bool nextPage = false; // Флаг, указывающий, следует ли переходить на следующую страницу
    std::vector<Variable> variables; // Вектор для хранения переменных

    bool ifBlock = false; // Флаг, указывающий, находимся ли мы внутри блока if
    bool conditionSatisfied = true; // Флаг, указывающий, выполнено ли условие в текущем блоке if
    bool previousConditionSatisfied = true;

    while (std::getline(finScript, line)) {
        std::istringstream fin(line);
        std::string command;
        fin >> command;

        if (command == "/set") { // Команда /set используется для установки значения переменной
            std::string variable, value; 
            fin >> variable;
            std::getline(fin, value); // Читаем остаток строки в переменную value
            value = value.substr(1); // Избавляемся от пробела в начале значения
            variables.push_back({variable, value}); // Добавляем переменную в вектор переменных
        } else if (command == "/input") {
            std::string variable;
            fin >> variable;

            std::cout << "Ввод: ";
            std::string input; 
            std::getline(std::cin, input); // Читаем ввод пользователя
            variables.push_back({variable, input});
        } else if (command == "/if") { // Команда /if используется для начала блока условия
            std::string condition;
            std::getline(fin, condition);
            condition = condition.substr(1);

            if (ifBlock) { // Если уже находимся в блоке if, то это вложенный блок
                previousConditionSatisfied = conditionSatisfied;
                conditionSatisfied = previousConditionSatisfied && signs(condition, variables);
                continue; 
            }

            conditionSatisfied = signs(condition, variables);
            ifBlock = true; // Устанавливаем флаг, что находимся в блоке if
        } else if (command == "/end") { // Команда /end используется для завершения блока условия
            if (ifBlock) { // Если находимся в блоке if, то сбрасываем флаги
                ifBlock = false;
                conditionSatisfied = true;
            }
            continue;
        } else if (command == "/say") { // Команда /say используется для вывода сообщения
            if (ifBlock && !conditionSatisfied) { // Если находимся внутри блока if и условие не выполнено, пропускаем команду
                continue;
            }

            std::string person;
            std::getline(fin, person, ':'); // Читаем имя отправителя сообщения

            std::string message;
            std::getline(fin, message);

            std::string variablePlaceholder = "{";
            size_t variableStartPos = message.find(variablePlaceholder);
            while (variableStartPos != std::string::npos) { // Заменяем затычки переменных на их значения
                size_t variableEndPos = message.find('}', variableStartPos);
                if (variableEndPos != std::string::npos) {
                    std::string variableName = message.substr(variableStartPos + 1, variableEndPos - variableStartPos - 1); // Ищем переменную по имени
                    for (const Variable& variable : variables) { // Если найдена, заменяем плейсхолдер на значение переменной
                        if (variable.name == variableName) {
                            message.replace(variableStartPos, variableEndPos - variableStartPos + 1, variable.value);
                            variableStartPos = message.find(variablePlaceholder, variableStartPos + variable.value.length());
                            break;
                        }
                    }
                } else {
                    break;
                }
            }

            std::cout << person << ": " << message << std::endl;
        }
    }
}

int main() {
    std::string scriptFilename = "script.txt";
    parserScript(scriptFilename);

    return 0;
}
