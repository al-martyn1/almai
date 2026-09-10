- [Максимально подробный разбор: реализация слоёв CDT для AI-анализа](#максимально-подробный-разбор-реализация-слоёв-cdt-для-ai-анализа)
- [Часть I. Слой наблюдения](#часть-i-слой-наблюдения)
  - [1.1. Полный список событий, которые надо перехватывать](#11-полный-список-событий-которые-надо-перехватывать)
    - [1.1.1. Network: события жизненного цикла запроса](#111-network-события-жизненного-цикла-запроса)
    - [1.1.2. `Network.requestWillBeSent` — полная структура](#112--networkrequestwillbesent---полная-структура)
    - [1.1.3. `Network.requestWillBeSentExtraInfo` — детали](#113--networkrequestwillbesentextrainfo---детали)
    - [1.1.4. `Network.responseReceived` — детали](#114--networkresponsereceived---детали)
    - [1.1.5. `Network.responseReceivedExtraInfo`](#115--networkresponsereceivedextrainfo-)
    - [1.1.6. `Network.dataReceived`](#116--networkdatareceived-)
    - [1.1.7. `Network.loadingFinished`](#117--networkloadingfinished-)
    - [1.1.8. `Network.loadingFailed`](#118--networkloadingfailed-)
    - [1.1.9. WebSocket-события](#119-websocket-события)
    - [1.1.10. EventSource](#1110-eventsource)
    - [1.1.11. Runtime: события](#1111-runtime-события)
    - [1.1.12. Page: события](#1112-page-события)
    - [1.1.13. DOM: события](#1113-dom-события)
    - [1.1.14. Target: события](#1114-target-события)
    - [1.1.15. Debugger: события](#1115-debugger-события)
    - [1.1.16. Log: события](#1116-log-события)
    - [1.1.17. Inspector: события](#1117-inspector-события)
  - [1.2. Полная реализация журналов на C++](#12-полная-реализация-журналов-на-c)
    - [1.2.1. Структуры данных](#121-структуры-данных)
    - [1.2.2. `NetworkJournal`](#122--networkjournal-)
    - [1.2.3. `ConsoleJournal`](#123--consolejournal-)
    - [1.2.4. `DomJournal`](#124--domjournal-)
    - [1.2.5. `TargetJournal` (сессии)](#125--targetjournal--сессии)
    - [1.2.6. `ScriptJournal`](#126--scriptjournal-)
  - [1.3. Интеграция в `Connection`](#13-интеграция-в--connection-)
- [Часть II. Слой ожидания](#часть-ii-слой-ожидания)
  - [2.1. Архитектура event-loop](#21-архитектура-event-loop)
    - [2.1.1. `PendingWait`](#211--pendingwait-)
    - [2.1.2. `WaitManager`](#212--waitmanager-)
    - [2.1.3. Интеграция](#213-интеграция)
  - [2.2. `cdtWaitForEvent` — базовый комбинатор](#22--cdtwaitforevent---базовый-комбинатор)
  - [2.3. `cdtWaitForSelector` — реализация](#23--cdtwaitforselector---реализация)
  - [2.4. `cdtWaitForFunction`](#24--cdtwaitforfunction-)
  - [2.5. `cdtWaitForNetworkIdle`](#25--cdtwaitfornetworkidle-)
  - [2.6. `cdtWaitForNetworkResponse`](#26--cdtwaitfornetworkresponse-)
  - [2.7. Правильный polling vs event-based](#27-правильный-polling-vs-event-based)
  - [2.8. Подводные камни слоя ожидания](#28-подводные-камни-слоя-ожидания)
- [Часть III. Слой действий](#часть-iii-слой-действий)
  - [3.1. Мышь — полный API](#31-мышь--полный-api)
    - [3.1.1. Одиночный клик](#311-одиночный-клик)
    - [3.1.2. Двойной клик](#312-двойной-клик)
    - [3.1.3. Drag & drop](#313-drag--drop)
    - [3.1.4. Скролл](#314-скролл)
    - [3.1.5. Touch (мобильная эмуляция)](#315-touch-мобильная-эмуляция)
  - [3.2. Клавиатура — полный API](#32-клавиатура--полный-api)
    - [3.2.1. `cdtPressKey`](#321--cdtpresskey-)
    - [3.2.2. `cdtPressEnter`](#322--cdtpressenter-)
    - [3.2.3. `cdtPressTab`](#323--cdtpresstab-)
    - [3.2.4. `cdtInsertText`](#324--cdtinserttext-)
    - [3.2.5. `cdtTypeText`](#325--cdttypetext-)
    - [3.2.6. Модификаторы](#326-модификаторы)
    - [3.2.7. Стрелки](#327-стрелки)
    - [3.2.8. Специальные клавиши](#328-специальные-клавиши)
  - [3.3. Файлы](#33-файлы)
  - [3.4. Клик через JS (обход покрытий)](#34-клик-через-js-обход-покрытий)
  - [3.5. Проверка попадания клика](#35-проверка-попадания-клика)
  - [3.6. Скриншоты](#36-скриншоты)
  - [3.7. Эмуляция](#37-эмуляция)
  - [3.8. Подводные камни слоя действий](#38-подводные-камни-слоя-действий)
- [Часть IV. Слой извлечения тел и исходников](#часть-iv-слой-извлечения-тел-и-исходников)
  - [4.1. `Network.getResponseBody`](#41--networkgetresponsebody-)
  - [4.2. `Debugger.getScriptSource`](#42--debuggergetscriptsource-)
  - [4.3. Source maps](#43-source-maps)
    - [4.3.1. Скачивание](#431-скачивание)
    - [4.3.2. Парсинг source map v3](#432-парсинг-source-map-v3)
    - [4.3.3. Восстановление исходников](#433-восстановление-исходников)
    - [4.3.4. Обработка `sourceRoot`](#434-обработка--sourceroot-)
  - [4.4. Кэширование](#44-кэширование)
  - [4.5. Ограничения](#45-ограничения)
  - [4.6. Обход всех скриптов](#46-обход-всех-скриптов)
  - [4.7. Обход всех тел](#47-обход-всех-тел)
  - [4.8. Подводные камни слоя тел](#48-подводные-камни-слоя-тел)
- [Часть V. Слой учёта контекстов](#часть-v-слой-учёта-контекстов)
  - [5.1. Открытие сессий](#51-открытие-сессий)
  - [5.2. Обёртка `wsSendCommand` с sessionId](#52-обёртка--wssendcommand--с-sessionid)
  - [5.3. Определение контекста для evaluate](#53-определение-контекста-для-evaluate)
  - [5.4. `cdtEvaluateInFrame`](#54--cdtevaluateinframe-)
  - [5.5. Смещение координат в iframe](#55-смещение-координат-в-iframe)
  - [5.6. Клик в iframe через JS](#56-клик-в-iframe-через-js)
  - [5.7. Worker и Service Worker](#57-worker-и-service-worker)
  - [5.8. Подводные камни слоя контекстов](#58-подводные-камни-слоя-контекстов)
- [Часть VI. Слой нормализации (AI bundle)](#часть-vi-слой-нормализации-ai-bundle)
  - [6.1. `meta.json`](#61--metajson-)
  - [6.2. `summary.json`](#62--summaryjson-)
  - [6.3. `requests.tsv`](#63--requeststsv-)
  - [6.4. `dom.html`](#64--domhtml-)
  - [6.5. `interactive.json`](#65--interactivejson-)
  - [6.6. `console.log`](#66--consolelog-)
  - [6.7. `scripts/index.json`](#67--scriptsindexjson-)
  - [6.8. `cdtBuildAiBundle` — общая функция](#68--cdtbuildaibundle---общая-функция)
  - [6.9. Размер bundle](#69-размер-bundle)
  - [6.10. Подводные камни слоя нормализации](#610-подводные-камни-слоя-нормализации)
- [Часть VII. Слой диффов](#часть-vii-слой-диффов)
  - [7.1. Snapshot](#71-snapshot)
  - [7.2. Diff](#72-diff)
  - [7.3. Сценарий использования](#73-сценарий-использования)
  - [7.4. Подводные камни слоя диффов](#74-подводные-камни-слоя-диффов)
- [Часть VIII. Дополнительные домены CDP](#часть-viii-дополнительные-домены-cdp)
  - [8.1. Performance](#81-performance)
  - [8.2. Coverage](#82-coverage)
  - [8.3. Breakpoints на XHR](#83-breakpoints-на-xhr)
  - [8.4. Инжект до загрузки](#84-инжект-до-загрузки)
  - [8.5. CSP bypass](#85-csp-bypass)
  - [8.6. Block URL](#86-block-url)
  - [8.7. Virtual time](#87-virtual-time)
  - [8.8. Binding](#88-binding)
  - [8.9. Cookies](#89-cookies)
  - [8.10. Storage](#810-storage)
  - [8.11. Navigation history](#811-navigation-history)
- [Часть IX. Порядок реализации с тестированием](#часть-ix-порядок-реализации-с-тестированием)
  - [9.1. Итерации](#91-итерации)
    - [Итерация 1 (1–2 дня)](#итерация-1-12-дня)
    - [Итерация 2 (1 день)](#итерация-2-1-день)
    - [Итерация 3 (1 день)](#итерация-3-1-день)
    - [Итерация 4 (2–3 дня)](#итерация-4-23-дня)
    - [Итерация 5 (1 день)](#итерация-5-1-день)
    - [Итерация 6 (1 день)](#итерация-6-1-день)
    - [Итерация 7 (2 дня)](#итерация-7-2-дня)
  - [9.2. Критерии готовности](#92-критерии-готовности)
  - [9.3. Хорошие тестовые сайты](#93-хорошие-тестовые-сайты)
- [Часть X. Резюме и общий совет](#часть-x-резюме-и-общий-совет)
  - [10.1. Что критично, а что можно отложить](#101-что-критично-а-что-можно-отложить)
  - [10.2. Один общий совет](#102-один-общий-совет)

# Максимально подробный разбор: реализация слоёв CDT для AI-анализа

Это большой текст. Разбит на 10 больших разделов, каждый — с полным описанием структур, CDP-сообщений, кода на C++, подводных камней и порядка реализации.

---

# Часть I. Слой наблюдения

## 1.1. Полный список событий, которые надо перехватывать

### 1.1.1. Network: события жизненного цикла запроса

Полный порядок событий для одного запроса:

```
1. Network.requestWillBeSent                    — начало
2. Network.requestWillBeSentExtraInfo           — реальные заголовки (после cookies)
3. Network.responseReceived                     — первый байт ответа
4. Network.responseReceivedExtraInfo            — заголовки от сервера (включая Set-Cookie)
5. Network.dataReceived (0..N раз)              — байты
6. Network.loadingFinished / loadingFailed      — конец
```

Для редиректа:
```
1. Network.requestWillBeSent (req1)
2. Network.requestWillBeSentExtraInfo (req1)
3. Network.responseReceived (req1, status 301/302)
4. Network.responseReceivedExtraInfo (req1)
5. Network.requestWillBeSent (req2, тот же requestId, redirectResponse=req1)
6. Network.requestWillBeSentExtraInfo (req2)
7. Network.responseReceived (req2)
8. Network.responseReceivedExtraInfo (req2)
9. Network.loadingFinished (req2, только для финального)
```

**Важно:** один `requestId` может породить несколько записей в `requestWillBeSent`. Различайте по `redirectResponse`.

### 1.1.2. `Network.requestWillBeSent` — полная структура

```json
{
  "method": "Network.requestWillBeSent",
  "params": {
    "requestId": "12345.67",
    "loaderId": "ABCDEF1234567890",
    "documentURL": "https://example.com/page",
    "request": {
      "url": "https://api.example.com/data?x=1",
      "urlFragment": null,
      "method": "POST",
      "headers": {
        "Accept": "application/json",
        "Content-Type": "application/json",
        "Referer": "https://example.com/page"
      },
      "postData": "{\"query\":\"hello\"}",
      "postDataEntries": [
        { "bytes": "eyJxdWVyeSI6ImhlbGxvIn0=" }
      ],
      "hasPostData": true,
      "mixedContentType": "none",
      "initialPriority": "High",
      "referrerPolicy": "strict-origin-when-cross-origin",
      "isSameSite": false
    },
    "timestamp": 123456.789,
    "wallTime": 1757500000.123,
    "initiator": {
      "type": "script",
      "stack": {
        "description": "...",
        "callFrames": [
          {
            "functionName": "fetchData",
            "scriptId": "33",
            "url": "https://example.com/app.js",
            "lineNumber": 142,
            "columnNumber": 8
          },
          {
            "functionName": "",
            "scriptId": "33",
            "url": "https://example.com/app.js",
            "lineNumber": 200,
            "columnNumber": 3
          }
        ],
        "parent": { "description": "...", "callFrames": [] }
      }
    },
    "redirectResponse": null,
    "type": "XHR",
    "frameId": "F1",
    "hasUserGesture": false
  }
}
```

Поля, которые надо хранить:

- `requestId` — ключ.
- `loaderId` — меняется при навигации.
- `documentURL` — URL документа, из которого сделан запрос.
- `request.url` — может отличаться от `documentURL`.
- `request.method` — `GET`, `POST`, `PUT`, `DELETE`, `OPTIONS`, `HEAD`, `PATCH`.
- `request.headers` — уже без cookies (они придут в `ExtraInfo`).
- `request.postData` — тело запроса, если есть.
- `request.postDataEntries` — если тело multipart, приходит массив.
- `timestamp` — monotonic, секунды с плавающей точкой (не путать с `Date.now()`).
- `wallTime` — unix timestamp в секундах с плавающей точкой.
- `initiator.type` — `parser`, `script`, `preload`, `preflight`, `signedExchange`, `other`.
- `initiator.stack.callFrames[0]` — верхний кадр, откуда инициирован запрос.
- `type` — тип ресурса: `Document`, `Stylesheet`, `Image`, `Media`, `Font`, `Script`, `TextTrack`, `XHR`, `Fetch`, `Prefetch`, `EventSource`, `WebSocket`, `Manifest`, `SignedExchange`, `Ping`, `CSPViolationReport`, `Preflight`, `Other`.
- `frameId` — если в iframe, иначе ID главного фрейма.

### 1.1.3. `Network.requestWillBeSentExtraInfo` — детали

```json
{
  "method": "Network.requestWillBeSentExtraInfo",
  "params": {
    "requestId": "12345.67",
    "associatedCookies": [
      {
        "cookie": {
          "name": "sessionid",
          "value": "abc123",
          "domain": ".example.com",
          "path": "/",
          "expires": 1757600000.0,
          "size": 20,
          "httpOnly": true,
          "secure": true,
          "session": false,
          "priority": "Medium",
          "sameParty": false,
          "sourceScheme": "Secure",
          "sourcePort": 443
        },
        "blockedReasons": [],
        "accessResult": "Allowed"
      }
    ],
    "headers": {
      "Cookie": "sessionid=abc123; theme=dark",
      "User-Agent": "Mozilla/5.0 ...",
      "Accept-Language": "ru-RU,ru;q=0.9"
    },
    "connectTiming": {
      "requestTime": 123456.123,
      "dnsStart": 0.0,
      "dnsEnd": 0.5,
      "connectStart": 0.5,
      "connectEnd": 2.1,
      "sslStart": 2.1,
      "sslEnd": 8.4,
      "workerStart": -1,
      "workerReady": -1,
      "sendStart": 8.4,
      "sendEnd": 8.5
    },
    "clientSecurityState": {
      "initiatorIsSecureContext": true,
      "initiatorIPAddressSpace": "Public",
      "privateNetworkRequestPolicy": "PreflightBlock"
    },
    "siteHasCookieInOtherPartition": false
  }
}
```

### 1.1.4. `Network.responseReceived` — детали

```json
{
  "method": "Network.responseReceived",
  "params": {
    "requestId": "12345.67",
    "loaderId": "ABCDEF1234567890",
    "timestamp": 123456.900,
    "type": "XHR",
    "response": {
      "url": "https://api.example.com/data?x=1",
      "status": 200,
      "statusText": "OK",
      "headers": {
        "content-type": "application/json; charset=utf-8",
        "cache-control": "no-cache",
        "date": "Sat, 10 Sep 2026 12:00:00 GMT",
        "vary": "Accept-Encoding"
      },
      "mimeType": "application/json",
      "charset": "utf-8",
      "connectionReused": true,
      "connectionId": 42,
      "remoteIPAddress": "1.2.3.4",
      "remotePort": 443,
      "fromDiskCache": false,
      "fromServiceWorker": false,
      "fromPrefetchCache": false,
      "encodedDataLength": 5432,
      "timing": {
        "requestTime": 123456.123,
        "proxyStart": -1,
        "proxyEnd": -1,
        "dnsStart": 0.5,
        "dnsEnd": 1.2,
        "connectStart": 1.2,
        "connectEnd": 8.1,
        "sslStart": 3.0,
        "sslEnd": 8.1,
        "workerStart": -1,
        "workerReady": -1,
        "workerFetchStart": -1,
        "workerRespondWithSettled": -1,
        "sendStart": 8.5,
        "sendEnd": 8.6,
        "pushStart": 0,
        "pushEnd": 0,
        "receiveHeadersStart": 120.0,
        "receiveHeadersEnd": 150.0
      },
      "serviceWorkerResponseSource": "cache-storage",
      "responseTime": 1757500000.456,
      "cacheStorageCacheName": "...",
      "protocol": "h2",
      "securityState": "secure",
      "securityDetails": {
        "protocol": "TLS 1.3",
        "keyExchange": "ECDHE_RSA",
        "cipher": "AES_128_GCM",
        "certificateId": 0,
        "subjectName": "*.example.com",
        "sanList": ["example.com", "*.example.com"],
        "issuer": "Let's Encrypt",
        "validFrom": 1750000000.0,
        "validTo": 1780000000.0,
        "signedCertificateTimestampList": [...],
        "certificateTransparencyCompliance": "compliant",
        "serverSignatureAlgorithm": 1025,
        "encryptedClientHello": false
      }
    },
    "hasExtraInfo": true,
    "frameId": "F1"
  }
}
```

**Важно:** `timing` в `responseReceived` — это относительные времена в миллисекундах от `requestTime`. Поля `-1` означают «фаза не выполнялась».

### 1.1.5. `Network.responseReceivedExtraInfo`

```json
{
  "method": "Network.responseReceivedExtraInfo",
  "params": {
    "requestId": "12345.67",
    "blockedCookies": [],
    "headers": {
      "content-type": "application/json; charset=utf-8",
      "content-encoding": "gzip",
      "set-cookie": "token=xyz; Path=/; Secure; HttpOnly"
    },
    "resourceIPAddressSpace": "Public",
    "statusCode": 200,
    "headersText": "HTTP/1.1 200 OK\r\nContent-Type: ...\r\n\r\n",
    "cookiePartitionKey": null,
    "cookiePartitionKeyOpaque": false
  }
}
```

### 1.1.6. `Network.dataReceived`

```json
{
  "method": "Network.dataReceived",
  "params": {
    "requestId": "12345.67",
    "timestamp": 123456.910,
    "dataLength": 16384,
    "encodedDataLength": 16384
  }
}
```

`dataLength` — длина распакованных данных, `encodedDataLength` — сколько реально пришло по сети.

### 1.1.7. `Network.loadingFinished`

```json
{
  "method": "Network.loadingFinished",
  "params": {
    "requestId": "12345.67",
    "timestamp": 123456.920,
    "encodedDataLength": 5432,
    "shouldReportCorbBlocking": false
  }
}
```

**Тут же — вызывать `Network.getResponseBody`.**

### 1.1.8. `Network.loadingFailed`

```json
{
  "method": "Network.loadingFailed",
  "params": {
    "requestId": "12345.68",
    "timestamp": 123456.930,
    "type": "Image",
    "errorText": "net::ERR_BLOCKED_BY_CLIENT",
    "canceled": true,
    "blockedReason": "inspector",
    "corsErrorStatus": {
      "corsError": "MissingAllowOriginHeader",
      "failedParameter": "Origin"
    }
  }
}
```

`blockedReason` бывает:
- `csp` — блокировка Content Security Policy.
- `mixed-content` — HTTP на HTTPS-странице.
- `origin` — Same-Origin Policy.
- `inspector` — блокировка через DevTools / `Network.setBlockedURLs`.
- `subresource-filter` — блокировка adblock'ом.
- `content-type` — неверный MIME.
- `coep-frame-resource-needs-coep-header`
- `coop-sandboxed-iframe-cannot-navigate-to-coop-page`
- `corp-not-same-origin`
- `corp-not-same-origin-after-defaulted-to-same-origin-by-coep`
- `corp-not-same-origin-after-defaulted-to-same-origin-by-dip`
- `corp-not-same-site`

### 1.1.9. WebSocket-события

```json
{ "method": "Network.webSocketCreated", "params": { "requestId": "...", "url": "wss://...", "initiator": {...} } }
{ "method": "Network.webSocketWillSendHandshakeRequest", "params": { "requestId": "...", "timestamp": ..., "wallTime": ..., "request": { "headers": {...} } } }
{ "method": "Network.webSocketHandshakeResponseReceived", "params": { "requestId": "...", "timestamp": ..., "response": { "status": 101, "headers": {...} } } }
{ "method": "Network.webSocketFrameSent", "params": { "requestId": "...", "timestamp": ..., "response": { "opcode": 1, "mask": true, "payloadData": "hello" } } }
{ "method": "Network.webSocketFrameReceived", "params": { "requestId": "...", "timestamp": ..., "response": { "opcode": 1, "mask": false, "payloadData": "world" } } }
{ "method": "Network.webSocketFrameError", "params": { "requestId": "...", "timestamp": ..., "errorMessage": "..." } }
{ "method": "Network.webSocketClosed", "params": { "requestId": "...", "timestamp": ... } }
```

`opcode`: 1 = text, 2 = binary, 8 = close, 9 = ping, 10 = pong.

### 1.1.10. EventSource

Отдельных событий нет — SSE приходит как `dataReceived` для запроса типа `EventSource`. Чтобы читать сообщения, надо собирать тело через `getResponseBody` после закрытия — но SSE не закрывается. Практически — пропускать.

### 1.1.11. Runtime: события

**`Runtime.consoleAPICalled`** — полная структура:

```json
{
  "method": "Runtime.consoleAPICalled",
  "params": {
    "type": "log",
    "args": [
      { "type": "string", "value": "hello" },
      { "type": "number", "value": 42, "description": "42" },
      { "type": "object", "className": "Object", "description": "Object", "objectId": "12345.67.1" },
      { "type": "function", "className": "Function", "description": "function foo() {}", "objectId": "12345.67.2" },
      { "type": "undefined" }
    ],
    "executionContextId": 3,
    "timestamp": 1757500000.123,
    "stackTrace": {
      "callFrames": [
        { "functionName": "doSomething", "scriptId": "33", "url": "https://.../app.js", "lineNumber": 42, "columnNumber": 8 }
      ]
    },
    "context": "https://example.com/page"
  }
}
```

Типы `type`: `log`, `debug`, `info`, `error`, `warning`, `dir`, `dirxml`, `table`, `trace`, `clear`, `startGroup`, `startGroupCollapsed`, `endGroup`, `assert`, `profile`, `profileEnd`, `count`, `timeEnd`.

Типы значений в `args[].type`: `object`, `function`, `undefined`, `string`, `number`, `boolean`, `symbol`, `bigint`.

Если нужен `value` для `object` — вызывать `Runtime.getProperties({objectId, ownProperties:true, generatePreview:true})`.

**`Runtime.exceptionThrown`**:

```json
{
  "method": "Runtime.exceptionThrown",
  "params": {
    "timestamp": 1757500000.123,
    "exceptionDetails": {
      "exceptionId": 1,
      "text": "Uncaught",
      "lineNumber": 42,
      "columnNumber": 8,
      "scriptId": "33",
      "url": "https://.../app.js",
      "stackTrace": {
        "description": "TypeError: x is undefined",
        "callFrames": [
          { "functionName": "foo", "scriptId": "33", "url": "...", "lineNumber": 42, "columnNumber": 8 }
        ],
        "parent": null,
        "parentId": { "scriptId": "33", "url": "...", "lineNumber": 0, "columnNumber": 0 }
      },
      "exception": {
        "type": "object",
        "subtype": "error",
        "className": "TypeError",
        "description": "TypeError: x is undefined\n    at foo (app.js:42:8)",
        "objectId": "12345.67.3",
        "preview": {
          "type": "object",
          "subtype": "error",
          "description": "TypeError: x is undefined",
          "overflow": false,
          "properties": [
            { "name": "message", "type": "string", "value": "x is undefined" },
            { "name": "stack", "type": "string", "value": "TypeError: x is undefined\n    at ..." }
          ]
        }
      },
      "executionContextId": 3,
      "exceptionMetaData": null
    }
  }
}
```

**`Runtime.executionContextCreated`**:

```json
{
  "method": "Runtime.executionContextCreated",
  "params": {
    "context": {
      "id": 3,
      "origin": "https://example.com",
      "name": "",
      "uniqueId": "12345.3",
      "auxData": {
        "isDefault": true,
        "type": "default",
        "frameId": "F1"
      }
    }
  }
}
```

`auxData.type`: `default`, `isolated`, `worker`.

**`Runtime.executionContextDestroyed`**:

```json
{ "method": "Runtime.executionContextDestroyed", "params": { "executionContextId": 3, "executionContextUniqueId": "12345.3" } }
```

**`Runtime.executionContextsCleared`** — все контексты сброшены (обычно при навигации).

**`Runtime.bindingCalled`**:

```json
{
  "method": "Runtime.bindingCalled",
  "params": {
    "name": "__aiCallback",
    "payload": "{\"event\":\"ready\"}",
    "executionContextId": 3
  }
}
```

### 1.1.12. Page: события

**`Page.frameAttached`**:
```json
{ "params": { "frameId": "F2", "parentFrameId": "F1", "stack": { "callFrames": [...] } } }
```

**`Page.frameNavigated`**:
```json
{
  "params": {
    "frame": {
      "id": "F2",
      "parentId": "F1",
      "loaderId": "ABC123",
      "name": "widget",
      "url": "https://widget.example.com/",
      "urlFragment": "#section",
      "domainAndRegistry": "example.com",
      "securityOrigin": "https://widget.example.com",
      "mimeType": "text/html",
      "secureContextType": "SecureLocalhost",
      "crossOriginIsolated": false,
      "adFrameType": "none"
    },
    "type": "Navigation"
  }
}
```

`adFrameType`: `none`, `child`, `root`.

**`Page.frameDetached`**:
```json
{ "params": { "frameId": "F2", "reason": "remove" } }
```

`reason`: `remove`, `swap`.

**`Page.lifecycleEvent`**:
```json
{
  "params": {
    "frameId": "F1",
    "loaderId": "ABC123",
    "name": "networkIdle",
    "timestamp": 123456.789
  }
}
```

Имена фаз (в порядке):
- `init`
- `firstPaint`
- `firstContentfulPaint`
- `firstImagePaint`
- `firstMeaningfulPaintCandidate` (deprecated)
- `DOMContentLoaded`
- `load`
- `networkAlmostIdle`
- `networkIdle`
- `commit`

**`Page.loadEventFired`**:
```json
{ "params": { "timestamp": 123456.789 } }
```

**`Page.domContentEventFired`**:
```json
{ "params": { "timestamp": 123456.789 } }
```

**`Page.javascriptDialogOpening`**:
```json
{
  "params": {
    "url": "https://example.com",
    "message": "Are you sure?",
    "type": "confirm",
    "hasBrowserHandler": false,
    "defaultPrompt": ""
  }
}
```

`type`: `alert`, `confirm`, `prompt`, `beforeunload`.

Закрывать через `Page.handleJavaScriptDialog({accept: true, promptText: ""})`.

**`Page.fileChooserOpened`**:
```json
{ "params": { "frameId": "F1", "mode": "selectSingle", "backendNodeId": 123 } }
```

**`Page.downloadWillBegin`** / **`Page.downloadProgress`** — для скачиваний.

### 1.1.13. DOM: события

**`DOM.documentUpdated`** — новый документ. Все старые `nodeId` невалидны.

**`DOM.childNodeInserted`**:
```json
{
  "params": {
    "parentNodeId": 42,
    "previousNodeId": 41,
    "node": {
      "nodeId": 43,
      "backendNodeId": 100,
      "nodeType": 1,
      "nodeName": "DIV",
      "localName": "div",
      "nodeValue": "",
      "attributes": ["class", "result", "id", "output"]
    }
  }
}
```

`attributes` — плоский массив `[name1, value1, name2, value2, ...]`.

**`DOM.childNodeRemoved`**, **`DOM.attributeModified`**, **`DOM.attributeRemoved`**, **`DOM.characterDataModified`** — аналогично.

**`DOM.setChildNodes`**:
```json
{
  "params": {
    "parentId": 42,
    "nodes": [ { "nodeId": 43, ... }, { "nodeId": 44, ... } ]
  }
}
```

Приходит, когда `DOM.getDocument(depth: 0)` не смог вернуть всё сразу.

**`DOM.attributeModified`**:
```json
{ "params": { "nodeId": 43, "name": "class", "value": "result highlighted" } }
```

### 1.1.14. Target: события

**`Target.attachedToTarget`**:
```json
{
  "params": {
    "sessionId": "S1",
    "targetInfo": {
      "targetId": "T1",
      "type": "iframe",
      "title": "Widget",
      "url": "https://widget.example.com/",
      "attached": true,
      "canAccessOpener": false,
      "browserContextId": "BC1"
    },
    "waitingForDebugger": false
  }
}
```

`type`: `page`, `iframe`, `worker`, `shared_worker`, `service_worker`, `browser`, `other`, `tab`, `auction_worklet`, `shared_storage_worklet`.

**`Target.detachedFromTarget`**:
```json
{ "params": { "sessionId": "S1", "targetId": "T1" } }
```

**`Target.targetCreated`** / **`targetInfoChanged`** / **`targetDestroyed`** — если `Target.setDiscoverTargets({discover: true})`.

### 1.1.15. Debugger: события

**`Debugger.scriptParsed`**:
```json
{
  "params": {
    "scriptId": "33",
    "url": "https://example.com/app.js",
    "startLine": 0,
    "startColumn": 0,
    "endLine": 10000,
    "endColumn": 5,
    "executionContextId": 3,
    "hash": "abc123...",
    "buildId": "",
    "executionContextAuxData": { "isDefault": true, "type": "default", "frameId": "F1" },
    "isLiveEdit": false,
    "sourceMapURL": "app.js.map",
    "hasSourceURL": false,
    "isModule": true,
    "length": 1234567,
    "stackTrace": null,
    "codeOffset": 0,
    "scriptLanguage": "JavaScript",
    "embedderName": null,
    "debugSymbols": null
  }
}
```

**`Debugger.scriptFailedToParse`** — синтаксическая ошибка при парсинге.

**`Debugger.paused`** — если стоит breakpoint или `debugger;` в коде:

```json
{
  "params": {
    "callFrames": [
      {
        "callFrameId": "0",
        "functionName": "foo",
        "functionLocation": { "scriptId": "33", "lineNumber": 42, "columnNumber": 0 },
        "location": { "scriptId": "33", "lineNumber": 45, "columnNumber": 8 },
        "url": "https://.../app.js",
        "scopeChain": [...],
        "this": {...},
        "returnValue": null
      }
    ],
    "reason": "other",
    "data": null,
    "hitBreakpoints": [],
    "asyncStackTrace": null
  }
}
```

### 1.1.16. Log: события

**`Log.entryAdded`**:
```json
{
  "params": {
    "entry": {
      "source": "network",
      "level": "error",
      "text": "Mixed Content: The page at 'https://...' was loaded over HTTPS, but requested an insecure resource 'http://...'",
      "timestamp": 1757500000.123,
      "url": "https://example.com/",
      "lineNumber": 0,
      "stackTrace": null,
      "networkRequestId": "12345.67",
      "workerId": "",
      "args": null
    }
  }
}
```

`source`: `xml`, `javascript`, `network`, `storage`, `appcache`, `rendering`, `security`, `deprecation`, `worker`, `violation`, `intervention`, `recommendation`, `other`.
`level`: `verbose`, `info`, `warning`, `error`.

### 1.1.17. Inspector: события

**`Inspector.targetCrashed`**, **`Inspector.detached`** — если target падает.

---

## 1.2. Полная реализация журналов на C++

### 1.2.1. Структуры данных

```cpp
namespace cdt_analysis {

using json = marty::cdt::json;

struct HeaderMap : public std::map<std::string, std::string, std::less<>> {
    void fromJson(const json& j) {
        for (auto it = j.begin(); it != j.end(); ++it) {
            if (it.value().is_string())
                emplace(it.key(), it.value().get<std::string>());
            else
                emplace(it.key(), it.value().dump());
        }
    }
    json toJson() const {
        json j = json::object();
        for (auto& [k, v] : *this) j[k] = v;
        return j;
    }
};

struct InitiatorFrame {
    std::string functionName;
    std::string scriptId;
    std::string url;
    int lineNumber = -1;
    int columnNumber = -1;
};

struct Initiator {
    std::string type;                        // parser/script/preload/...
    std::vector<InitiatorFrame> frames;      // верхние 5-10
    std::string topUrl;                      // frames.empty() ? "" : frames[0].url
    int topLine = -1;
};

struct RequestWillBeSentRecord {
    std::string requestId;
    std::string loaderId;
    std::string documentURL;
    std::string url;
    std::string urlFragment;
    std::string method;
    HeaderMap   headers;
    std::string postData;                    // как пришло
    std::vector<std::pair<std::string,std::string>> postDataEntries;  // name -> base64
    bool        hasPostData = false;
    std::string mixedContentType;
    std::string initialPriority;
    std::string referrerPolicy;
    bool        isSameSite = false;
    double      timestamp = 0.0;             // monotonic
    double      wallTime = 0.0;              // unix
    Initiator   initiator;
    std::string type;                        // Document/Script/XHR/...
    std::string frameId;
    bool        hasUserGesture = false;

    // Заполняется, если это редирект
    bool        isRedirect = false;
    std::string redirectFromRequestId;
    int         redirectFromStatus = 0;
};

struct RequestExtraInfoRecord {
    HeaderMap   headers;
    std::vector<json> associatedCookies;     // сохраняем как есть
    json        connectTiming;
    json        clientSecurityState;
    bool        siteHasCookieInOtherPartition = false;
};

struct TimingBreakdown {
    double requestTime = 0.0;
    double proxyStart = -1, proxyEnd = -1;
    double dnsStart = -1, dnsEnd = -1;
    double connectStart = -1, connectEnd = -1;
    double sslStart = -1, sslEnd = -1;
    double workerStart = -1, workerReady = -1;
    double workerFetchStart = -1, workerRespondWithSettled = -1;
    double sendStart = -1, sendEnd = -1;
    double pushStart = -1, pushEnd = -1;
    double receiveHeadersStart = -1, receiveHeadersEnd = -1;

    static TimingBreakdown fromJson(const json& j) {
        TimingBreakdown t;
        auto g = [&](const char* k, double& out) {
            if (j.contains(k) && j[k].is_number()) out = j[k].get<double>();
        };
        g("requestTime", t.requestTime);
        g("proxyStart", t.proxyStart); g("proxyEnd", t.proxyEnd);
        g("dnsStart", t.dnsStart); g("dnsEnd", t.dnsEnd);
        g("connectStart", t.connectStart); g("connectEnd", t.connectEnd);
        g("sslStart", t.sslStart); g("sslEnd", t.sslEnd);
        g("workerStart", t.workerStart); g("workerReady", t.workerReady);
        g("workerFetchStart", t.workerFetchStart);
        g("workerRespondWithSettled", t.workerRespondWithSettled);
        g("sendStart", t.sendStart); g("sendEnd", t.sendEnd);
        g("pushStart", t.pushStart); g("pushEnd", t.pushEnd);
        g("receiveHeadersStart", t.receiveHeadersStart);
        g("receiveHeadersEnd", t.receiveHeadersEnd);
        return t;
    }

    // Все времена в миллисекундах относительно requestTime
    double tcpConnectDuration() const { return connectEnd - connectStart; }
    double dnsDuration() const { return dnsEnd - dnsStart; }
    double sslDuration() const { return sslEnd - sslStart; }
    double ttfb() const { return receiveHeadersEnd - sendStart; }
    double totalDuration() const { return receiveHeadersEnd; }
};

struct ResponseReceivedRecord {
    std::string requestId;
    std::string loaderId;
    double      timestamp = 0.0;
    std::string type;
    std::string url;
    int         status = 0;
    std::string statusText;
    HeaderMap   headers;
    std::string mimeType;
    std::string charset;
    bool        connectionReused = false;
    int         connectionId = 0;
    std::string remoteIPAddress;
    int         remotePort = 0;
    bool        fromDiskCache = false;
    bool        fromServiceWorker = false;
    bool        fromPrefetchCache = false;
    std::int64_t encodedDataLength = 0;
    TimingBreakdown timing;
    std::string serviceWorkerResponseSource;
    double      responseTime = 0.0;
    std::string protocol;
    std::string securityState;
    json        securityDetails;
    std::string frameId;
};

struct ResponseExtraInfoRecord {
    HeaderMap   headers;
    std::vector<json> blockedCookies;
    std::string resourceIPAddressSpace;
    int         statusCode = 0;
    std::string headersText;                 // сырой текст ответа
    json        cookiePartitionKey;
    bool        cookiePartitionKeyOpaque = false;
};

struct BodyRecord {
    std::string requestId;
    std::string body;                        // если base64Encoded — то base64
    bool        base64 = false;
    bool        truncated = false;
    bool        available = false;
    std::string errorMessage;                // если не удалось получить
};

struct LoadingFailedRecord {
    std::string requestId;
    double      timestamp = 0.0;
    std::string type;
    std::string errorText;
    bool        canceled = false;
    std::string blockedReason;
    json        corsErrorStatus;
};

struct UnifiedRequest {
    std::string requestId;
    std::string loaderId;
    std::string frameId;
    std::string documentURL;
    std::string url;
    std::string method;
    std::string type;

    // Может быть несколько (при редиректах)
    std::vector<RequestWillBeSentRecord> requests;
    std::vector<ResponseReceivedRecord>  responses;

    // Финальное состояние
    bool        finished = false;
    bool        failed = false;
    double      startTime = 0.0;             // timestamp первого requestWillBeSent
    double      endTime = 0.0;               // timestamp loadingFinished
    std::int64_t totalEncodedDataLength = 0;

    // Заголовки из ExtraInfo (последние актуальные)
    HeaderMap   finalRequestHeaders;
    HeaderMap   finalResponseHeaders;
    std::vector<json> associatedCookies;

    // Тело финального ответа
    BodyRecord  body;
};

} // namespace cdt_analysis
```

### 1.2.2. `NetworkJournal`

```cpp
class NetworkJournal {
public:
    using Ptr = std::shared_ptr<NetworkJournal>;

    void onRequestWillBeSent(const json& p) {
        std::lock_guard lk(mtx_);

        auto& u = getOrCreateUnified_(p["requestId"].get<std::string>());

        RequestWillBeSentRecord r;
        r.requestId  = p["requestId"];
        r.loaderId   = p.value("loaderId", "");
        r.documentURL= p.value("documentURL", "");

        const auto& req = p["request"];
        r.url        = req.value("url", "");
        r.urlFragment= req.value("urlFragment", "");
        r.method     = req.value("method", "GET");
        if (req.contains("headers") && req["headers"].is_object())
            r.headers.fromJson(req["headers"]);
        r.postData   = req.value("postData", "");
        r.hasPostData= req.value("hasPostData", false);
        if (req.contains("postDataEntries") && req["postDataEntries"].is_array()) {
            for (auto& e : req["postDataEntries"])
                r.postDataEntries.emplace_back(e.value("bytes", ""), "");
        }
        r.mixedContentType  = req.value("mixedContentType", "");
        r.initialPriority   = req.value("initialPriority", "");
        r.referrerPolicy    = req.value("referrerPolicy", "");
        r.isSameSite        = req.value("isSameSite", false);

        r.timestamp = p.value("timestamp", 0.0);
        r.wallTime  = p.value("wallTime", 0.0);

        if (p.contains("initiator") && p["initiator"].is_object()) {
            const auto& ini = p["initiator"];
            r.initiator.type = ini.value("type", "");
            if (ini.contains("stack") && ini["stack"].is_object()) {
                const auto& st = ini["stack"];
                if (st.contains("callFrames") && st["callFrames"].is_array()) {
                    size_t n = std::min<size_t>(10, st["callFrames"].size());
                    for (size_t i = 0; i < n; ++i) {
                        const auto& cf = st["callFrames"][i];
                        InitiatorFrame f;
                        f.functionName = cf.value("functionName", "");
                        f.scriptId     = cf.value("scriptId", "");
                        f.url          = cf.value("url", "");
                        f.lineNumber   = cf.value("lineNumber", -1);
                        f.columnNumber = cf.value("columnNumber", -1);
                        r.initiator.frames.push_back(std::move(f));
                    }
                    if (!r.initiator.frames.empty()) {
                        r.initiator.topUrl  = r.initiator.frames[0].url;
                        r.initiator.topLine = r.initiator.frames[0].lineNumber;
                    }
                }
            }
        }

        r.type    = p.value("type", "Other");
        r.frameId = p.value("frameId", "");
        r.hasUserGesture = p.value("hasUserGesture", false);

        if (p.contains("redirectResponse") && !p["redirectResponse"].is_null()) {
            r.isRedirect = true;
            if (p["redirectResponse"].contains("status"))
                r.redirectFromStatus = p["redirectResponse"]["status"].get<int>();
            // Связь с предыдущим — по requestId (он тот же), но previous лежит в responses
            if (!u.responses.empty())
                r.redirectFromRequestId = u.responses.back().requestId;
        }

        // Обновляем агрегат
        u.url    = r.url;
        u.method = r.method;
        u.type   = r.type;
        u.frameId = r.frameId;
        if (u.documentURL.empty()) u.documentURL = r.documentURL;
        if (u.startTime == 0.0)    u.startTime   = r.timestamp;

        u.requests.push_back(std::move(r));

        order_.push_back(u.requestId);
    }

    void onRequestExtraInfo(const json& p) {
        std::lock_guard lk(mtx_);
        auto it = unified_.find(p["requestId"].get<std::string>());
        if (it == unified_.end()) return;
        auto& u = it->second;

        RequestExtraInfoRecord r;
        if (p.contains("headers")) r.headers.fromJson(p["headers"]);
        if (p.contains("associatedCookies"))
            for (auto& c : p["associatedCookies"]) r.associatedCookies.push_back(c);
        if (p.contains("connectTiming")) r.connectTiming = p["connectTiming"];
        if (p.contains("clientSecurityState")) r.clientSecurityState = p["clientSecurityState"];
        r.siteHasCookieInOtherPartition = p.value("siteHasCookieInOtherPartition", false);

        u.finalRequestHeaders = std::move(r.headers);
        u.associatedCookies   = std::move(r.associatedCookies);
    }

    void onResponseReceived(const json& p) {
        std::lock_guard lk(mtx_);
        auto it = unified_.find(p["requestId"].get<std::string>());
        if (it == unified_.end()) return;
        auto& u = it->second;

        ResponseReceivedRecord r;
        r.requestId = p["requestId"];
        r.loaderId  = p.value("loaderId", "");
        r.timestamp = p.value("timestamp", 0.0);
        r.type      = p.value("type", "");
        r.frameId   = p.value("frameId", "");

        const auto& res = p["response"];
        r.url            = res.value("url", "");
        r.status         = res.value("status", 0);
        r.statusText     = res.value("statusText", "");
        if (res.contains("headers")) r.headers.fromJson(res["headers"]);
        r.mimeType       = res.value("mimeType", "");
        r.charset        = res.value("charset", "");
        r.connectionReused = res.value("connectionReused", false);
        r.connectionId   = res.value("connectionId", 0);
        r.remoteIPAddress= res.value("remoteIPAddress", "");
        r.remotePort     = res.value("remotePort", 0);
        r.fromDiskCache  = res.value("fromDiskCache", false);
        r.fromServiceWorker = res.value("fromServiceWorker", false);
        r.fromPrefetchCache = res.value("fromPrefetchCache", false);
        if (res.contains("encodedDataLength") && res["encodedDataLength"].is_number())
            r.encodedDataLength = (std::int64_t)res["encodedDataLength"].get<double>();
        if (res.contains("timing")) r.timing = TimingBreakdown::fromJson(res["timing"]);
        r.serviceWorkerResponseSource = res.value("serviceWorkerResponseSource", "");
        r.responseTime   = res.value("responseTime", 0.0);
        r.protocol       = res.value("protocol", "");
        r.securityState  = res.value("securityState", "");
        if (res.contains("securityDetails")) r.securityDetails = res["securityDetails"];

        u.responses.push_back(std::move(r));
    }

    void onResponseExtraInfo(const json& p) {
        std::lock_guard lk(mtx_);
        auto it = unified_.find(p["requestId"].get<std::string>());
        if (it == unified_.end()) return;
        auto& u = it->second;

        ResponseExtraInfoRecord r;
        if (p.contains("headers")) r.headers.fromJson(p["headers"]);
        if (p.contains("blockedCookies"))
            for (auto& c : p["blockedCookies"]) r.blockedCookies.push_back(c);
        r.resourceIPAddressSpace = p.value("resourceIPAddressSpace", "");
        r.statusCode = p.value("statusCode", 0);
        r.headersText = p.value("headersText", "");
        if (p.contains("cookiePartitionKey")) r.cookiePartitionKey = p["cookiePartitionKey"];
        r.cookiePartitionKeyOpaque = p.value("cookiePartitionKeyOpaque", false);

        u.finalResponseHeaders = std::move(r.headers);
    }

    void onLoadingFinished(const json& p) {
        std::lock_guard lk(mtx_);
        auto it = unified_.find(p["requestId"].get<std::string>());
        if (it == unified_.end()) return;
        auto& u = it->second;
        u.finished = true;
        u.endTime = p.value("timestamp", 0.0);
        if (p.contains("encodedDataLength") && p["encodedDataLength"].is_number())
            u.totalEncodedDataLength = (std::int64_t)p["encodedDataLength"].get<double>();
        // ВАЖНО: тело запросит внешний код, когда получит уведомление
        pendingBodies_.push_back(u.requestId);
    }

    void onLoadingFailed(const json& p) {
        std::lock_guard lk(mtx_);
        auto it = unified_.find(p["requestId"].get<std::string>());
        if (it == unified_.end()) return;
        auto& u = it->second;
        u.failed = true;
        u.finished = true;
        u.endTime = p.value("timestamp", 0.0);

        LoadingFailedRecord r;
        r.requestId = p["requestId"];
        r.timestamp = p.value("timestamp", 0.0);
        r.type      = p.value("type", "");
        r.errorText = p.value("errorText", "");
        r.canceled  = p.value("canceled", false);
        r.blockedReason = p.value("blockedReason", "");
        if (p.contains("corsErrorStatus")) r.corsErrorStatus = p["corsErrorStatus"];
        u.failure = std::move(r);
    }

    void setBody(const std::string& requestId, BodyRecord body) {
        std::lock_guard lk(mtx_);
        auto it = unified_.find(requestId);
        if (it != unified_.end()) it->second.body = std::move(body);
    }

    // Возвращает requestId, для которых надо запросить тело
    std::vector<std::string> takePendingBodies() {
        std::lock_guard lk(mtx_);
        auto out = std::move(pendingBodies_);
        pendingBodies_.clear();
        return out;
    }

    // Чтение
    std::optional<UnifiedRequest> get(const std::string& requestId) const {
        std::lock_guard lk(mtx_);
        auto it = unified_.find(requestId);
        if (it == unified_.end()) return std::nullopt;
        return it->second;
    }

    std::vector<UnifiedRequest> getAllOrdered() const {
        std::lock_guard lk(mtx_);
        std::vector<UnifiedRequest> out;
        out.reserve(order_.size());
        for (auto& id : order_) {
            auto it = unified_.find(id);
            if (it != unified_.end()) out.push_back(it->second);
        }
        return out;
    }

private:
    UnifiedRequest& getOrCreateUnified_(const std::string& id) {
        auto [it, inserted] = unified_.try_emplace(id);
        if (inserted) it->second.requestId = id;
        return it->second;
    }

    mutable std::mutex mtx_;
    std::unordered_map<std::string, UnifiedRequest> unified_;
    std::vector<std::string> order_;
    std::vector<std::string> pendingBodies_;
};
```

### 1.2.3. `ConsoleJournal`

```cpp
class ConsoleJournal {
public:
    struct Message {
        std::string level;                    // log/warn/error/info/debug/...
        std::vector<json> args;
        std::vector<std::string> argTexts;    // отрендеренные строки
        double timestamp = 0.0;
        std::string url;
        int line = -1;
        std::vector<std::string> stackTop;    // до 5 кадров
    };

    struct Exception {
        std::string text;
        std::string className;
        std::string description;
        std::string url;
        int line = -1, column = -1;
        std::vector<std::string> stackTop;
        double timestamp = 0.0;
    };

    void onConsoleAPICalled(const json& p) {
        std::lock_guard lk(mtx_);
        Message m;
        m.level = p.value("type", "log");
        m.timestamp = p.value("timestamp", 0.0);

        if (p.contains("args")) {
            for (auto& a : p["args"]) {
                m.args.push_back(a);
                m.argTexts.push_back(renderArg_(a));
            }
        }
        if (p.contains("stackTrace") && p["stackTrace"].contains("callFrames")) {
            auto& cfs = p["stackTrace"]["callFrames"];
            size_t n = std::min<size_t>(5, cfs.size());
            for (size_t i = 0; i < n; ++i) {
                const auto& cf = cfs[i];
                std::string s = cf.value("functionName", "(anonymous)");
                s += " (";
                s += cf.value("url", "?");
                s += ":";
                s += std::to_string(cf.value("lineNumber", 0) + 1);
                s += ")";
                m.stackTop.push_back(std::move(s));
            }
            if (!cfs.empty()) {
                m.url = cfs[0].value("url", "");
                m.line = cfs[0].value("lineNumber", -1);
            }
        }
        messages_.push_back(std::move(m));
    }

    void onExceptionThrown(const json& p) {
        std::lock_guard lk(mtx_);
        Exception e;
        e.timestamp = p.value("timestamp", 0.0);

        if (p.contains("exceptionDetails")) {
            const auto& d = p["exceptionDetails"];
            e.text = d.value("text", "");
            e.url  = d.value("url", "");
            e.line = d.value("lineNumber", -1);
            e.column = d.value("columnNumber", -1);

            if (d.contains("exception") && d["exception"].is_object()) {
                const auto& ex = d["exception"];
                e.className   = ex.value("className", "");
                e.description = ex.value("description", "");
            }

            if (d.contains("stackTrace") && d["stackTrace"].contains("callFrames")) {
                auto& cfs = d["stackTrace"]["callFrames"];
                size_t n = std::min<size_t>(5, cfs.size());
                for (size_t i = 0; i < n; ++i) {
                    const auto& cf = cfs[i];
                    std::string s = cf.value("functionName", "(anonymous)");
                    s += " (" + cf.value("url", "?") + ":";
                    s += std::to_string(cf.value("lineNumber", 0) + 1) + ")";
                    e.stackTop.push_back(std::move(s));
                }
                if (!cfs.empty()) {
                    if (e.url.empty()) e.url = cfs[0].value("url", "");
                    if (e.line < 0)    e.line = cfs[0].value("lineNumber", -1);
                }
            }
        }
        exceptions_.push_back(std::move(e));
    }

    std::vector<Message> messages() const { std::lock_guard lk(mtx_); return messages_; }
    std::vector<Exception> exceptions() const { std::lock_guard lk(mtx_); return exceptions_; }

private:
    static std::string renderArg_(const json& a) {
        if (a.contains("value")) {
            if (a["value"].is_string())  return a["value"].get<std::string>();
            if (a["value"].is_number())  return a["value"].dump();
            if (a["value"].is_boolean()) return a["value"].get<bool>() ? "true" : "false";
            if (a["value"].is_null())    return "null";
            return a["value"].dump();
        }
        if (a.contains("description")) return a["description"].get<std::string>();
        return "<" + a.value("type", "unknown") + ">";
    }

    mutable std::mutex mtx_;
    std::vector<Message> messages_;
    std::vector<Exception> exceptions_;
};
```

### 1.2.4. `DomJournal`

```cpp
class DomJournal {
public:
    struct Frame {
        std::string frameId;
        std::string parentFrameId;
        std::string url;
        std::string name;
        std::string securityOrigin;
        std::string mimeType;
        bool isMain = false;
        double lastNavigated = 0.0;
    };

    struct Context {
        int id = 0;
        std::string uniqueId;
        std::string origin;
        std::string name;
        std::string type;      // default/isolated/worker
        std::string frameId;   // пусто, если worker
        bool isDefault = false;
    };

    void onFrameAttached(const json& p, const std::string& sessionId) {
        std::lock_guard lk(mtx_);
        Frame f;
        f.frameId = p.value("frameId", "");
        f.parentFrameId = p.value("parentFrameId", "");
        frames_[f.frameId] = std::move(f);
        frameSession_[frames_[f.frameId].frameId] = sessionId;
    }

    void onFrameNavigated(const json& p, const std::string& sessionId) {
        std::lock_guard lk(mtx_);
        const auto& fr = p["frame"];
        Frame f;
        f.frameId = fr.value("id", "");
        f.parentFrameId = fr.value("parentId", "");
        f.url = fr.value("url", "");
        f.name = fr.value("name", "");
        f.securityOrigin = fr.value("securityOrigin", "");
        f.mimeType = fr.value("mimeType", "");
        f.isMain = f.parentFrameId.empty();
        f.lastNavigated = p.value("timestamp", 0.0);
        frames_[f.frameId] = std::move(f);
        frameSession_[frames_[f.frameId].frameId] = sessionId;
    }

    void onFrameDetached(const json& p) {
        std::lock_guard lk(mtx_);
        auto id = p.value("frameId", "");
        frames_.erase(id);
        frameSession_.erase(id);
    }

    void onExecutionContextCreated(const json& p, const std::string& sessionId) {
        std::lock_guard lk(mtx_);
        const auto& c = p["context"];
        Context ctx;
        ctx.id = c.value("id", 0);
        ctx.uniqueId = c.value("uniqueId", "");
        ctx.origin = c.value("origin", "");
        ctx.name = c.value("name", "");
        if (c.contains("auxData") && c["auxData"].is_object()) {
            const auto& a = c["auxData"];
            ctx.type = a.value("type", "");
            ctx.frameId = a.value("frameId", "");
            ctx.isDefault = a.value("isDefault", false);
        }
        contexts_[ctx.id] = std::move(ctx);
        contextSession_[ctx.id] = sessionId;
    }

    void onExecutionContextDestroyed(const json& p) {
        std::lock_guard lk(mtx_);
        int id = p.value("executionContextId", 0);
        contexts_.erase(id);
        contextSession_.erase(id);
    }

    void onExecutionContextsCleared() {
        std::lock_guard lk(mtx_);
        contexts_.clear();
        contextSession_.clear();
    }

    std::optional<Frame> getFrame(const std::string& frameId) const {
        std::lock_guard lk(mtx_);
        auto it = frames_.find(frameId);
        if (it == frames_.end()) return std::nullopt;
        return it->second;
    }

    std::string getMainFrameId() const {
        std::lock_guard lk(mtx_);
        for (auto& [id, f] : frames_) if (f.isMain) return id;
        return "";
    }

    std::string getSessionForFrame(const std::string& frameId) const {
        std::lock_guard lk(mtx_);
        auto it = frameSession_.find(frameId);
        return it == frameSession_.end() ? "" : it->second;
    }

    std::optional<Context> getContextForFrame(const std::string& frameId) const {
        std::lock_guard lk(mtx_);
        for (auto& [id, c] : contexts_)
            if (c.frameId == frameId && c.isDefault) return c;
        return std::nullopt;
    }

    std::vector<Frame> allFrames() const {
        std::lock_guard lk(mtx_);
        std::vector<Frame> out;
        for (auto& [id, f] : frames_) out.push_back(f);
        return out;
    }

private:
    mutable std::mutex mtx_;
    std::unordered_map<std::string, Frame> frames_;
    std::unordered_map<std::string, std::string> frameSession_;
    std::unordered_map<int, Context> contexts_;
    std::unordered_map<int, std::string> contextSession_;
};
```

### 1.2.5. `TargetJournal` (сессии)

```cpp
class TargetJournal {
public:
    void onAttached(const json& p) {
        std::lock_guard lk(mtx_);
        auto sessionId = p["sessionId"].get<std::string>();
        const auto& t = p["targetInfo"];
        TargetInfo info;
        info.targetId = t.value("targetId", "");
        info.type = t.value("type", "");
        info.title = t.value("title", "");
        info.url = t.value("url", "");
        info.attached = t.value("attached", true);
        info.browserContextId = t.value("browserContextId", "");

        sessions_[sessionId] = info;
        targetToSession_[info.targetId] = sessionId;
    }

    void onDetached(const json& p) {
        std::lock_guard lk(mtx_);
        auto sessionId = p.value("sessionId", "");
        auto it = sessions_.find(sessionId);
        if (it != sessions_.end()) {
            targetToSession_.erase(it->second.targetId);
            sessions_.erase(it);
        }
    }

    std::optional<TargetInfo> getSession(const std::string& sessionId) const {
        std::lock_guard lk(mtx_);
        auto it = sessions_.find(sessionId);
        if (it == sessions_.end()) return std::nullopt;
        return it->second;
    }

    std::string getSessionForTarget(const std::string& targetId) const {
        std::lock_guard lk(mtx_);
        auto it = targetToSession_.find(targetId);
        return it == targetToSession_.end() ? "" : it->second;
    }

    std::vector<std::pair<std::string, TargetInfo>> all() const {
        std::lock_guard lk(mtx_);
        std::vector<std::pair<std::string, TargetInfo>> out;
        for (auto& [sid, info] : sessions_) out.emplace_back(sid, info);
        return out;
    }

    struct TargetInfo {
        std::string targetId;
        std::string type;
        std::string title;
        std::string url;
        bool attached = false;
        std::string browserContextId;
    };

private:
    mutable std::mutex mtx_;
    std::unordered_map<std::string, TargetInfo> sessions_;
    std::unordered_map<std::string, std::string> targetToSession_;
};
```

### 1.2.6. `ScriptJournal`

```cpp
class ScriptJournal {
public:
    struct Script {
        std::string scriptId;
        std::string url;
        std::string sourceMapURL;
        int executionContextId = 0;
        int startLine = 0, endLine = 0;
        bool isModule = false;
        std::int64_t length = 0;
        std::string hash;
        std::string language = "JavaScript";

        // Заполняется позже
        bool sourceFetched = false;
        std::string source;
        std::string sourceError;
    };

    void onScriptParsed(const json& p, const std::string& sessionId) {
        std::lock_guard lk(mtx_);
        Script s;
        s.scriptId = p.value("scriptId", "");
        s.url = p.value("url", "");
        s.sourceMapURL = p.value("sourceMapURL", "");
        s.executionContextId = p.value("executionContextId", 0);
        s.startLine = p.value("startLine", 0);
        s.endLine = p.value("endLine", 0);
        s.isModule = p.value("isModule", false);
        if (p.contains("length") && p["length"].is_number())
            s.length = (std::int64_t)p["length"].get<double>();
        s.hash = p.value("hash", "");
        s.language = p.value("scriptLanguage", "JavaScript");

        scripts_[s.scriptId] = std::move(s);
        scriptSession_[scripts_[s.scriptId].scriptId] = sessionId;
    }

    std::vector<Script> all() const {
        std::lock_guard lk(mtx_);
        std::vector<Script> out;
        for (auto& [id, s] : scripts_) out.push_back(s);
        return out;
    }

    std::string getSessionForScript(const std::string& scriptId) const {
        std::lock_guard lk(mtx_);
        auto it = scriptSession_.find(scriptId);
        return it == scriptSession_.end() ? "" : it->second;
    }

    void setSource(const std::string& scriptId, const std::string& src) {
        std::lock_guard lk(mtx_);
        auto it = scripts_.find(scriptId);
        if (it != scripts_.end()) {
            it->second.source = src;
            it->second.sourceFetched = true;
        }
    }

private:
    mutable std::mutex mtx_;
    std::unordered_map<std::string, Script> scripts_;
    std::unordered_map<std::string, std::string> scriptSession_;
};
```

---

## 1.3. Интеграция в `Connection`

Ваш `GenericMethodHandler` вызывается на каждое WS-сообщение. Внутри надо диспетчеризовать:

```cpp
struct JournalDispatcher {
    NetworkJournal* network = nullptr;
    ConsoleJournal* console_ = nullptr;
    DomJournal*     dom = nullptr;
    TargetJournal*  target = nullptr;
    ScriptJournal*  script = nullptr;

    void operator()(marty::cdt::Connection* con,
                    const marty::cdt::WebSocketMessage& msg,
                    marty::cdt::MessageIdVariant idVariant,
                    marty::cdt::json j) const
    {
        std::visit([&](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                // arg — это method
                const auto& method = arg;
                std::string sessionId;
                if (j.contains("sessionId") && j["sessionId"].is_string())
                    sessionId = j["sessionId"].get<std::string>();
                auto params = j.value("params", marty::cdt::json::object());

                if (network) {
                    if (method == "Network.requestWillBeSent")          network->onRequestWillBeSent(params);
                    else if (method == "Network.requestWillBeSentExtraInfo") network->onRequestExtraInfo(params);
                    else if (method == "Network.responseReceived")      network->onResponseReceived(params);
                    else if (method == "Network.responseReceivedExtraInfo") network->onResponseExtraInfo(params);
                    else if (method == "Network.loadingFinished") {
                        network->onLoadingFinished(params);
                        // Запросить тела
                        auto pending = network->takePendingBodies();
                        for (auto& rid : pending) fetchBodyAsync_(con, network, rid, sessionId);
                    }
                    else if (method == "Network.loadingFailed")         network->onLoadingFailed(params);
                }

                if (console_) {
                    if (method == "Runtime.consoleAPICalled")   console_->onConsoleAPICalled(params);
                    else if (method == "Runtime.exceptionThrown") console_->onExceptionThrown(params);
                }

                if (dom) {
                    if (method == "Page.frameAttached")            dom->onFrameAttached(params, sessionId);
                    else if (method == "Page.frameNavigated")      dom->onFrameNavigated(params, sessionId);
                    else if (method == "Page.frameDetached")       dom->onFrameDetached(params);
                    else if (method == "Runtime.executionContextCreated")  dom->onExecutionContextCreated(params, sessionId);
                    else if (method == "Runtime.executionContextDestroyed") dom->onExecutionContextDestroyed(params);
                    else if (method == "Runtime.executionContextsCleared") dom->onExecutionContextsCleared();
                }

                if (target) {
                    if (method == "Target.attachedToTarget")    target->onAttached(params);
                    else if (method == "Target.detachedFromTarget") target->onDetached(params);
                }

                if (script) {
                    if (method == "Debugger.scriptParsed")      script->onScriptParsed(params, sessionId);
                }
            }
        }, idVariant);
    }

private:
    static void fetchBodyAsync_(marty::cdt::Connection* con,
                                NetworkJournal* journal,
                                const std::string& requestId,
                                const std::string& sessionId)
    {
        // Отправляем Network.getResponseBody
        // В реальности — надо ловить ответ по id, но так как handler — статeless,
        // лучше сделать отдельный sync-метод ниже
    }
};
```

**Проблема:** `getResponseBody` требует ответа от CDP, а мы внутри обработчика. Правильный путь — очередь: скопировать `requestId` в очередь и обрабатывать её в основном цикле.

---

# Часть II. Слой ожидания

## 2.1. Архитектура event-loop

Сейчас у вас:

```cpp
for (auto i = 0; i < 20; ++i) {
    if (!wsConnection->wsDispatchMessages())
        umba::sleepMs(100);
}
```

Это polling. Для wait-примитивов надо переделать на **callback + promise + очередь**.

### 2.1.1. `PendingWait`

```cpp
struct PendingWait {
    std::string method;                       // какой event ждём
    std::function<bool(const json&)> predicate;
    std::shared_ptr<std::promise<json>> promise;
    std::chrono::steady_clock::time_point deadline;
    std::string sessionId;                    // если ждём в конкретной сессии (или пусто = любая)
    uint64_t id = 0;
};
```

### 2.1.2. `WaitManager`

```cpp
class WaitManager {
public:
    uint64_t add(PendingWait w) {
        std::lock_guard lk(mtx_);
        w.id = ++nextId_;
        waits_.push_back(std::move(w));
        return waits_.back().id;
    }

    void cancel(uint64_t id) {
        std::lock_guard lk(mtx_);
        waits_.erase(
            std::remove_if(waits_.begin(), waits_.end(),
                [id](const PendingWait& w){ return w.id == id; }),
            waits_.end());
    }

    // Вызывать при каждом событии из Connection
    void dispatch(const std::string& method, const json& params, const std::string& sessionId) {
        std::vector<PendingWait> ready;
        {
            std::lock_guard lk(mtx_);
            auto it = waits_.begin();
            while (it != waits_.end()) {
                if (it->method == method && it->predicate(params)
                    && (it->sessionId.empty() || it->sessionId == sessionId)) {
                    ready.push_back(std::move(*it));
                    it = waits_.erase(it);
                } else {
                    ++it;
                }
            }
        }
        for (auto& w : ready) {
            try { w.promise->set_value(params); }
            catch (...) {}
        }
    }

    // Вызывать регулярно для проверки таймаутов
    void checkTimeouts() {
        auto now = std::chrono::steady_clock::now();
        std::vector<PendingWait> expired;
        {
            std::lock_guard lk(mtx_);
            auto it = waits_.begin();
            while (it != waits_.end()) {
                if (it->deadline <= now) {
                    expired.push_back(std::move(*it));
                    it = waits_.erase(it);
                } else {
                    ++it;
                }
            }
        }
        for (auto& w : expired) {
            try { w.promise->set_exception(
                std::make_exception_ptr(std::runtime_error("wait timeout: " + w.method))); }
            catch (...) {}
        }
    }

private:
    std::mutex mtx_;
    std::vector<PendingWait> waits_;
    uint64_t nextId_ = 0;
};
```

### 2.1.3. Интеграция

В `Connection` завести `WaitManager` и вызывать `dispatch` + `checkTimeouts` при каждом `wsDispatchMessages`:

```cpp
// вместо старого GenericMethodHandler
struct ConnectionObserver {
    WaitManager* waits = nullptr;

    void operator()(marty::cdt::Connection* con,
                    const marty::cdt::WebSocketMessage& msg,
                    marty::cdt::MessageIdVariant idVariant,
                    marty::cdt::json j) const
    {
        std::visit([&](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                std::string sessionId;
                if (j.contains("sessionId") && j["sessionId"].is_string())
                    sessionId = j["sessionId"].get<std::string>();
                if (waits) waits->dispatch(arg, j.value("params", json::object()), sessionId);
            }
        }, idVariant);
    }
};
```

А в основном цикле `wsDispatchMessages` — вызывать `waits->checkTimeouts()`.

## 2.2. `cdtWaitForEvent` — базовый комбинатор

```cpp
template<typename Predicate>
json cdtWaitForEvent(const std::string& method,
                     Predicate pred,
                     int timeoutMs = 30000,
                     const std::string& sessionId = "")
{
    auto promise = std::make_shared<std::promise<json>>();
    auto future = promise->get_future();

    PendingWait w;
    w.method = method;
    w.predicate = pred;
    w.promise = promise;
    w.deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    w.sessionId = sessionId;

    uint64_t id = waitManager_->add(std::move(w));

    // Пока ждём — крутим event loop
    while (future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
        wsDispatchMessages();
        waitManager_->checkTimeouts();
        umba::sleepMs(10);
    }

    return future.get();
}
```

## 2.3. `cdtWaitForSelector` — реализация

```cpp
struct ElementRect {
    bool found = false;
    double x = 0, y = 0, width = 0, height = 0;
    std::string tagName;
    std::string text;
    int nodeId = 0;
};

ElementRect cdtWaitForSelector(const std::string& selector,
                               int timeoutMs = 30000,
                               const std::string& frameId = "",
                               bool requireVisible = true)
{
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    std::string js = R"JS(
        (() => {
          const el = document.querySelector(__SEL__);
          if (!el) return null;
          __VISIBLE_CHECK__
          el.scrollIntoView({block:'center', inline:'center', behavior:'instant'});
          const r = el.getBoundingClientRect();
          if (r.width <= 0 || r.height <= 0) return null;
          const cx = r.left + r.width/2;
          const cy = r.top  + r.height/2;
          const hit = document.elementFromPoint(cx, cy);
          const covered = hit && !el.contains(hit) && hit !== el;
          return {
            x: cx, y: cy, width: r.width, height: r.height,
            tagName: el.tagName.toLowerCase(),
            text: (el.innerText||'').slice(0,100),
            covered: !!covered,
            coveringTag: covered ? hit.tagName.toLowerCase() : null
          };
        })()
    )JS;

    replaceAll(js, "__SEL__", json(selector).dump());
    replaceAll(js, "__VISIBLE_CHECK__",
        requireVisible
        ? "if (el.offsetWidth===0 || el.offsetHeight===0) return null;"
        : "");

    while (std::chrono::steady_clock::now() < deadline) {
        json res = evaluateInFrame(frameId, js, /*returnByValue*/true);
        if (res.contains("result") && res["result"].contains("value")
            && !res["result"]["value"].is_null()) {
            const auto& v = res["result"]["value"];
            ElementRect r;
            r.found = true;
            r.x = v.value("x", 0.0);
            r.y = v.value("y", 0.0);
            r.width = v.value("width", 0.0);
            r.height = v.value("height", 0.0);
            r.tagName = v.value("tagName", "");
            r.text = v.value("text", "");
            return r;
        }
        wsDispatchMessages();
        umba::sleepMs(120);
    }
    return {};
}
```

## 2.4. `cdtWaitForFunction`

```cpp
json cdtWaitForFunction(const std::string& expr,
                        int timeoutMs = 30000,
                        const std::string& frameId = "")
{
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    std::string js = "(async () => {"
                     "  const deadline = performance.now() + " + std::to_string(timeoutMs) + ";"
                     "  while (performance.now() < deadline) {"
                     "    const v = (function(){ return (" + expr + "); })();"
                     "    if (v) return v;"
                     "    await new Promise(r => setTimeout(r, 80));"
                     "  }"
                     "  return null;"
                     "})()";

    json params = {
        {"expression", js},
        {"awaitPromise", true},
        {"returnByValue", true},
        {"userGesture", true},
        {"timeout", timeoutMs + 1000}
    };

    std::string sessionId = domJournal_->getSessionForFrame(frameId);
    auto res = wsSendCommand("Runtime.evaluate", params, sessionId);

    if (res.contains("exceptionDetails")) return nullptr;
    if (res.contains("result")) {
        if (res["result"].value("type", "") == "object" && res["result"].value("subtype", "") == "null")
            return nullptr;
        return res["result"].value("value", json());
    }
    return nullptr;
}
```

## 2.5. `cdtWaitForNetworkIdle`

```cpp
void cdtWaitForNetworkIdle(int idleMs = 500, int timeoutMs = 30000) {
    auto start = std::chrono::steady_clock::now();
    auto idleSince = start;
    auto lastCount = inflightCount();

    while (true) {
        wsDispatchMessages();
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > timeoutMs)
            throw std::runtime_error("network idle timeout");

        auto count = inflightCount();
        if (count != lastCount) {
            lastCount = count;
            idleSince = now;
        }
        if (count == 0 &&
            std::chrono::duration_cast<std::chrono::milliseconds>(now - idleSince).count() >= idleMs)
            return;

        umba::sleepMs(30);
    }
}

int inflightCount() {
    auto all = networkJournal_->getAllOrdered();
    int count = 0;
    for (auto& u : all) {
        if (u.type == "WebSocket" || u.type == "EventSource") continue;
        if (!u.finished) ++count;
    }
    return count;
}
```

## 2.6. `cdtWaitForNetworkResponse`

```cpp
UnifiedRequest cdtWaitForNetworkResponse(
    std::function<bool(const UnifiedRequest&)> filter,
    int timeoutMs = 30000)
{
    // Сначала проверяем уже накопленные
    for (auto& u : networkJournal_->getAllOrdered())
        if (filter(u) && u.finished) return u;

    // Ждём новое
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    while (std::chrono::steady_clock::now() < deadline) {
        wsDispatchMessages();
        auto all = networkJournal_->getAllOrdered();
        for (auto it = all.rbegin(); it != all.rend(); ++it) {
            if (filter(*it)) return *it;
        }
        umba::sleepMs(50);
    }
    throw std::runtime_error("wait for network response timeout");
}
```

## 2.7. Правильный polling vs event-based

Есть два подхода:

**Polling** — простой, но жрёт CPU и плохо реагирует. Использовать для `Runtime.evaluate`-based проверок (селектор, функция).

**Event-based** — точно, но сложнее. Использовать для `Network.*`, `Page.*`, `Runtime.consoleAPICalled`.

Хорошая практика — **гибрид**: для селекторов, где нет события, использовать polling с разумным интервалом (100–150 мс), для остального — подписки. Тогда `cdtWaitForSelector` тратит ~10% CPU, а сетевые ожидания — 0%.

## 2.8. Подводные камни слоя ожидания

- **`Runtime.evaluate` в фоновом окне может тормозить**, если Chrome throttles. `Page.bringToFront` решает.
- **Polling `Runtime.evaluate` каждые 20 мс** — это 50 rps в V8. На тяжёлой странице может вызвать layout thrashing. 100–150 мс безопаснее.
- **`awaitPromise` в JS внутри `Runtime.evaluate`** — если промис никогда не резолвится, `Runtime.evaluate` вернёт ошибку по своему таймауту. Всегда задавать `timeout` в самом вызове.
- **`Runtime.evaluate` возвращает `null` для `null`, `undefined` для `undefined`** — различайте по `type` в ответе.
- **В iframe нужно указывать `contextId`, а не `sessionId`** для evaluate. Для `Input.dispatchMouseEvent` — только глобальные координаты, `sessionId` не важен, но координаты пересчитывать.
- **`Page.frameNavigated` не приходит для SPA-переходов через History API.** Слушайте `Page.navigatedWithinDocument`.
- **`Network.loadingFinished` для `XHR`** приходит, но `getResponseBody` может упасть, если сервер отдал `204 No Content`. Обрабатывать исключение.
- **Если вкладка в фоне, `Network.loadingFinished` может приходить с задержкой** — Chrome throttles idle pages. `Page.setWebLifecycleState({state:"active"})` помогает.

---

# Часть III. Слой действий

## 3.1. Мышь — полный API

### 3.1.1. Одиночный клик

```cpp
void cdtClick(double x, double y,
              const std::string& button = "left",
              int clickCount = 1,
              int modifiers = 0)
{
    int buttons = (button == "left") ? 1
                : (button == "right") ? 2
                : (button == "middle") ? 4 : 0;

    sendInput_("mouseMoved",  x, y, "none", 0, 0, modifiers);
    umba::sleepMs(10);
    sendInput_("mousePressed", x, y, button, buttons, clickCount, modifiers);
    umba::sleepMs(20);
    sendInput_("mouseReleased", x, y, button, 0, clickCount, modifiers);
}

void sendInput_(const std::string& type,
                double x, double y,
                const std::string& button,
                int buttons,
                int clickCount,
                int modifiers)
{
    json p = {
        {"type", type},
        {"x", x},
        {"y", y},
        {"button", button},
        {"buttons", buttons},
        {"clickCount", clickCount},
        {"modifiers", modifiers},
        {"pointerType", "mouse"}
    };
    wsSendCommand("Input.dispatchMouseEvent", p);
}
```

### 3.1.2. Двойной клик

```cpp
void cdtDoubleClick(double x, double y, int modifiers = 0) {
    // Первый клик
    sendInput_("mouseMoved", x, y, "none", 0, 0, modifiers);
    sendInput_("mousePressed", x, y, "left", 1, 1, modifiers);
    sendInput_("mouseReleased", x, y, "left", 0, 1, modifiers);
    umba::sleepMs(30);
    // Второй клик
    sendInput_("mousePressed", x, y, "left", 1, 2, modifiers);
    sendInput_("mouseReleased", x, y, "left", 0, 2, modifiers);
}
```

### 3.1.3. Drag & drop

```cpp
void cdtDrag(double x1, double y1, double x2, double y2,
             int steps = 15,
             const std::string& button = "left")
{
    int btnMask = (button == "left") ? 1 : (button == "right") ? 2 : 4;

    sendInput_("mouseMoved", x1, y1, "none", 0, 0, 0);
    umba::sleepMs(20);
    sendInput_("mousePressed", x1, y1, button, btnMask, 1, 0);
    umba::sleepMs(20);

    for (int i = 1; i <= steps; ++i) {
        double t = double(i) / steps;
        double x = x1 + (x2 - x1) * t;
        double y = y1 + (y2 - y1) * t;
        sendInput_("mouseMoved", x, y, button, btnMask, 0, 0);
        umba::sleepMs(10);
    }

    umba::sleepMs(20);
    sendInput_("mouseReleased", x2, y2, button, 0, 1, 0);
}
```

### 3.1.4. Скролл

```cpp
void cdtScroll(double x, double y, double deltaX, double deltaY, int modifiers = 0) {
    sendInput_("mouseMoved", x, y, "none", 0, 0, modifiers);
    umba::sleepMs(5);

    json p = {
        {"type", "mouseWheel"},
        {"x", x},
        {"y", y},
        {"deltaX", deltaX},
        {"deltaY", deltaY},
        {"modifiers", modifiers}
    };
    wsSendCommand("Input.dispatchMouseEvent", p);
}
```

**Ориентиры:** одно «колесо» = 100–120 px в браузере. Для бесконечной ленты обычно 300–500 px.

### 3.1.5. Touch (мобильная эмуляция)

```json
{ "method": "Input.dispatchTouchEvent",
  "params": {
    "type": "touchStart",
    "touchPoints": [{ "x": 100, "y": 200, "radiusX": 10, "radiusY": 10, "force": 1, "id": 0 }],
    "modifiers": 0
  }
}
```

Для пиinch-zoom — два `touchPoints` с разными id.

Перед использованием — `Emulation.setTouchEmulationEnabled({enabled:true, maxTouchPoints:5})`.

## 3.2. Клавиатура — полный API

### 3.2.1. `cdtPressKey`

```cpp
void cdtPressKey(const std::string& key,
                 const std::string& code,
                 int vk,
                 int modifiers = 0,
                 const std::string& text = "")
{
    json down = {
        {"type", "keyDown"},
        {"modifiers", modifiers},
        {"key", key},
        {"code", code},
        {"windowsVirtualKeyCode", vk},
        {"nativeVirtualKeyCode", vk}
    };
    if (!text.empty()) {
        down["text"] = text;
        down["unmodifiedText"] = text;
    }
    wsSendCommand("Input.dispatchKeyEvent", down);

    json up = down;
    up["type"] = "keyUp";
    up.erase("text");
    up.erase("unmodifiedText");
    wsSendCommand("Input.dispatchKeyEvent", up);
}
```

### 3.2.2. `cdtPressEnter`

```cpp
void cdtPressEnter() {
    cdtPressKey("Enter", "Enter", 13, 0, "\r");
}
```

### 3.2.3. `cdtPressTab`

```cpp
void cdtPressTab() {
    cdtPressKey("Tab", "Tab", 9, 0, "\t");
}
```

### 3.2.4. `cdtInsertText`

```cpp
void cdtInsertText(const std::string& text) {
    wsSendCommand("Input.insertText", { {"text", text} });
}
```

Одно событие, мгновенно. Для контролируемых React-компонентов может не сработать.

### 3.2.5. `cdtTypeText`

```cpp
void cdtTypeText(const std::string& text, int delayMs = 0) {
    for (size_t i = 0; i < text.size(); ) {
        // UTF-8 границы
        unsigned char c = text[i];
        size_t len = (c < 0x80) ? 1 : ((c & 0xE0) == 0xC0) ? 2 : ((c & 0xF0) == 0xE0) ? 3 : 4;
        std::string ch = text.substr(i, len);
        i += len;

        // Код для ASCII
        int vk = 0;
        std::string code = "";
        if (len == 1) {
            if (ch >= "a" && ch <= "z") { vk = 'A' + (ch[0] - 'a'); code = "Key" + std::string(1, ch[0] - 32); }
            else if (ch >= "A" && ch <= "Z") { vk = ch[0]; code = "Key" + ch; }
            else if (ch >= "0" && ch <= "9") { vk = ch[0]; code = "Digit" + ch; }
            else if (ch == " ") { vk = 32; code = "Space"; }
        }

        json down = {
            {"type", "keyDown"},
            {"key", ch},
            {"code", code},
            {"windowsVirtualKeyCode", vk},
            {"nativeVirtualKeyCode", vk},
            {"text", ch},
            {"unmodifiedText", ch}
        };
        wsSendCommand("Input.dispatchKeyEvent", down);

        json up = down;
        up["type"] = "keyUp";
        up.erase("text");
        up.erase("unmodifiedText");
        wsSendCommand("Input.dispatchKeyEvent", up);

        if (delayMs > 0) umba::sleepMs(delayMs);
    }
}
```

### 3.2.6. Модификаторы

```cpp
void cdtPressCtrl(char key) {
    // Ctrl down
    json ctrlDown = {
        {"type", "keyDown"}, {"key", "Control"}, {"code", "ControlLeft"},
        {"windowsVirtualKeyCode", 17}, {"nativeVirtualKeyCode", 17},
        {"modifiers", 2}
    };
    wsSendCommand("Input.dispatchKeyEvent", ctrlDown);

    // Key with Ctrl
    char upper = std::toupper(key);
    json keyDown = {
        {"type", "keyDown"}, {"key", std::string(1, key)},
        {"code", "Key" + std::string(1, upper)},
        {"windowsVirtualKeyCode", int(upper)},
        {"nativeVirtualKeyCode", int(upper)},
        {"modifiers", 2}
    };
    wsSendCommand("Input.dispatchKeyEvent", keyDown);

    json keyUp = keyDown;
    keyUp["type"] = "keyUp";
    wsSendCommand("Input.dispatchKeyEvent", keyUp);

    // Ctrl up
    json ctrlUp = ctrlDown;
    ctrlUp["type"] = "keyUp";
    wsSendCommand("Input.dispatchKeyEvent", ctrlUp);
}

void cdtSelectAll() { cdtPressCtrl('a'); }
void cdtCopy()      { cdtPressCtrl('c'); }
void cdtPaste()     { cdtPressCtrl('v'); }
```

**Важно:** `modifiers` — битовая маска: `Alt=1`, `Ctrl=2`, `Meta=4`, `Shift=8`. Её надо передавать **в событиях для самой клавиши**, а не только для модификатора.

### 3.2.7. Стрелки

```cpp
void cdtPressArrow(const std::string& dir, int count) {
    struct Info { const char* key; const char* code; int vk; };
    static const std::unordered_map<std::string, Info> kMap = {
        {"Up",    {"ArrowUp",    "ArrowUp",    38}},
        {"Down",  {"ArrowDown",  "ArrowDown",  40}},
        {"Left",  {"ArrowLeft",  "ArrowLeft",  37}},
        {"Right", {"ArrowRight", "ArrowRight", 39}},
    };
    auto it = kMap.find(dir);
    if (it == kMap.end()) return;
    for (int i = 0; i < count; ++i) {
        cdtPressKey(it->second.key, it->second.code, it->second.vk);
    }
}
```

### 3.2.8. Специальные клавиши

| Key | Code | VK |
|---|---|---|
| `Escape` | `Escape` | 27 |
| `Backspace` | `Backspace` | 8 |
| `Delete` | `Delete` | 46 |
| `Home` | `Home` | 36 |
| `End` | `End` | 35 |
| `PageUp` | `PageUp` | 33 |
| `PageDown` | `PageDown` | 34 |
| `F1..F12` | `F1..F12` | 112..123 |

## 3.3. Файлы

```cpp
void cdtSetFileInputFiles(const std::string& selector,
                          const std::vector<std::string>& files)
{
    // 1. Получить document
    auto doc = wsSendCommand("DOM.getDocument", { {"depth", 0} });
    int rootId = doc["root"]["nodeId"].get<int>();

    // 2. Найти узел
    auto node = wsSendCommand("DOM.querySelector",
                              { {"nodeId", rootId}, {"selector", selector} });
    int nodeId = node["nodeId"].get<int>();
    if (nodeId == 0) throw std::runtime_error("file input not found: " + selector);

    // 3. Установить файлы
    wsSendCommand("DOM.setFileInputFiles", {
        {"files", files}, {"nodeId", nodeId}
    });
}
```

**Важно:** `DOM.setFileInputFiles` не делает реальной загрузки, а сразу устанавливает `files` в `input`. После этого надо вручную дёрнуть `change` event, если приложение ждёт:

```js
document.querySelector('input[type=file]').dispatchEvent(new Event('change', {bubbles:true}));
```

## 3.4. Клик через JS (обход покрытий)

Иногда координатный клик не проходит (элемент покрыт). Тогда:

```js
el.click();
```

Однако это не генерирует `mousedown`/`mouseup`. Для большинства кнопок сработает, для сложных виртуальных — нет.

Синтетические события:

```js
const evt = new MouseEvent('mousedown', { bubbles: true, cancelable: true, view: window });
el.dispatchEvent(evt);
el.dispatchEvent(new MouseEvent('mouseup', { bubbles: true, cancelable: true }));
el.dispatchEvent(new MouseEvent('click', { bubbles: true, cancelable: true }));
```

Помогает для SPA с делегированием событий.

## 3.5. Проверка попадания клика

Перед кликом всегда проверять, что элемент в точке `(x, y)` — это то, что надо:

```js
const el = document.querySelector(SEL);
const r = el.getBoundingClientRect();
const cx = r.left + r.width/2, cy = r.top + r.height/2;
const hit = document.elementFromPoint(cx, cy);
const ok = hit === el || el.contains(hit);
```

## 3.6. Скриншоты

```cpp
std::string cdtScreenshotPng(bool fullPage = false) {
    json p = {
        {"format", "png"},
        {"fromSurface", true},
        {"captureBeyondViewport", fullPage}
    };
    auto res = wsSendCommand("Page.captureScreenshot", p);
    return res.value("data", "");   // base64
}

std::string cdtScreenshotJpeg(int quality = 80) {
    json p = { {"format", "jpeg"}, {"quality", quality}, {"fromSurface", true} };
    auto res = wsSendCommand("Page.captureScreenshot", p);
    return res.value("data", "");
}
```

## 3.7. Эмуляция

```cpp
void cdtSetUserAgent(const std::string& ua,
                     const std::string& acceptLang = "en-US,en;q=0.9",
                     const std::string& platform = "Win32") {
    wsSendCommand("Emulation.setUserAgentOverride", {
        {"userAgent", ua},
        {"acceptLanguage", acceptLang},
        {"platform", platform}
    });
}

void cdtSetDeviceMetrics(int w, int h, double scale, bool mobile) {
    wsSendCommand("Emulation.setDeviceMetricsOverride", {
        {"width", w}, {"height", h},
        {"deviceScaleFactor", scale},
        {"mobile", mobile}
    });
}

void cdtSetTimezone(const std::string& tz) {
    wsSendCommand("Emulation.setTimezoneOverride", { {"timezoneId", tz} });
}

void cdtSetLocale(const std::string& locale) {
    wsSendCommand("Emulation.setLocaleOverride", { {"locale", locale} });
}

void cdtSetGeolocation(double lat, double lon, double acc) {
    wsSendCommand("Emulation.setGeolocationOverride", {
        {"latitude", lat}, {"longitude", lon}, {"accuracy", acc}
    });
}

void cdtGrantPermissions(const std::vector<std::string>& perms,
                         const std::string& origin) {
    wsSendCommand("Browser.grantPermissions", {
        {"permissions", perms},
        {"origin", origin}
    });
}
```

`permissions`: `geolocation`, `notifications`, `camera`, `microphone`, `clipboard-read`, `clipboard-write`, `midi`, `background-sync`, `sensors`, `accessibility-events`, `payment-handler`, `idle-detection`.

## 3.8. Подводные камни слоя действий

- **`Input.dispatchMouseEvent` в headless-режиме** может не работать для некоторых элементов из-за отсутствия compositor. `--headless=new` решает.
- **`Input.insertText` не генерирует `keydown`/`keyup`.** Для сайтов, слушающих `keydown`, использовать `dispatchKeyEvent`.
- **`dispatchKeyEvent` с `text` для непечатной клавиши** игнорируется — не надо передавать `text` для Enter.
- **`modifiers` — маска**, надо аккуратно комбинировать: `Ctrl+Shift+A` = `2|8 = 10`.
- **`Input.dispatchKeyEvent` требует timestamp** в некоторых сборках Chrome, иначе события «залипают». Не передавайте — браузер сам поставит текущее.
- **На macOS `Meta` требует `commands`** для клавиш вроде `Cmd+C`. Использовать `commands: ["selectAll"]` не надо, достаточно `modifiers: 4`.
- **Координаты — CSS-пиксели в viewport, origin — верхний левый угол viewport.** Для элемента в iframe нужно прибавить смещение iframe в главном документе.
- **`Input.dispatchMouseEvent` не вызывает hover автоматически** для CSS `:hover`. Только `mouseMoved` активирует hover.
- **`clickCount`**: для одиночного клика `1`, для второго клика в двойном — `2`, для третьего в тройном — `3`.
- **`Page.bringToFront` в фоне не всегда помогает**, если окно минимизировано. Запускать Chrome не минимизированным.

---

# Часть IV. Слой извлечения тел и исходников

## 4.1. `Network.getResponseBody`

```cpp
struct BodyResponse {
    std::string body;
    bool base64Encoded = false;
    bool truncated = false;
    std::string error;
};

BodyResponse cdtGetResponseBody(const std::string& requestId,
                                const std::string& sessionId = "")
{
    try {
        auto res = wsSendCommand("Network.getResponseBody",
                                 { {"requestId", requestId} }, sessionId);
        BodyResponse b;
        b.body = res.value("body", "");
        b.base64Encoded = res.value("base64Encoded", false);
        if (res.contains("truncated")) b.truncated = res["truncated"].get<bool>();
        return b;
    } catch (const std::exception& e) {
        BodyResponse b;
        b.error = e.what();
        return b;
    }
}
```

Обязательно вызывать **сразу в `loadingFinished`**, иначе после следующей навигации `requestId` станет невалидным.

## 4.2. `Debugger.getScriptSource`

```cpp
std::string cdtGetScriptSource(const std::string& scriptId,
                               const std::string& sessionId = "")
{
    auto res = wsSendCommand("Debugger.getScriptSource",
                             { {"scriptId", scriptId} }, sessionId);
    return res.value("scriptSource", "");
}
```

## 4.3. Source maps

### 4.3.1. Скачивание

Source map — это JSON, обычно лежит по URL, относительному от JS-файла:

```cpp
std::string resolveUrl(const std::string& base, const std::string& rel);
std::string downloadUrl(const std::string& url);   // ваш HTTP-клиент
```

Скачивать **вне страницы**, своим HTTP-клиентом: так обходятся CORS.

Если `sourceMapURL` начинается с `data:application/json;base64,` — это inline map:

```cpp
std::string extractInlineSourceMap(const std::string& url) {
    const std::string prefix = "data:application/json;base64,";
    if (url.substr(0, prefix.size()) == prefix) {
        std::string b64 = url.substr(prefix.size());
        return base64Decode(b64);
    }
    return "";
}
```

### 4.3.2. Парсинг source map v3

```cpp
struct SourceMap {
    int version = 3;
    std::string file;
    std::vector<std::string> sources;
    std::vector<std::string> sourcesContent;   // может быть пустым
    std::vector<std::string> names;
    std::string mappings;
    std::string sourceRoot;
};

SourceMap parseSourceMap(const std::string& jsonText) {
    auto j = json::parse(jsonText);
    SourceMap m;
    m.version = j.value("version", 3);
    m.file = j.value("file", "");
    if (j.contains("sources") && j["sources"].is_array())
        for (auto& s : j["sources"]) m.sources.push_back(s.get<std::string>());
    if (j.contains("sourcesContent") && j["sourcesContent"].is_array())
        for (auto& s : j["sourcesContent"])
            m.sourcesContent.push_back(s.is_string() ? s.get<std::string>() : "");
    if (j.contains("names") && j["names"].is_array())
        for (auto& s : j["names"]) m.names.push_back(s.get<std::string>());
    m.mappings = j.value("mappings", "");
    m.sourceRoot = j.value("sourceRoot", "");
    return m;
}
```

### 4.3.3. Восстановление исходников

```cpp
struct RecoveredFile {
    std::string path;      // как в sources[i]
    std::string content;
    bool wasInline = false;
};

std::vector<RecoveredFile> recoverSources(const std::string& mapUrl,
                                         const std::string& jsUrl)
{
    std::string mapText = downloadUrl(mapUrl);
    if (mapText.empty()) return {};

    auto m = parseSourceMap(mapText);
    std::vector<RecoveredFile> out;

    for (size_t i = 0; i < m.sources.size(); ++i) {
        RecoveredFile f;
        f.path = m.sources[i];
        if (i < m.sourcesContent.size() && !m.sourcesContent[i].empty()) {
            f.content = m.sourcesContent[i];
            f.wasInline = true;
        } else {
            // Пробуем скачать
            std::string url = resolveUrl(mapUrl, m.sources[i]);
            f.content = downloadUrl(url);
        }
        out.push_back(std::move(f));
    }
    return out;
}
```

### 4.3.4. Обработка `sourceRoot`

Если `sourceRoot` не пустой, все `sources[i]` резолвятся относительно `sourceRoot`, а не относительно карты. Учитывайте.

## 4.4. Кэширование

Source maps часто повторяются (одна карта на много скриптов). Держите `std::unordered_map<std::string, std::string> mapCache` — URL → содержимое карты.

## 4.5. Ограничения

- **Размер тел.** Один JS-бандл может быть 5–50 MB. Ограничивайте `maxResourceBufferSize` в `Network.enable`, иначе OOM.
- **`getResponseBody` для больших ответов** может занять секунды. Не вызывайте в критичном пути.
- **Source map может не иметь `sourcesContent`.** Тогда придётся качать каждый `sources[i]` — это ещё N запросов.
- **Source map может быть обфусцирован сам** (некоторые сборщики). Тогда исходников не восстановить.
- **Для inline-скриптов source map может быть тоже inline.** Извлекайте из `sourceMapURL`.

## 4.6. Обход всех скриптов

```cpp
void cdtRecoverAllScripts(const std::string& outDir) {
    auto scripts = scriptJournal_->all();
    for (auto& s : scripts) {
        // Пропускаем служебные
        if (s.url.empty() || s.url.starts_with("chrome-extension://")) continue;
        if (s.url.starts_with("devtools://")) continue;

        std::string src = cdtGetScriptSource(s.scriptId,
                                             scriptJournal_->getSessionForScript(s.scriptId));
        if (src.empty()) continue;

        std::string hash = sha256(s.url).substr(0, 16);
        std::string jsFile = outDir + "/scripts/" + hash + ".js";
        writeFile(jsFile, src);

        if (!s.sourceMapURL.empty()) {
            std::string mapUrl = resolveUrl(s.url, s.sourceMapURL);
            std::string mapText = downloadUrl(mapUrl);
            if (!mapText.empty()) {
                writeFile(outDir + "/scripts/" + hash + ".js.map", mapText);

                auto recovered = recoverSources(mapUrl, s.url);
                for (auto& f : recovered) {
                    std::string safePath = sanitizePath(f.path);
                    writeFile(outDir + "/scripts/src/" + hash + "/" + safePath, f.content);
                }
            }
        }
    }
}
```

## 4.7. Обход всех тел

```cpp
void cdtRecoverAllBodies(const std::string& outDir) {
    auto all = networkJournal_->getAllOrdered();
    for (auto& u : all) {
        if (!u.finished || u.failed) continue;
        if (u.body.available) continue;
        if (u.type == "WebSocket" || u.type == "EventSource") continue;

        auto body = cdtGetResponseBody(u.requestId);
        if (!body.error.empty()) continue;

        std::string data = body.base64Encoded ? base64Decode(body.body) : body.body;
        std::string ext = mimeToExt(u.responses.empty() ? "" : u.responses[0].mimeType);
        std::string hash = sha256(u.url).substr(0, 16);
        std::string file = outDir + "/bodies/" + hash + ext;
        writeFile(file, data);
    }
}
```

## 4.8. Подводные камни слоя тел

- **`getResponseBody` не работает после `Page.navigate`.** Собирайте всё до навигации.
- **`base64Encoded` = true для gzip?** Нет. `base64Encoded` — это про бинарность MIME, а gzip распаковывается прозрачно.
- **`truncated`** — если вы не задали `maxResourceBufferSize`, тело может быть обрезано.
- **Source map может лежать на другом origin** — тогда скачивание требует CORS или своего клиента.
- **`data:` source maps** — не все сборщики их делают, но поддержать надо.
- **`sourcesContent` может содержать null** для некоторых источников. Проверяйте `is_string()`.

---

# Часть V. Слой учёта контекстов

## 5.1. Открытие сессий

`Target.setAutoAttach({autoAttach:true, waitForDebuggerOnStart:false, flatten:true})`.

Всё дальнейшее — через `sessionId` в корне команды.

## 5.2. Обёртка `wsSendCommand` с sessionId

```cpp
json wsSendCommand(const std::string& method,
                   const json& params,
                   const std::string& sessionId = "")
{
    json msg = { {"id", nextId_++}, {"method", method}, {"params", params} };
    if (!sessionId.empty()) msg["sessionId"] = sessionId;

    auto future = sendAndWait(msg);
    return future.get();
}
```

## 5.3. Определение контекста для evaluate

Три уровня:

1. **Главный фрейм** — `Runtime.evaluate` без `contextId`.
2. **iframe в том же target** — `Runtime.evaluate` с `contextId`, полученным из `Runtime.executionContextCreated`.
3. **iframe/worker в другом target** — `Runtime.enable` в его `sessionId`, затем evaluate в этой сессии.

## 5.4. `cdtEvaluateInFrame`

```cpp
json cdtEvaluateInFrame(const std::string& frameId,
                        const std::string& expression,
                        bool returnByValue = true,
                        bool awaitPromise = false)
{
    auto frame = domJournal_->getFrame(frameId);
    if (!frame) throw std::runtime_error("frame not found: " + frameId);

    auto sessionId = domJournal_->getSessionForFrame(frameId);
    auto ctx = domJournal_->getContextForFrame(frameId);

    json params = {
        {"expression", expression},
        {"returnByValue", returnByValue},
        {"awaitPromise", awaitPromise},
        {"userGesture", true}
    };
    if (ctx) params["contextId"] = ctx->id;

    return wsSendCommand("Runtime.evaluate", params, sessionId);
}
```

## 5.5. Смещение координат в iframe

`Input.dispatchMouseEvent` работает в системе координат **топа**. Координаты внутри iframe — локальны. Надо пересчитать:

```cpp
struct Point { double x, y; };

Point localToGlobal(const std::string& frameId, double localX, double localY) {
    // Рекурсивно идём по parentFrame
    auto frame = domJournal_->getFrame(frameId);
    if (!frame) return {localX, localY};

    if (frame->parentFrameId.empty()) return {localX, localY};

    // Координаты iframe в родителе
    // Если iframe в том же target:
    auto parentCtx = domJournal_->getContextForFrame(frame->parentFrameId);
    if (parentCtx) {
        std::string js = "(function(){"
                         "  const frames = document.querySelectorAll('iframe');"
                         "  for (const f of frames) {"
                         "    try { if (f.contentWindow.frameElement === null) continue; } catch(e) {}"
                         "    try {"
                         "      const inner = f.contentWindow;"
                         "      if (!inner) continue;"
                         "      if (inner.__cdt_frameId !== '" + frameId + "') continue;"
                         "    } catch(e) {}"
                         "  }"
                         "  return null;"
                         "})()";
        // На практике проще: смотреть через page.getFrameTree + tree children
    }

    // Проще и надёжнее: если iframe в другом target, узнать его rect в родителе через JS
    // с использованием window.frameElement (если same-origin) или через DOM.

    // Универсальный путь: пройтись по всем iframe в родителе и найти тот, у которого
    // contentWindow.location.href совпадает с frame.url — но это грубо.

    // Ещё вариант: в parent runtime выполнить:
    //   [...document.querySelectorAll('iframe')].find(f => f.contentWindow === <ref to window>)
    // Но cross-origin не даст доступа.

    // Практично: использовать CDP-модель:
    //   DOM.getDocument (в родителе)
    //   DOM.querySelector с selector на iframe
    //   DOM.getBoxModel → quad → rect
    // Это даёт позицию iframe в родителе, даже если cross-origin.

    Point parentOffset = getIframeRectInParent(frame->parentFrameId, frameId);
    Point parentPoint = { localX + parentOffset.x, localY + parentOffset.y };
    return localToGlobal(frame->parentFrameId, parentPoint.x, parentPoint.y);
}
```

`getIframeRectInParent` — сложная функция. Реализация через `DOM.getBoxModel`:

```cpp
Point getIframeRectInParent(const std::string& parentFrameId, const std::string& childFrameId) {
    auto sessionId = domJournal_->getSessionForFrame(parentFrameId);

    // Получить document родителя
    auto doc = wsSendCommand("DOM.getDocument", { {"depth", 0} }, sessionId);
    int rootId = doc["root"]["nodeId"].get<int>();

    // Найти все iframe и выбрать нужный
    // Сопоставление: используем Runtime.evaluate в родителе, который вернёт массив
    // { frameId, rect } — frameId можно получить через window.frames[i].frameId? Нет,
    // так не получится. Используем порядок: Page.getFrameTree даёт порядок, но не DOM.

    // Надёжный путь: получить список iframe, для каждого — boxModel, и найти тот,
    // чья позиция совпадает с координатами клика, ожидаемого внутри.

    // На практике: используем Runtime.evaluate в родителе:
    std::string js = R"JS(
      (() => {
        const out = [];
        const frames = document.querySelectorAll('iframe');
        for (let i = 0; i < frames.length; i++) {
          const f = frames[i];
          const r = f.getBoundingClientRect();
          out.push({ i, x: r.left, y: r.top, w: r.width, h: r.height,
                     src: f.src, name: f.name });
        }
        return out;
      })()
    )JS;

    auto res = cdtEvaluateInFrame(parentFrameId, js, true, false);
    // Сопоставляем с frame.url или frame.name
    // ...
}
```

На практике: используйте `frame.name` или `frame.url` для сопоставления. Если iframe без `name` и cross-origin, придётся полагаться на порядок — а это ненадёжно. В крайнем случае делайте клик через JS-эмуляцию в самом iframe (см. 5.6).

## 5.6. Клик в iframe через JS

Если координатные клики в iframe невозможны — генерируйте события через `Runtime.evaluate`:

```js
const el = document.querySelector(SEL);
const r = el.getBoundingClientRect();
const cx = r.left + r.width/2, cy = r.top + r.height/2;
['mousedown', 'mouseup', 'click'].forEach(type => {
  el.dispatchEvent(new MouseEvent(type, {
    bubbles: true, cancelable: true, view: window,
    clientX: cx, clientY: cy, button: 0
  }));
});
```

Работает для большинства SPA.

## 5.7. Worker и Service Worker

Аналогично iframe, но с sessionId, привязанным к target типа `worker`/`service_worker`.

`Runtime.evaluate` в worker даёт доступ к `self`. Нельзя обратиться к DOM. Для сбора данных из worker'а:

```js
self.__cdt_collected = self.__cdt_collected || [];
self.addEventListener('message', e => self.__cdt_collected.push(e.data));
```

Потом evaluate и забрать.

## 5.8. Подводные камни слоя контекстов

- **`Target.setAutoAttach` без `flatten:true`** создаёт вложенные сессии — сложно.
- **`Runtime.executionContextCreated` приходит для каждого контекста** — не путать изолированные контексты (например, созданные расширениями).
- **`Runtime.evaluate` с `contextId` не работает, если контекст был удалён.** Проверять актуальность.
- **`iframe srcdoc` не создаёт target**, но создаёт execution context в родительском target. `sessionId` тот же, разный `contextId`.
- **OOPIF с cross-origin** — при клике координаты всё равно глобальные, но `getBoundingClientRect` внутри OOPIF даёт локальные. Рекурсивный пересчёт обязателен.
- **Service worker общий между вкладками**, но у каждой сессии своя связь с ним. `Target.getTargets` показывает.
- **`Page.frameTree` даёт только iframe в главной сессии.** Для OOPIF — тоже, но их `Runtime` в отдельной сессии.

---

# Часть VI. Слой нормализации (AI bundle)

## 6.1. `meta.json`

```cpp
json buildMeta() {
    json m;
    m["collectedAt"] = currentIso8601();
    m["url"] = lastNavigatedUrl_;
    m["frames"] = domJournal_->allFrames().size();
    m["sessions"] = targetJournal_->all().size();
    m["scripts"] = scriptJournal_->all().size();
    m["requests"] = networkJournal_->getAllOrdered().size();
    m["consoleMessages"] = consoleJournal_->messages().size();
    m["exceptions"] = consoleJournal_->exceptions().size();
    m["warnings"] = warnings_;   // накопленные в процессе
    return m;
}
```

## 6.2. `summary.json`

```cpp
json buildSummary() {
    json s;
    s["url"] = lastNavigatedUrl_;
    s["title"] = evaluateInMain_("document.title");
    s["finalUrl"] = evaluateInMain_("location.href");
    s["loadedAt"] = lastLoadTime_;

    auto domStats = evaluateInMain_(R"JS(
      ({
        nodes: document.getElementsByTagName('*').length,
        scripts: document.scripts.length,
        stylesheets: document.styleSheets.length,
        iframes: document.querySelectorAll('iframe').length,
        forms: document.querySelectorAll('form').length,
        inputs: document.querySelectorAll('input, textarea, select').length,
        buttons: document.querySelectorAll('button, [role=button]').length,
        links: document.querySelectorAll('a[href]').length
      })
    )JS");
    s["dom"] = domStats;

    json reqStats = json::object();
    for (auto& u : networkJournal_->getAllOrdered())
        reqStats[u.type] = reqStats.value(u.type, 0) + 1;
    s["requests"] = reqStats;
    s["totalRequests"] = networkJournal_->getAllOrdered().size();

    return s;
}
```

## 6.3. `requests.tsv`

```cpp
void writeRequestsTsv(const std::string& path) {
    std::ostringstream out;
    out << "time\tmethod\tstatus\ttype\tsize\turl\tinitiator\n";
    for (auto& u : networkJournal_->getAllOrdered()) {
        auto t = formatTime(u.startTime);
        auto status = u.responses.empty() ? 0 : u.responses.back().status;
        auto size = humanSize(u.totalEncodedDataLength);
        std::string init = u.type == "Document" ? "navigation"
                         : u.requests[0].initiator.type + " (" + u.requests[0].initiator.topUrl + ")";
        out << t << "\t" << u.method << "\t" << status << "\t" << u.type
            << "\t" << size << "\t" << u.url << "\t" << init << "\n";
    }
    writeFile(path, out.str());
}
```

## 6.4. `dom.html`

```cpp
std::string buildNormalizedDom() {
    std::string js = R"JS(
      (() => {
        const clone = document.documentElement.cloneNode(true);
        const LIMIT = 200;

        // Убрать служебное
        clone.querySelectorAll('script, style, link[rel="preload"], link[rel="prefetch"]')
             .forEach(n => n.remove());
        clone.querySelectorAll('noscript').forEach(n => n.remove());
        clone.querySelectorAll('meta[name="viewport"]').forEach(n => n.remove());

        // Упростить SVG
        clone.querySelectorAll('svg').forEach(svg => { svg.innerHTML = ''; });

        // Скрыть base64
        clone.querySelectorAll('[src^="data:"]').forEach(n => n.setAttribute('src', '<data>'));

        // Удалить шумные атрибуты
        const noiseAttrs = ['data-reactid', 'data-react-checksum', 'data-v-',
                            'data-testid', 'data-node-id', 'aria-busy'];
        clone.querySelectorAll('*').forEach(el => {
          for (const attr of [...el.attributes]) {
            const name = attr.name;
            if (noiseAttrs.some(n => name.startsWith(n))) { el.removeAttribute(name); continue; }
            if (attr.value.length > LIMIT) {
              el.setAttribute(name, attr.value.slice(0, LIMIT) + '...');
            }
          }
        });

        // Убрать невидимые элементы (display:none)
        clone.querySelectorAll('[style*="display:none"], [style*="display: none"]')
             .forEach(el => el.remove());

        return clone.outerHTML;
      })()
    )JS;

    auto res = evaluateInMain_(js);
    return res.is_string() ? res.get<std::string>() : "";
}
```

## 6.5. `interactive.json`

```cpp
std::string buildInteractive() {
    std::string js = R"JS(
      (() => {
        const out = [];
        const sel = 'input:not([type=hidden]), textarea, select, button, a[href], [role=button], [role=link], [role=textbox], [contenteditable="true"]';
        for (const el of document.querySelectorAll(sel)) {
          const r = el.getBoundingClientRect();
          if (r.width <= 0 || r.height <= 0) continue;
          const style = getComputedStyle(el);
          if (style.visibility === 'hidden' || style.display === 'none') continue;

          out.push({
            tag: el.tagName.toLowerCase(),
            type: el.type || null,
            id: el.id || null,
            name: el.name || null,
            role: el.getAttribute('role') || null,
            ariaLabel: el.getAttribute('aria-label') || null,
            placeholder: el.placeholder || null,
            text: (el.innerText || '').trim().slice(0, 80),
            href: el.href || null,
            value: el.value != null ? String(el.value).slice(0, 80) : null,
            disabled: el.disabled || false,
            checked: el.checked != null ? el.checked : null,
            selector: buildSelector(el),
            rect: { x: Math.round(r.left + r.width/2), y: Math.round(r.top + r.height/2),
                    w: Math.round(r.width), h: Math.round(r.height) }
          });
        }
        return out;

        function buildSelector(el) {
          if (el.id) return '#' + CSS.escape(el.id);
          let parts = [];
          while (el && el.nodeType === 1 && parts.length < 6) {
            let s = el.tagName.toLowerCase();
            const name = el.getAttribute('name');
            if (name) { s += '[name="' + CSS.escape(name) + '"]'; parts.unshift(s); break; }
            const role = el.getAttribute('role');
            if (role) s += '[role="' + CSS.escape(role) + '"]';
            else if (el.className && typeof el.className === 'string') {
              const first = el.className.trim().split(/\s+/)[0];
              if (first && !/^[a-z]-?\d+$/.test(first) && !/^css-/.test(first))
                s += '.' + CSS.escape(first);
            }
            parts.unshift(s);
            el = el.parentElement;
          }
          return parts.join(' > ');
        }
      })()
    )JS;

    auto res = evaluateInMain_(js);
    return res.dump(2);
}
```

## 6.6. `console.log`

```cpp
void writeConsoleLog(const std::string& path) {
    std::ostringstream out;
    for (auto& m : consoleJournal_->messages()) {
        out << "[" << formatTime(m.timestamp) << "] "
            << uppercase(m.level) << "  ";
        bool first = true;
        for (auto& t : m.argTexts) { if (!first) out << " "; out << t; first = false; }
        if (!m.url.empty()) out << " (at " << m.url << ":" << (m.line + 1) << ")";
        out << "\n";
    }
    for (auto& e : consoleJournal_->exceptions()) {
        out << "[" << formatTime(e.timestamp) << "] "
            << "EXCEPTION  " << e.className << ": " << e.text;
        if (!e.url.empty()) out << " (at " << e.url << ":" << (e.line + 1) << ")";
        out << "\n";
        for (auto& fr : e.stackTop) out << "    " << fr << "\n";
    }
    writeFile(path, out.str());
}
```

## 6.7. `scripts/index.json`

```cpp
json buildScriptsIndex() {
    json arr = json::array();
    for (auto& s : scriptJournal_->all()) {
        json e;
        e["id"] = s.scriptId;
        e["url"] = s.url;
        e["size"] = s.length;
        e["isModule"] = s.isModule;
        e["hasSourceMap"] = !s.sourceMapURL.empty();
        e["language"] = s.language;
        // bundleFile и mapFile добавляются при сохранении
        arr.push_back(std::move(e));
    }
    return arr;
}
```

## 6.8. `cdtBuildAiBundle` — общая функция

```cpp
void cdtBuildAiBundle(const std::string& outDir) {
    mkdirs(outDir);
    mkdirs(outDir + "/scripts");
    mkdirs(outDir + "/scripts/src");
    mkdirs(outDir + "/bodies");
    mkdirs(outDir + "/network");
    mkdirs(outDir + "/storage");
    mkdirs(outDir + "/screenshots");

    writeFile(outDir + "/meta.json", buildMeta().dump(2));
    writeFile(outDir + "/summary.json", buildSummary().dump(2));
    writeFile(outDir + "/dom.html", buildNormalizedDom());
    writeFile(outDir + "/interactive.json", buildInteractive());
    writeFile(outDir + "/console.log", buildConsoleLogStr());
    writeFile(outDir + "/scripts/index.json", buildScriptsIndex().dump(2));
    writeFile(outDir + "/network/requests.tsv", buildRequestsTsvStr());
    writeFile(outDir + "/network/post-load.tsv", buildPostLoadTsvStr());

    // Скачать все скрипты и source maps
    cdtRecoverAllScripts(outDir);
    cdtRecoverAllBodies(outDir);

    // Storage
    writeFile(outDir + "/storage/cookies.json", cdtGetAllCookies().dump(2));
    writeFile(outDir + "/storage/localStorage.json", cdtGetLocalStorage().dump(2));
    writeFile(outDir + "/storage/sessionStorage.json", cdtGetSessionStorage().dump(2));

    // Скриншот
    std::string png = cdtScreenshotPng(true);
    if (!png.empty()) writeFile(outDir + "/screenshots/full.png", base64Decode(png));
}
```

## 6.9. Размер bundle

Bundle часто получается 50–500 MB (source maps + все тела). Для ИИ нужна **выжимка**:

- `summary.json`, `requests.tsv`, `interactive.json`, `dom.html`, `console.log` — маленькие, всегда.
- `scripts/` — только те, что не «vendor» (jquery, react, moment). Фильтровать по URL.
- `bodies/` — только текстовые и небольшие.

Сделайте параметры:

```cpp
struct AiBundleOptions {
    bool includeScripts = true;
    bool includeBodies = false;
    bool includeSourceMaps = true;
    bool includeScreenshots = true;
    size_t maxScriptSize = 5 * 1024 * 1024;
    size_t maxBodySize = 1 * 1024 * 1024;
    std::vector<std::string> scriptUrlExcludePatterns = {
        "jquery", "react", "vue", "angular", "polyfill", "vendor", "chunk-vendors"
    };
};
```

## 6.10. Подводные камни слоя нормализации

- **`outerHTML` может быть > 10 MB** на тяжёлых страницах. Ограничивайте.
- **`buildSelector` не гарантирует уникальность.** Проверяйте `document.querySelectorAll(sel).length` и поднимайтесь, если >1.
- **`getBoundingClientRect` может дать нули, если элемент за границей viewport.** Проверяйте width > 0.
- **iframe не входит в главный `outerHTML`.** Собирайте по фреймам отдельно.
- **Shadow DOM не в `outerHTML`.** Обходите через `element.shadowRoot.innerHTML`.
- **`console.log` с объектами не даёт реальные значения** — только `objectId`. Ленивое `getProperties` — медленно. Для bundle ограничивайтесь `description`.
- **Source map может быть 10 MB и содержать 1000 файлов.** Ограничивайте.
- **Base64 в теле надо декодировать до сохранения**, иначе на диске будет нечитаемо.

---

# Часть VII. Слой диффов

## 7.1. Snapshot

```cpp
struct Snapshot {
    std::string url;
    double takenAt = 0.0;
    std::string domHtml;
    std::vector<std::string> requestIds;
    std::unordered_map<std::string, std::string> localStorage;
    std::unordered_map<std::string, std::string> sessionStorage;
    std::vector<json> interactive;
    std::unordered_map<std::string, std::string> cookies;
};

Snapshot cdtSnapshot() {
    Snapshot s;
    s.takenAt = monotonicSeconds();
    s.url = evaluateInMain_("location.href").get<std::string>();
    s.domHtml = buildNormalizedDom();
    s.interactive = json::parse(buildInteractive());

    for (auto& u : networkJournal_->getAllOrdered())
        s.requestIds.push_back(u.requestId);

    s.localStorage = parseStorage_(evaluateInMain_(
        "JSON.stringify(Object.fromEntries(Object.entries(localStorage)))"));
    s.sessionStorage = parseStorage_(evaluateInMain_(
        "JSON.stringify(Object.fromEntries(Object.entries(sessionStorage)))"));
    s.cookies = parseCookies_(evaluateInMain_("document.cookie"));

    return s;
}
```

## 7.2. Diff

```cpp
struct SnapshotDiff {
    std::vector<std::string> newRequestIds;
    std::vector<std::pair<std::string, std::string>> domChanges;
    std::unordered_map<std::string, std::string> storageAdded;
    std::unordered_map<std::string, std::string> storageChanged;
    std::unordered_map<std::string, std::string> storageRemoved;
    std::vector<json> interactiveAdded;
    std::vector<json> interactiveRemoved;
};

SnapshotDiff cdtDiff(const Snapshot& a, const Snapshot& b) {
    SnapshotDiff d;

    // Новые запросы
    std::unordered_set<std::string> aReqs(a.requestIds.begin(), a.requestIds.end());
    for (auto& id : b.requestIds)
        if (!aReqs.count(id)) d.newRequestIds.push_back(id);

    // DOM diff — построчно
    auto aLines = splitLines(a.domHtml);
    auto bLines = splitLines(b.domHtml);
    auto changes = diffLines(aLines, bLines);
    d.domChanges = changes;  // ограничить до 200

    // Storage
    diffMap(a.localStorage, b.localStorage,
            d.storageAdded, d.storageChanged, d.storageRemoved);
    diffMap(a.sessionStorage, b.sessionStorage,
            d.storageAdded, d.storageChanged, d.storageRemoved);

    // Interactive
    std::unordered_set<std::string> aInts;
    for (auto& el : a.interactive) aInts.insert(interactiveKey(el));
    std::unordered_set<std::string> bInts;
    for (auto& el : b.interactive) bInts.insert(interactiveKey(el));

    for (auto& el : b.interactive)
        if (!aInts.count(interactiveKey(el))) d.interactiveAdded.push_back(el);
    for (auto& el : a.interactive)
        if (!bInts.count(interactiveKey(el))) d.interactiveRemoved.push_back(el);

    return d;
}

std::string interactiveKey(const json& el) {
    return el.value("tag", "") + "|" +
           el.value("id", "") + "|" +
           el.value("name", "") + "|" +
           el.value("placeholder", "") + "|" +
           el.value("text", "");
}
```

## 7.3. Сценарий использования

```cpp
auto s0 = wsConnection->cdtSnapshot();

auto rect = wsConnection->cdtWaitForSelector("textarea#text");
wsConnection->cdtClick(rect.x, rect.y);
wsConnection->cdtInsertText("котики");

auto s1 = wsConnection->cdtSnapshot();
auto diff1 = wsConnection->cdtDiff(s0, s1);
// diff1.newRequestIds — что ушло после ввода
// diff1.domChanges — что изменилось в DOM

wsConnection->cdtPressEnter();
wsConnection->cdtWaitForSelector("ul.search-results", 30000);
wsConnection->cdtWaitForNetworkIdle();

auto s2 = wsConnection->cdtSnapshot();
auto diff2 = wsConnection->cdtDiff(s1, s2);
// Здесь — переход на страницу результатов
```

## 7.4. Подводные камни слоя диффов

- **DOM diff шумный** из-за анимаций, timing-метрик, `data-*`. Фильтруйте.
- **Порядок атрибутов** в `outerHTML` может различаться между snapshot'ами. Нормализуйте: сортируйте атрибуты.
- **`requestIds` — нестабильны между навигациями.** Только в пределах одной.
- **Diff по строкам не учитывает семантику** (переместился узел, атрибут изменился). Для серьёзного diff нужен структурный подход (сравнение DOM-деревьев по селектору). Это уже сложнее.
- **`takenAt` — monotonic**, не сравнивайте с wallTime.

---

# Часть VIII. Дополнительные домены CDP

## 8.1. Performance

```cpp
json cdtGetPerformanceMetrics() {
    auto res = wsSendCommand("Performance.getMetrics", {});
    json out = json::object();
    if (res.contains("metrics"))
        for (auto& m : res["metrics"])
            out[m["name"].get<std::string>()] = m["value"];
    return out;
}
```

Возвращает:
```
Timestamp, Documents, Frames, JSEventListeners, Nodes, LayoutCount, RecalcStyleCount,
LayoutDuration, RecalcStyleDuration, ScriptDuration, TaskDuration, JSHeapUsedSize,
JSHeapTotalSize
```

## 8.2. Coverage

```cpp
void cdtStartCoverage() {
    wsSendCommand("Profiler.enable", {});
    wsSendCommand("Profiler.startPreciseCoverage", {
        {"callCount", false}, {"detailed", true}
    });
}

json cdtTakeCoverage() {
    auto res = wsSendCommand("Profiler.takePreciseCoverage", {});
    return res.value("result", json::array());
}
```

Формат — массив по скриптам с интервалами `[startOffset, endOffset]`, которые реально исполнялись.

## 8.3. Breakpoints на XHR

```cpp
void cdtSetXhrBreakpoint(const std::string& urlSubstring) {
    wsSendCommand("DOMDebugger.setXHRBreakpoint", { {"url", urlSubstring} });
}
```

При срабатывании — `Debugger.paused`. Стек вызовов покажет, какой код дёрнул XHR.

## 8.4. Инжект до загрузки

```cpp
std::string cdtAddScriptToEvaluateOnNewDocument(const std::string& source) {
    auto res = wsSendCommand("Page.addScriptToEvaluateOnNewDocument", { {"source", source} });
    return res.value("identifier", "");
}

void cdtRemoveScriptToEvaluateOnNewDocument(const std::string& id) {
    wsSendCommand("Page.removeScriptToEvaluateOnNewDocument", { {"identifier", id} });
}
```

Пример патча `fetch`:

```js
(() => {
  const origFetch = window.fetch;
  window.__cdt_fetchLog = window.__cdt_fetchLog || [];
  window.fetch = function(...args) {
    window.__cdt_fetchLog.push({
      url: typeof args[0] === 'string' ? args[0] : args[0].url,
      method: (args[1] && args[1].method) || 'GET',
      ts: performance.now(),
      stack: new Error().stack
    });
    return origFetch.apply(this, args);
  };
})();
```

## 8.5. CSP bypass

```cpp
void cdtSetBypassCSP(bool enabled) {
    wsSendCommand("Page.setBypassCSP", { {"enabled", enabled} });
}
```

## 8.6. Block URL

```cpp
void cdtSetBlockedURLs(const std::vector<std::string>& patterns) {
    wsSendCommand("Network.setBlockedURLs", { {"urls", patterns} });
}

void cdtEnableBlockedURLs() {
    wsSendCommand("Network.setBlockedURLs", { {"urls", {"*"}} });
}
```

## 8.7. Virtual time

```cpp
void cdtSetVirtualTime(double budgetMs) {
    wsSendCommand("Emulation.setVirtualTimePolicy", {
        {"policy", "pauseIfNetworkFetchesPending"},
        {"budget", budgetMs},
        {"maxVirtualTimeTaskStarvationCount", 10000}
    });
}
```

## 8.8. Binding

```cpp
void cdtAddBinding(const std::string& name) {
    wsSendCommand("Runtime.addBinding", { {"name", name} });
}
```

В JS: `__name(payload)`. Событие `Runtime.bindingCalled`.

## 8.9. Cookies

```cpp
json cdtGetAllCookies() {
    return wsSendCommand("Network.getAllCookies", {}).value("cookies", json::array());
}

void cdtSetCookie(const std::string& name, const std::string& value,
                  const std::string& domain, const std::string& path = "/")
{
    wsSendCommand("Network.setCookie", {
        {"name", name}, {"value", value}, {"domain", domain}, {"path", path}
    });
}
```

## 8.10. Storage

```cpp
json cdtGetLocalStorage() {
    auto res = evaluateInMain_(
        "JSON.stringify(Object.fromEntries(Object.entries(localStorage)))");
    return json::parse(res.get<std::string>());
}

json cdtGetSessionStorage() {
    auto res = evaluateInMain_(
        "JSON.stringify(Object.fromEntries(Object.entries(sessionStorage)))");
    return json::parse(res.get<std::string>());
}
```

## 8.11. Navigation history

```cpp
json cdtGetNavigationHistory() {
    return wsSendCommand("Page.getNavigationHistory", {});
}

void cdtNavigateToHistoryEntry(int entryId) {
    wsSendCommand("Page.navigateToHistoryEntry", { {"entryId", entryId} });
}
```

---

# Часть IX. Порядок реализации с тестированием

## 9.1. Итерации

### Итерация 1 (1–2 дня)

- `NetworkJournal`, `ConsoleJournal`.
- Хендлер, который складывает `Network.*` и `Runtime.consoleAPICalled`.
- Тест: открыть ya.ru, вывести количество запросов и все ошибки консоли.

### Итерация 2 (1 день)

- `WaitManager` + `cdtWaitForEvent`.
- `cdtWaitForSelector`, `cdtWaitForFunction`.
- `cdtWaitForNetworkIdle`.
- Тест: открыть ya.ru, дождаться появления `textarea#text`, напечатать время ожидания.

### Итерация 3 (1 день)

- `cdtClick`, `cdtInsertText`, `cdtPressKey`.
- `cdtBringToFront`.
- Тест: кликнуть в `textarea#text`, вставить «котики», нажать Enter, дождаться результатов.

### Итерация 4 (2–3 дня)

- `ScriptJournal` + `cdtGetResponseBody` + `cdtGetScriptSource`.
- Source map parsing.
- Тест: собрать все скрипты ya.ru, сохранить в папку, проверить, что можно открыть в VS Code.

### Итерация 5 (1 день)

- `Target.setAutoAttach`.
- `cdtEvaluateInFrame`.
- Тест: страница с iframe (например, встроенное видео), выполнить JS внутри iframe.

### Итерация 6 (1 день)

- `cdtSnapshot`, `cdtDiff`.
- Тест: снять snapshot до и после ввода, увидеть изменения.

### Итерация 7 (2 дня)

- `cdtBuildAiBundle` + все вспомогательные.
- Тест: получить папку bundle для ya.ru, отдать ИИ, спросить «как найти поле поиска и ввести запрос».

## 9.2. Критерии готовности

Для каждой итерации:

- код компилируется без warnings;
- есть пример `_testNN.cpp`, демонстрирующий функцию;
- есть тестовый прогон на 3-5 разных сайтах (статический, SPA, с iframe, с динамикой, с ошибками);
- нет зависаний и утечек памяти.

## 9.3. Хорошие тестовые сайты

- `example.com` — простейший.
- `ya.ru` — динамика, suggest, много запросов.
- `github.com` — сложный SPA, source maps.
- `stackoverflow.com` — обычная страница с iframe рекламы.
- `youtube.com` — стримы, воркеры, service worker.
- Локальный `localhost:8080` — сгенерированная страница с известной структурой.

---

# Часть X. Резюме и общий совет

## 10.1. Что критично, а что можно отложить

**Критично:**
1. Network + Console журналы.
2. Wait-примитивы (selector, network idle).
3. Input-примитивы (click, insertText, pressKey).
4. `cdtBuildAiBundle` хотя бы в базовой форме.

**Сильно повышает качество:**
5. Source maps.
6. Извлечение тел.
7. Учёт iframe/worker.
8. Snapshot/Diff.

**Приятные бонусы:**
9. Coverage.
10. Screenshots/PDF.
11. Performance.
12. Emulation.

## 10.2. Один общий совет

Стройте библиотеку как набор **маленьких независимых сервисов**, а не как один большой класс. `NetworkJournal`, `ConsoleJournal`, `WaitManager`, `TargetJournal` — это отдельные сущности, каждая со своим `std::mutex`, без перекрёстных зависимостей. `Connection` их только связывает.

Это даёт:

- тестируемость (каждый сервис можно проверять отдельно);
- потокобезопасность (один мьютекс на сервис, а не общий);
- заменяемость (например, заменить `NetworkJournal` на версию с записью в SQLite — не трогая остальное);
- расширяемость (добавить `StorageJournal`, `PerformanceJournal` — по аналогии).

И главное: **не пытайтесь сделать всё сразу**. Сначала базовый сценарий «открыть → дождаться → кликнуть → ввести → Enter → дождаться результатов». Затем — сбор данных. Затем — bundle. Затем — диффы.

