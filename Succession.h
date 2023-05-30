#include <vector>
#include <stack>
#include <memory>
#include "Role.h"

#pragma once
#ifndef _there_is_Succesion
#define _there_is_Succesion

class Succession
{
private:
    class Node;
    class NodeRole;
    class NodeAnd;
    typedef std::shared_ptr<Node> ptrNode;
    typedef std::shared_ptr<NodeRole> ptrNodeRole;
    typedef std::shared_ptr<NodeAnd> ptrNodeAnd;

public:
    // Конструктор по умолчанию
    Succession();

    // Конструктор создания
    Succession(const std::string &command);

    // Устанавливает последовательность выполнения; код ошибки: 0-нет ошибок; 1-недопустимые символы; 2-неравное кол-во скобок; 3-неверное название роли; 4-неверная команда; 5-повторение роли
    int setCommand(const std::string &command);

    // Находит узел по роли и устанавливает готовность; если найдёт, то вернёт true, иначе false
    bool setReady(const Role &role, const bool &ready);

    // Устанавливает готовность всем узлам
    bool setAllReady(const bool &ready);

    // Находит узлы, которые не готовы, и возвращает роли
    Role getUnReady();

    // Возвращает роли, используемые в последовательности
    Role getRoles();

    // Возвращает команду
    std::string getCommad();

private:
    class Node
    {
    public:
        virtual bool pushPrev(const ptrNode &prev) = 0;
        virtual bool getReady() = 0;
        virtual void setReady(const bool &ready) = 0;
        virtual Role getRole() = 0;
        virtual std::vector<ptrNode> getPrevNode() = 0;
    };
    class NodeAnd : public Node
    {
    public:
        // Вставляет в массив предыдущих узлов узел
        bool pushPrev(const ptrNode &prev) override;
        // Возвращает готовность узла
        bool getReady() override;
        // Устанавливает готовность
        void setReady(const bool &ready) override;
        // Возвращает роль узла
        Role getRole() override;
        // Возвращает массив предыдущих узлов
        std::vector<ptrNode> getPrevNode() override;

    private:
        std::vector<ptrNode> prevNode; // Массив предыдущих узлов
    };
    class NodeRole : public Node
    {
    public:
        // Конструктор создания
        NodeRole(const Role &role);
        // Устанавливает в предыдущий узел другой узел
        bool pushPrev(const ptrNode &prev) override;
        // Возвращает роль узла
        Role getRole() override;
        // Возвращает готовность узла
        bool getReady() override;
        // Устанавливает готовность узлу
        void setReady(const bool &ready) override;
        // Возвращает массив предыдущих узлов с одним элементом
        std::vector<ptrNode> getPrevNode() override;

    private:
        ptrNode prevNode; // Предыдущий узел
        Role role;        // Роль узла
        bool isReady;     // Готовность
    };

    // Рекурсивно ищет узлы, которые не готовы и записывает их в массив
    int recurUnReady(ptrNode queue);

    // Рекурсивно ищет узел по роли и записывает его в указатель
    void recurSetReady(ptrNode queue, const Role &role, std::vector<ptrNode> &selectUser);

    // Рекурсивно устанавливает готовность всем узлам
    void recurSetAllReady(ptrNode queue, const bool &ready);

    // Используется только в setCommandPrivate, проверяет, есть ли данная роль в команде, если есть, то возвращает true, иначе записывает в массив thereIsRoles true и возвращает false
    bool checkRoles(const Role &numRole);

    // Устанавливает последовательность выполнения; возвращает код ошибки
    int setCommandPrivate(std::string command);

    Role thereIsRoles;   // Хранит в себе присутствие ролей в команде
    Role isReadyRoles;   // Хранит в себе готовность ролей
    std::string command; // Команда
    ptrNode queue;       // Указывает на последний узел в последовательности
};

#endif // !_there_is_Succesion