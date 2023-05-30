#include "Succession.h"

Succession::Succession() {}

Succession::Succession(const std::string &command)
{
    setCommand(command);
}

int Succession::setCommand(const std::string &command)
{
    this->thereIsRoles = Role::none;
    if (this->command.empty())
        this->isReadyRoles = Role::none;
    this->command = command;
    this->queue.reset();
    int num = setCommandPrivate(this->command);
    if (num != 0)
        this->command.clear();
    return num;
}

bool Succession::setReady(const Role &roles, const bool &ready)
{
    if (this->command.empty())
        return false;
    std::vector<ptrNode> selectUsers;
    recurSetReady(this->queue, roles, selectUsers);
    if (selectUsers.size() == 0)
        return false;
    for (int i = 0; i < selectUsers.size(); i++)
        selectUsers[i]->setReady(ready);

    if (ready)
        this->isReadyRoles = pushRole(this->isReadyRoles, roles);
    else
        this->isReadyRoles = eraseRole(this->isReadyRoles, roles);

    return true;
}

bool Succession::setAllReady(const bool &ready)
{
    if (this->command.empty())
        return false;
    if (ready)
        this->isReadyRoles = static_cast<Role>(~(((-1)) << (countRole - 1)));
    else
        this->isReadyRoles = Role::none;
    recurSetAllReady(this->queue, ready);
    return true;
}

Role Succession::getUnReady()
{
    Role roles = Role::none;
    if (!this->queue->getReady())
    {
        roles = static_cast<Role>(recurUnReady(this->queue));
        Role temp = static_cast<Role>(roles & this->isReadyRoles);
        if (temp != Role::none)
        {
            setReady(temp, true);
            roles = getUnReady();
        }
    }
    return roles;
}

Role Succession::getRoles()
{
    return this->thereIsRoles;
}

std::string Succession::getCommad()
{
    return this->command;
}

int Succession::recurUnReady(ptrNode queue)
{
    int tempRole = 0;
    std::vector<ptrNode> tempPrev = queue->getPrevNode();
    while (true)
    {
        if (tempPrev.size() == 0)
        {
            if (!queue->getReady())
                tempRole = queue->getRole();
            break;
        }
        else if (queue->getRole() == Role::none)
        {
            for (int i = 0; i < tempPrev.size(); i++)
                tempRole = tempRole | recurUnReady(tempPrev[i]);
            break;
        }
        else if (tempPrev[0]->getReady())
        {
            tempRole = queue->getRole();
            break;
        }

        queue = std::move(tempPrev[0]);
        tempPrev = queue->getPrevNode();
    }
    return tempRole;
}

void Succession::recurSetReady(ptrNode queue, const Role &role, std::vector<ptrNode> &selectUsers)
{
    std::vector<ptrNode> tempPrev;
    while (true)
    {
        if (crossRole(queue->getRole(), role))
            selectUsers.push_back(queue);

        tempPrev = queue->getPrevNode();
        if (tempPrev.size() == 0)
            break;

        if (queue->getRole() == Role::none)
        {
            for (int i = 0; i < tempPrev.size(); i++)
                recurSetReady(tempPrev[i], role, selectUsers);
            break;
        }
        queue = tempPrev[0];
    }
}

void Succession::recurSetAllReady(ptrNode queue, const bool &ready)
{
    std::vector<ptrNode> tempPrev;
    while (true)
    {
        if (queue->getRole() != Role::none)
            queue->setReady(ready);

        tempPrev = queue->getPrevNode();
        if (tempPrev.size() == 0)
            break;

        if (queue->getRole() == Role::none)
        {
            for (int i = 0; i < tempPrev.size(); i++)
                recurSetAllReady(tempPrev[i], ready);
            break;
        }
        queue = tempPrev[0];
    }
}

bool Succession::checkRoles(const Role &numRole)
{
    if (crossRole(this->thereIsRoles, numRole))
        return true;
    else
    {
        this->thereIsRoles = pushRole(this->thereIsRoles, numRole);
        return false;
    }
}

int Succession::setCommandPrivate(std::string command)
{
    command = '(' + command + ')';
    uint32_t countLeft = 0, countRight = 0;

    for (int i = 0; i < command.length(); i++)
    {
        if (command[i] == ' ')
        {
            command.erase(i, 1);
            i--;
            continue;
        }
        if (((command[i] >= 'a' && command[i] <= 'z') || command[i] == '&' || command[i] == '>' || command[i] == '(' || command[i] == ')') == false)
        {
            return 1;
        }
        if (command[i] == '(')
            countLeft++;
        else if (command[i] == ')')
            countRight++;
    }
    if (countLeft != countRight)
        return 2;

    std::stack<int> leftBracket;
    std::stack<ptrNode> stackNodes;
    std::string str, tempStr;
    Role numRole;
    for (int i = 0; i < command.length(); i++)
    {
        if (command[i] == '(')
            leftBracket.push(i);
        else if (command[i] == ')')
        {
            str = std::move(command.substr(leftBracket.top() + 1, i - leftBracket.top() - 1));
            if (str[0] == '>' || str[0] == '&')
                return 4;
            bool tempA = false, tempB = false;
            for (int i = 0; i < str.length(); i++)
            {
                if (str[i] == '>')
                    tempA = true;
                if (str[i] == '&')
                    tempB = true;
            }
            if (tempA && tempB)
                return 4;
            command.replace(leftBracket.top(), i - leftBracket.top() + 1, "N");
            i = leftBracket.top();
            leftBracket.pop();
            int32_t numSymbol, startSymbol;
            if ((startSymbol = str.rfind('>')) != std::string::npos)
            {
                numSymbol = str.length();
                ptrNode lastNode, firstNode;
                startSymbol++;
                tempStr = std::move(str.substr(startSymbol, numSymbol - startSymbol));
                if (tempStr == "N")
                {
                    lastNode = stackNodes.top();
                    stackNodes.pop();
                }
                else
                {
                    numRole = nameToNum(tempStr);
                    if (numRole == Role::none)
                        return 3;
                    if (checkRoles(numRole))
                    {
                        this->queue.reset();
                        return 5;
                    }
                    lastNode = ptrNode(new NodeRole(numRole));
                }
                firstNode = lastNode;

                numSymbol = startSymbol - 1;
                startSymbol = str.rfind('>', numSymbol - 1);
                if (startSymbol == std::string::npos)
                    startSymbol = 0;
                else
                    startSymbol++;

                if (startSymbol >= numSymbol)
                {
                    while (!stackNodes.empty())
                        stackNodes.pop();
                    return 4;
                }

                while (numSymbol > -1)
                {
                    tempStr = std::move(str.substr(startSymbol, numSymbol - startSymbol));

                    if (tempStr == "N")
                    {
                        firstNode->pushPrev(stackNodes.top());
                        firstNode = stackNodes.top();
                        stackNodes.pop();
                    }
                    else
                    {
                        numRole = nameToNum(tempStr);
                        if (numRole == Role::none)
                            return 3;
                        if (checkRoles(numRole))
                        {
                            this->queue.reset();
                            return 5;
                        }
                        ptrNode tempNode(new NodeRole(numRole));
                        firstNode->pushPrev(tempNode);
                        firstNode = tempNode;
                    }
                    numSymbol = startSymbol - 1;
                    startSymbol = str.rfind('>', numSymbol - 1) + 1;
                    if (startSymbol == std::string::npos)
                        startSymbol = 0;
                }
                stackNodes.push(lastNode);
            }
            else if ((numSymbol = str.find('&')) != std::string::npos)
            {
                startSymbol = 0;
                ptrNodeAnd nodeAnd = ptrNodeAnd(new NodeAnd());
                while (startSymbol < str.length())
                {
                    tempStr = std::move(str.substr(startSymbol, numSymbol - startSymbol));
                    if (tempStr == "N")
                    {
                        nodeAnd->pushPrev(stackNodes.top());
                        stackNodes.pop();
                    }
                    else
                    {
                        numRole = nameToNum(tempStr);
                        if (numRole == Role::none)
                            return 3;
                        if (checkRoles(numRole))
                        {
                            this->queue.reset();
                            return 5;
                        }
                        nodeAnd->pushPrev(ptrNode(new NodeRole(numRole)));
                    }
                    startSymbol = numSymbol + 1;
                    numSymbol = str.find('&', startSymbol);
                    if (numSymbol == std::string::npos)
                        numSymbol = str.length();
                }
                stackNodes.push(nodeAnd);
            }
            else
            {
                if (str == "N")
                    continue;
                numRole = nameToNum(str);
                if (numRole == Role::none)
                    return 3;
                if (checkRoles(numRole))
                {
                    this->queue.reset();
                    return 5;
                }
                stackNodes.push(ptrNode(new NodeRole(numRole)));
            }
        }
    }
    this->queue = stackNodes.top();
    stackNodes.pop();
    return 0;
}

bool Succession::NodeAnd::pushPrev(const ptrNode &prev)
{
    for (int i = 0; i < this->prevNode.size(); i++)
    {
        if (bool(prev))
        {
            if (this->prevNode[i] == prev)
                return false;
        }
    }
    this->prevNode.push_back(prev);
    return true;
}

bool Succession::NodeAnd::getReady()
{
    for (int i = 0; i < this->prevNode.size(); i++)
        if (this->prevNode[i]->getReady() == false)
            return false;
    return true;
}

void Succession::NodeAnd::setReady(const bool &ready)
{
    for (int i = 0; i < this->prevNode.size(); i++)
        this->prevNode[i]->setReady(ready);
}

Role Succession::NodeAnd::getRole()
{
    return Role::none;
}

std::vector<Succession::ptrNode> Succession::NodeAnd::getPrevNode()
{
    return this->prevNode;
}

Succession::NodeRole::NodeRole(const Role &role)
{
    this->isReady = false;
    this->role = role;
    prevNode = nullptr;
}

bool Succession::NodeRole::pushPrev(const ptrNode &prev)
{
    if (!bool(this->prevNode))
    {
        this->prevNode = prev;
        return true;
    }
    return false;
}

Role Succession::NodeRole::getRole()
{
    return this->role;
}

bool Succession::NodeRole::getReady()
{
    return this->isReady;
}

void Succession::NodeRole::setReady(const bool &ready)
{
    this->isReady = ready;
}

std::vector<Succession::ptrNode> Succession::NodeRole::getPrevNode()
{
    std::vector<Succession::ptrNode> temp;
    if (bool(this->prevNode))
        temp.push_back(this->prevNode);
    return temp;
}