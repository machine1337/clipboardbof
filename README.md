# Clipboard BOF - Cobalt Strike Clipboard Retrieval

A Beacon Object File (BOF) for Cobalt Strike that retrieves clipboard text content from the target system.

## Features

- **Text retrieval** - Supports both Unicode (UTF-16) and ANSI clipboard formats
- **No dependencies** - Dynamically resolves all required Windows APIs
- **BOF compatible** - Runs in Beacon's memory without dropping files
- **Simple usage** - Single command operation

## Usage

```
clipboard get
```

### Example Output

```
beacon> help clipboard
[*] Usage: clipboard get|help

Commands:
  get   - Retrieve clipboard text content
  help  - Show help menu

Example:
  clipboard get
Developed By: Machine1337

beacon> clipboard get
[*] clipboard get
This is the clipboard text content from the target system.
```

```
beacon> clipboard get
[*] clipboard get
[*] Clipboard: [empty or non-text data]
```

## Files

| File | Description |
|------|-------------|
| `clipboard.c` | Main BOF source code |
| `clipboard.cna` | Cobalt Strike Aggressor script |
| `clipboard.x64.o` | Compiled x64 BOF (build yourself) |
| `clipboard.x86.o` | Compiled x86 BOF (build yourself) |
| `beacon.h` | Beacon API header (from Cobalt Strike) |

## Compilation

### Requirements
- MinGW-w64 (`x86_64-w64-mingw32-gcc`)
- MinGW-w32 (`i686-w64-mingw32-gcc`) for 32-bit support

### Build

```bash
# Build for x64 (64-bit Beacon)
x86_64-w64-mingw32-gcc -Os -fno-stack-check -mno-stack-arg-probe -c clipboard.c -o clipboard.x64.o

# Build for x86 (32-bit Beacon)
i686-w64-mingw32-gcc -Os -fno-stack-check -mno-stack-arg-probe -c clipboard.c -o clipboard.x86.o
```

## Installation

1. Copy `clipboard.cna` and the compiled `.o` files to your Cobalt Strike scripts folder
2. Load `clipboard.cna` in Cobalt Strike: **Cobalt Strike** → **Script Manager** → **Load**
3. Use `clipboard get` command in Beacon console

## Technical Details

- **Max clipboard size**: 64KB
- **Supported formats**: CF_UNICODETEXT (UTF-16), CF_TEXT (ANSI)
- **API Resolution**: Dynamically loads `user32.dll` and `kernel32.dll` functions
- **Output**: Plain text with Unicode characters converted to ASCII (non-ASCII chars shown as `?`)

## Notes

- The BOF uses `OpenClipboard(NULL)` which requires the current process to have clipboard access
- If the clipboard contains non-text data (images, files, etc.), it will report `[empty or non-text data]`
- Some applications may lock the clipboard; the BOF will report failure to open in such cases

## Troubleshooting

| Issue | Solution |
|-------|----------|
| "Failed to resolve required APIs" | Ensure user32.dll and kernel32.dll are accessible |
| "Failed to open clipboard" | Another application may be holding the clipboard lock |
| "[empty or non-text data]" | Clipboard contains images, files, or other non-text data |
| BOF not found errors | Ensure .o files are in the same folder as .cna |

## Credits

BOF implementation for Cobalt Strike clipboard access.
