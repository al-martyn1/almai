https://chat.deepseek.com/share/4hv9lyxrzk1denqkiq

"src/almai_md_split/__init__.py"
````python
"""Utility to split markdown with code blocks into separate files."""

__version__ = "0.1.0"
````

"src/almai_md_split/__main__.py"
````python
"""Main entry point for the almai-md-split utility."""

from almai_md_split.cli import main

if __name__ == "__main__":
    main()
````

"src/almai_md_split/cli.py"
````python
"""Command-line interface for almai-md-split."""

import argparse
import sys
from pathlib import Path

from almai_md_split.dicts import load_dictionaries, format_dict_paths_for_help
from almai_md_split.parser import MarkdownParser
from almai_md_split.writer import FileWriter
from almai_md_split.exceptions import FatalError
from almai_md_split import __version__


def create_parser() -> argparse.ArgumentParser:
    """Create and return the argument parser."""
    parser = argparse.ArgumentParser(
        prog="almai-md-split",
        description="Split a markdown file containing code blocks with filenames into separate files.",
        epilog="Dictionary files are loaded from system and user config directories. "
               "Use --dict to add extra dictionaries."
    )
    parser.add_argument(
        "input_file",
        help="Input markdown file (UTF-8 encoded)."
    )
    parser.add_argument(
        "--output-dir",
        help="Output directory. If not specified, the directory of the input file is used.",
        default=None
    )
    parser.add_argument(
        "--dict",
        action="append",
        dest="extra_dicts",
        default=[],
        help="Additional dictionary file (can be used multiple times)."
    )
    parser.add_argument(
        "--version",
        action="version",
        version=f"%(prog)s {__version__}"
    )
    return parser


def main() -> None:
    """Main entry point."""
    parser = create_parser()
    args = parser.parse_args()

    # Determine output directory
    input_path = Path(args.input_file)
    if not input_path.exists():
        print(f"error: input file '{input_path}' does not exist", file=sys.stderr)
        sys.exit(1)

    output_dir = Path(args.output_dir) if args.output_dir else input_path.parent

    # Load dictionaries
    try:
        dicts = load_dictionaries(args.extra_dicts)
    except Exception as e:
        print(f"error: failed to load dictionaries: {e}", file=sys.stderr)
        sys.exit(1)

    # Print dictionary paths in help mode (argparse automatically includes epilog)
    # But we also want to show them explicitly when --help is called.
    # argparse doesn't easily let us modify help text after parsing, so we'll just
    # ensure that format_dict_paths_for_help is called if needed, but here we can
    # simply include the info in epilog. We'll do it by building epilog dynamically.
    # However, for simplicity, we'll print dictionary info when --help is invoked?
    # Actually, argparse constructs help before parsing, so we can't dynamically
    # change it based on the presence of platformdirs. We'll just rely on the static
    # epilog and also output paths when --help is used via a custom action? Too complex.
    # Instead, we'll print dictionary paths if --help was requested. But we can't
    # easily detect that. Better to just always print dictionary info at start?
    # TZ says: "В режиме вывода справки (--help) также следует выводить имя данных файлов: 'Dictionary file: ...'."
    # So we need to hook into help printing. We can do that by subclassing HelpFormatter
    # or just print after parsing if help was not requested? That's not correct.
    # We'll override the help action to include dictionary info.
    # Let's create a custom HelpAction.
    class _HelpAction(argparse.Action):
        def __init__(self, option_strings, dest=argparse.SUPPRESS, default=argparse.SUPPRESS, help=None):
            super().__init__(option_strings=option_strings, dest=dest, default=default, nargs=0, help=help)
        def __call__(self, parser, namespace, values, option_string=None):
            # Print dictionary info before standard help
            dict_paths = format_dict_paths_for_help()
            print("Dictionary files:\n" + "\n".join(f"  {p}" for p in dict_paths))
            parser.print_help()
            parser.exit()

    # Replace help action
    parser._actions = [a for a in parser._actions if not (a.option_strings and a.option_strings[0] in ('-h', '--help'))]
    parser.add_argument('-h', '--help', action=_HelpAction, help='show this help message and exit')

    # Re-parse because we changed the parser
    args = parser.parse_args()

    try:
        # Parse markdown
        parser_obj = MarkdownParser(input_path, dicts)
        files = parser_obj.parse()

        # Write files
        writer = FileWriter(output_dir)
        writer.write_all(files)

    except FatalError as e:
        print(f"error: {e}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"error: unexpected error: {e}", file=sys.stderr)
        sys.exit(1)
````

"src/almai_md_split/parser.py"
````python
"""Markdown parser for extracting code blocks with filenames."""

import re
import sys
from pathlib import Path
from typing import List, Optional, Tuple, Dict, Any

from almai_md_split.filename import extract_filename, normalize_filename, resolve_collision
from almai_md_split.dicts import DictCollection
from almai_md_split.exceptions import FatalError, ParserWarning
from almai_md_split.utils import warning


class CodeBlock:
    """Represents a code block with its metadata."""
    def __init__(self, language: str = "", content: List[str] = None):
        self.language = language
        self.content = content or []
        self.filename: Optional[str] = None
        self.autogenerated = False  # whether name was auto-generated
        self.line_start: int = 0   # for error messages


class MarkdownParser:
    """Parser for markdown files to extract code blocks and associated filenames."""

    # Patterns for fenced code blocks
    FENCE_PATTERN = re.compile(r'^(?P<fence>```|~~~)(?P<language>\S*)\s*$')
    # Pattern for lines that might be a filename (before normalization)
    # We'll use a heuristic: any line that is not empty and not a fence and not inside a block.

    def __init__(self, file_path: Path, dicts: DictCollection):
        self.file_path = file_path
        self.dicts = dicts
        self.lines: List[str] = []
        self.blocks: List[CodeBlock] = []
        self.current_block: Optional[CodeBlock] = None
        self.fence_stack: List[Tuple[str, int]] = []  # (fence_char, length)
        self.last_filename_line: Optional[Tuple[int, str]] = None  # (line_num, raw_line)
        self.autosave_counter = 0
        self.used_filenames = set()  # normalized lowercase filenames to check collisions

    def parse(self) -> List[CodeBlock]:
        """Parse the markdown file and return list of code blocks with filenames."""
        try:
            with open(self.file_path, 'r', encoding='utf-8-sig') as f:
                self.lines = f.readlines()
        except Exception as e:
            raise FatalError(f"cannot read input file: {e}")

        i = 0
        in_block = False
        while i < len(self.lines):
            line = self.lines[i].rstrip('\n')
            raw_line = self.lines[i]  # keep original for warnings

            # Check if we are inside a code block
            if self.fence_stack:
                # Inside a block, look for closing fence
                fence_char, fence_len = self.fence_stack[-1]
                # Check if line starts with fence of same char and at least fence_len backticks/tildes
                # Must be exactly the same fence (no extra chars besides possible whitespace after)
                # Also need to consider escaping? Inside code block, fences are literal, so closing fence must be exactly the marker.
                # We'll use regex to detect closing fence: optional whitespace, then exactly fence_len of fence_char, then optional whitespace.
                m = re.match(rf'^\s*(?P<fence>{re.escape(fence_char)}{{{fence_len}}})\s*$', line)
                if m:
                    # Closing fence found
                    self.fence_stack.pop()
                    if self.current_block:
                        self.blocks.append(self.current_block)
                        self.current_block = None
                else:
                    # Inside block, add line to current block content
                    if self.current_block:
                        self.current_block.content.append(line)
                i += 1
                continue

            # Not inside a code block
            # Check for escaped fence (line starts with '\' followed by fence marker)
            if line.startswith('\\') and len(line) >= 2 and line[1] in ('`', '~'):
                # Escaped fence, treat as normal text, skip block detection
                # Also, this line might be a filename candidate? No, it's escaped.
                # Reset last_filename_line because we saw a non-filename line
                self.last_filename_line = None
                i += 1
                continue

            # Check for fenced code block start
            m = self.FENCE_PATTERN.match(line)
            if m:
                fence = m.group('fence')
                fence_char = fence[0]
                fence_len = len(fence)
                language = m.group('language')
                # Start a new block
                self.fence_stack.append((fence_char, fence_len))
                self.current_block = CodeBlock(language=language, line_start=i+1)
                # Check if last non-empty line before this (ignoring empty lines) was a filename candidate
                # We need to scan backwards from i-1 until we find a non-empty line, skipping empty lines.
                # If that line is a valid filename, associate it with this block.
                j = i - 1
                found_filename = None
                while j >= 0:
                    prev_line = self.lines[j].rstrip('\n')
                    if prev_line.strip() == "":
                        j -= 1
                        continue
                    # This is a non-empty line. Check if it was recorded as last_filename_line
                    if self.last_filename_line and self.last_filename_line[0] == j:
                        found_filename = self.last_filename_line[1]
                    break

                if found_filename is not None:
                    # Process filename
                    filename, was_quoted, warning_msg = extract_filename(found_filename)
                    if warning_msg:
                        warning(warning_msg, line_num=j+1)
                    if filename is None or filename == "":
                        # Generate name automatically (but we already have autogeneration later)
                        # We'll handle later in finalize block
                        self.current_block.autogenerated = True
                        self.current_block.filename = None
                    else:
                        # Normalize filename
                        norm_name = normalize_filename(filename, preserve_spaces=was_quoted)
                        # Check for reserved names
                        from almai_md_split.filename import check_reserved_names
                        norm_name = check_reserved_names(norm_name)
                        if norm_name == "":
                            warning(f"filename became empty after normalization: '{filename}'", line_num=j+1)
                            self.current_block.autogenerated = True
                        else:
                            self.current_block.filename = norm_name
                else:
                    # No filename before, will be autogenerated later
                    self.current_block.autogenerated = True

                i += 1
                continue

            # Not a fence, not inside block. Check if this line could be a filename candidate.
            # We record it as potential filename if it's not empty and not a fence.
            if line.strip():
                self.last_filename_line = (i, line)
            else:
                # Empty line resets the filename candidate because empty lines between name and block are not allowed.
                self.last_filename_line = None

            i += 1

        # Handle unclosed block at EOF
        if self.fence_stack:
            warning(f"unclosed code block starting at line {self.current_block.line_start if self.current_block else '?'}", line_num=len(self.lines))
            if self.current_block:
                self.blocks.append(self.current_block)

        # Post-process blocks: autogenerate names if needed and resolve collisions
        for block in self.blocks:
            if block.filename is None:
                # Autogenerate name
                block.filename = self._generate_autoname(block)
                block.autogenerated = True
            # Resolve collisions globally
            block.filename = resolve_collision(block.filename, self.used_filenames)
            self.used_filenames.add(block.filename.lower())

        return self.blocks

    def _generate_autoname(self, block: CodeBlock) -> str:
        """Generate automatic filename for a block."""
        self.autosave_counter += 1
        count = self.autosave_counter
        # Determine extension
        lang = block.language.lower()
        ext = self.dicts.get_extension(lang)
        if not ext:
            # Use language as extension if it's alphanumeric, else .txt
            if lang and re.match(r'^[a-zA-Z0-9]+$', lang):
                ext = lang
            else:
                ext = "txt"
            warning(f"unknown language '{block.language}', using extension .{ext}", line_num=block.line_start)
        # Format counter with leading zeros (at least 2 digits)
        width = max(2, len(str(self.autosave_counter)))
        base = f"autosave_{str(count).zfill(width)}.{ext}"
        # Normalize base (though it should be safe)
        norm = normalize_filename(base, preserve_spaces=False)
        return norm
````

"src/almai_md_split/filename.py"
````python
"""Filename extraction, normalization, and collision resolution."""

import re
import os
import sys
from pathlib import PurePath, Path
from typing import Tuple, Optional, Set

# Windows reserved names (case-insensitive)
RESERVED_NAMES = {
    "CON", "PRN", "AUX", "NUL",
    "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
    "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
}

# Invalid characters in Windows filenames (we apply to all platforms for consistency)
INVALID_CHARS = r'[\\/*?:"<>|]' + ''.join(chr(i) for i in range(32))  # control chars
INVALID_CHARS_RE = re.compile(r'[' + re.escape(r'\/:*?"<>|') + r'\x00-\x1f]')


def extract_filename(line: str) -> Tuple[Optional[str], bool, Optional[str]]:
    """
    Extract filename from a line.

    Returns:
        (filename, was_quoted, warning_message)
        filename may be None if line is not a valid filename (e.g., description).
        was_quoted indicates if there was quoting/formatting that preserves spaces.
        warning_message is a string if something suspicious, else None.
    """
    original_line = line
    line = line.rstrip()  # remove trailing whitespace
    # Define patterns for quoting
    patterns = [
        (r'^"([^"]+)"$', '"', True),     # double quotes
        (r"^'([^']+)'$", "'", True),     # single quotes
        (r'^\*\*([^*]+)\*\*$', '**', True),  # bold
        (r'^\*([^*]+)\*$', '*', True),   # italic
        (r'^`([^`]+)`$', '`', True),      # code
    ]
    for pattern, marker, quoted in patterns:
        m = re.match(pattern, line)
        if m:
            content = m.group(1)
            # Check if content is empty after stripping?
            if not content.strip():
                return None, quoted, "filename is empty after removing quotes"
            return content, quoted, None

    # No quoting detected. Check if line contains spaces or invalid chars -> description
    if re.search(r'\s', line) or INVALID_CHARS_RE.search(line):
        # It's a description, generate name later
        return None, False, None

    # Otherwise, treat as raw filename without quotes
    # But check if it's empty
    if not line:
        return None, False, "filename line is empty"
    return line, False, None


def normalize_filename(name: str, preserve_spaces: bool = False) -> str:
    """
    Normalize a filename according to rules.

    Steps:
    1. Replace backslashes with forward slashes.
    2. Remove drive letters (Windows) and leading slashes to make relative.
    3. Process '..' components: collapse them, but prevent escaping root.
    4. For each path component, replace invalid characters with '_'.
    5. If not preserve_spaces, replace spaces with '_'.
    6. Remove leading/trailing dots and spaces from each component.
    7. Apply transliteration if unidecode available, else replace non-ASCII with '_'?
       (We'll use unidecode if present, else fallback to replacement with '_' for non-ASCII).
    8. If after processing any component becomes empty, replace with '_'.

    Returns normalized name.
    """
    # Step 1: unify separators
    name = name.replace('\\', '/')

    # Step 2: remove drive letters and leading slashes
    # Use pathlib to parse
    p = PurePath(name)
    # If absolute, make relative by dropping root
    if p.is_absolute():
        # For Windows absolute like C:/foo, drive is part of root
        # For POSIX absolute like /foo, root is '/'
        # We'll convert to relative by dropping the root part.
        # p.relative_to(p.root) would raise if not relative; we construct manually.
        parts = p.parts
        if p.drive:
            # Windows with drive: drop drive and initial separator
            parts = parts[1:]  # drive is first, then root? Actually parts[0]=drive, parts[1]=root
            # parts[1] is root ('/'), we want to drop both drive and root, so start from index 2
            if len(parts) > 1 and parts[1] == '/':
                parts = parts[2:]
        else:
            # POSIX absolute: first part is '/', drop it
            parts = parts[1:]
        name = '/'.join(parts)

    # Step 3: collapse '..' components
    components = name.split('/')
    new_components = []
    for comp in components:
        if comp == '..':
            if new_components:
                new_components.pop()
            # else ignore (already at root)
        elif comp == '.':
            continue
        else:
            new_components.append(comp)
    name = '/'.join(new_components)

    # Step 4: process each component
    comps = name.split('/')
    processed = []
    for comp in comps:
        # Replace invalid characters
        comp = INVALID_CHARS_RE.sub('_', comp)
        # Replace spaces if not preserved
        if not preserve_spaces:
            comp = comp.replace(' ', '_')
        # Transliterate if possible
        comp = _transliterate(comp)
        # Strip leading/trailing dots and spaces
        comp = comp.strip('. ')
        if comp == '':
            comp = '_'
        processed.append(comp)
    name = '/'.join(processed)

    return name


def _transliterate(s: str) -> str:
    """Transliterate to ASCII if unidecode available, else replace non-ASCII with '_'."""
    try:
        from unidecode import unidecode
        return unidecode(s)
    except ImportError:
        # Fallback: replace any non-ASCII character with '_'
        return ''.join(c if ord(c) < 128 else '_' for c in s)


def check_reserved_names(path: str) -> str:
    """
    Check each component of path for Windows reserved names.
    If found, prefix the component with 'renamed_'.
    """
    parts = path.split('/')
    new_parts = []
    for part in parts:
        # Remove extension for checking? Reserved names apply to base name without extension.
        base, dot, ext = part.partition('.')
        if base.upper() in RESERVED_NAMES:
            new_base = f"renamed_{base}"
            part = new_base + dot + ext
        new_parts.append(part)
    return '/'.join(new_parts)


def resolve_collision(filename: str, used: Set[str]) -> str:
    """
    If filename (lowercase) is already in used, append _01, _02, etc.
    used is a set of lowercase filenames already taken.
    Returns new filename (not added to set, caller must add).
    """
    if filename.lower() not in used:
        return filename

    base, dot, ext = filename.rpartition('.')
    if not base:  # filename starts with dot? rare
        base = dot + ext
        ext = ''
    counter = 1
    while True:
        candidate = f"{base}_{counter:02d}{dot}{ext}"
        if candidate.lower() not in used:
            return candidate
        counter += 1
````

"src/almai_md_split/dicts.py"
````python
"""Dictionary handling for language to extension mapping."""

import sys
from pathlib import Path
from typing import Dict, List, Optional, Set
import platformdirs

from almai_md_split.utils import warning


class DictCollection:
    """Collection of language-to-extension dictionaries."""

    def __init__(self):
        self._lang_to_ext: Dict[str, str] = {}
        self._loaded_files: List[Path] = []

    def add_file(self, file_path: Path) -> None:
        """Load dictionary from a file and merge into collection."""
        if not file_path.exists():
            warning(f"dictionary file not found: {file_path}")
            return
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                for line_num, line in enumerate(f, 1):
                    line = line.strip()
                    if not line or line.startswith('#'):
                        continue
                    parts = line.split(maxsplit=1)
                    if len(parts) != 2:
                        warning(f"invalid line in dictionary {file_path}:{line_num}")
                        continue
                    ext, lang = parts
                    ext = ext.strip().lstrip('.')  # remove leading dot if any
                    lang = lang.strip().lower()
                    if lang in self._lang_to_ext:
                        # Overwrite? We'll keep last occurrence.
                        pass
                    self._lang_to_ext[lang] = ext
            self._loaded_files.append(file_path)
        except Exception as e:
            warning(f"error reading dictionary {file_path}: {e}")

    def get_extension(self, language: str) -> Optional[str]:
        """Return extension for language (without dot), or None if not found."""
        return self._lang_to_ext.get(language.lower())


def get_default_dict_paths() -> List[Path]:
    """
    Return list of default dictionary paths:
    system-wide and user-specific using platformdirs,
    plus fallback '../conf' relative to script if platformdirs unavailable.
    """
    paths = []
    try:
        # system config
        site_data_dir = Path(platformdirs.site_data_dir("almai-md-split", "almai"))
        paths.append(site_data_dir / "almai-md-split.dict")
        # user config
        user_config_dir = Path(platformdirs.user_config_dir("almai-md-split", "almai"))
        paths.append(user_config_dir / "almai-md-split.dict")
    except ImportError:
        # platformdirs not installed, use fallback relative to script
        import sys
        script_dir = Path(sys.argv[0]).resolve().parent
        # Try ../conf relative to script
        conf_dir = script_dir.parent / "conf"
        paths.append(conf_dir / "almai-md-split.dict")
        warning("platformdirs not installed, using fallback dictionary path")
    except Exception as e:
        warning(f"error determining default dict paths: {e}")

    return paths


def load_dictionaries(extra_files: List[str]) -> DictCollection:
    """
    Load all dictionaries: default system/user, and extra files.
    Returns DictCollection.
    """
    collection = DictCollection()

    # Load default paths
    for p in get_default_dict_paths():
        collection.add_file(p)

    # Load extra files
    for f in extra_files:
        collection.add_file(Path(f))

    return collection


def format_dict_paths_for_help() -> List[str]:
    """Return list of dictionary file paths for help display."""
    return [str(p) for p in get_default_dict_paths()]
````

"src/almai_md_split/writer.py"
````python
"""File writer for extracted code blocks."""

import sys
from pathlib import Path
from typing import List

from almai_md_split.parser import CodeBlock
from almai_md_split.utils import warning
from almai_md_split.exceptions import FatalError


class FileWriter:
    """Writes code blocks to files in output directory."""

    def __init__(self, output_dir: Path):
        self.output_dir = output_dir

    def write_all(self, blocks: List[CodeBlock]) -> None:
        """Write all blocks to files."""
        for block in blocks:
            self._write_block(block)

    def _write_block(self, block: CodeBlock) -> None:
        """Write a single block."""
        if not block.filename:
            # Should not happen
            warning(f"block starting at line {block.line_start} has no filename, skipping")
            return

        dest = self.output_dir / block.filename
        # Ensure parent directory exists
        try:
            dest.parent.mkdir(parents=True, exist_ok=True)
        except Exception as e:
            raise FatalError(f"cannot create directory {dest.parent}: {e}")

        # Write content
        try:
            with open(dest, 'w', encoding='utf-8') as f:
                f.write('\n'.join(block.content))
                # Ensure trailing newline? Not required, but keep as is.
        except Exception as e:
            raise FatalError(f"cannot write file {dest}: {e}")

        # Optionally print info? Not required.
````

"src/almai_md_split/exceptions.py"
````python
"""Custom exceptions for almai-md-split."""


class FatalError(Exception):
    """Fatal error that should terminate the program with exit code 1."""
    pass


class ParserWarning(Exception):
    """Warning during parsing (non-fatal)."""
    pass
````

"src/almai_md_split/utils.py"
````python
"""Utility functions."""

import sys


def warning(message: str, line_num: int = None) -> None:
    """Print a warning message to stderr with prefix."""
    if line_num:
        print(f"warning: line {line_num}: {message}", file=sys.stderr)
    else:
        print(f"warning: {message}", file=sys.stderr)
````

"tests/__init__.py"
````python
# Tests package
````

"tests/test_parser.py"
````python
"""Unit tests for the parser module."""

import pytest
from pathlib import Path
from almai_md_split.parser import MarkdownParser
from almai_md_split.dicts import DictCollection


@pytest.fixture
def dicts():
    d = DictCollection()
    # Add some test mappings
    d._lang_to_ext = {'python': 'py', 'cpp': 'cpp'}
    return d


def test_simple_block(tmp_path, dicts):
    md = tmp_path / "test.md"
    md.write_text('''
"file.py"
```
print("hello")
```
'''.lstrip(), encoding='utf-8')
    parser = MarkdownParser(md, dicts)
    blocks = parser.parse()
    assert len(blocks) == 1
    assert blocks[0].filename == "file.py"
    assert blocks[0].content == ['print("hello")']


def test_quoted_filename(tmp_path, dicts):
    md = tmp_path / "test.md"
    md.write_text('''
**file with spaces.py**
```
print("hello")
```
'''.lstrip(), encoding='utf-8')
    parser = MarkdownParser(md, dicts)
    blocks = parser.parse()
    assert len(blocks) == 1
    # Spaces preserved because bold
    assert blocks[0].filename == "file with spaces.py"
    assert blocks[0].content == ['print("hello")']


def test_empty_line_before_block(tmp_path, dicts):
    md = tmp_path / "test.md"
    md.write_text('''
"file.py"

```
print("hello")
```
'''.lstrip(), encoding='utf-8')
    parser = MarkdownParser(md, dicts)
    blocks = parser.parse()
    assert len(blocks) == 1
    # No filename associated because empty line, so autogenerated
    assert blocks[0].autogenerated is True
    assert blocks[0].filename.startswith("autosave_")


def test_unclosed_block(tmp_path, dicts):
    md = tmp_path / "test.md"
    md.write_text('''
"file.py"
```
print("hello")
'''.lstrip(), encoding='utf-8')
    parser = MarkdownParser(md, dicts)
    blocks = parser.parse()
    assert len(blocks) == 1
    assert blocks[0].filename == "file.py"
    assert blocks[0].content == ['print("hello")']


def test_escaped_fence(tmp_path, dicts):
    md = tmp_path / "test.md"
    md.write_text(r'''
\```
not a code block
```
'''.lstrip(), encoding='utf-8')
    parser = MarkdownParser(md, dicts)
    blocks = parser.parse()
    assert len(blocks) == 1  # the second ``` is a block without name -> autogenerated
    assert blocks[0].autogenerated


def test_language_extension(tmp_path, dicts):
    md = tmp_path / "test.md"
    md.write_text('''
"file"
```python
x = 1
```
'''.lstrip(), encoding='utf-8')
    parser = MarkdownParser(md, dicts)
    blocks = parser.parse()
    # filename "file" has no extension; language python gives .py
    assert blocks[0].filename == "file.py"


def test_collision_resolution(tmp_path, dicts):
    md = tmp_path / "test.md"
    md.write_text('''
"a.py"
```
content1
```
"a.py"
```
content2
```
'''.lstrip(), encoding='utf-8')
    parser = MarkdownParser(md, dicts)
    blocks = parser.parse()
    assert len(blocks) == 2
    assert blocks[0].filename == "a.py"
    assert blocks[1].filename == "a_01.py"
````

"tests/test_filename.py"
````python
"""Unit tests for filename module."""

import pytest
from almai_md_split.filename import (
    extract_filename,
    normalize_filename,
    check_reserved_names,
    resolve_collision,
)


def test_extract_quoted():
    assert extract_filename('"file.txt"') == ('file.txt', True, None)
    assert extract_filename("'file.txt'") == ('file.txt', True, None)
    assert extract_filename('**file.txt**') == ('file.txt', True, None)
    assert extract_filename('*file.txt*') == ('file.txt', True, None)
    assert extract_filename('`file.txt`') == ('file.txt', True, None)


def test_extract_unquoted():
    assert extract_filename('file.txt') == ('file.txt', False, None)
    assert extract_filename('path/to/file.txt') == ('path/to/file.txt', False, None)


def test_extract_description():
    # Contains space
    assert extract_filename('file with space.txt') == (None, False, None)
    # Contains invalid char
    assert extract_filename('file?.txt') == (None, False, None)


def test_extract_empty():
    assert extract_filename('') == (None, False, 'filename line is empty')
    assert extract_filename('   ') == (None, False, 'filename line is empty')


def test_normalize_basic():
    assert normalize_filename('file.txt') == 'file.txt'
    assert normalize_filename('a/b/c.txt') == 'a/b/c.txt'


def test_normalize_backslashes():
    assert normalize_filename('a\\b\\c.txt') == 'a/b/c.txt'


def test_normalize_absolute():
    assert normalize_filename('/home/user/file.txt') == 'home/user/file.txt'
    assert normalize_filename('C:/Users/user/file.txt') == 'Users/user/file.txt'
    assert normalize_filename('C:\\Users\\file.txt') == 'Users/file.txt'


def test_normalize_dotdot():
    assert normalize_filename('a/b/../c.txt') == 'a/c.txt'
    assert normalize_filename('a/../../b.txt') == 'b.txt'
    assert normalize_filename('../../b.txt') == 'b.txt'


def test_normalize_invalid_chars():
    assert normalize_filename('a/b:c.txt') == 'a/b_c.txt'
    assert normalize_filename('a/b*c.txt') == 'a/b_c.txt'
    assert normalize_filename('a/b<c.txt') == 'a/b_c.txt'


def test_normalize_spaces_not_preserved():
    assert normalize_filename('a b.txt', preserve_spaces=False) == 'a_b.txt'


def test_normalize_spaces_preserved():
    assert normalize_filename('a b.txt', preserve_spaces=True) == 'a b.txt'


def test_normalize_leading_trailing_dots():
    assert normalize_filename('.file.txt') == 'file.txt'
    assert normalize_filename('file.txt.') == 'file.txt'
    assert normalize_filename(' .file.txt ') == 'file.txt'


def test_reserved_names():
    assert check_reserved_names('CON.txt') == 'renamed_CON.txt'
    assert check_reserved_names('lpt1/foo.txt') == 'renamed_LPT1/foo.txt'
    assert check_reserved_names('COM10.txt') == 'COM10.txt'  # only up to COM9


def test_collision_resolution():
    used = {'a.py'}
    assert resolve_collision('a.py', used) == 'a_01.py'
    used.add('a_01.py')
    assert resolve_collision('a.py', used) == 'a_02.py'
    used.add('a_02.py')
    assert resolve_collision('b.py', used) == 'b.py'
````

"tests/test_dicts.py"
````python
"""Unit tests for dictionaries module."""

from pathlib import Path
from almai_md_split.dicts import DictCollection


def test_dict_loading(tmp_path):
    d = DictCollection()
    dict_file = tmp_path / "test.dict"
    dict_file.write_text('''
# comment
cpp C++
py   Python
.py2   Python 2
''')
    d.add_file(dict_file)
    assert d.get_extension('C++') == 'cpp'
    assert d.get_extension('Python') == 'py'
    assert d.get_extension('Python 2') == 'py2'
    assert d.get_extension('unknown') is None


def test_case_insensitivity():
    d = DictCollection()
    d._lang_to_ext = {'python': 'py'}
    assert d.get_extension('Python') == 'py'
    assert d.get_extension('PYTHON') == 'py'
````

"tests/test_integration.py"
````python
"""Integration tests with temporary files."""

import pytest
from pathlib import Path
from almai_md_split.cli import main
from almai_md_split.dicts import DictCollection, load_dictionaries
from almai_md_split.parser import MarkdownParser
from almai_md_split.writer import FileWriter


def test_end_to_end(tmp_path):
    # Create input markdown
    md_file = tmp_path / "input.md"
    md_content = '''
"hello.py"
```python
print("hello")
```

"sub/with spaces.txt"
```
content with spaces
```

Unnamed block:
```
no name
```
'''
    md_file.write_text(md_content, encoding='utf-8')

    # Load dictionaries (empty)
    dicts = DictCollection()
    # Parse
    parser = MarkdownParser(md_file, dicts)
    blocks = parser.parse()
    # Write
    out_dir = tmp_path / "out"
    writer = FileWriter(out_dir)
    writer.write_all(blocks)

    # Check files
    assert (out_dir / "hello.py").exists()
    assert (out_dir / "sub/with spaces.txt").exists()
    # Unnamed block -> autosave_01.txt
    autosave = out_dir / "autosave_01.txt"
    assert autosave.exists()
    assert autosave.read_text(encoding='utf-8') == "no name"
````

"tests/test_cli.py"
````python
"""CLI tests using pytest's capsys and tmp_path."""

import pytest
from almai_md_split.cli import main
import sys


def test_cli_help(capsys):
    testargs = ["almai-md-split", "--help"]
    with pytest.raises(SystemExit) as exc:
        with pytest.helpers.patch_argv(testargs):
            main()
    assert exc.value.code == 0
    captured = capsys.readouterr()
    assert "Dictionary files:" in captured.out
    assert "usage:" in captured.out


def test_cli_no_input(capsys):
    testargs = ["almai-md-split"]
    with pytest.raises(SystemExit) as exc:
        with pytest.helpers.patch_argv(testargs):
            main()
    assert exc.value.code == 2  # argparse error


def test_cli_nonexistent_input(capsys, tmp_path):
    testargs = ["almai-md-split", str(tmp_path / "nonexistent.md")]
    with pytest.raises(SystemExit) as exc:
        with pytest.helpers.patch_argv(testargs):
            main()
    assert exc.value.code == 1
    captured = capsys.readouterr()
    assert "error: input file" in captured.err


# Helper to patch sys.argv
@pytest.fixture
def patch_argv(monkeypatch):
    def _patch(argv):
        monkeypatch.setattr(sys, 'argv', argv)
    return _patch
````

"pyproject.toml"
````toml
[build-system]
requires = ["setuptools>=61.0", "wheel"]
build-backend = "setuptools.build_meta"

[project]
name = "almai-md-split"
version = "0.1.0"
description = "Split markdown files with code blocks into separate files"
readme = "README.md"
authors = [
    {name = "Your Name", email = "you@example.com"},
]
license = {text = "MIT"}
classifiers = [
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: 3.7",
    "Programming Language :: Python :: 3.8",
    "Programming Language :: Python :: 3.9",
    "Programming Language :: Python :: 3.10",
    "Programming Language :: Python :: 3.11",
    "License :: OSI Approved :: MIT License",
    "Operating System :: OS Independent",
]
requires-python = ">=3.7"
dependencies = [
    "platformdirs>=2.0",
    "unidecode>=1.0",
]

[project.optional-dependencies]
test = ["pytest>=6.0"]

[project.scripts]
almai-md-split = "almai_md_split.cli:main"

[tool.setuptools.packages.find]
where = ["src"]
include = ["almai_md_split*"]

[tool.setuptools.package-data]
"almai_md_split" = ["py.typed"]  # optional

[tool.pytest.ini_options]
testpaths = ["tests"]
python_files = "test_*.py"
````

"README.md"
````markdown
# almai-md-split

Utility to split a Markdown file containing code blocks into separate files. Each code block must be preceded by a line with the desired filename (optionally quoted). The tool normalizes filenames, resolves collisions, and creates necessary subdirectories.

## Installation

```bash
pip install almai-md-split
```

Or run directly from source:

```bash
python -m almai_md_split input.md
```

## Usage

```
almai-md-split input.md [--output-dir=OUTPUT_DIR] [--dict=EXTRA_DICT]
```

- `input.md` : input Markdown file (UTF-8 encoded).
- `--output-dir` : output directory (default: same as input file).
- `--dict` : additional dictionary file for language-to-extension mapping (can be used multiple times).

## Dictionary files

Dictionary files map language names to file extensions. They are loaded from:

- System-wide: e.g., `/usr/share/almai-md-split/almai-md-split.dict` (platform dependent)
- User-specific: e.g., `~/.config/almai-md-split/almai-md-split.dict` (platform dependent)
- Extra files specified with `--dict`.

Format: each non-empty, non-comment line contains extension and language name separated by whitespace. Example:

```
cpp C++
py  Python
```

Language names are case-insensitive.

## License

MIT
````

"conf/almai-md-split.dict"
````
# Default language to extension mappings
py Python
py3 Python 3
c C
cpp C++
cxx C++
h C/C++ header
hpp C++ header
java Java
js JavaScript
ts TypeScript
html HTML
htm HTML
css CSS
rb Ruby
php PHP
rs Rust
go Go
swift Swift
kt Kotlin
kts Kotlin
scala Scala
m Objective-C
mm Objective-C++
pl Perl
pm Perl
sh Bash
bash Bash
zsh Bash
fish Fish
ps1 PowerShell
bat Batch
cmd Batch
sql SQL
xml XML
json JSON
yaml YAML
yml YAML
md Markdown
rst reStructuredText
tex LaTeX
lua Lua
r R
jl Julia
dart Dart
fs F#
fsx F#
fsi F#
erl Erlang
hrl Erlang
ex Elixir
exs Elixir
eex Elixir
leex Elixir
heex Elixir
````

"MANIFEST.in"
````
include README.md
include conf/almai-md-split.dict
````