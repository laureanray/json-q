# json-q

A stripped-down [jq](https://jqlang.github.io/jq/) written by hand in C11.

`json-q` reads JSON from a file or stdin, applies a small filter language, and
prints the result. The goal is **not** full jq compatibility — it is a small,
readable implementation that covers the everyday one-liners, built as a way to
practice C.

> **Practice rule:** the C code is written by hand, one milestone at a time.
> This README is the plan and the progress tracker. No code is generated.

---

## Final result (definition of done)

At v1.0, `json-q` is a single dependency-free executable that can do this:

```console
$ cat input.json
{
  "name": "json-q",
  "version": 0.1,
  "tags": ["c", "json", "jq"],
  "author": { "name": "lr" }
}

$ json-q '.name' input.json
"json-q"

$ json-q -r '.tags[]' input.json
c
json
jq

$ json-q '.author.name' input.json
"lr"

$ json-q 'keys[]' input.json
"author"
"name"
"tags"
"version"

$ json-q -c '{name, tags}' input.json
{"name":"json-q","tags":["c","json","jq"]}

$ echo '{"a":[1,2,3]}' | json-q '(.a | length)'
3
```

In other words:

- parse **all** valid JSON (objects, arrays, strings, numbers, booleans, null),
  including nesting and escapes;
- evaluate a **small filter language** over that JSON;
- print results either pretty or compact, with correct JSON escaping;
- fail with a clear message and a meaningful exit code on bad input;
- be leak-free and crash-free on malformed input.

Everything outside the scope below is intentionally rejected with a syntax
error — never silently ignored or evaluated incorrectly.

---

## Supported CLI (target)

```text
json-q [OPTIONS] FILTER [FILE]

  -c, --compact        compact output (no pretty printing)
  -r, --raw            print strings without quotes
  -e, --exit-status    exit 1 if the last output is false/null or there is no output
  -h, --help           show usage
      --version        show version

If FILE is omitted or is "-", read from stdin.
```

### Supported filters (v1.0)

| Feature | Example |
| --- | --- |
| Identity | `.` |
| Field access | `.foo`, `.foo.bar` |
| Bracket access | `.["foo"]` |
| Array index | `.[0]`, `.[-1]` |
| Iteration | `.[]` |
| Pipe | `.a \| .b` |
| Multiple outputs | `.a, .b` |
| Parentheses | `(.a \| length)` |
| Literals in filters | `"text"`, `1`, `true`, `false`, `null` |
| Array construction | `[.items[]]` |
| Object construction | `{name: .a, count: (.b \| length)}`, shorthand `{name}` |
| Builtins | `keys`, `length`, `type`, `has`, `select`, `map` |

### Exit codes (our contract, simpler than jq's)

| Code | Meaning |
| --- | --- |
| 0 | success |
| 1 | `-e` and last output was `false`/`null`, or no output |
| 2 | usage error or I/O error (missing file, bad option) |
| 3 | filter syntax error |
| 4 | invalid JSON input |

---

## Pipeline

```text
             ┌────────────┐   ┌────────────┐   ┌───────────┐
file/stdin ──┤ JSON lexer ├──►│ JSON parser├──►│ JsonValue │
             └────────────┘   └────────────┘   └─────┬─────┘
                                                     │
             ┌─────────────┐   ┌────────────┐   ┌────▼─────┐   ┌────────┐
filter ─────►│ filter lexer├──►│ filter AST │──►│ evaluator├──►│ printer│──► stdout
             └─────────────┘   └────────────┘   └──────────┘   └────────┘
```

Two separate lexers/parsers: one for JSON data, one for the filter language.
Keeping them separate is deliberate and simpler than trying to share tokens.

---

## Current state (snapshot)

- [x] CMake project builds the `jsonq` executable from all `src/*.c`
- [x] `loadFile()` reads a whole file into a buffer
- [x] Lexer handles `{ } [ ] : ,` and simple `"strings"`
- [x] Parser handles a flat object of `string -> string` only
- [x] `main` prints top-level keys from a hardcoded `../input.json`
- [ ] everything else (see milestones)

Known problems to fix along the way (all are useful exercises):

- `_expect_peek` recurses forever on a mismatch instead of reporting an error
- `JsonMember` fields (`key`, `value`, `next`) are not always initialized
- `loadFile` allocates exactly `file_size` bytes, so the buffer is not
  NUL-terminated (lexer calls `strlen` on it)
- `_parse_json_value` leaks/behaves badly on token types it does not handle
- nothing is freed anywhere, and the input path is hardcoded

---

## Roadmap

Work top-down. Do not start the next milestone until the current one is green
under ASan and has at least one test. One checkbox = one small commit.

### Milestone 0 — Build & workflow

- [x] C11, `-Wall -Wextra`
- [x] CMake target `jsonq`
- [ ] build out-of-source: `cmake -S . -B build && cmake --build build`
- [ ] remove or ignore the generated `Makefile` at the repo root (pick **one**
      build system; don't keep a generated Makefile checked in)
- [ ] add a strict debug config with `-Werror`
- [ ] add ASan + UBSan flags for debug builds
- [ ] add a `test` target (`ctest` or a shell runner)
- [ ] document the build commands in this README

### Milestone 1 — CLI & input handling

- [ ] parse `-c`, `-r`, `-e`, `-h`, `--version`
- [ ] positional `FILTER`, optional `FILE`
- [ ] read stdin when `FILE` is omitted or `-`
- [ ] read stdin fully (works with pipes; no seeking required)
- [ ] NUL-terminate the input buffer
- [ ] stop hardcoding `../input.json`
- [ ] usage message to stderr on bad args, exit 2
- [ ] clear error for missing/unreadable file, exit 2
- [ ] accept input with no trailing newline
- [ ] reject extra positional arguments

### Milestone 2 — JSON lexer

- [x] punctuation: `{ } [ ] : ,`
- [x] basic strings
- [ ] numbers: `-?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][+-]?[0-9]+)?`
- [ ] keywords: `true`, `false`, `null`
- [ ] string escapes: `\"  \\  \/  \b  \f  \n  \r  \t`
- [ ] unicode escapes `\uXXXX`, including surrogate pairs → UTF-8
- [ ] reject raw control characters inside strings
- [ ] line/column tracking for error messages
- [ ] skip only space, tab, CR, LF
- [ ] reject unknown characters with position info
- [ ] table-driven token tests

### Milestone 3 — JSON parser & value model

- [x] flat object with string values (partial)
- [ ] arrays
- [ ] numbers
- [ ] booleans and null
- [ ] nesting: objects/arrays inside objects/arrays
- [ ] strict RFC 8259: no trailing commas, no comments, no single quotes
- [ ] duplicate keys: last one wins (document this)
- [ ] parse errors as `line:column: message`
- [ ] recursive `free` for the whole value tree
- [ ] fix the known parser bugs listed above
- [ ] parse every file in the test corpus and compare with `jq .`

### Milestone 4 — Filter lexer & parser

- [ ] decide against reusing the JSON lexer; write a dedicated filter lexer
- [ ] tokens: `.`, `[`, `]`, `{`, `}`, `|`, `,`, `(`, `)`, `:`, identifiers,
      string/number literals
- [ ] AST node kinds: identity, field, index, iterate, pipe, comma, literal,
      array construction, object construction, builtin call
- [ ] `.foo` and `.foo.bar`
- [ ] `.["foo"]`
- [ ] `.[0]`, `.[-1]`
- [ ] `.[]`
- [ ] `|` precedence
- [ ] `,` precedence (lower than `|`)
- [ ] parentheses
- [ ] filter literals: `"s"`, `1`, `true`, `false`, `null`
- [ ] object shorthand `{name}`
- [ ] array construction `[filter]`
- [ ] syntax errors with position, exit 3
- [ ] optional `--debug-ast` dump for learning/debugging

### Milestone 5 — Evaluator

- [ ] write down the ownership rule (who owns a `JsonValue` after each step)
- [ ] evaluate a filter to a **stream** of values
- [ ] `.` identity
- [ ] field access: missing key → `null`
- [ ] index: out of range → `null`
- [ ] negative array index
- [ ] `.[]` over array elements and object values
- [ ] `|` feeds every output into the next stage
- [ ] `,` concatenates output streams
- [ ] `[filter]` collects the stream into one array
- [ ] `{...}` builds an object, preserving insertion order
- [ ] decide and document behavior of field access on non-objects
- [ ] `-e` exit-status logic
- [ ] no leaks on any evaluation path

### Milestone 6 — Builtins

- [ ] `keys` (object: sorted keys; array: indices)
- [ ] `length` (string length; array/object size)
- [ ] `type` (`"object"`, `"array"`, `"string"`, `"number"`, `"boolean"`, `"null"`)
- [ ] `has(k)` (string for objects, number for arrays)
- [ ] `select(f)` (keep input when `f` is not `false`/`null`)
- [ ] `map(f)` (equivalent to `[.[] | f]`)
- [ ] builtin lookup table: name → arity → function pointer
- [ ] wrong number of arguments is a **compile** error (exit 3)

### Milestone 7 — Output

- [ ] pretty printer: 2-space indent, `": "`, one array/object element per line
- [ ] `-c` compact output
- [ ] `-r` raw output for strings only
- [ ] correct JSON escaping on output
- [ ] one result printed after another; no separators, no "null" for empty streams
- [ ] empty stream → no output (and exit 1 with `-e`)
- [ ] numbers keep their original spelling (no arithmetic in v1, so no float
      formatting pitfalls) — document this decision
- [ ] round-trip test: `json-q '.' input.json` equals `jq '.' input.json`
      (modulo key order, see below)

### Milestone 8 — Tests & hardening

- [ ] `tests/cases/<name>/` with `input.json`, `filter`, `expected.out`
- [ ] shell runner `tests/run.sh` comparing actual vs expected
- [ ] table-driven lexer/parser unit tests
- [ ] differential test against real `jq` for the supported subset
- [ ] malformed-JSON corpus: no crash, exit 4
- [ ] malformed-filter corpus: no crash, exit 3
- [ ] ASan/UBSan clean over the whole corpus
- [ ] leak check (`leaks` on macOS, Valgrind on Linux)
- [ ] optional: simple random mutator for fuzzing
- [ ] optional: GitHub Actions CI

### Milestone 9 — v1.0 release

- [ ] `--version` prints `json-q 1.0.0`
- [ ] README has final usage + limitation list
- [ ] every acceptance command below passes
- [ ] tag `v1.0.0`

---

## v1.0 acceptance criteria

These commands must all work against a fixture containing an object with
strings, numbers, booleans, null, nested objects, and arrays:

```sh
json-q '.' input.json                 # round-trip
json-q '.name' input.json
json-q '.author.name' input.json
json-q '.tags[0]' input.json
json-q '.tags[-1]' input.json
json-q '.tags[]' input.json
json-q -r '.tags[]' input.json
json-q 'keys[]' input.json
json-q '.author | .name' input.json
json-q '[.tags[]]' input.json
json-q '{name, count: (.tags | length)}' input.json
json-q -c '.' input.json
cat input.json | json-q '.name'      # stdin
json-q --version                      # json-q 1.0.0
```

Hard requirements:

- invalid JSON → exit 4, message on stderr, no crash
- invalid filter → exit 3, message on stderr, no crash
- `-e` with `false`/`null`/empty result → exit 1
- no leaks, no undefined behavior under ASan/UBSan

---

## Non-goals (v1.x)

Deliberately **out of scope**; unsupported syntax must produce a clear error:

- arithmetic and comparison operators (`+`, `-`, `*`, `==`, `<`, …)
- `//` alternative operator, `?` error suppression, `try/catch`
- assignment (`=`, `|=`, `+=`, `del`)
- variables (`as $x`), `reduce`, `foreach`
- user-defined functions, modules, imports
- string interpolation `"\(.x)"`
- regex (`test`, `match`, `capture`)
- format strings (`@base64`, `@csv`, `@tsv`, `@json`)
- `--slurp`, `--arg`, `--argjson`, `--rawfile`
- streaming (`--stream`), date functions
- full jq semantic compatibility

If a feature is missing, say so in the error message. Silent wrong output is
the only real failure mode.

---

## Stretch goals (after v1.0)

- comparison and arithmetic operators
- `sort`, `sort_by`, `unique`, `group_by`, `min`, `max`, `add`
- `to_entries`, `from_entries`, `with_entries`
- `del`, assignment operators
- `--slurp`, `--arg`, `--argjson`
- `@base64`, `@csv`, `@tsv`
- `try/catch`, `//`, `?`
- variables and `reduce`
- simple `test`/`match` without full regex
- user-defined functions
- CMake install target and man page

---

## C skills this exercises

| Milestone | C practice |
| --- | --- |
| 0 | build systems, compiler warnings, sanitizers |
| 1 | `argc`/`argv`, stdio, dynamic input, exit codes |
| 2 | state machines, char classification, growable buffers, UTF-8 |
| 3 | recursive descent, tagged unions, linked lists/dynamic arrays, ownership |
| 4 | tokenizer design, precedence, AST allocation |
| 5 | tree walking, streams, memory ownership, error propagation |
| 6 | function-pointer tables, arity checking, dispatch |
| 7 | formatting, escaping, buffered output |
| 8 | testing, `lldb`, ASan/UBSan, leak checking, fuzzing |
| 9 | CLI/API polish, documentation |

---

## Working conventions

- C11, zero third-party dependencies.
- `-Wall -Wextra -Werror` clean at all times.
- One concern per module (`lexer`, `parser`, `eval`, `print`, `file`).
- Pick one naming style and use it everywhere; the current code mixes
  `new_parser` and `lexerCreate`. Decide now, then be consistent.
- Functions that can fail return a status; no `exit()` deep inside library code.
- Every allocation has a documented owner. Free in reverse order.
- Run ASan after every milestone, not at the end.
- Commit per checkbox. Small commits make the learning visible.

---

## References

- jq manual — <https://jqlang.github.io/jq/manual/>
- JSON spec (RFC 8259) — <https://www.rfc-editor.org/rfc/rfc8259>
- jq source (for exact semantics) — <https://github.com/jqlang/jq>
