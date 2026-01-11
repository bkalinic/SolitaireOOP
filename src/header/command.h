#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>
#include <vector>
#include "card.h"
#include "pile.h"

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual std::string getDescription() const = 0;
};

class MoveCommand : public Command {
private:
    std::vector<std::shared_ptr<Card>> m_cards;
    std::shared_ptr<Pile> m_from;
    std::shared_ptr<Pile> m_to;
    std::vector<bool> m_wasFaceUp;

public:
    MoveCommand(std::shared_ptr<Card> card,
        std::shared_ptr<Pile> from,
        std::shared_ptr<Pile> to);
    MoveCommand(const std::vector<std::shared_ptr<Card>>& cards,
        std::shared_ptr<Pile> from,
        std::shared_ptr<Pile> to);

    void execute() override;
    void undo() override;
    void redo() override;
    std::string getDescription() const override;
};

#endif