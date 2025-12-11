# Directory Management System

A lightweight C-based utility for automatically organizing files into categorized directories based on file type. This tool reads files from a source directory, analyzes their content and extensions, categorizes them intelligently, and moves them to organized subdirectories.

## Features

- **Automatic File Categorization**: Intelligently categorizes files into:
  - **Documents**: PDF, TXT, MD, DOCX
  - **Images**: JPG, JPEG, PNG, GIF, BMP
  - **Code**: PY, C, CPP, JAVA, JS, HTML, CSS
  - **Media**: MP4, AVI, MKV, MP3, WAV
  - **Archives**: ZIP, RAR, TAR, GZ
  - **Others**: Uncategorized files

- **Text Content Extraction**: Reads text and markdown files to assist with categorization
- **Collision Handling**: Automatically handles filename conflicts by appending counters
- **Preview Mode**: Test the organization without moving files
- **Configurable Limits**: Supports processing up to a specified number of files
- **Error Handling**: Robust error management for edge cases like missing directories

## Requirements

- C compiler (GCC, Clang, etc.)
- POSIX-compliant system (Linux, macOS, Unix)
- Standard C libraries

## Compilation

```bash
gcc -o dir_manager directory_management.c
```

## Usage

Run the program interactively:

```bash
./dir_manager
```

You will be prompted to provide:

1. **Source Directory**: The directory containing files to organize
2. **Target Directory**: Where organized files will be placed (defaults to `source_dir/Organized`)
3. **Preview Mode**: Enter `y` or `Y` to preview changes without moving files, any other input to proceed

### Example

```
Enter source directory: /home/user/Downloads
Enter target directory (optional, press Enter for default): /home/user/Organized
Preview only? (y/n): y
```

## How It Works

1. **Scans** the source directory for regular files
2. **Extracts** text content from `.txt` and `.md` files
3. **Categorizes** each file based on its extension
4. **Creates** category subdirectories in the target location
5. **Moves** files to appropriate subdirectories with organized names
6. **Handles** duplicate filenames automatically

## Configuration

Edit the constants at the top of the file to customize:

```c
#define MAX_PATH 4096      // Maximum path length
#define MAX_CONTENT 2000   // Maximum content buffer size
#define MAX_NAME 50        // Maximum filename length
```

You can also modify the `max_files` variable in `main()` to change the file processing limit (default: 50).

## Limitations

- Only processes regular files (not directories)
- File content extraction limited to text and markdown files
- Maximum path length of 4096 characters
- Limited to 50 files by default (configurable)
- Processes files only one directory level deep (non-recursive)

## Function Reference

### `extract_text_content()`
Extracts text from `.txt` and `.md` files for content analysis.

### `simple_categorize()`
Determines file category and generates a new standardized filename based on file extension.

### `organize_files()`
Main processing function that scans the directory, categorizes files, and performs the organization.

## License

This project is open source. Feel free to modify and distribute as needed.

## Contributing

Contributions are welcome! Feel free to fork, modify, and submit pull requests.

## Author

Created as a directory management utility for organizing files efficiently.

## Support

If you encounter any issues, please check:
- Ensure the source directory exists and is readable
- Verify write permissions in the target directory
- Check available disk space
- Review file permissions

For bugs or feature requests, please open an issue on the GitHub repository.
