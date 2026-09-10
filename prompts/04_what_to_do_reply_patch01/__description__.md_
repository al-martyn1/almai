Да, можно. Есть несколько уровней «вытащить всё», от простого к сложному.

---

## 1. Просто сохранить всё, что уже загружено браузером

### Вариант A: `Page.captureSnapshot` — MHTML

CDP умеет отдавать MHTML-снимок страницы (как «Сохранить как → Веб-страница, один файл»):

```json
{"method":"Page.captureSnapshot","params":{"format":"mhtml"}}
```

Возвращает строку MHTML со всеми ресурсами, которые браузер уже подтянул: HTML, inline- и внешние CSS, скрипты, картинки, шрифты, iframe-документы. Один файл — можно открыть в любом браузере, распаковать MIME-парсером и анализировать.

Ограничения:
- только то, что успело загрузиться;
- некоторые ресурсы могут быть не включены (blob:, CORS-ограничения, dynamic imports, которые ещё не запрашивались);
- нет «живого» состояния после скриптов — это статический снимок.

Отлично подходит как «первый заход»: вы получаете один файл, где рядом лежат HTML, все CSS и все JS.

### Вариант B: обойти `document` и вытащить всё вручную через `Runtime.evaluate`

Можно пройтись по DOM и собрать всё в JSON:

```js
(async () => {
  const out = {html: null, styles: [], scripts: [], resources: []};

  out.html = document.documentElement.outerHTML;

  // Внешние и inline CSS
  for (const sheet of document.styleSheets) {
    let cssText = null, href = sheet.href;
    try {
      cssText = Array.from(sheet.cssRules).map(r => r.cssText).join('\n');
    } catch (e) {
      cssText = '<CORS-blocked: ' + e.message + '>';
    }
    out.styles.push({href, cssText});
  }

  // Скрипты в DOM
  for (const s of document.scripts) {
    out.scripts.push({
      src: s.src || null,
      type: s.type || null,
      inline: s.src ? null : s.textContent
    });
  }

  // Все ресурсы, которые браузер реально загрузил
  out.resources = performance.getEntriesByType('resource').map(e => ({
    name: e.name,
    initiatorType: e.initiatorType,
    duration: e.duration,
    transferSize: e.transferSize,
    encodedBodySize: e.encodedBodySize
  }));

  return out;
})()
```

Запускать через `Runtime.evaluate` с `awaitPromise:true, returnByValue:true`. Получите большой JSON, где отдельно HTML, все CSS-правила и список всех загруженных URL.

Полезно ещё дополнить:

```js
// inline-скрипты, не только те, что в <script>
document.querySelectorAll('script:not([src])').forEach(...)
// <link rel="stylesheet">, <link rel="modulepreload">, <link rel="preload">
// все data-* атрибуты
// <template> и <slot>
```

---

## 2. Сохранить всё как «папку файлов» — как в DevTools

Чтобы получить отдельные файлы, а не MHTML, надо:

1. **Собрать список URL**:

```js
performance.getEntriesByType('resource').map(e => e.name)
```

Плюс отдельно `document.styleSheets[i].href`, `document.scripts[i].src`, `document.querySelectorAll('img[src]')` и т.д.

2. **Скачать их через `Network.getResponseBody`**:

Сначала включаете `Network.enable`, потом приходит событие `Network.responseReceived` с `requestId`. Для каждого нужного `requestId` вызываете:

```json
{"method":"Network.getResponseBody","params":{"requestId":"..."}}
```

Получите `{body, base64Encoded}`. Декодируете base64 и получаете ровно те байты, которые браузер получил (включая gzip-распакованное).

3. **Сохранить на диск**, сохранив структуру путей, чтобы можно было открыть HTML локально.

Это уже полноценная «офлайн-копия». Плюс — реальные исходники; минус — нужно самим писать рекурсию, поддерживать относительные пути, переписывать URL в HTML или хранить как есть и открывать через `file://` с исходной структурой.

---

## 3. Получить исходники, а не то, что «получилось» после минификации

Браузер в общем случае возвращает уже собранные бандлы. Чтобы читать их по-человечески, надо:

### 3.1. Красиво отформатировать

Через `Debugger.getScriptSource` вы получаете те же минифицированные исходники. Но DevTools умеет притворяться, что отформатировал их. Есть CDP-метод:

```json
{"method":"Debugger.getScriptSource","params":{"scriptId":"..."}}
```

И для source maps:

```json
{"method":"Debugger.enable"}
```

`Debugger.scriptParsed` приходит с полем `sourceMapURL`. Скачиваете `.map`-файл как обычный ресурс, и у вас есть исходные файлы + связь со скомпилированными. Можно восстановить исходный код.

### 3.2. Принудительно заставить браузер отдавать исходники

`Page.addScriptToEvaluateOnNewDocument` + `Debugger.setBlackboxPatterns` не помогут — это не то.

Реально помогает другое: серверы часто отдают не только минифицированный бандл, но и `.map`, `.ts`, `.jsx`-версии по прямым URL. Можно попробовать подставить суффиксы `.map`, заглянуть в `sourceMappingURL=` в конце скрипта, и скачать source map — там будут оригинальные файлы.

### 3.3. Перехват `network` на старте

Чтобы получить всё, включая lazy-load, надо:

- создать новую вкладку/страницу через `Target.createTarget`,
- сразу `Network.enable`, `Page.enable`, `Runtime.enable`, `Debugger.enable`,
- потом `Page.navigate`.

Тогда поймаете все запросы с самого начала, включая те, что делаются до `document.load`.

---

## 4. Достать «эффективный документ» с уже изменённым DOM

Учтите важный момент: `outerHTML` — это то, что реально в DOM сейчас, уже после всех скриптов. А в MHTML попадёт, скорее, то, что прислал сервер (плюс правки, сделанные скриптами до снимка).

Если нужно «что прислал сервер»:

- либо сразу после навигации, до `Page.loadEventFired`,
- либо `Network.getResponseBody` для запроса типа `Document`,
- либо `Fetch.enable` с `patterns` на `Document` — тогда вы перехватите ответ и сами решите, отдавать его странице или нет.

Если нужно «что стало после скриптов»:

- `Runtime.evaluate` `document.documentElement.outerHTML`,
- либо `DOM.getDocument` с `depth:-1, pierce:true` — вернёт развёрнутое дерево целиком.

---

## 5. Практическая сборка «полного слепка»

Минимальный сценарий, который даёт вам всё для анализа:

1. Запустить Chrome с `--remote-debugging-port`.
2. Подключиться по WS.
3. `Network.enable`, `Page.enable`, `Runtime.enable`, `Debugger.enable`, `Target.setAutoAttach` (чтобы поймать iframe и воркеры).
4. `Page.navigate(url)`.
5. Ждать `Page.loadEventFired` + ещё N секунд (или конкретный селектор), чтобы отработали lazy-инициализации.
6. Выполнить:

```js
JSON.stringify({
  html: document.documentElement.outerHTML,
  scripts: Array.from(document.scripts).map(s => ({src: s.src, type: s.type, text: s.textContent})),
  styles: Array.from(document.styleSheets).map(s => ({href: s.href, ownerNode: s.ownerNode && s.ownerNode.outerHTML})),
  resources: performance.getEntriesByType('resource').map(e => ({name: e.name, type: e.initiatorType})),
  links: Array.from(document.querySelectorAll('link')).map(l => ({rel: l.rel, href: l.href, as: l.as, type: l.type}))
})
```

7. Сохранить в файл.
8. Отдельно пройтись по всем `resources[i].name` через `Network.getResponseBody` и сложить тела.
9. `Page.captureSnapshot` для MHTML — на случай, если что-то потеряли.

---

## 6. Готовые инструменты, если не хочется всё писать

- **`chrome --save-page-as-mhtml`** — из CLI.
- **Puppeteer / Playwright**: `page.content()`, `page.mhtml()`, `page.route('**', ...)` — на порядок короче писать, чем на голом CDP. Можно использовать из C++ через SDK, но это отдельная история.
- **`wget --page-requisites --convert-links`** — грубо, но иногда хватает; не отдаёт SPA-динамику.
- **Chrome DevTools → Sources → Save all as HAR / Copy all as cURL** — вручную.
- **HAR-файл**: `Network`-события можно писать в HAR, там будут все запросы с телами. Это ровно то, что нужно для офлайн-анализа сети. Если своего HAR-писателя нет — можно посмотреть, что делает puppeteer-har, и повторить на CDP.

---

## 7. Что реально стоит сделать в вашей библиотеке

Я бы добавил такие функции:

- `cdtPageCaptureSnapshotMhtml()` → `Page.captureSnapshot`.
- `cdtGetFullPageDump()` → один `Runtime.evaluate` со сбором html/scripts/styles/resources.
- `cdtDownloadAllResources(outDir)` → из `performance.getEntriesByType('resource')` + `Network.getResponseBody` для каждого `requestId`, с сохранением относительных путей.
- `cdtEnableHar(url)` → пишет HAR во время навигации.
- `cdtGetScriptSource(scriptId)` и автоскачивание source maps.

Тогда «вытащить всё» — это вызов одной функции, а дальше уже парсите локально сколько угодно.

---

## 8. Мелочи, о которые легко споткнуться

- `Network.getResponseBody` работает, только пока `requestId` ещё в кэше вкладки. Если сделаете `Network.disable`/`clearBrowserCache` — тела не отдаст. Собирайте тела сразу по `Network.loadingFinished`.
- `performance.getEntriesByType('resource')` не показывает запросы из iframe (они в `performance` самого iframe). Нужно обходить фреймы через `Page.getFrameTree` и для каждого — свой `Runtime.evaluate`.
- Source map URL может быть относительным (`//# sourceMappingURL=main.js.map`). Резолвить относительно скрипта.
- Некоторые скрипты приходят через `blob:` / `data:` / service worker — их нет в `Network`. Service worker вообще отдельный `Target`.
- `Debugger.getScriptSource` отдаёт то же, что и `Network.getResponseBody`, но для inline-скриптов это единственный способ получить их «как их увидел движок» (уже с учётом `eval`).
- Если на странице есть `iframe srcdoc`, его содержимое не придёт в `Network` вообще — только через DOM.
