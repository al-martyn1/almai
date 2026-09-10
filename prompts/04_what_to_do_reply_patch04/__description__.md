# Чего не хватает вашей CDT-библиотеке для AI-анализа страниц

Судя по `_test17.cpp`, у вас уже есть фундамент: соединение по WS, подписка на события через `wsEventSubscribe`, `cdtPageNavigate`, `cdtDomGetDocument`, `cdtRuntimeEvaluate`, `cdtGetHtml`. Это уровень «получить сырьё». Для AI-анализа не хватает **четырёх слоёв**: наблюдение, ожидание, действия и нормализация. Плюс отдельный слой «bundle для ИИ».

Ниже — по слоям, с указанием конкретных CDP-методов, которые у вас сейчас не задействованы, и того, что нужно добавить в API.

---

## 1. Слой наблюдения (сбор сырья)

Сейчас вы получаете разовый снимок — `DOM.getDocument`, `outerHTML`, `sessionStorage`. Чтобы анализировать произвольную страницу, нужно **накопить события за время жизни страницы**.

Что добавить:

- `Network.enable`, `Runtime.enable`, `Page.enable`, `DOM.enable`, `Debugger.enable` — у вас включаются, но вы на них почти не реагируете.
- Обработчики на:
  - `Network.requestWillBeSent` — начало запроса (`requestId`, `method`, `url`, `headers`, `postData`).
  - `Network.requestWillBeSentExtraInfo` — реальные заголовки отправителя (куки, авторизация).
  - `Network.responseReceived` — статус, заголовки, тип (`Document`, `Script`, `XHR`, `Fetch`, `Img`, `Stylesheet`, `Font`).
  - `Network.responseReceivedExtraInfo` — заголовки от сервера как есть, включая `Set-Cookie`.
  - `Network.loadingFinished` / `loadingFailed` — конец запроса, размер, ошибки.
  - `Network.webSocketFrameSent` / `webSocketFrameReceived` — если страница держит WS.
  - `Network.dataReceived` — если нужны стримы.
  - `Runtime.consoleAPICalled` — лог в консоль.
  - `Runtime.exceptionThrown` — необработанные исключения.
  - `Log.entryAdded` — deprecated-логи из самого браузера (CSP, mixed content).
  - `Page.frameNavigated`, `Page.frameAttached`, `Page.frameDetached` — навигация по фреймам.
  - `Page.domContentEventFired`, `Page.loadEventFired`, `Page.lifecycleEvent` — фазы жизни документа.
  - `DOM.documentUpdated`, `DOM.childNodeInserted`, `DOM.attributeModified` — если нужны изменения DOM.
  - `Runtime.executionContextCreated` / `executionContextDestroyed` — все контексты (главный, iframe, worker).
  - `Target.attachedToTarget` — iframe / worker / service worker как отдельные сессии.

В API это должно превратиться в **накопители**:

```cpp
class NetworkJournal {
    std::vector<RequestRecord> requests;   // url, method, headers, postData, timings
    std::vector<ResponseRecord> responses; // status, headers, mimeType, size
    std::unordered_map<std::string, std::string> bodies; // requestId -> body
};

class ConsoleJournal {
    std::vector<ConsoleMessage> messages;  // level, text, stack, timestamp
    std::vector<ExceptionRecord> exceptions;
};

class DomJournal {
    std::vector<DomMutation> mutations;
    std::vector<FrameInfo> frames;
    std::vector<ContextInfo> contexts;
};
```

И функция `connection.attachJournals(networkJournal, consoleJournal, domJournal)` — чтобы события автоматически писались в эти структуры, а не просто «приходили и логировались».

---

## 2. Слой ожидания (устранение гонок)

Сейчас у вас есть `cdtPageNavigate` с таймаутом, но нет «подождать, пока вот это появится».

Что добавить:

- `cdtWaitForSelector(selector, timeoutMs)` — polling через `Runtime.evaluate`, возвращает `{found, x, y, width, height, nodeId}`.
- `cdtWaitForSelectorVisible(selector, timeoutMs)` — то же, но с проверкой `getBoundingClientRect().width > 0`.
- `cdtWaitForFunction(jsExpression, timeoutMs)` — ждать, пока JS-выражение вернёт truthy. Для случаев вида `window.__appReady === true`.
- `cdtWaitForNetworkIdle(idleMs, timeoutMs)` — сеть утихла хотя бы на `idleMs`.
- `cdtWaitForNetworkResponse(urlSubstring, timeoutMs)` — конкретный запрос ушёл/пришёл.
- `cdtWaitForNavigation(timeoutMs)` — `Page.frameNavigated` + `Page.loadEventFired`.
- `cdtWaitForExecutionContext(frameId)` — после появления iframe.
- `cdtWaitForEvent(method, predicate, timeoutMs)` — общий комбинатор, на котором всё выше строится.

Реализация — через общий event-loop: `wsDispatchMessages` уже крутится в цикле, надо превратить его в «ожидаемо-событийный» с очередью futures/promises.

---

## 3. Слой действий (чтобы ИИ мог что-то делать, а не только смотреть)

Сейчас действий нет вообще. Без них нельзя проверить гипотезу «нажали — что произошло».

Что добавить (домены `Input`, `Page`, `Emulation`):

- `Input.dispatchMouseEvent` — `mouseMoved`, `mousePressed`, `mouseReleased`, `mouseWheel`. Обёртки:
  - `cdtClick(x, y, button = "left")`
  - `cdtDoubleClick(x, y)`
  - `cdtRightClick(x, y)`
  - `cdtMouseMove(x, y)`
  - `cdtDrag(x1, y1, x2, y2)`
  - `cdtScroll(x, y, dx, dy)`
- `Input.dispatchKeyEvent` — обёртки:
  - `cdtPressKey(key, code, vk, modifiers)`
  - `cdtPressEnter()`, `cdtPressEscape()`, `cdtPressTab()`
  - `cdtPressCtrl(key)`, `cdtPressShift(key)`
- `Input.insertText` — `cdtInsertText(text)` (простая вставка).
- `Input.dispatchKeyEvent` с `type: "char"` — для посимвольного набора, если сайт слушает `keypress`.
- `Page.bringToFront` — перед вводом, если окно фоновое.
- `Page.setInterceptFileChooserDialog`, `DOM.setFileInputFiles` — для загрузки файлов.
- `Emulation.setDeviceMetricsOverride`, `Emulation.setUserAgentOverride` — если надо эмулировать мобильный.
- `Page.captureScreenshot` — скриншот (для отладки и как «глаза» мультимодальной модели).
- `Page.printToPDF` — если нужен PDF-снимок.
- `Page.captureSnapshot` (`mhtml`) — полноценный архив.

Всё это — тонкие обёртки над `wsSendCommand`, но их надо собрать в удобный фасад вида `connection.input().click(x, y)` или `connection.page().screenshot(...)`.

---

## 4. Слой извлечения тел и исходников

`Network.getResponseBody` вы ещё не используете. Это ключ ко всему «вытащить исходники».

Что добавить:

- `cdtGetResponseBody(requestId)` — возвращает `{body, base64Encoded}`.
- `cdtGetScriptSource(scriptId)` — из `Debugger.scriptParsed`.
- `cdtGetSourceMap(scriptId)` — скачать `.map` по URL из `sourceMappingURL`.
- `cdtGetAllResponseBodies(filter)` — для всех записей из `NetworkJournal`, у кого есть `requestId` и тип ресурса.
- `cdtGetSourceMapTree(scriptId)` — распарсить source map, развернуть в исходные файлы.

Важный нюанс: `Network.getResponseBody` работает, только пока `requestId` жив в кэше страницы. То есть надо собирать тела сразу в `Network.loadingFinished`, а не «потом». Значит, `NetworkJournal` должен на каждое `loadingFinished` дёргать `getResponseBody` и класть результат в себя.

---

## 5. Слой учёта контекстов

Произвольная страница почти всегда — это:
- основной документ,
- iframe'ы (иногда `srcdoc`),
- воркеры (dedicated, shared, service),
- worklet'ы.

У каждого — свой `executionContextId` и свой `Runtime`. `Runtime.evaluate` по умолчанию бьёт только в основной контекст. Чтобы дотянуться до iframe или воркера, нужен отдельный `sessionId`.

Что добавить:

- `Target.setAutoAttach({autoAttach:true, waitForDebuggerOnStart:false, flatten:true})` — тогда новые iframe/worker приходят как отдельные сессии.
- Слушать `Target.attachedToTarget` / `Target.detachedFromTarget`.
- Хранить маппинг `frameId -> sessionId`, `targetId -> sessionId`.
- `cdtEvaluateInFrame(frameId, expression)`, `cdtEvaluateInWorker(targetId, expression)`.
- `cdtGetFrameTree()` через `Page.getFrameTree`.
- `cdtGetAllContexts()` — собирает контексты со всех сессий.

Без этого «анализ произвольной страницы» часто будет упираться в «а результат-то не в главном фрейме».

---

## 6. Слой нормализации (то, что вы подаёте ИИ)

Это то, чего у вас сейчас нет совсем. Сырьё собрано — но его надо превратить в «AI bundle» из предыдущего ответа.

Что добавить:

- `cdtBuildAiBundle(outDir, options)` — собирает папку:
  - `summary.json` — url, title, счётчики.
  - `requests.tsv` — компактная таблица запросов.
  - `dom.html` — нормализованный DOM (убрать SVG, base64, служебные атрибуты).
  - `interactive.json` — все интерактивные элементы: `input`, `textarea`, `button`, `a`, `[role]`, `[contenteditable]`, с селектором и координатами.
  - `console.log` — сообщения консоли и исключения.
  - `post-load.tsv` — что ушло после `loadEventFired`.
  - `scripts/` — исходники (по возможности распакованные из source maps).
  - `meta.json` — что удалось, что нет.

- Вспомогательные функции:
  - `cdtExtractInteractiveElements()` — один `Runtime.evaluate` с JS-скриптом, который обходит DOM и выдаёт JSON.
  - `cdtNormalizeDom()` — JS-скрипт чистки.
  - `cdtFormatScriptForHuman(scriptId)` — prettier-подобное форматирование (в C++ это проще всего сделать вызовом внешней утилиты или отправкой в JS-нагрузку на самой странице).
  - `cdtBuildHar()` — собрать HAR 1.2 из `NetworkJournal`.

---

## 7. Слой «диффов»

Для обучения ИИ и для понимания поведения страницы очень полезно сравнивать два состояния:

- `cdtSnapshot()` — снять слепок (DOM + журнал сети с отметкой времени + интерактивные элементы).
- `cdtDiff(snapA, snapB)` — вернуть:
  - новые/удалённые узлы DOM,
  - новые запросы,
  - изменения `sessionStorage`/`localStorage`/куки,
  - сместившиеся интерактивные элементы.

Это то, что превращает «статический анализ» в «анализ поведения».

---

## 8. Чего ещё нет, но пригодится

- **Хранилище состояний**: `DOMStorage`, `Runtime.evaluate` вокруг `localStorage`/`sessionStorage`, `Network.getCookies`, `Network.setCookie`, `Network.getAllCookies`.
- **Разрешения**: `Browser.grantPermissions` — иначе многие API (geolocation, notifications) не заработают.
- **Отключение кэша** при сборе сырья: `Network.setCacheDisabled(true)`.
- **User-Agent и locale**: `Emulation.setUserAgentOverride`, `Emulation.setLocaleOverride`.
- **Управление временем**: `Emulation.setVirtualTimePolicy` — иногда нужно, чтобы страница «прокрутилась».
- **Мониторинг долгих задач**: `Performance.enable` + `Performance.getMetrics`.
- **Доменные события покрытия**: `Profiler.startPreciseCoverage` — узнать, какой JS реально исполнялся.

---

## 9. Чего не хватает вне библиотеки (важно понимать)

Даже с полным CDT-API «анализировать произвольные странички с ИИ» требует ещё трёх вещей, которые библиотека не решает:

1. **Модель не читает HAR**. Нужен слой нормализации — это ваш `cdtBuildAiBundle`.
2. **Модель не хранит контекст между вызовами**. Нужен внешний слой, который подаёт только нужные части bundle на каждый вопрос.
3. **Модель не «кликает» сама**. Нужен агентный цикл: `наблюдай → реши → подействуй → снова наблюдай`. Библиотека должна давать этому циклу примитивы (ждать, кликать, вводить, снимать слепок), но сам цикл — уже ваша логика поверх библиотеки.

---

## 10. Минимальный MVP, чтобы «оно заработало»

Если брать по-минимуму, чтобы впервые получить осмысленный AI-анализ произвольной страницы:

1. `NetworkJournal` — собирает запросы и тела через `Network.*`.
2. `ConsoleJournal` — собирает `Runtime.consoleAPICalled` и `Runtime.exceptionThrown`.
3. `cdtWaitForSelector` + `cdtWaitForFunction` + `cdtWaitForNetworkIdle`.
4. `cdtClick`, `cdtInsertText`, `cdtPressKey`.
5. `cdtExtractInteractiveElements` — один JS-вызов.
6. `cdtBuildAiBundle` — папка с `summary.json`, `requests.tsv`, `dom.html`, `interactive.json`, `console.log`, `scripts/`.
7. `cdtSnapshot` + `cdtDiff` — опционально, но сильно повышает качество.

Этого хватит, чтобы:
- запустить Chrome,
- открыть URL,
- подождать загрузки,
- собрать сеть + DOM + консоль,
- сложить в папку,
- отдать ИИ,
- получить ответ вида «чтобы ввести запрос, кликни по `textarea#text`, вставь текст, нажми Enter»,
- и **проверить** это, реально кликнув через `Input.*`.

Дальше добавляете iframe'ы, воркеры, source maps, HAR, скриншоты — по мере необходимости.

---

## 11. Итог

Вашей библиотеке не хватает:

- **накопителей событий** (журналы сети, консоли, DOM, фреймов);
- **примитивов ожидания** (selector, network idle, function, navigation);
- **примитивов действия** (мышь, клавиатура, скролл, скриншот);
- **извлечения тел и скриптов** (`getResponseBody`, `getScriptSource`, source maps);
- **учёта контекстов** (Target/iframe/worker);
- **слоя нормализации** в AI bundle.
