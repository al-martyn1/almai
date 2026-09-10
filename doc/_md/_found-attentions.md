---
Generator: Umba Brief Scanner
---

# _libs/encoding

- `[_libs/encoding/encoding.h:318]`
  Надо бы тут разобраться с файлами, которые UTF-16



# _libs/marty_cpp

- `[_libs/marty_cpp/marty_cpp.h:3284]`
  название? UPD: чего я тут хотел, уже не помню, надо бы коменты более развёрнуто
  делать

- `[_libs/marty_cpp/marty_cpp.h:4240]`
  Тут получить локализацию

- `[_libs/marty_cpp/marty_cpp.h:4415]`
  Тут получить локализацию

- `[_libs/marty_cpp/marty_cpp.h:5372]`
  std::make_tuple - что тут сказать хотел, непонятно, забыл уже

- `[_libs/marty_cpp/marty_cpp.h:5557]`
  заменить std::stoll, чтоб умело парсить двоичные константы и разделители try



# _libs/marty_yaml_toml_json

- `[_libs/marty_yaml_toml_json/yaml_utils.h:3]`
  MSVC starts to got warnings on yaml-cpp lib

- `[_libs/marty_yaml_toml_json/yaml_utils.h:4]`
  Update yaml-cpp?



# _libs/tmp_bak/results/umba

- `[_libs/tmp_bak/results/umba/assert.h:153]`
  Сделать как надо UMBA_ASSERT для GCC/Clang

- `[_libs/tmp_bak/results/umba/time_service.cpp:184]`
  Сделать как надо umba::time_service::init()

- `[_libs/tmp_bak/results/umba/time_service.cpp:196]`
  Сделать как надо umba::time_service::start()

- `[_libs/tmp_bak/results/umba/time_service.cpp:205]`
  Сделать как надо umba::time_service::stop()

- `[_libs/tmp_bak/results/umba/time_service.cpp:232]`
  Сделать как надо

- `[_libs/tmp_bak/results/umba/time_service.cpp:258]`
  Сделать как надо umba::time_service::getCurTimeHires()



# _libs/tmp_bak/results/umba/internal

- `[_libs/tmp_bak/results/umba/internal/filesys.h:965]`
  На не винде проверяем доступность на запись и чтение через плюсовые потоки



# _libs/umba

- `[_libs/umba/assert.h:155]`
  Сделать как надо UMBA_ASSERT для GCC/Clang



- `[_libs/umba/cmd_line.h:1837]`
  Тут закоменчено что-то старое, хз зачем я тут внимание акцентировал.
    std::map<std::string, int>::const_iterator eit = optInfo.enumVals.find(optArgCopy);
    if (eit == optInfo.enumVals.end())
    {
    errMsg = std::string("Invalid option value taken. Option value can be one of: ") + optInfo.getAllEnumNames(", ", " or ") + std::string(" (") + optInfo.getAllOptionNames("/") + std::string(")");
    return false;
    }

- `[_libs/umba/cmd_line.h:3271]`
  Надо ProgramLocation проверить на юникод

- `[_libs/umba/cmd_line.h:5063]`
  Пока не даём подсказку по вызову хелпа конкретной опции конкретной команды,
  потому что этот механизм пока не работает, и всегда выводится информация по
  всем опциям команды

- `[_libs/umba/cmd_line.h:5069]`
  Нужна склейка двух inline блоков кода auto optPrintText =
  listItem(inline_code(optFullName) + tt("    " + fillStr) +
  it->makeCommandOptionHelpCommand(exeName, optFullName) );

- `[_libs/umba/cmd_line.h:5084]`
  Для конкретной команды у нас почему-то опции не собираются Пока и так сойдёт,
  на потом надо разобраться

- `[_libs/umba/macros.h:421]`
  Чего-то с прокси не срослось - компилятор помирает от вложенности шаблонов
  Порешал, сделав getter нешаблонным параметром с виртуальным оператором ()

- `[_libs/umba/macros.h:425]`
  Пока не будем ничего делать, потом разберёмся

- `[_libs/umba/program_location.h:145]`
  Надо сделать через dl_*

- `[_libs/umba/shellapi.h:628]`
  тут нужен замут через dl*, но пока лень и не особо нужно

- `[_libs/umba/shellapi.h:1073]`
  Надо добавить проверку атрибута X

- `[_libs/umba/shellapi.h:1092]`
  Если EXE/COM не присутствуют в списке, надо добавить

- `[_libs/umba/shellapi.h:1521]`
  Undone posix_spawn Standard C library (libc, -lc) #include <spawn.h> ?
  https://man7.org/linux/man-pages/man3/posix_spawn.3.html
  https://pubs.opengroup.org/onlinepubs/9799919799/functions/posix_spawn.html
  https://www.opennet.ru/man.shtml?topic=posix_spawn&category=3&russian=5

- `[_libs/umba/shellapi.h:1529]`
  open O_CLOEXEC

- `[_libs/umba/time_service.cpp:186]`
  Сделать как надо umba::time_service::init()

- `[_libs/umba/time_service.cpp:198]`
  Сделать как надо umba::time_service::start()

- `[_libs/umba/time_service.cpp:207]`
  Сделать как надо umba::time_service::stop()

- `[_libs/umba/time_service.cpp:234]`
  Сделать как надо

- `[_libs/umba/time_service.cpp:260]`
  Сделать как надо umba::time_service::getCurTimeHires()

- `[_libs/umba/zz_detect_environment.h:691]`
  ??? Чо сказать хотел, не понятно

- `[_libs/umba/zz_mcu_low_level.h:34]`
  Сделать как надо UMBA_INSTRUCTION_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:58]`
  Сделать как надо UMBA_DATA_MEMORY_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:76]`
  Сделать как надо UMBA_DATA_SYNCHRONIZATION_BARRIER

- `[_libs/umba/zz_mcu_low_level.h:127]`
  Сделать как надо UMBA_INTERRUPTS_DISABLED

- `[_libs/umba/zz_mcu_low_level.h:143]`
  Сделать как надо UMBA_DISABLE_IRQ

- `[_libs/umba/zz_mcu_low_level.h:152]`
  Сделать как надо UMBA_ENABLE_IRQ



# _libs/umba/internal

- `[_libs/umba/internal/filesys.h:1003]`
  На не винде проверяем доступность на запись и чтение через плюсовые потоки



# _src/almai

- `[_src/almai/almai.cpp:162]`
  --------------

- `[_src/almai/md_utils.h:963]`
  Не нужно. Просто был кривоватый документ, в нем были листинги на питоне, и
  внутри них были маркдаун маркеры блоков кода

- `[_src/almai/PrepromptDatabase.h:500]`
  Нужно поправить зависимости, привести всё в plural

- `[_src/almai/Test08ArgParser.h:277]`
  setDescription обязателеь, он "финализирует" опцию. Может быть пустым. if
  (opt.isOption("recursive") || opt.setDescription(""))

- `[_src/almai/_test08.cpp:194]`
  --------------

- `[_src/almai/_test09.cpp:162]`
  --------------

