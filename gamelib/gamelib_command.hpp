#ifndef GAMELIB_COMMAND_HPP
#define GAMELIB_COMMAND_HPP

#include <gamelib_base.hpp>

namespace GameLib {
    class Actor;

    class Command {
    public:
        virtual ~Command() {}

        virtual const char* type() const { return "Command"; }
    };

    class NormalCommand : public Command {
        virtual ~NormalCommand() {}

        const char* type() const override { return "NormalCommand"; }

        virtual void execute(){};
        virtual void undo(){};
    };

    class ActorCommand : public Command {
    public:
        virtual ~ActorCommand() {}

        const char* type() const override { return "ActorCommand"; }

        virtual void execute(Actor& actor){};
        virtual void undo(Actor& actor){};
    };

    template <typename CommandBaseType>
    class CommandFlyweight {
    public:
        void addCommand(CommandBaseType* command) {
            if (!command)
                return;
            commands[command->type()] = command;
        }

        CommandBaseType* getCommand(CommandBaseType* command) {
            if (!command)
                return nullptr;
            return isCommand(command->type()) ? commands[command->type()] : nullptr;
        }

        bool isCommand(const std::string& type) const { return commands.count(type) != 0; }

        std::map<std::string, CommandBaseType*> commands;
    };

    using NormalCommandFlyweight = CommandFlyweight<NormalCommand>;
    using ActorCommandFlyweight = CommandFlyweight<ActorCommand>;

	extern NormalCommandFlyweight normalCommands;
    extern ActorCommandFlyweight actorCommands;
}

#endif
