Я хочу сделать что-то вроде ИИ-агента типа Claude Code для упрощения разработки (в основном меня интересует C++).

Мой механизм должен работать через общедоступные чат-боты типа deepseek или qwen через браузер.

Я умею читать препромпты из файлов. В начале сканируются каталоги препромптов из поставки программы, потом пользовательские каталоги, потом каталоги проекта.

Я умею читать общие препромпты, и специализированные, например, для deepseek или qwen.

Препромпты имеют двух уровневую иерархию. Первый уровень - это "тип" препромпта, например: 
skills, instructions, knowledges, formats, outputs и тому подобное.

В препропмптах можно ссылаться на зависимые препромпты - делать "сборные" препромпты.

У меня есть файл проекта, PROJECT.yaml. У него на текущий момент такой вид:
```
roles:
  - uni-tester:
    - "-" # Сбрасываем все предыдущие скилы для роли (а это будет валидно в yml?)
    - Skill/auto-tester
    - skills/manual-tester
  - super-architect:
    - sw-architect # Архитектор ПО
    - sys-architect # Архитектор всей системы
  # Можно записать и как object, но массив гарантирует порядок элементов, а object - не факт. Впрочем, другой нефакт в том, что при настройке ролей нам порядок не особо и важен
  - c-cpp-dev: -, skills/cpp-dev; skills/c-dev, -skills/jni-master # тут в одну строку сбросили все предыдущие скилы для роли, и добавили skills/cpp-dev и skills/c-dev
  - c-cpp-no-java-dev: skills/cpp-dev; skills/c-dev, -skills/jni-master # тут только добавили skills/cpp-dev и skills/c-dev
  - py-master: py-fullstack-master
```

Файл препромпта выглядит так:
```
---
name: java-dev
description: Investigates bugs using scientific method, manages debug sessions, handles checkpoints. Spawned by /gsd:debug orchestrator.
requires:
---

Common Java developer
```

Препромпты размечены на языке markdown. Кроме этого, препромпты могут быть размечены частями по типу xml/html тэгов.

Также я умею формировать markdown-документ с включенными файлами с их именами - этакий "markdown-архив".

Включение файлов оформляется примерно так:
~~~
**path/to/file
```
file content
```
~~~

Также, я умею разбирать markdown-документ и извлекать включенные файлы из него - разархивирование файлов из "markdown-архива".

---------

Мне не очень понятно, что мне делать дальше. Какие у тебя есть идеи на эту тему?



https://chat.deepseek.com/share/vos7kz31tn0r0x6nkv


