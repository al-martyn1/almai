Доработка CommandInfo/CommandSequenceController
- [X] addGlobalOptions - сделать версию с std::initializer_list
- [X] setOptions - сделать версию с std::initializer_list
- [X] getFullCommandStr - добавить версию, принимающую строку, и добавляющую эту строку к формируемой полной команде
- [X] при попытке вызвать неизвестную команду ошибочно выдаётся сообщение с пустой командой - учитываются только накопленные валидные команды, а новая невалидная не выводится
- [X] в CommandInfo::transformCommandParameter считать пути, начинающиеся с символа '$' абсолютными
