# Заметки по almai.yaml (переосмысление)


## Полезные ссылки

- YAML комментарии - https://chat.deepseek.com/share/1jv1quu9hsctxtl8ej


## Роли

Роли - это набор скилов разных типов.


### Задание ролей через командную строку

Роли могут задаваться в командной строке.

Предопределённые роли задаются в builtins файлах опций.

Роли могу переопределяться.

Командная строка, задаём роль `c-cpp-dev` со скилами `skills/cpp-dev` и `skills/c-dev`, и исключаем `skills/jni-master`:
```
--role-setup=c-cpp-dev:+skills/cpp-dev;skills/c-dev,-skills/jni-master
```

или, с предварительной очисткой всего, ранее связанного с ролью:
```
--role-setup=-,c-cpp-dev:skills/cpp-dev;skills/c-dev
```

Отдельный минус, если не является префиксом, просто всё сбрасывает.
Если минус является префиксом, то он удаляет элемент, если он был ранее установлен.
Если префиксом является плюс - это добавление. Это обычное поведение по умолчанию, и плюс можно не указывать.


### Задание ролей в almai.yaml

Проверено - https://chat.deepseek.com/share/u82e7ywtoghxchn7mi
Генерализованный синтаксис в итоге - https://chat.deepseek.com/share/0emeku52b77afjsuzb

```
---
roles:
  - c-cpp-dev: -, skills/cpp-dev; skills/c-dev, -skills/jni-master # тут в одну строку сбросили все предыдущие скилы для роли, и добавили skills/cpp-dev и skills/c-dev
  - c-cpp-no-java-dev: skills/cpp-dev; skills/c-dev, -skills/jni-master # тут только добавили skills/cpp-dev и skills/c-dev
  - uni-tester:
    - - # Сбрасываем все предыдущие скилы для роли (а это будет валидно в yml?)
    - skills/auto-tester
    - skills/manual-tester
  - super-architect:
    - sw-architect # Архитектор ПО
    - sys-architect # Архитектор всей системы

```

