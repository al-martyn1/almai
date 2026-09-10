- [Подробный разбор по слоям](#подробный-разбор-по-слоям)
- [1. Слой наблюдения (журналы событий)](#1-слой-наблюдения-журналы-событий)
  - [1.1. Что включать на старте](#11-что-включать-на-старте)
  - [1.2. События `Network.*` — что хранить](#12-события--network---что-хранить)
  - [1.3. События `Runtime.*`](#13-события--runtime-)
  - [1.4. События `Page.*` и `DOM.*`](#14-события--page--и--dom-)
  - [1.5. События `Target.*` и `Debugger.*`](#15-события--target--и--debugger-)
  - [1.6. C++ API](#16-c-api)
  - [1.7. Подводные камни](#17-подводные-камни)
- [2. Слой ожидания](#2-слой-ожидания)
  - [2.1. Базовая идея](#21-базовая-идея)
  - [2.2. `cdtWaitForSelector`](#22--cdtwaitforselector-)
  - [2.3. `cdtWaitForFunction`](#23--cdtwaitforfunction-)
  - [2.4. `cdtWaitForNetworkIdle`](#24--cdtwaitfornetworkidle-)
  - [2.5. `cdtWaitForNetworkResponse`](#25--cdtwaitfornetworkresponse-)
  - [2.6. `cdtWaitForNavigation`](#26--cdtwaitfornavigation-)
  - [2.7. `cdtWaitForExecutionContext`](#27--cdtwaitforexecutioncontext-)
  - [2.8. Общий `EventLoop` внутри Connection](#28-общий--eventloop--внутри-connection)
  - [2.9. Подводные камни](#29-подводные-камни)
- [3. Слой действий](#3-слой-действий)
  - [3.1. Мышь](#31-мышь)
  - [3.2. Клавиатура](#32-клавиатура)
  - [3.3. Работа со скроллом внутри страницы](#33-работа-со-скроллом-внутри-страницы)
  - [3.4. Файлы, разрешения, эмуляция](#34-файлы-разрешения-эмуляция)
  - [3.5. Скриншоты](#35-скриншоты)
  - [3.6. PDF](#36-pdf)
  - [3.7. Прочее](#37-прочее)
  - [3.8. Подводные камни](#38-подводные-камни)
- [4. Слой извлечения тел и исходников](#4-слой-извлечения-тел-и-исходников)
  - [4.1. `Network.getResponseBody`](#41--networkgetresponsebody-)
  - [4.2. `Debugger.getScriptSource`](#42--debuggergetscriptsource-)
  - [4.3. Source maps](#43-source-maps)
  - [4.4. C++ API](#44-c-api)
  - [4.5. Красивое форматирование JS](#45-красивое-форматирование-js)
  - [4.6. Подводные камни](#46-подводные-камни)
- [5. Слой учёта контекстов](#5-слой-учёта-контекстов)
  - [5.1. Как CDP видит контексты](#51-как-cdp-видит-контексты)
  - [5.2. Структура сессий](#52-структура-сессий)
  - [5.3. Как связать frame → context → session](#53-как-связать-frame--context--session)
  - [5.4. Работа с сессией](#54-работа-с-сессией)
  - [5.5. GetFrameTree для полного обхода](#55-getframetree-для-полного-обхода)
  - [5.6. Оценка «сложных» контекстов](#56-оценка-сложных-контекстов)
  - [5.7. Подводные камни](#57-подводные-камни)
- [6. Слой нормализации (AI bundle)](#6-слой-нормализации-ai-bundle)
  - [6.1. Структура папки](#61-структура-папки)
  - [6.2. `summary.json`](#62--summaryjson-)
  - [6.3. `requests.tsv`](#63--requeststsv-)
  - [6.4. `dom.html`](#64--domhtml-)
  - [6.5. `interactive.json`](#65--interactivejson-)
  - [6.6. `console.log`](#66--consolelog-)
  - [6.7. `network/post-load.tsv`](#67--networkpost-loadtsv-)
  - [6.8. `scripts/index.json`](#68--scriptsindexjson-)
  - [6.9. Восстановление исходников](#69-восстановление-исходников)
  - [6.10. Скриншоты (опционально)](#610-скриншоты-опционально)
  - [6.11. Хранилище](#611-хранилище)
  - [6.12. Метаданные](#612-метаданные)
  - [6.13. Подводные камни](#613-подводные-камни)
- [7. Слой диффов](#7-слой-диффов)
  - [7.1. Что снимается в слепок](#71-что-снимается-в-слепок)
  - [7.2. `cdtSnapshot`](#72--cdtsnapshot-)
  - [7.3. `cdtDiff`](#73--cdtdiff-)
  - [7.4. Типичный сценарий](#74-типичный-сценарий)
  - [7.5. Подводные камни](#75-подводные-камни)
- [8. Дополнительные полезные вещи](#8-дополнительные-полезные-вещи)
  - [8.1. Performance](#81-performance)
  - [8.2. Coverage](#82-coverage)
  - [8.3. Breaking points на XHR/fetch](#83-breaking-points-на-xhrfetch)
  - [8.4. `Page.addScriptToEvaluateOnNewDocument`](#84--pageaddscripttoevaluateonnewdocument-)
  - [8.5. `Page.setBypassCSP`](#85--pagesetbypasscsp-)
  - [8.6. `Network.setBlockedURLs`](#86--networksetblockedurls-)
  - [8.7. Виртуальное время](#87-виртуальное-время)
  - [8.8. `Runtime.addBinding`](#88--runtimeaddbinding-)
- [9. Порядок реализации](#9-порядок-реализации)
- [10. Что осталось за кадром](#10-что-осталось-за-кадром)

# Подробный разбор по слоям

Ниже — по каждому пункту: какие CDP-методы и события задействованы, как выглядят структуры данных, как это собирается в C++-API, какие подводные камни и в каком порядке реализовывать.

---

# 1. Слой наблюдения (журналы событий)

Цель: превратить поток WebSocket-сообщений в структурированные накопители, по которым потом строить bundle.

## 1.1. Что включать на старте

Перед `Page.navigate` надо отправить серию `*.enable`:

```
Network.enable          { maxTotalBufferSize, maxResourceBufferSize, maxPostDataSize }
Page.enable             {}
Runtime.enable          {}
DOM.enable              {}
Debugger.enable         { maxScriptsCacheSize }
Log.enable              {}
Performance.enable      {}
Target.setAutoAttach    { autoAttach:true, waitForDebuggerOnStart:false, flatten:true }
Network.setCacheDisabled { cacheDisabled:true }   // если нужна честная сеть
```

- `Network.enable` — параметры буферов задают, сколько байт на запрос/сессию браузер согласен держать в памяти для последующего `getResponseBody`. Если поставить 0 — он будет стримить без хранения, и тела не достанутся. Разумно: `maxTotalBufferSize = 100 MB`, `maxResourceBufferSize = 10 MB`, `maxPostDataSize = 1 MB`.
- `Page.enable` — нужен для `Page.loadEventFired`, `Page.frameNavigated`, `Page.lifecycleEvent`, `Page.captureSnapshot`.
- `Runtime.enable` — нужен для `Runtime.consoleAPICalled`, `Runtime.exceptionThrown`, `Runtime.executionContextCreated`, `Runtime.bindingCalled`, `Runtime.compileScript`.
- `DOM.enable` — нужен для `DOM.documentUpdated`, `DOM.childNodeInserted`, `DOM.attributeModified`, `DOM.setChildNodes`.
- `Debugger.enable` — нужен для `Debugger.scriptParsed` (там `scriptId`, `url`, `sourceMapURL`), `Debugger.paused`, `Debugger.getScriptSource`.
- `Log.enable` — старые логи браузера (CSP, mixed content, деприкейшены). Дополняет консоль.
- `Performance.enable` — даёт `Performance.metrics` (JS heap, DOM nodes count, layout count).
- `Target.setAutoAttach` — ловит новые iframe/worker как отдельные сессии.

## 1.2. События `Network.*` — что хранить

**`Network.requestWillBeSent`** — начало запроса:
```json
{
  "requestId": "100001.2",
  "loaderId": "A1B2...",
  "documentURL": "https://ya.ru/",
  "request": {
    "url": "https://yastatic.net/s3/.../app.js",
    "method": "GET",
    "headers": { "Accept": "*/*", "Referer": "https://ya.ru/" },
    "postData": "{\"q\":\"кот\"}",
    "hasPostData": true,
    "mixedContentType": "none",
    "initialPriority": "High",
    "referrerPolicy": "origin"
  },
  "timestamp": 12345.678,
  "wallTime": 1757500000.123,
  "initiator": { "type": "script", "stack": { "callFrames": [...] } },
  "redirectResponse": null,
  "type": "Script",
  "frameId": "FRAME123"
}
```

Хранить:
- `requestId`, `frameId`, `documentURL`, `timestamp`, `wallTime`, `type`;
- метод, URL, заголовки, `postData` (может быть пустым при `hasPostData: false`);
- `initiator.type` и `initiator.stack` — по стеку видно, какой скрипт запустил запрос. Полезно для bundle.

**`Network.requestWillBeSentExtraInfo`** — реальные заголовки отправителя (после того, как браузер добавил куки, авторизацию, `User-Agent`):
```json
{
  "requestId": "100001.2",
  "headers": { "Cookie": "...", "Authorization": "..." },
  "associatedCookies": [...]
}
```

Полезно, потому что в `requestWillBeSent` куки не приходят — они навешиваются позже.

**`Network.responseReceived`** — ответ пришёл:
```json
{
  "requestId": "100001.2",
  "loaderId": "A1B2...",
  "timestamp": 12345.700,
  "type": "Script",
  "response": {
    "url": "https://yastatic.net/.../app.js",
    "status": 200,
    "statusText": "OK",
    "headers": { "content-type": "application/javascript", "cache-control": "..." },
    "mimeType": "application/javascript",
    "connectionReused": true,
    "connectionId": 17,
    "encodedDataLength": 123456,
    "fromDiskCache": false,
    "fromServiceWorker": false,
    "fromPrefetchCache": false,
    "timing": { "requestTime": ..., "sendStart": ..., "receiveHeadersEnd": ... },
    "protocol": "h2",
    "securityState": "secure",
    "remoteIPAddress": "5.255.255.55",
    "remotePort": 443
  },
  "hasExtraInfo": true,
  "frameId": "FRAME123"
}
```

**`Network.responseReceivedExtraInfo`** — заголовки как их реально отдал сервер, включая `Set-Cookie`, до всякой нормализации:
```json
{
  "requestId": "100001.2",
  "blockedCookies": [],
  "headers": { "set-cookie": "...", "content-encoding": "gzip" },
  "resourceIPAddressSpace": "Public",
  "statusCode": 200
}
```

**`Network.dataReceived`** — пришли байты:
```json
{
  "requestId": "100001.2",
  "timestamp": ...,
  "dataLength": 16384,
  "encodedDataLength": 16384
}
```

Нужен только если интересно видеть динамику прихода (например, стриминг ответа). Для bundle обычно не обязателен.

**`Network.loadingFinished`** — запрос завершён:
```json
{
  "requestId": "100001.2",
  "timestamp": ...,
  "encodedDataLength": 123456
}
```

Это **правильный момент**, чтобы запросить `Network.getResponseBody`.

**`Network.loadingFailed`** — ошибка:
```json
{
  "requestId": "...",
  "timestamp": ...,
  "type": "Fetch",
  "errorText": "net::ERR_ABORTED",
  "canceled": true,
  "blockedReason": "csp"
}
```

**`Network.webSocketWillSendHandshakeRequest` / `webSocketHandshakeResponseReceived` / `webSocketCreated` / `webSocketClosed` / `webSocketFrameSent` / `webSocketFrameReceived` / `webSocketFrameError`** — если страница держит WS. Для AI-анализа редко нужно, но если анализируем чаты/торговые терминалы — критично.

## 1.3. События `Runtime.*`

**`Runtime.consoleAPICalled`**:
```json
{
  "type": "log",
  "args": [{ "type": "string", "value": "hello" }, { "type": "object", "objectId": "..." }],
  "executionContextId": 3,
  "timestamp": 1757500000123.4,
  "stackTrace": { "callFrames": [...] },
  "context": "https://ya.ru/"
}
```

Хранить: `type`, `args` (сериализованные значения), `timestamp`, `executionContextId`, `stackTrace` (первые 3-5 кадров). Если arg — `object`, то у него только `objectId`; чтобы прочитать — `Runtime.getProperties({objectId})`. Обычно достаточно только скаляров; объекты вытягивать лениво.

**`Runtime.exceptionThrown`**:
```json
{
  "timestamp": ...,
  "exceptionDetails": {
    "exceptionId": 1,
    "text": "Uncaught TypeError",
    "lineNumber": 42,
    "columnNumber": 17,
    "scriptId": "33",
    "url": "https://ya.ru/static/app.js",
    "stackTrace": { "callFrames": [...] },
    "exception": { "type": "object", "className": "TypeError", "description": "...", "objectId": "..." },
    "executionContextId": 3
  }
}
```

**`Runtime.executionContextCreated`**:
```json
{
  "context": {
    "id": 3,
    "origin": "https://ya.ru",
    "name": "",
    "uniqueId": "12345.3",
    "auxData": { "isDefault": true, "type": "default", "frameId": "FRAME123" }
  }
}
```

Хранить маппинг `contextId -> frameId` и `frameId -> contextId`. `auxData.type` бывает `default`, `isolated`, `worker`.

**`Runtime.executionContextDestroyed`**, **`Runtime.executionContextsCleared`** — чистить маппинги.

**`Runtime.bindingCalled`** — если вы сами добавляли `Runtime.addBinding`. В базовом сценарии не нужен.

**`Runtime.consoleAPICalled` `context`** — URL контекста, полезно для группировки.

## 1.4. События `Page.*` и `DOM.*`

**`Page.frameAttached`** / **`frameNavigated`** / **`frameDetached`** — дерево фреймов. Хранить `frameId`, `parentFrameId`, `url`, `name`, `securityOrigin`.

**`Page.lifecycleEvent`** — фазы: `init`, `DOMContentLoaded`, `load`, `networkAlmostIdle`, `networkIdle`, `firstPaint`, `firstContentfulPaint`, `firstMeaningfulPaint` (deprecated), `DOMContentLoaded`, `commit`, `load`. Самое полезное — `networkIdle`.

**`Page.domContentEventFired`** / **`Page.loadEventFired`** — старые, дублируют части lifecycle.

**`Page.javascriptDialogOpening`** — `alert`/`confirm`/`prompt`. Надо обрабатывать, иначе страница зависнет.

**`Page.fileChooserOpened`** — если нужна загрузка файла.

**`DOM.documentUpdated`** — новый документ. Сбросить кэш `nodeId`.

**`DOM.childNodeInserted`** / **`childNodeRemoved`** / **`attributeModified`** / **`attributeRemoved`** / **`characterDataModified`** — мутации. Поток огромный. Если нужно, фильтровать по `nodeName` или включать только на короткие окна.

**`DOM.setChildNodes`** — приходит, когда `DOM.getDocument(depth)` не смог вернуть всё сразу; надо склеивать.

## 1.5. События `Target.*` и `Debugger.*`

**`Target.attachedToTarget`**:
```json
{
  "sessionId": "S1",
  "targetInfo": {
    "targetId": "T1",
    "type": "iframe",
    "title": "",
    "url": "https://widget.example/",
    "attached": true,
    "browserContextId": "..."
  },
  "waitingForDebugger": false
}
```

Каждому `sessionId` соответствует «отдельная» сессия WS. Все команды для этой сессии идут с полем `sessionId`.

**`Target.detachedFromTarget`** — закрылась сессия.

**`Debugger.scriptParsed`**:
```json
{
  "scriptId": "33",
  "url": "https://yastatic.net/.../app.js",
  "startLine": 0,
  "startColumn": 0,
  "endLine": 10000,
  "endColumn": 5,
  "executionContextId": 3,
  "hash": "...",
  "sourceMapURL": "app.js.map",
  "hasSourceURL": false,
  "isModule": true,
  "length": 1234567
}
```

**Тут самое важное — `scriptId` + `url` + `sourceMapURL`.** Это ключ ко всем исходникам.

**`Debugger.paused`** — если ставите breakpoint. Обычно не нужно.

**`Debugger.scriptFailedToParse`** — синтаксическая ошибка.

## 1.6. C++ API

```cpp
struct RequestRecord {
    std::string requestId;
    std::string frameId;
    std::string url;
    std::string method;
    std::map<std::string, std::string> headers;
    std::string postData;           // может быть пустым
    std::string type;               // Document / Script / XHR / Fetch / ...
    double      timestamp = 0.0;    // monotonic
    double      wallTime  = 0.0;    // unix
    std::string initiatorType;      // parser / script / preload / other
    std::string initiatorUrl;       // из stackTrace[0].url
    int         initiatorLine = -1;
};

struct ResponseRecord {
    std::string requestId;
    int         status = 0;
    std::string statusText;
    std::string mimeType;
    std::string protocol;           // "h2" / "http/1.1"
    std::map<std::string, std::string> headers;   // из responseReceivedExtraInfo
    std::int64_t encodedDataLength = 0;
    bool        fromCache       = false;
    bool        fromServiceWorker = false;
    std::string remoteIPAddress;
    int         remotePort = 0;
    double      requestTime = 0.0;
    double      sendStart = 0.0;
    double      receiveHeadersEnd = 0.0;
    std::string remoteUrl;          // может отличаться после редиректа
};

struct BodyRecord {
    std::string requestId;
    std::string body;
    bool        base64 = false;
    bool        truncated = false;
};

struct NetworkJournal {
    std::unordered_map<std::string, RequestRecord> requests;
    std::unordered_map<std::string, ResponseRecord> responses;
    std::unordered_map<std::string, BodyRecord> bodies;
    std::vector<std::string> order;   // порядок requestId по timestamp
    std::mutex mtx;

    void onRequestWillBeSent(const json& params);
    void onResponseReceived(const json& params);
    void onLoadingFinished(const marty::cdt::Connection* con,
                           const json& params);   // тут дёргает getResponseBody
};

struct ConsoleMessage {
    std::string level;    // log / warn / error / info / debug
    std::vector<std::string> textArgs;
    double timestamp = 0.0;
    std::string url;
    int line = -1;
    std::vector<std::string> stackTop;   // 3-5 верхних кадров
};

struct ExceptionRecord {
    std::string text;
    std::string className;
    std::string description;
    std::string url;
    int line = -1;
    int column = -1;
    std::vector<std::string> stackTop;
    double timestamp = 0.0;
};

struct ConsoleJournal {
    std::vector<ConsoleMessage> messages;
    std::vector<ExceptionRecord> exceptions;
    std::mutex mtx;
};

struct FrameInfo {
    std::string frameId;
    std::string parentFrameId;
    std::string url;
    std::string name;
    std::string securityOrigin;
    bool        isMain = false;
};

struct DomJournal {
    std::unordered_map<std::string, FrameInfo> frames;
    std::unordered_map<std::string, int> contextToFrame;
    std::unordered_map<std::string, std::string> frameToContext;
    std::mutex mtx;
};
```

Подключение:

```cpp
connection.setNetworkJournal(&networkJournal);
connection.setConsoleJournal(&consoleJournal);
connection.setDomJournal(&domJournal);
```

Внутри вашего `GenericMethodHandler` (который уже вызывается на все события) делаете диспетчеризацию по `method`:

```cpp
if (method == "Network.requestWillBeSent")
    networkJournal.onRequestWillBeSent(j);
else if (method == "Network.responseReceived")
    networkJournal.onResponseReceived(j);
else if (method == "Network.loadingFinished")
    networkJournal.onLoadingFinished(con, j);
else if (method == "Runtime.consoleAPICalled")
    consoleJournal.onConsoleApi(j);
// ...
```

## 1.7. Подводные камни

- **Порядок событий не гарантирован между доменами.** `Network.responseReceived` может прийти раньше `Runtime.consoleAPICalled` для одного и того же действия. Для хронологии используйте `timestamp`.
- **`requestWillBeSent` может прийти несколько раз для одного `requestId`** — при редиректах. В `redirectResponse` будет предыдущий ответ. Различайте по `loaderId`.
- **`responseReceivedExtraInfo` приходит асинхронно** — иногда после `loadingFinished`. Если нужны точные заголовки от сервера, придётся подождать.
- **`Network.getResponseBody` не работает, если сервер отдал ответ с `content-encoding` в байтах, а `maxResourceBufferSize` маленький.** Увеличивайте буфер.
- **Событий очень много.** На тяжёлой странице — десятки тысяч за минуту. Храните в `unordered_map` по `requestId`, а не в векторе.
- **`Runtime.consoleAPICalled` args** — по умолчанию там `objectId` без значения. Чтобы не делать `getProperties` на каждый чих, храните только примитивы, а объекты — как `"<object: {...}>"` с типом.
- **`Debugger.enable` замедляет страницу**, особенно с `maxScriptsCacheSize`. Для сбора исходников без отладки — норм. Для продакшн-скорости — включайте временно.

---

# 2. Слой ожидания

Цель: примитивы вида «жди, пока выполнится условие», поверх которых пишутся сценарии.

## 2.1. Базовая идея

Общий комбинатор:

```cpp
using EventPredicate = std::function<bool(const marty::cdt::WebSocketMessage&, const json&)>;

std::future<json> wsWaitForEvent(std::string method, EventPredicate pred, int timeoutMs);
```

Внутри:
- регистрируем обработчик на `method`;
- заводим `std::promise<json>`;
- по таймауту — reject с ошибкой;
- при приходе события — если `pred(params)` вернул true, resolve.

Все остальные функции — надстройки.

## 2.2. `cdtWaitForSelector`

```cpp
struct ElementRect {
    bool found = false;
    double x = 0, y = 0, width = 0, height = 0;
    int nodeId = 0;
    std::string tagName;
};

ElementRect cdtWaitForSelector(
    const std::string& selector,
    int timeoutMs = 30000,
    const std::string& frameId = "",
    bool requireVisible = false);
```

Реализация: polling через `Runtime.evaluate` каждые 100 мс. JS:

```js
(() => {
  const el = document.querySelector(ARG_SELECTOR);
  if (!el) return null;
  if (ARG_VISIBLE && (el.offsetWidth === 0 || el.offsetHeight === 0)) return null;
  el.scrollIntoView({block: 'center', inline: 'center', behavior: 'instant'});
  const r = el.getBoundingClientRect();
  if (r.width <= 0 || r.height <= 0) return null;
  return {
    x: r.left + r.width / 2,
    y: r.top + r.height / 2,
    width: r.width,
    height: r.height,
    tagName: el.tagName.toLowerCase()
  };
})()
```

`Runtime.evaluate` с `returnByValue: true, awaitPromise: false`. Если вернулось `null` — спим и повторяем. Иначе — возвращаем.

Почему polling, а не `DOM.childNodeInserted`: событий слишком много, фильтровать сложно, а на динамических сайтах селектор может пересоздаваться. Polling каждые 100 мс — дёшево.

Для точного «появился/исчез» есть `DOM.getDocument` + `DOM.querySelector` — но это не работает для элементов в shadow DOM, поэтому JS-путь предпочтительнее.

## 2.3. `cdtWaitForFunction`

```cpp
json cdtWaitForFunction(const std::string& expression, int timeoutMs = 30000);
```

JS-обёртка:

```js
(async () => {
  const deadline = Date.now() + ARG_TIMEOUT;
  while (Date.now() < deadline) {
    const v = (0, eval)(ARG_EXPR);
    if (v) return v;
    await new Promise(r => setTimeout(r, 100));
  }
  return null;
})()
```

`Runtime.evaluate` с `awaitPromise: true, returnByValue: true`.

Полезно для флагов вроде `window.__APP_READY__`, `document.readyState === 'complete'`, `!!window.React`, и т.п.

## 2.4. `cdtWaitForNetworkIdle`

```cpp
void cdtWaitForNetworkIdle(int idleMs = 500, int timeoutMs = 30000);
```

Идея: держим счётчик «inflight». `requestWillBeSent` +1, `loadingFinished`/`loadingFailed` −1. Когда счётчик == 0 в течение `idleMs` — выходим.

Тонкость: `loadingFinished` для `XHR`/`Fetch` приходит не всегда вовремя, а иногда вообще не приходит для закрытых соединений. Считайте, что «inflight» — это `requestId`, у которого есть `requestWillBeSent`, но нет ни `loadingFinished`, ни `loadingFailed`.

Дополнительно можно исключать долгоживущие `EventSource`/`WebSocket` — они никогда не «finished».

## 2.5. `cdtWaitForNetworkResponse`

```cpp
ResponseRecord cdtWaitForNetworkResponse(
    std::function<bool(const RequestRecord&)> filter,
    int timeoutMs = 30000);
```

Проверяем сначала журнал (может уже пришёл), потом ждём новых событий.

Типичные использования:
- ждать ответ от `/api/search` — `url.find("/api/search") != npos`;
- ждать редирект на `/results` — `status >= 300 && status < 400 && location.find("/results")`.

## 2.6. `cdtWaitForNavigation`

```cpp
void cdtWaitForNavigation(int timeoutMs = 30000, bool waitForLoad = true);
```

Слушает `Page.frameNavigated` (у которого `frame.parentId.empty()` — main frame) и, если `waitForLoad`, ещё `Page.loadEventFired`.

Полезно, когда после клика страница сама уходит на другой URL. Но часто надёжнее — ждать конкретный селектор на новой странице.

## 2.7. `cdtWaitForExecutionContext`

```cpp
int cdtWaitForExecutionContext(const std::string& frameId, int timeoutMs = 10000);
```

Ждёт `Runtime.executionContextCreated` с `auxData.frameId == frameId`. Возвращает `contextId`.

Для iframe, которые создаются после загрузки главного документа.

## 2.8. Общий `EventLoop` внутри Connection

Сейчас у вас `wsDispatchMessages()` вызывается в цикле, а обработчики синхронные. Чтобы `wsWaitForEvent` работал, надо:

- хранить `std::vector<PendingWait>` — каждая с `method`, `pred`, `promise`, `deadline`;
- в `wsDispatchMessages` после разбора сообщения пробегать по всем `PendingWait` и, если метод совпал и `pred` true, резолвить;
- раз в N мс (или при каждом вызове) проверять `deadline` и отклонять просроченные.

Тогда сценарий выглядит так:

```cpp
auto rect = wsConnection->cdtWaitForSelector("textarea#text", 30000);
if (!rect.found) throw std::runtime_error("no textarea");
wsConnection->cdtClick(rect.x, rect.y);
wsConnection->cdtInsertText("котики");
wsConnection->cdtPressKey("Enter", "Enter", 13, 0, "\r");
wsConnection->cdtWaitForNetworkResponse(
    [](const RequestRecord& r){ return r.url.find("/search") != std::string::npos; }, 30000);
```

## 2.9. Подводные камни

- **`Runtime.evaluate` в polling нельзя делать слишком часто.** Каждые 50 мс на тяжёлой странице — уже заметная нагрузка. 100–150 мс — разумно.
- **`Runtime.evaluate` возвращает `undefined` для несериализуемых значений.** Для объектов нужен `returnByValue: true`; для функций и DOM-узлов — `objectId`.
- **`awaitPromise` блокирует до завершения промиса**, но не дольше таймаута самого `Runtime.evaluate`. Ставьте `timeout` в вызове метода, иначе зависнет.
- **В iframe `Runtime.evaluate` идёт не туда** — нужен `contextId` из `Runtime.executionContextCreated`. Либо использовать `Page.createIsolatedWorld` + `Runtime.evaluate({contextId})`.
- **`Page.loadEventFired` не срабатывает для SPA** — там нет полной навигации. Ждите селектор.
- **`Network.loadingFinished` не приходит для SSE/WS** — они вечные. Исключайте их из idle-подсчёта.
- **Поллинг селектора может найти элемент, который тут же исчезнет.** После нахождения сделайте ещё `elementFromPoint(x, y)` в JS, чтобы убедиться, что клик попадёт по нужному элементу.

---

# 3. Слой действий

Цель: тонкие обёртки над `Input.*`, `Page.*`, `Emulation.*`.

## 3.1. Мышь

`Input.dispatchMouseEvent`:

```json
{
  "type": "mousePressed",   // mouseMoved / mousePressed / mouseReleased / mouseWheel
  "x": 100,
  "y": 200,
  "modifiers": 0,           // Alt=1, Ctrl=2, Meta=4, Shift=8
  "timestamp": 12345.678,   // секунды
  "button": "left",         // none / left / middle / right / back / forward
  "buttons": 1,             // битовая маска нажатых
  "clickCount": 1,
  "force": 0.5,             // для touch
  "tangentialPressure": 0,
  "tiltX": 0, "tiltY": 0,
  "pointerType": "mouse"    // mouse / pen
}
```

- `x`,`y` — в CSS-пикселях относительно viewport.
- `timestamp` — в секундах с плавающей точкой; можно не задавать, браузер подставит текущее.
- `buttons` — накопительно: при `mousePressed left` — `1`; при движении с зажатой левой — `1`; при отпускании — `0`.
- Для `mouseMoved` без нажатой кнопки — `button: "none"`, `buttons: 0`.

Обёртки:

```cpp
void cdtMouseMove(double x, double y, int modifiers = 0);
void cdtClick(double x, double y, const std::string& button = "left", int clickCount = 1, int modifiers = 0);
void cdtDoubleClick(double x, double y, int modifiers = 0);
void cdtRightClick(double x, double y);
void cdtMouseDown(double x, double y, const std::string& button = "left", int modifiers = 0);
void cdtMouseUp(double x, double y, const std::string& button = "left", int modifiers = 0);
void cdtScroll(double x, double y, double deltaX, double deltaY);
void cdtDrag(double x1, double y1, double x2, double y2, int steps = 10, const std::string& button = "left");
```

`cdtClick` реализуется как:
```
mouseMoved   (x, y)
mousePressed (x, y, button=left, buttons=1, clickCount=1)
mouseReleased(x, y, button=left, buttons=0, clickCount=1)
```

`cdtDoubleClick` — две пары press/release с `clickCount=1` и `clickCount=2`, с интервалом ~50 мс.

`cdtScroll` — `mouseWheel`:
```json
{
  "type": "mouseWheel",
  "x": 100, "y": 200,
  "deltaX": 0, "deltaY": 300,
  "modifiers": 0
}
```
Обратите внимание: `deltaY` положительный — вниз, отрицательный — вверх. Плюс часто нужно предварительно `mouseMoved` в точку скролла, иначе событие уйдёт не тому элементу.

`cdtDrag`:
```
mouseMoved   (x1, y1)
mousePressed (x1, y1, buttons=1, clickCount=1)
mouseMoved   (x1 + dx*i/steps, y1 + dy*i/steps, buttons=1)  для i=1..steps-1
mouseMoved   (x2, y2, buttons=1)
mouseReleased(x2, y2, buttons=0, clickCount=1)
```
Интервал между движениями — 10-20 мс; некоторые drag-n-drop библиотеки требуют минимум 3-5 промежуточных точек.

## 3.2. Клавиатура

`Input.dispatchKeyEvent`:

```json
{
  "type": "keyDown",          // keyDown / keyUp / rawKeyDown / char
  "modifiers": 0,
  "timestamp": ...,
  "text": "a",                // только для char / keyDown с текстом
  "unmodifiedText": "a",
  "key": "a",                 // "Enter", "Tab", "Escape", "a", "A", "ArrowLeft", ...
  "code": "KeyA",             // US-код физической клавиши
  "windowsVirtualKeyCode": 65,
  "nativeVirtualKeyCode": 65,
  "autoRepeat": false,
  "isKeypad": false,
  "isSystemKey": false,
  "location": 0,              // 0=standard, 1=left, 2=right, 3=numpad
  "commands": []              // для macOS
}
```

Правила:
- **`keyDown`** — с `text` (для печатных клавиш) или без (для функциональных).
- **`rawKeyDown`** — без `text`, часто нужен для предотвращения генерации текста.
- **`char`** — генерирует только текстовый ввод, без keydown/keyup. Для больших текстов использовать `Input.insertText`.
- **`keyUp`** — завершение.

Обёртки:

```cpp
void cdtPressKey(const std::string& key, const std::string& code, int vk, int modifiers = 0, const std::string& text = "");
void cdtInsertText(const std::string& text);
void cdtTypeText(const std::string& text, int delayMs = 0);   // посимвольно
void cdtPressEnter();
void cdtPressTab();
void cdtPressEscape();
void cdtPressBackspace();
void cdtPressArrow(const std::string& dir, int count = 1);    // Up/Down/Left/Right
void cdtPressCtrl(const std::string& key, const std::string& code, int vk);
void cdtPressShift(const std::string& key, const std::string& code, int vk);
```

`cdtInsertText`:
```json
{ "method": "Input.insertText", "params": { "text": "привет" } }
```
Работает мгновенно, но не генерирует `keydown`/`keyup`. Хорошо для простых форм, плохо для редакторов, следящих за событиями.

`cdtTypeText` — посимвольно через `dispatchKeyEvent` с `type:"char"` или через пары `keyDown`/`keyUp`:
```
для каждого char c:
  keyDown с key=c, code=..., vk=..., text=c
  keyUp
  sleep(delayMs)
```

`vk` для букв: `'A'..'Z'` = `65..90`, `'a'..'z'` = `65..90` (по факту code = upper), `'0'..'9'` = `48..57`.
Спец: `Enter=13`, `Tab=9`, `Escape=27`, `Backspace=8`, `Delete=46`, `Space=32`, `Arrow*=37..40`, `Home=36`, `End=35`, `PageUp=33`, `PageDown=34`.

`cdtPressCtrl` — сначала `keyDown` модификатора (`Control`), потом самой клавиши с `modifiers=2`, потом `keyUp` клавиши, потом `keyUp` модификатора.

## 3.3. Работа со скроллом внутри страницы

Помимо `mouseWheel`, часто нужно прокрутить к элементу. Проще через JS:

```js
document.querySelector(SEL).scrollIntoView({block:'center', inline:'center'});
```

В `cdtWaitForSelector` это уже делается. Отдельно:

```cpp
void cdtScrollIntoView(const std::string& selector);
void cdtScrollToDocumentEnd();   // window.scrollTo(0, document.body.scrollHeight)
void cdtScrollBy(double dx, double dy);
```

`cdtScrollToDocumentEnd` полезен для лент, которые дозагружают контент по скроллу (infinite scroll). После прокрутки — `cdtWaitForNetworkIdle`.

## 3.4. Файлы, разрешения, эмуляция

```cpp
void cdtSetFileInputFiles(const std::string& selector, const std::vector<std::string>& files);
void cdtGrantPermissions(const std::vector<std::string>& permissions, const std::string& origin);
void cdtSetUserAgent(const std::string& ua, const std::string& acceptLanguage = "", const std::string& platform = "");
void cdtSetDeviceMetrics(int width, int height, double deviceScaleFactor, bool mobile);
void cdtSetTimezone(const std::string& tz);
void cdtSetLocale(const std::string& locale);
void cdtSetGeolocation(double lat, double lon, double accuracy);
```

- `DOM.setFileInputFiles({files, nodeId})` — для `<input type="file">`. Сначала `DOM.querySelector` — получить `nodeId`.
- `Browser.grantPermissions({permissions, origin})` — иначе `geolocation`, `notifications`, `camera`, `microphone` будут выдавать отказ.
- `Emulation.setUserAgentOverride` — менять UA + `acceptLanguage` + `platform` в `navigator`.
- `Emulation.setDeviceMetricsOverride` — мобильный viewport. Влияет на media queries и на `window.innerWidth`.
- `Emulation.setTimezoneOverride` — `Intl.DateTimeFormat().resolvedOptions().timeZone`.
- `Emulation.setLocaleOverride` — локаль.
- `Emulation.setGeolocationOverride` — координаты.

## 3.5. Скриншоты

```cpp
std::string cdtScreenshotPng(bool fullPage = false);
std::string cdtScreenshotJpeg(int quality = 80);
```

`Page.captureScreenshot({format:"png", fromSurface:true, captureBeyondViewport:fullPage})`.

Результат — base64 PNG. Для полноразмерного скриншота `captureBeyondViewport:true` (новый параметр) даёт всю страницу, но всё равно ограничен `maxTextureSize` (обычно 16384 px). Для длинных страниц — нарезать по viewport и склеивать.

Полезно для отладки и для мультимодальных моделей, которые «смотрят» на страницу.

## 3.6. PDF

```cpp
std::string cdtPrintToPdf();
```

`Page.printToPDF({printBackground:true, preferCSSPageSize:false})`. Возвращает base64 PDF.

## 3.7. Прочее

```cpp
void cdtBringToFront();           // Page.bringToFront
void cdtSetWindowBounds(...);     // Browser.setWindowBounds
void cdtReloadPage(bool ignoreCache = false);
void cdtGoBack();
void cdtGoForward();
```

`Page.reload({ignoreCache})`, `Page.navigateToHistoryEntry({entryId})` после `Page.getNavigationHistory`.

## 3.8. Подводные камни

- **Клик в пустоту, если элемент под фиксированным хедером.** Перед кликом — `document.elementFromPoint(x, y)` и проверка, что вернулся ожидаемый элемент.
- **`Input.dispatchMouseEvent` не работает, если вкладка в фоне.** Сначала `Page.bringToFront` или запускать Chrome с `--headless=new` в режиме, где compositor активен.
- **`dispatchKeyEvent` требует `keydown` перед `keyup`**, иначе некоторые JS-обработчики не сработают.
- **`Input.insertText` не вызывает `input` event для некоторых фреймворков** (например, React с контролируемыми input). Если после вставки состояние не обновилось — используйте `cdtTypeText` или `dispatchKeyEvent` + `char`.
- **Модификаторы в `keyDown` не накапливаются автоматически.** Если нажимаете `Ctrl+C`, надо явно передавать `modifiers=2` в событии для `c`, а сам `Ctrl` — отдельными `keyDown`/`keyUp`.
- **Координаты для `Input.dispatchMouseEvent` — относительно viewport, а не документа.** При скролле `getBoundingClientRect` уже даёт правильные. Если у вас координаты из `DOM.getBoxModel`, они тоже в viewport, но у `getBoxModel` они в «абсолютных» координатах документа, и при скролле могут быть отрицательными или больше viewport — надо конвертировать.
- **Если страница в iframe, координаты в `Input.dispatchMouseEvent` — глобальные по всему окну браузера**, а `getBoundingClientRect` внутри iframe даёт локальные. Нужно добавить смещение iframe относительно родителя. Проще кликать через JS `el.click()` — но тогда часть событий (focus, mousedown) может не сработать.

---

# 4. Слой извлечения тел и исходников

Цель: получить содержимое каждого ответа, включая JS/CSS/HTML, и восстановить исходники через source maps.

## 4.1. `Network.getResponseBody`

```json
{ "method": "Network.getResponseBody", "params": { "requestId": "100001.2" } }
```

Ответ:
```json
{ "body": "base64...", "base64Encoded": true }
```

или
```json
{ "body": "{\"ok\":true}", "base64Encoded": false }
```

Правила:
- Для текстовых MIME (html, css, js, json, xml, svg, txt) — `base64Encoded: false`.
- Для бинарных (png, jpg, woff2, wasm) — `base64Encoded: true`.
- Если тела нет (204, HEAD, ошибка в сеть) — ошибка «No resource with given identifier found».
- После навигации (`Page.navigate`) буфер сбрасывается, старые `requestId` перестают работать.

Значит, **вызывать сразу в обработчике `loadingFinished`**, складывать в `NetworkJournal::bodies`.

Ограничение по размеру: если вы в `Network.enable` задали `maxResourceBufferSize: 1 MB`, то для больших JS-бандлов тело вернётся обрезанным (`truncated: true`). Ставьте 10–100 MB.

## 4.2. `Debugger.getScriptSource`

```json
{ "method": "Debugger.getScriptSource", "params": { "scriptId": "33" } }
```

Ответ:
```json
{ "scriptSource": "function foo(){...}", "bytecode": null }
```

Отличия от `getResponseBody`:
- Работает для inline-`<script>` (у них нет URL/requestId).
- Возвращает то, что реально скомпилировал V8, включая `eval`-код и код из `new Function`.
- Работает даже после окончания навигации, если скрипт ещё «живёт» в контексте.
- Не даёт бинарные ресурсы.

Для inline-скриптов это единственный путь получить «как их видит движок».

## 4.3. Source maps

В `Debugger.scriptParsed` поле `sourceMapURL` — это либо абсолютный URL, либо относительный. Часто в конце JS-файла есть `//# sourceMappingURL=app.js.map`. Скачать — как обычный ресурс через `Page.navigate` не надо; проще:

```cpp
std::string cdtDownloadUrl(const std::string& url);   // через Node.js? нет, через саму страницу
```

Вариант через JS на странице:
```js
fetch(SRC, { credentials: 'include' }).then(r => r.text())
```
Но CORS может не пустить. Значит, скачивать **из C++** своим HTTP-клиентом (у вас уже есть ixwebsocket/HTTP).

Source map — JSON формата v3:

```json
{
  "version": 3,
  "file": "app.js",
  "sources": ["../src/index.ts", "../src/App.tsx"],
  "sourcesContent": ["import React from 'react';...", "..."] | null,
  "names": ["foo", "bar"],
  "mappings": "AAAA,GAAG,CAAC,GAAG;...",
  "sourceRoot": ""
}
```

`sourcesContent` — если он есть, у вас сразу исходники. Если нет — придётся качать `sources[i]` отдельно (по URL относительно карты).

## 4.4. C++ API

```cpp
struct ScriptInfo {
    std::string scriptId;
    std::string url;
    std::string sourceMapURL;
    int  executionContextId = 0;
    int  startLine = 0;
    bool isModule = false;
    std::int64_t length = 0;
    std::string hash;
};

struct BodyRecord {
    std::string requestId;
    std::string body;          // если base64Encoded, то base64
    bool        base64 = false;
    bool        truncated = false;
    bool        available = false;
};

class SourceJournal {
public:
    std::unordered_map<std::string, ScriptInfo> scripts;   // scriptId -> info

    void onScriptParsed(const json& params);

    // Получить исходник скрипта
    bool getScriptSource(const std::string& scriptId, std::string& out);

    // Скачать source map (если есть) и распаковать
    bool getSourceMapTree(const std::string& scriptId,
                          std::vector<SourceFile>& out);
};
```

## 4.5. Красивое форматирование JS

Минифицированный бандл читать глазами нельзя. Варианты:

- **Внешний prettier**: `npx prettier --parser babel` — но это требует Node.js на машине.
- **Реализация в C++**: сложно. Но можно **деобфусцировать в самой странице** через DevTools «Pretty print»: у CDP нет такой команды, но есть `Debugger.setBlackboxPatterns` — не то.
- **Отправка кода в JS-нагрузку, запущенную на странице**: у вас уже есть `Runtime.evaluate`. Можно исполнить небольшой формататор-скрипт в контексте страницы, передав ему бандл через `Runtime.callFunctionOn`. Но проще подключать внешний `prettier.js` через `Runtime.evaluate` с `includeCommandLineAPI:true` — не сработает для больших файлов.
- **Практический вариант**: сохраняйте сырой текст и отдавайте ИИ **с префиксом**, объясняющим «это минифицированный JS, вот ключевые имена функций». Часто этого достаточно.

Более надёжно: реализовать деобфускацию в C++ по простому правилу — вставить `\n` после `;`, `{`, `}` с отступами. Не идеально, но читаемо. Либо держите в бандле и `.js`, и `.js.map` — ИИ может сам разобраться.

## 4.6. Подводные камни

- **`Network.getResponseBody` падает после `Page.reload`** — все `requestId` меняются.
- **`Debugger.getScriptSource` не работает для уже выгруженных скриптов.** Если `Debugger.disable` вызывали — скрипты забываются.
- **`sourceMapURL` бывает с `data:` схемой** (inline map в base64). Тогда карта прямо в `sourceMapURL` после запятой — надо декодировать из base64.
- **Source map может не иметь `sourcesContent`.** Тогда без запроса к серверу не восстановить исходник. Но многие современные сборщики (vite, esbuild) включают его по умолчанию.
- **Восстановленные исходники не всегда идентичны оригиналу** — минификатор мог переименовать импорты, или webpack runtime вставил свои обёртки.
- **Размер**. Сотни мегабайт JS в памяти. Складывайте сразу на диск, в `unordered_map` держите только метаданные.
- **`base64` тела** надо декодировать перед сохранением как `.js`. Иначе на диске будет base64 — нечитаемо.

---

# 5. Слой учёта контекстов

Цель: работать с iframe, worker, service worker, а не только с главной страницей.

## 5.1. Как CDP видит контексты

Всё, что за пределами главного документа, — это отдельный **target**:

- `<iframe>` — target типа `iframe`.
- Dedicated Worker — `worker`.
- Service Worker — `service_worker`.
- Shared Worker — `shared_worker`.
- `<webview>` / OOPIF (out-of-process iframe) — тоже `iframe`, но может быть в другом процессе.

Чтобы их видеть, нужно:

```json
{ "method": "Target.setAutoAttach",
  "params": {
    "autoAttach": true,
    "waitForDebuggerOnStart": false,
    "flatten": true
  }
}
```

- `flatten: true` — события `Target.attachedToTarget` приходят с полем `sessionId`, и все команды к этой сессии надо отправлять с этим `sessionId` в корне сообщения.
- `flatten: false` — старая модель с вложенными WS-соединениями. Неудобно.

## 5.2. Структура сессий

При `flatten: true` каждое CDP-сообщение имеет вид:

```json
{
  "sessionId": "S1",
  "method": "Runtime.evaluate",
  "params": { "expression": "..." },
  "id": 42
}
```

Если `sessionId` отсутствует — команда идёт в «корневую» сессию (браузер).

Все ответы и события тоже приходят с `sessionId`. Ваш `GenericMethodHandler` должен сохранять `sessionId` и складывать в контекст.

## 5.3. Как связать frame → context → session

Цепочка:
1. `Target.attachedToTarget` → `{sessionId: S1, targetInfo: {targetId: T1, type: "iframe", url: "..."}}`.
2. Внутри сессии S1 включаете `Runtime.enable` и `Page.enable`.
3. Приходит `Runtime.executionContextCreated` с `context.auxData.frameId` — это frame ID внутри этого target.
4. Приходит `Page.frameNavigated` с `frame.id` — тот же frameId.

Маппинги:
```cpp
std::unordered_map<std::string, std::string> targetToSession;   // targetId -> sessionId
std::unordered_map<std::string, std::string> sessionToTarget;
std::unordered_map<std::string, std::string> frameToTarget;     // frameId -> targetId
std::unordered_map<std::string, std::string> targetToFrame;
std::unordered_map<std::string, int> contextToTarget;
```

## 5.4. Работа с сессией

Все команды через `wsSendCommand` должны уметь пробрасывать `sessionId`. Обёртки:

```cpp
json wsSendCommand(const std::string& method, const json& params, const std::string& sessionId = "");
```

`Runtime.evaluate` в iframe:

```cpp
json cdtEvaluateInFrame(const std::string& frameId, const std::string& expr) {
    auto targetId = frameToTarget.at(frameId);
    auto sessionId = targetToSession.at(targetId);
    return wsSendCommand("Runtime.evaluate", {{"expression", expr}, {"returnByValue", true}}, sessionId);
}
```

Аналогично `cdtGetDocumentInFrame`, `cdtClickInFrame` (координаты внутри iframe — локальные; для глобальных надо добавить смещение iframe).

## 5.5. GetFrameTree для полного обхода

`Page.getFrameTree` даёт дерево:

```json
{
  "frameTree": {
    "frame": { "id": "F1", "url": "https://ya.ru/", "parentId": null },
    "childFrames": [
      { "frame": { "id": "F2", "url": "https://widget/", "parentId": "F1" } }
    ]
  }
}
```

Но это только iframe одного target. Для OOPIF и worker — отдельно через `Target.getTargets`.

## 5.6. Оценка «сложных» контекстов

- **iframes** — почти всегда. Обходить обязательно.
- **dedicated workers** — реже. Если в них логика, то важные.
- **service workers** — живут между сессиями, регистрируются на origin. Их можно ловить через `Target.attachedToTarget` с `type: "service_worker"`. Многие SPA кэшируют через них; если нужен честный offline-анализ — важны.
- **worklets** (paint, audio) — совсем редко.
- **shared workers** — редко.

## 5.7. Подводные камни

- **Координаты мыши в OOPIF.** `Input.dispatchMouseEvent` для OOPIF принимает координаты относительно родительского окна. Координаты внутри iframe (`getBoundingClientRect` внутри iframe) — локальные, надо добавить смещение iframe в родителе (или наоборот, вычесть — зависит от того, из чьего контекста вы получали rect).
- **`Runtime.evaluate` в sessionId** работает независимо от главного `Runtime.evaluate`. Два независимых executionContext.
- **`Target.attachedToTarget` может приходить на уже «присоединённый» target** — проверяйте, не завели ли вы второй маппинг.
- **`Target.setDiscoverTargets`** — отдельная команда, чтобы получать события `Target.targetCreated`/`targetInfoChanged`/`targetDestroyed`. Для autoAttach не обязательна, но полезна.
- **`waitForDebuggerOnStart: true`** останавливает target до `Runtime.runIfWaitingForDebugger`. Для сбора журналов лучше `false`.
- **Service workers** живут в отдельном процессе и работают фоном. Автоматически они не атачатся, если вы не разрешите — нужно `Target.setAutoAttach` с `filter` на типы.
- **`iframe srcdoc`** — не создаёт отдельный target, живёт в родителе. Его DOM доступен через `contentDocument` в JS.

---

# 6. Слой нормализации (AI bundle)

Цель: из сырых журналов собрать папку, которая объясняет страницу ИИ.

## 6.1. Структура папки

```
bundle/
  meta.json                 — что собрано, чего не хватило
  summary.json              — ключевые цифры
  requests.tsv              — таблица всех запросов
  dom.html                  — нормализованный DOM
  interactive.json          — интерактивные элементы с селекторами
  console.log               — сообщения консоли и исключения
  network/
    post-load.tsv           — что ушло после loadEventFired
    timing.json             — ключевые тайминги
  scripts/
    index.json              — список скриптов с метаданными
    <hash>.js               — восстановленные исходники (из source maps)
    <hash>.js.map           — если был map
  storage/
    cookies.json
    localStorage.json
    sessionStorage.json
  screenshots/
    initial.png             — опционально
```

## 6.2. `summary.json`

```json
{
  "url": "https://ya.ru/",
  "title": "Яндекс — быстрый поиск в интернете",
  "finalUrl": "https://ya.ru/",
  "loadedAt": "2026-09-10T12:00:00Z",
  "domNodes": 1234,
  "scripts": 42,
  "stylesheets": 7,
  "requests": { "total": 138, "byType": { "Script": 20, "XHR": 12, "Document": 1 } },
  "errors": { "console": 0, "exceptions": 0, "networkFailures": 1 },
  "frames": 3,
  "workers": 1,
  "interactive": { "inputs": 2, "buttons": 15, "links": 42, "forms": 1 }
}
```

Заполняется из журналов. Не нужен `Runtime.evaluate` — всё уже в памяти.

## 6.3. `requests.tsv`

Одна строка на запрос, порядок — по времени.

Колонки:
```
time    method  status  type     size    url     initiator
```

Строки:
```
12:00:00.010  GET  200  Document  51K   https://ya.ru/                 navigation
12:00:00.123  GET  200  Script    1.2M  https://yastatic.net/...      parser
12:00:00.234  GET  200  XHR       3K    https://ya.ru/suggest/...     script (app.js:42)
```

Генерация — из `NetworkJournal`. Никаких CDP-вызовов, чистая сериализация.

## 6.4. `dom.html`

Нормализованный HTML. Один `Runtime.evaluate`:

```js
(() => {
  const clone = document.documentElement.cloneNode(true);

  // Убрать inline SVG-содержимое
  clone.querySelectorAll('svg').forEach(svg => {
    svg.innerHTML = '';
  });

  // Заменить base64-картинки
  clone.querySelectorAll('img[src^="data:"]').forEach(img => {
    img.setAttribute('src', 'data:<removed>');
  });

  // Убрать служебные script-теги
  clone.querySelectorAll('script').forEach(s => s.remove());

  // Убрать style-теги (их отдельно)
  clone.querySelectorAll('style').forEach(s => s.remove());

  // Убрать скрытые узлы (display:none, visibility:hidden)
  clone.querySelectorAll('[hidden]').forEach(n => n.remove());
  clone.querySelectorAll('[style*="display:none"]').forEach(n => n.remove());

  // Ограничить длину атрибутов
  const LIMIT = 200;
  clone.querySelectorAll('*').forEach(el => {
    for (const attr of [...el.attributes]) {
      if (attr.value.length > LIMIT) {
        el.setAttribute(attr.name, attr.value.slice(0, LIMIT) + '...');
      }
    }
  });

  return clone.outerHTML;
})()
```

Дополнительно можно убрать «шумные» классы (`class="..."` с хешами типа `css-1x2y3z`). Но это уже эвристика; иногда важные селекторы.

## 6.5. `interactive.json`

Один `Runtime.evaluate`:

```js
(() => {
  const result = [];
  const selectors = 'input, textarea, button, select, a[href], [role="button"], [role="link"], [role="textbox"], [contenteditable="true"]';
  const els = document.querySelectorAll(selectors);

  for (const el of els) {
    const r = el.getBoundingClientRect();
    const visible = r.width > 0 && r.height > 0 && getComputedStyle(el).visibility !== 'hidden';
    if (!visible) continue;

    result.push({
      tag: el.tagName.toLowerCase(),
      type: el.type || null,
      id: el.id || null,
      name: el.name || null,
      role: el.getAttribute('role') || null,
      ariaLabel: el.getAttribute('aria-label') || null,
      placeholder: el.placeholder || null,
      text: (el.innerText || '').trim().slice(0, 100),
      href: el.href || null,
      value: el.value != null ? String(el.value).slice(0, 100) : null,
      selector: buildSelector(el),
      rect: { x: r.left + r.width/2, y: r.top + r.height/2, w: r.width, h: r.height }
    });
  }
  return result;

  function buildSelector(el) {
    if (el.id) return '#' + CSS.escape(el.id);
    let parts = [];
    while (el && el.nodeType === 1 && parts.length < 5) {
      let sel = el.tagName.toLowerCase();
      if (el.className && typeof el.className === 'string') {
        const cls = el.className.trim().split(/\s+/).filter(Boolean)[0];
        if (cls) sel += '.' + CSS.escape(cls);
      }
      parts.unshift(sel);
      el = el.parentElement;
    }
    return parts.join(' > ');
  }
})()
```

Результат — массив. Складывать как JSON.

## 6.6. `console.log`

Формат — построчный текст:
```
[12:00:00.123] LOG   "hello" (at app.js:42)
[12:00:00.456] ERROR Uncaught TypeError: x is undefined (at react.js:102)
```

Генерируется из `ConsoleJournal` без CDP-вызовов.

## 6.7. `network/post-load.tsv`

Те же колонки, что в `requests.tsv`, но только для запросов с `timestamp > loadEventFired.timestamp`. Это то, что делает страница после загрузки: аналитика, XHR-за-данными, кэш-инвалидация.

## 6.8. `scripts/index.json`

```json
[
  {
    "id": "33",
    "url": "https://yastatic.net/.../app.js",
    "size": 1234567,
    "isModule": true,
    "hasSourceMap": true,
    "sourceFiles": ["src/index.ts", "src/App.tsx"],
    "bundleFile": "scripts/abc123.js",
    "mapFile": "scripts/abc123.js.map"
  }
]
```

## 6.9. Восстановление исходников

Псевдокод:

```cpp
for (auto& [scriptId, info] : sourceJournal.scripts) {
    std::string src;
    if (!getScriptSource(scriptId, src)) continue;

    // сохранить как <hash>.js
    std::string bundleFile = outDir + "/scripts/" + hash(info.url) + ".js";
    writeFile(bundleFile, src);

    if (!info.sourceMapURL.empty()) {
        std::string mapBody;
        if (downloadUrl(resolve(info.url, info.sourceMapURL), mapBody)) {
            std::string mapFile = outDir + "/scripts/" + hash(info.url) + ".js.map";
            writeFile(mapFile, mapBody);

            // распарсить
            auto sm = parseSourceMap(mapBody);
            for (size_t i = 0; i < sm.sources.size(); ++i) {
                std::string content = sm.sourcesContent.size() > i ? sm.sourcesContent[i] : "";
                if (content.empty()) {
                    // попытаться скачать sources[i]
                    downloadUrl(resolve(resolve(info.url, info.sourceMapURL), sm.sources[i]), content);
                }
                std::string srcFile = outDir + "/scripts/src/" + sanitize(sm.sources[i]);
                writeFile(srcFile, content);
            }
        }
    }
}
```

Ключевые моменты:
- **Скачивать source map — вне страницы**, своим HTTP-клиентом, потому что CORS мешает.
- **Проверять кэш**, чтобы не качать один и тот же map дважды.
- **Ограничивать размер**, чтобы не залиться на 1 GB.

## 6.10. Скриншоты (опционально)

`Page.captureScreenshot({format: "jpeg", quality: 70, captureBeyondViewport: true})` — сохранить как `screenshots/initial.jpg`. Полезно для мультимодальных моделей и для отладки.

## 6.11. Хранилище

```cpp
json cookies = cdtGetAllCookies();   // Network.getAllCookies
writeFile(outDir + "/storage/cookies.json", cookies.dump(2));
```

`localStorage`/`sessionStorage` — через `Runtime.evaluate`:

```js
JSON.stringify(Object.fromEntries(Object.entries(localStorage)))
```

## 6.12. Метаданные

`meta.json` — что собрано, чего не хватило:

```json
{
  "warnings": [
    "Source map for https://.../app.js not available",
    "Response body for https://.../video.mp4 skipped (too large)",
    "Frame F2: evaluation failed: Access denied"
  ],
  "truncations": [
    "dom.html truncated at 2 MB"
  ]
}
```

## 6.13. Подводные камни

- **`outerHTML` вернёт огромный текст**, если в нём inline-SVG. Нормализация обязательна.
- **Shadow DOM не виден в `outerHTML`** — если элементы в shadow root, их надо обойти отдельно через `element.shadowRoot.innerHTML`.
- **iframe-контент не входит в главный `outerHTML`** — собирать отдельно по фреймам и складывать в подпапки.
- **Source map может не прилагаться** — тогда в bundle попадёт минифицированный JS, и ИИ будет читать его с трудом. Это нормально, но добавьте предупреждение.
- **Ссылки между файлами в bundle** (index.json → bundleFile) должны быть относительные, иначе ИИ не поймёт.
- **Размер bundle легко доходит до 50–200 MB.** Для передачи ИИ используйте только часть: `summary`, `requests`, `interactive`, `dom`. Исходники — отдельно, по запросу.
- **`buildSelector` даёт селектор, но не гарантирует уникальность.** Проверяйте через `querySelectorAll(sel).length === 1` и поднимайтесь выше, пока не станет уникальным.
- **`getBoundingClientRect` возвращает координаты в момент вызова**, но DOM может измениться. Для стабильности делайте это в конце сбора, когда страница замерла.

---

# 7. Слой диффов

Цель: сравнивать два слепка и показывать, что изменилось после действия.

## 7.1. Что снимается в слепок

- **DOM snapshot** (нормализованный `outerHTML`).
- **Network snapshot** (список `requestId` с URL и timestamp).
- **Storage snapshot** (`localStorage`, `sessionStorage`, cookies).
- **Interactive snapshot** (массив интерактивных элементов).

Всё — с отметкой `monotonic timestamp` для связи с журналами.

## 7.2. `cdtSnapshot`

```cpp
struct Snapshot {
    std::string url;
    double takenAt = 0.0;
    std::string domHtml;
    std::vector<std::string> requestIds;      // за всё время до этого момента
    std::unordered_map<std::string, std::string> localStorage;
    std::unordered_map<std::string, std::string> sessionStorage;
    std::vector<InteractiveElement> interactive;
};

Snapshot cdtSnapshot();
```

Реализация — комбинация `Runtime.evaluate` (для DOM, storage, interactive) и сериализации журнала.

## 7.3. `cdtDiff`

```cpp
struct Diff {
    std::vector<std::string> newRequestIds;
    std::vector<std::pair<std::string, std::string>> domChanges;   // old, new
    std::unordered_map<std::string, std::string> storageAdded;
    std::unordered_map<std::string, std::string> storageChanged;
    std::unordered_map<std::string, std::string> storageRemoved;
    std::vector<InteractiveElement> interactiveAdded;
    std::vector<InteractiveElement> interactiveRemoved;
};

Diff cdtDiff(const Snapshot& a, const Snapshot& b);
```

Дифф DOM — простой построчный (`difflib`-подобный), с ограничением на количество строк (первые 200 изменений — уже достаточно).

Дифф storage — множества по ключам и значениям.

Дифф интерактивных элементов — по ключу `(tag, id, name, placeholder, text)`, а не по координатам. Координаты могут сместиться, но элемент тот же.

## 7.4. Типичный сценарий

```cpp
auto s0 = wsConnection->cdtSnapshot();
auto rect = wsConnection->cdtWaitForSelector("textarea#text");
wsConnection->cdtClick(rect.x, rect.y);
wsConnection->cdtInsertText("котики");
auto s1 = wsConnection->cdtSnapshot();
wsConnection->cdtPressKey("Enter", "Enter", 13, 0, "\r");
wsConnection->cdtWaitForSelector("ul.search-results", 30000);
auto s2 = wsConnection->cdtSnapshot();

auto diff1 = wsConnection->cdtDiff(s0, s1);
auto diff2 = wsConnection->cdtDiff(s1, s2);
```

`diff1` покажет, что появились запросы к suggest API и что DOM textarea перешёл в состояние «заполнено». `diff2` — что ушёл поисковый запрос и появился блок результатов.

Это именно то, что нужно ИИ для обучения: пары «действие → что изменилось».

## 7.5. Подводные камни

- **Дифф DOM шумный.** Многие сайты постоянно меняют `class` у анимаций, добавляют `data-*` метки. Фильтруйте: перед диффом уберите атрибуты `data-reactid`, `data-ts`, `aria-busy`, анимационные классы.
- **Дифф сети надо привязать к времени.** `takenAt` у каждого snapshot сравнивается с `request.timestamp`. Если часы плывут — используйте `wallTime`.
- **Дифф не заменяет анализ.** Он показывает «что изменилось», но не «почему». Для «почему» — исходники, DOM-атрибуты, стек инициатора.
- **Снимок storage может быть большим**, если сайт пишет туда мегабайты (например, кэш статей). Ограничьте 100 ключей и 10 КБ на значение.

---

# 8. Дополнительные полезные вещи

## 8.1. Performance

```json
{ "method": "Performance.enable" }
{ "method": "Performance.getMetrics" }
```

Возвращает массив `{name, value}`:
```
Timestamp, Documents, Frames, JSEventListeners, Nodes, LayoutCount, RecalcStyleCount,
LayoutDuration, RecalcStyleDuration, ScriptDuration, TaskDuration, JSHeapUsedSize,
JSHeapTotalSize, FirstMeaningfulPaint, DomContentLoaded, NavigationStart
```

Полезно для «почему страница тормозит».

## 8.2. Coverage

```json
{ "method": "Profiler.startPreciseCoverage", "params": { "callCount": false, "detailed": true } }
{ "method": "Profiler.takePreciseCoverage" }
```

Возвращает для каждого скрипта массив интервалов `[startOffset, endOffset]`, которые **реально исполнились**. Остальное — мёртвый код.

Полезно, чтобы показать ИИ «вот эти функции реально работают, остальное можно не читать».

## 8.3. Breaking points на XHR/fetch

```json
{ "method": "DOMDebugger.setXHRBreakpoint", "params": { "url": "api/" } }
```

При срабатывании `Debugger.paused` — можно снять стек вызовов. Полезно, чтобы понять, какой именно код дёрнул API.

## 8.4. `Page.addScriptToEvaluateOnNewDocument`

Позволяет инжектировать JS до того, как загрузится любой скрипт страницы. Например:

```js
window.__AI_MONKEY_PATCH__ = true;
const origFetch = window.fetch;
window.fetch = function(...args) {
    window.__AI_FETCH_LOG__ = window.__AI_FETCH_LOG__ || [];
    window.__AI_FETCH_LOG__.push({ url: args[0], ts: Date.now() });
    return origFetch.apply(this, args);
};
```

После загрузки — `Runtime.evaluate("JSON.stringify(window.__AI_FETCH_LOG__)")`. Это дублирует `Network.*`, но даёт взгляд со стороны страницы и работает даже там, где CDP-события потеряны.

## 8.5. `Page.setBypassCSP`

```json
{ "method": "Page.setBypassCSP", "params": { "enabled": true } }
```

Снимает Content-Security-Policy — полезно, если хотите инжектить свои скрипты в страницу, а CSP не даёт.

## 8.6. `Network.setBlockedURLs`

```json
{ "method": "Network.setBlockedURLs", "params": { "urls": ["*.png", "*.jpg", "*google-analytics*"] } }
```

Блокирует ресурсы. Ускоряет загрузку и убирает шум аналитики.

## 8.7. Виртуальное время

```json
{ "method": "Emulation.setVirtualTimePolicy", "params": {
    "policy": "pauseIfNetworkFetchesPending",
    "budget": 5000,
    "maxVirtualTimeTaskStarvationCount": 10000
} }
```

Ускоряет `setTimeout`/`setInterval` и даёт детерминированное поведение. Иногда полезно для воспроизведения.

## 8.8. `Runtime.addBinding`

Позволяет странице вызвать C++-функцию:

```json
{ "method": "Runtime.addBinding", "params": { "name": "__aiCallback" } }
```

Из JS:
```js
__aiCallback(JSON.stringify({ event: 'ready' }));
```

В C++ приходит событие `Runtime.bindingCalled`. Удобно для точной синхронизации «страница готова», без polling.

---

# 9. Порядок реализации

Если делать по одному шагу:

1. **`NetworkJournal` + `ConsoleJournal` + `Runtime.enable`/`Network.enable`.** Полдня. Уже даёт огромную пользу: видно все запросы и все ошибки.
2. **`cdtWaitForSelector` + `cdtWaitForFunction` + `cdtWaitForNetworkIdle`.** День. Убирает все гонки.
3. **`cdtClick` + `cdtInsertText` + `cdtPressKey`.** День. Появляется возможность делать, а не только смотреть.
4. **`SourceJournal` + `cdtGetResponseBody` + `cdtGetScriptSource` + source maps.** Два-три дня. Появляются исходники.
5. **`Target.setAutoAttach` + `cdtEvaluateInFrame` + учёт worker.** День. Перестают теряться iframe.
6. **`cdtSnapshot` + `cdtDiff`.** Полдня. Появляется анализ поведения.
7. **`cdtBuildAiBundle`.** Два дня. Финальная упаковка для ИИ.

На каждом шаге можно тестировать на одной и той же странице и сравнивать, стало ли ИИ проще отвечать. После 1–3 пунктов уже работает базовый «посмотри и кликни». После 4 — «прочитай исходники и объясни». После 5–6 — «пойми динамику». После 7 — «вот bundle, отвечай на любой вопрос».

---

# 10. Что осталось за кадром

Всё вышеописанное — про **анализ и управление страницей**. Есть ещё большой пласт:

- **Аутентификация**: логин, 2FA, CAPTCHA. CAPTCHA не решается без человека или специальных сервисов.
- **Антибот**: Cloudflare, Akamai, Datadome. Их детекторы смотрят на `navigator.webdriver`, `Runtime.enable`, паттерны ввода. Chrome под CDP виден — нужны прокси и подмена флагов.
- **Работа с файлами**: скачивание, загрузка, PDF.
- **Многопоточность**: несколько вкладок одновременно через разные `sessionId`.
- **Прокси**: `Target.createBrowserContext` + `--proxy-server`.
- **Хранение сессий**: `Network.setCookie` для восстановления авторизации.

