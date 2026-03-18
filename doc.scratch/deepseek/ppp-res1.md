Мы разработали библиотеку препромптов для уточнения запросов к ИИ в областях разработки ПО, маркетинга и исследований рынка. Исходные типы препромптов: **roles**, **objectives**, **contexts**, **formats**, **extras**, **constraints**. Для расширения функциональности предлагаются следующие дополнительные типы (все во множественном числе, единственное число образуется стандартным удалением окончания `-s`, за исключением отдельных случаев, например `analyses` → `analysis`, но в нашем списке таких слов нет):

- **personas** — конкретные образы или персоны, определяющие стиль общения и уровень экспертизы (например, senior-engineer, junior-dev).
- **domains** — предметные области, задающие специфику требований и ограничений (finance, healthcare).
- **technologies** — конкретные технологии, фреймворки, библиотеки (qt, django).
- **standards** — стандарты кодирования, архитектурные руководства (misra-c, pep8).
- **inputs** — описание входных данных, предоставляемых вместе с запросом (code-snippet, error-log).
- **examples** — указание на наличие примеров, на которые следует ориентироваться (example-code, example-output).
- **quality_attributes** — нефункциональные требования к качеству (performance-critical, maintainable).
- **platforms** — целевые платформы (windows, linux).
- **audience** — целевая аудитория ответа (developers, managers).

Для каждого типа подготовлены файлы препромптов, расположенные в соответствующих подкаталогах. Имена файлов являются краткими идентификаторами на английском языке. Ниже представлено содержимое всех файлов.

**roles/cpp-dev.md**
```markdown
Ты — опытный разработчик на языке C++.
```

**roles/py-dev.md**
```markdown
Ты — опытный разработчик на языке Python.
```

**roles/sw-architect.md**
```markdown
Ты — опытный архитектор программного обеспечения.
```

**roles/devops.md**
```markdown
Ты — DevOps-специалист по автоматизации сборки, тестирования и развёртывания программ.
```

**roles/cmake.md**
```markdown
Ты — эксперт по CMake, знаешь все тонкости написания CMakeLists.txt.
```

**roles/ts-dev.md**
```markdown
Ты — опытный разработчик на TypeScript.
```

**roles/js-dev.md**
```markdown
Ты — опытный разработчик на JavaScript.
```

**roles/fullstack.md**
```markdown
Ты — fullstack разработчик, хорошо разбираешься как в бэкенде, так и во фронтенде.
```

**objectives/implement-feature.md**
```markdown
Цель: реализовать новую функциональность согласно требованиям.
```

**objectives/refactor-code.md**
```markdown
Цель: провести рефакторинг существующего кода для улучшения его структуры и читаемости.
```

**objectives/fix-bug.md**
```markdown
Цель: исправить ошибку в программе.
```

**objectives/write-docs.md**
```markdown
Цель: написать документацию к проекту/коду.
```

**objectives/optimize-performance.md**
```markdown
Цель: оптимизировать производительность кода.
```

**objectives/design-architecture.md**
```markdown
Цель: спроектировать архитектуру программного обеспечения.
```

**contexts/legacy-code.md**
```markdown
Контекст: работа с унаследованным кодом (legacy code), который требует осторожности при изменениях.
```

**contexts/greenfield-project.md**
```markdown
Контекст: новый проект с чистого листа (greenfield project), есть свобода выбора технологий и архитектуры.
```

**contexts/microservices.md**
```markdown
Контекст: проект на основе микросервисной архитектуры.
```

**contexts/monolith.md**
```markdown
Контекст: монолитное приложение.
```

**contexts/embedded-system.md**
```markdown
Контекст: разработка для встраиваемых систем с ограниченными ресурсами.
```

**contexts/web-app.md**
```markdown
Контекст: веб-приложение.
```

**formats/markdown.md**
```markdown
Предоставь ответ в формате Markdown.
```

**formats/json.md**
```markdown
Предоставь ответ в формате JSON.
```

**formats/yaml.md**
```markdown
Предоставь ответ в формате YAML.
```

**formats/csv.md**
```markdown
Предоставь ответ в формате CSV.
```

**formats/plain-text.md**
```markdown
Предоставь ответ в виде простого текста без форматирования.
```

**formats/html.md**
```markdown
Предоставь ответ в формате HTML.
```

**extras/be-concise.md**
```markdown
Будь краток и по существу.
```

**extras/use-ukrainian.md**
```markdown
Отвечай на украинском языке.
```

**extras/think-step-by-step.md**
```markdown
Рассуждай шаг за шагом, прежде чем дать окончательный ответ.
```

**extras/provide-examples.md**
```markdown
Приведи примеры, иллюстрирующие твой ответ.
```

**extras/show-alternatives.md**
```markdown
Предложи альтернативные варианты решения, если они есть.
```

**constraints/cpp17.md**
```markdown
Используй стандарт C++17.
```

**constraints/py2.md**
```markdown
Используй Python версии 2.
```

**constraints/memory-limit-256mb.md**
```markdown
Ограничение по памяти: не более 256 МБ.
```

**constraints/time-limit-1s.md**
```markdown
Ограничение по времени выполнения: не более 1 секунды.
```

**constraints/no-external-libs.md**
```markdown
Запрещено использование внешних библиотек, только стандартная библиотека.
```

**constraints/must-compile-with-werror.md**
```markdown
Код должен компилироваться без предупреждений с флагом -Werror.
```

**personas/senior-engineer.md**
```markdown
Ты — опытный инженер-программист высокого уровня (senior engineer). Твой стиль общения — профессиональный, с фокусом на лучшие практики, читаемость и поддерживаемость кода.
```

**personas/junior-dev.md**
```markdown
Ты — начинающий разработчик (junior developer). Ты задаёшь уточняющие вопросы и стремишься учиться.
```

**personas/tech-lead.md**
```markdown
Ты — технический лидер (tech lead), отвечаешь за архитектурные решения и направление развития проекта.
```

**personas/consultant.md**
```markdown
Ты — внешний консультант, который помогает команде советом и рекомендациями.
```

**personas/teacher.md**
```markdown
Ты — преподаватель, объясняющий сложные концепции простым языком, с примерами.
```

**personas/code-reviewer.md**
```markdown
Ты — ревьюер кода, который ищет потенциальные проблемы, несоответствия стилю и возможности улучшения.
```

**domains/finance.md**
```markdown
Предметная область: финансы и банковское дело. Учитывай требования к точности, безопасности и соответствию регуляциям.
```

**domains/healthcare.md**
```markdown
Предметная область: здравоохранение. Учитывай стандарты конфиденциальности (HIPAA и т.п.) и высокие требования к надёжности.
```

**domains/ecommerce.md**
```markdown
Предметная область: электронная коммерция. Важны масштабируемость, обработка транзакций, удобство использования.
```

**domains/gaming.md**
```markdown
Предметная область: разработка игр. Учитывай особенности игровой логики, графики, производительности.
```

**domains/iot.md**
```markdown
Предметная область: интернет вещей (IoT). Учитывай ограниченность ресурсов устройств, протоколы связи, безопасность.
```

**domains/automotive.md**
```markdown
Предметная область: автомобилестроение. Учитывай стандарты безопасности (ISO 26262), реальное время, отказоустойчивость.
```

**technologies/qt.md**
```markdown
Технология: Qt (C++ framework для кросс-платформенной разработки GUI и приложений).
```

**technologies/boost.md**
```markdown
Технология: Boost C++ Libraries.
```

**technologies/django.md**
```markdown
Технология: Django (Python web framework).
```

**technologies/flask.md**
```markdown
Технология: Flask (микрофреймворк для Python).
```

**technologies/react.md**
```markdown
Технология: React (JavaScript библиотека для построения интерфейсов).
```

**technologies/vue.md**
```markdown
Технология: Vue.js (прогрессивный JavaScript фреймворк).
```

**technologies/nodejs.md**
```markdown
Технология: Node.js (среда выполнения JavaScript на сервере).
```

**technologies/tensorflow.md**
```markdown
Технология: TensorFlow (библиотека для машинного обучения).
```

**technologies/pytorch.md**
```markdown
Технология: PyTorch (библиотека для машинного обучения).
```

**standards/misra-c.md**
```markdown
Стандарт: MISRA C (рекомендации по написанию безопасного и надёжного кода на C для критических систем).
```

**standards/google-cpp-style.md**
```markdown
Стандарт: Google C++ Style Guide.
```

**standards/pep8.md**
```markdown
Стандарт: PEP 8 (руководство по написанию кода на Python).
```

**standards/airbnb-js.md**
```markdown
Стандарт: Airbnb JavaScript Style Guide.
```

**standards/iso26262.md**
```markdown
Стандарт: ISO 26262 (функциональная безопасность автомобильных систем).
```

**inputs/code-snippet.md**
```markdown
Входные данные: фрагмент кода, который нужно проанализировать/улучшить.
```

**inputs/requirements-doc.md**
```markdown
Входные данные: документ с требованиями к программному обеспечению.
```

**inputs/error-log.md**
```markdown
Входные данные: лог ошибок, на основе которого нужно найти причину и предложить исправление.
```

**inputs/api-spec.md**
```markdown
Входные данные: спецификация API (например, OpenAPI/Swagger).
```

**inputs/db-schema.md**
```markdown
Входные данные: схема базы данных (таблицы, связи, индексы).
```

**examples/example-code.md**
```markdown
При выполнении задания опирайся на предоставленный ниже пример кода. Старайся следовать его стилю и подходам.
```

**examples/example-output.md**
```markdown
При выполнении задания ориентируйся на предоставленный ниже пример желаемого вывода.
```

**examples/example-usage.md**
```markdown
При написании кода учитывай примеры использования, которые будут приведены.
```

**examples/example-test.md**
```markdown
При написании тестов опирайся на предоставленные примеры тестов.
```

**quality_attributes/performance-critical.md**
```markdown
Требование: производительность критически важна. Код должен быть максимально эффективным, избегать лишних операций.
```

**quality_attributes/high-availability.md**
```markdown
Требование: высокая доступность. Система должна быть устойчива к сбоям и обеспечивать бесперебойную работу.
```

**quality_attributes/secure-by-design.md**
```markdown
Требование: безопасность должна быть встроена в архитектуру. Избегать уязвимостей, следовать принципам secure coding.
```

**quality_attributes/maintainable.md**
```markdown
Требование: поддерживаемость. Код должен быть лёгким для понимания и модификации, с хорошей структурой и комментариями.
```

**quality_attributes/testable.md**
```markdown
Требование: тестируемость. Код должен быть написан так, чтобы его легко было покрыть автоматическими тестами.
```

**quality_attributes/scalable.md**
```markdown
Требование: масштабируемость. Система должна легко расширяться для работы с возросшими нагрузками.
```

**platforms/windows.md**
```markdown
Целевая платформа: Windows.
```

**platforms/linux.md**
```markdown
Целевая платформа: Linux.
```

**platforms/macos.md**
```markdown
Целевая платформа: macOS.
```

**platforms/android.md**
```markdown
Целевая платформа: Android.
```

**platforms/ios.md**
```markdown
Целевая платформа: iOS.
```

**platforms/embedded.md**
```markdown
Целевая платформа: встраиваемые системы (embedded).
```

**audience/developers.md**
```markdown
Целевая аудитория: разработчики. Ответ должен быть технически детальным, с примерами кода и объяснениями.
```

**audience/managers.md**
```markdown
Целевая аудитория: менеджеры. Ответ должен быть на высоком уровне, с фокусом на сроки, ресурсы, риски и бизнес-ценность.
```

**audience/clients.md**
```markdown
Целевая аудитория: клиенты (заказчики). Ответ должен быть понятен неспециалистам, избегать излишней технической сложности, объяснять выгоды.
```

**audience/end-users.md**
```markdown
Целевая аудитория: конечные пользователи. Ответ должен быть ориентирован на использование продукта, инструкции, советы.
```

**audience/testers.md**
```markdown
Целевая аудитория: тестировщики. Ответ должен содержать информацию, полезную для тестирования: сценарии, ожидаемое поведение, граничные случаи.
```