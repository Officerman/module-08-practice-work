//Команда для системы умного дома:

#include <iostream>
#include <vector>
#include <stack>

// Интерфейс команды
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~ICommand() = default;
};

// Классы устройств
class Light {
public:
    void on() { std::cout << "Light is ON\n"; }
    void off() { std::cout << "Light is OFF\n"; }
};

class AC {
public:
    void on() { std::cout << "AC is ON\n"; }
    void off() { std::cout << "AC is OFF\n"; }
};

class TV {
public:
    void on() { std::cout << "TV is ON\n"; }
    void off() { std::cout << "TV is OFF\n"; }
};

// Команды для управления устройствами
class LightOnCommand : public ICommand {
    Light* light;
public:
    LightOnCommand(Light* l) : light(l) {}
    void execute() override { light->on(); }
    void undo() override { light->off(); }
};

class LightOffCommand : public ICommand {
    Light* light;
public:
    LightOffCommand(Light* l) : light(l) {}
    void execute() override { light->off(); }
    void undo() override { light->on(); }
};

class ACOnCommand : public ICommand {
    AC* ac;
public:
    ACOnCommand(AC* a) : ac(a) {}
    void execute() override { ac->on(); }
    void undo() override { ac->off(); }
};

class ACOffCommand : public ICommand {
    AC* ac;
public:
    ACOffCommand(AC* a) : ac(a) {}
    void execute() override { ac->off(); }
    void undo() override { ac->on(); }
};

// Макрокоманда
class MacroCommand : public ICommand {
    std::vector<ICommand*> commands;
public:
    void addCommand(ICommand* command) { commands.push_back(command); }
    void execute() override {
        for (auto command : commands) command->execute();
    }
    void undo() override {
        for (auto command : commands) command->undo();
    }
};

// Пульт управления
class RemoteControl {
    std::stack<ICommand*> history;
    ICommand* lastCommand = nullptr;
public:
    void setCommand(ICommand* command) {
        command->execute();
        history.push(command);
        lastCommand = command;
    }

    void undoCommand() {
        if (!history.empty()) {
            ICommand* command = history.top();
            command->undo();
            history.pop();
        }
    }

    void redoCommand() {
        if (lastCommand) {
            lastCommand->execute();
            history.push(lastCommand);
        }
    }
};

int main() {
    Light light;
    AC ac;
    
    LightOnCommand lightOn(&light);
    LightOffCommand lightOff(&light);
    ACOnCommand acOn(&ac);
    ACOffCommand acOff(&ac);
    
    RemoteControl remote;
    
    remote.setCommand(&lightOn);
    remote.setCommand(&acOn);
    remote.undoCommand();  // Отключаем AC
    remote.redoCommand();  // Включаем AC снова

    // команда для включения всех устросйтв
    MacroCommand macro;
    macro.addCommand(&lightOn);
    macro.addCommand(&acOn);
    remote.setCommand(&macro);

    return 0;
}


/////////////////

/*
#include <iostream>

// Абстр класс для создания отчетов
class ReportGenerator {
public:
    void generateReport() {
        collectData();
        formatReport();
        saveReport();
        if (customerWantsNotification()) {
            notifyCustomer();
        }
    }

    virtual void collectData() = 0;
    virtual void formatReport() = 0;
    virtual void saveReport() = 0;

    // Перехватываемый метод
    virtual bool customerWantsNotification() { return true; }
    virtual void notifyCustomer() { std::cout << "Notifying customer...\n"; }
};

// PDF
class PdfReport : public ReportGenerator {
    void collectData() override { std::cout << "Collecting data for PDF...\n"; }
    void formatReport() override { std::cout << "Formatting PDF report...\n"; }
    void saveReport() override { std::cout << "Saving PDF report...\n"; }
};

// Excel
class ExcelReport : public ReportGenerator {
    void collectData() override { std::cout << "Collecting data for Excel...\n"; }
    void formatReport() override { std::cout << "Formatting Excel report...\n"; }
    void saveReport() override { std::cout << "Saving Excel report...\n"; }
};

int main() {
    PdfReport pdfReport;
    pdfReport.generateReport();

    ExcelReport excelReport;
    excelReport.generateReport();

    return 0;
}
*/
/////////////////////// Для отчет системы (посредник)
/*

#include <iostream>
#include <vector>
#include <string>

class IUser;

// Интерфейс Посредника
class IMediator {
public:
    virtual void sendMessage(const std::string& message, IUser* sender) = 0;
    virtual void addUser(IUser* user) = 0;
    virtual ~IMediator() = default;
};

class IUser {
protected:
    IMediator* mediator;
    std::string name;
public:
    IUser(IMediator* med, const std::string& userName) : mediator(med), name(userName) {}
    virtual void send(const std::string& message) = 0;
    virtual void receive(const std::string& message) = 0;
    virtual ~IUser() = default;
};

class ChatMediator : public IMediator {
    std::vector<IUser*> users;
public:
    void addUser(IUser* user) override {
        users.push_back(user);
    }

    void sendMessage(const std::string& message, IUser* sender) override {
        for (auto user : users) {
            if (user != sender) {
                user->receive(message);
            }
        }
    }
};

class User : public IUser {
public:
    User(IMediator* med, const std::string& userName) : IUser(med, userName) {}
    
    void send(const std::string& message) override {
        std::cout << name << " sends: " << message << "\n";
        mediator->sendMessage(message, this);
    }

    void receive(const std::string& message) override {
        std::cout << name << " receives: " << message << "\n";
    }
};

int main() {
    ChatMediator mediator;
    
    User user1(&mediator, "Alice");
    User user2(&mediator, "Bob");
    
    mediator.addUser(&user1);
    mediator.addUser(&user2);
    
    user1.send("Hello everyone!");
    user2.send("Hi, Alice!");

    return 0;
}


*/