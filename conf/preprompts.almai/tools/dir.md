---
author: Alexander Martynov (al-martyn1) <amart@mail.ru>
title : dir tool

---

<tool id="dir" version="1" category="filesystem">
  <purpose>
    List files and directories under a project-relative path, optionally
    filtered by one or more glob masks. Use this tool whenever you need to
    know what files exist before referencing them.
  </purpose>

  <when_to_use>
    - You need to enumerate files in a directory.
    - You need to confirm whether a file exists.
    - You need to find files matching a pattern (e.g. all *.c and *.h).
    - You are about to reference a path you have not seen in the context.
  </when_to_use>

  <when_not_to_use>
    - The file contents are already in `<docs>` or `<attachments>`.
    - The path is a single file already visible in the manifest.
    - You are guessing. If you guess, the call will fail.
  </when_not_to_use>

  <grammar>
    spec     := "/" path ("/" mask_list)?
    path     := segment ("/" segment)*
    segment  := name | "**"
    mask_list:= mask ("," mask)*
    mask     := ["-"] glob

    Notes:
    - "**" as a path segment means recursive descent from that point.
    - The last path segment may be either a literal filename or the first mask.
    - Masks are comma-separated. A "-" prefix excludes matching files even
      if they matched an earlier inclusion mask.
    - Paths are always absolute relative to the project root (start with "/").
  </grammar>

  <parameters>
    <param name="spec" required="true" type="string">
      The full path-and-mask specification. Exactly one spec per call.
      Do not put multiple specs into one call.
    </param>
  </parameters>

  <examples>
    <example spec="/CMakeLists.txt">
      Single file at project root.
    </example>
    <example spec="/firmware/board/**/CMakeLists.txt">
      All CMakeLists.txt files under firmware/board, recursively.
    </example>
    <example spec="/src/*.c*,*.h,-*.hex">
      All files starting with *.c* plus all *.h under /src (non-recursive),
      excluding *.hex.
    </example>
    <example spec="/startup/**/*.S,*.ld">
      All *.S and *.ld files anywhere under /startup, recursively.
    </example>
    <example spec="/.vscode/launch.json">
      Single config file.
    </example>
  </examples>

  <invocation>
    Emit a tool call as the LAST block of your response, using exactly this
    XML form. Do not wrap it in a code fence. Do not add commentary after it.

    <tool_call name="dir">
      <arg name="spec">/src/*.c*,*.h,-*.hex</arg>
    </tool_call>

    You may emit more than one tool call per turn, but only if they are
    truly independent. Never call the same spec twice in one turn.
  </invocation>

  <result_format>
    The user will return results in this format. Parse the tree; do not
    reinterpret it.

    <tool_result name="dir" spec="/src/*.c*,*.h,-*.hex" status="ok">
    /src/
      main.c
      utils.c
      utils.h
      platform/
        gpio.c
        gpio.h
    </tool_result>

    On failure:

    <tool_result name="dir" spec="/nonexistent/*.c" status="error">
      Path not found: /nonexistent/
    </tool_result>
  </result_format>

  <rules>
    - Emit at most one tool call per distinct spec per turn.
    - Never fabricate a result. If you need data, emit a tool call and stop.
    - After receiving a result, continue reasoning; do not re-request the
      same spec.
    - If a spec returns zero files, assume the spec was wrong — try a
      broader spec before concluding the files do not exist.
    - Never use `dir` to read file contents; it only lists names.
  </rules>
</tool>
