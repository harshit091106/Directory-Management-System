//  Directory Management System

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_PATH 4096
#define MAX_CONTENT 2000
#define MAX_NAME 50

// Function prototypes
char* extract_text_content(const char* filepath, char* buffer, size_t bufsize);
void simple_categorize(const char* filepath, const char* content, char* category, char* newname, char* description);
int organize_files(const char* source, const char* target, int preview, int max_files);


int main() {
    char source_dir[MAX_PATH];
    char target_dir[MAX_PATH];
    char preview_str[10];
    int preview = 0;
    int max_files = 50;

    printf("Enter source directory: ");
    fgets(source_dir, sizeof(source_dir), stdin);
    source_dir[strcspn(source_dir, "\n")] = 0;

    printf("Enter target directory (optional, press Enter for default): ");
    fgets(target_dir, sizeof(target_dir), stdin);
    target_dir[strcspn(target_dir, "\n")] = 0;

    if (strlen(target_dir) == 0) {
        if (snprintf(target_dir, sizeof(target_dir), "%s/Organized", source_dir)
            >= sizeof(target_dir)) {
            fprintf(stderr, "ERROR: Target directory path too long.\n");
            return 1;
        }
    }

    printf("Preview only? (y/n): ");
    fgets(preview_str, sizeof(preview_str), stdin);
    preview = (preview_str[0] == 'y' || preview_str[0] == 'Y');

    printf("Starting organization: %s -> %s\n", source_dir, target_dir);

    if (mkdir(target_dir, 0755) == -1 && errno != EEXIST) {
        perror("Failed to create target directory");
        return 1;
    }

    int processed = organize_files(source_dir, target_dir, preview, max_files);
    printf("Complete! %d files processed. Check %s\n", processed, target_dir);

    return 0;
}


// Extracting Text Content
char* extract_text_content(const char* filepath, char* buffer, size_t bufsize) {
    FILE* fp;
    struct stat st;
    char* ext = strrchr(filepath, '.');

    if (stat(filepath, &st) == -1) {
        snprintf(buffer, bufsize, "Could not read %s", filepath);
        return buffer;
    }

    if (ext && (strcmp(ext, ".txt") == 0 || strcmp(ext, ".md") == 0)) {
        fp = fopen(filepath, "r");
        if (fp) {
            fread(buffer, 1, MAX_CONTENT - 1, fp);
            buffer[MAX_CONTENT - 1] = 0;
            fclose(fp);
            return buffer;
        }
    }

    snprintf(buffer, bufsize, "File: %s", filepath);
    return buffer;
}


// Categorizing
void simple_categorize(const char* filepath, const char* content,
                       char* category, char* newname, char* description)
{
    char lowerpath[MAX_PATH];
    strncpy(lowerpath, filepath, sizeof(lowerpath) - 1);
    lowerpath[sizeof(lowerpath) - 1] = 0;

    for (int i = 0; lowerpath[i]; i++)
        lowerpath[i] = tolower(lowerpath[i]);

    char* ext = strrchr(lowerpath, '.');
    const char* clean_ext = (ext ? ext + 1 : "");

    if (ext && (strcmp(ext, ".pdf") == 0 || strcmp(ext, ".txt") == 0 ||
                strcmp(ext, ".md") == 0 || strcmp(ext, ".docx") == 0)) {
        strcpy(category, "Documents");
        snprintf(newname, MAX_NAME, "document.%s", clean_ext);

    } else if (ext && (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 ||
                       strcmp(ext, ".png") == 0 || strcmp(ext, ".gif") == 0 ||
                       strcmp(ext, ".bmp") == 0)) {
        strcpy(category, "Images");
        snprintf(newname, MAX_NAME, "image.%s", clean_ext);

    } else if (ext && (strcmp(ext, ".py") == 0 || strcmp(ext, ".c") == 0 ||
                       strcmp(ext, ".cpp") == 0 || strcmp(ext, ".java") == 0 ||
                       strcmp(ext, ".js") == 0 || strcmp(ext, ".html") == 0 ||
                       strcmp(ext, ".css") == 0)) {
        strcpy(category, "Code");
        snprintf(newname, MAX_NAME, "code.%s", clean_ext);

    } else if (ext && (strcmp(ext, ".mp4") == 0 || strcmp(ext, ".avi") == 0 ||
                       strcmp(ext, ".mkv") == 0 || strcmp(ext, ".mp3") == 0 ||
                       strcmp(ext, ".wav") == 0)) {
        strcpy(category, "Media");
        snprintf(newname, MAX_NAME, "media.%s", clean_ext);

    } else if (ext && (strcmp(ext, ".zip") == 0 || strcmp(ext, ".rar") == 0 ||
                       strcmp(ext, ".tar") == 0 || strcmp(ext, ".gz") == 0)) {
        strcpy(category, "Archives");
        snprintf(newname, MAX_NAME, "archive.%s", clean_ext);

    } else {
        strcpy(category, "Others");
        const char* fname = strrchr(filepath, '/');
        fname = (fname ? fname + 1 : filepath);
        strncpy(newname, fname, MAX_NAME - 1);
        newname[MAX_NAME - 1] = 0;
    }

    snprintf(description, 100, "Rule-based categorization for %s", category);
}


// Organizing Files
int organize_files(const char* source, const char* target_base, int preview, int max_files) {
    DIR* dir;
    struct dirent* entry;

    char content[MAX_CONTENT];
    char category[50];
    char newname[MAX_NAME];
    char description[100];

    char target_path[MAX_PATH];
    char new_path[MAX_PATH];
    char filepath[MAX_PATH];

    int files_processed = 0;
  //Accessing the source directory
    if ((dir = opendir(source)) == NULL) {
        perror("Cannot open source directory");
        return 0;
    }
    //reading each file in the directory 
    while ((entry = readdir(dir)) != NULL && files_processed < max_files) {
        if (entry->d_type != DT_REG)
            continue;

        if (snprintf(filepath, sizeof(filepath), "%s/%s", source, entry->d_name)
            >= sizeof(filepath)) {
            fprintf(stderr, "Path too long: %s/%s\n", source, entry->d_name);
            continue;
        }

        printf("Processing: %s\n", entry->d_name);

        extract_text_content(filepath, content, sizeof(content));
        simple_categorize(filepath, content, category, newname, description);

        printf("  -> Category: %s, New: %s (%s)\n", category, newname, description);

        if (!preview) {
            if (snprintf(target_path, sizeof(target_path), "%s/%s", target_base, category)
                >= sizeof(target_path)) {
                fprintf(stderr, "Target path too long.\n");
                continue;
            }

            if (mkdir(target_path, 0755) == -1 && errno != EEXIST) {
                perror("Warning: Could not create subdirectory");
                continue;
            }

            // First attempt
            if (snprintf(new_path, sizeof(new_path), "%s/%s", target_path, newname)
                >= sizeof(new_path)) {
                fprintf(stderr, "File path too long.\n");
                continue;
            }

            // Filename collision handling
            int counter = 1;
            char base[MAX_NAME];
            char ext[20];

            strcpy(base, newname);

            char* dot = strrchr(base, '.');
            if (dot) {
                strcpy(ext, dot);
                *dot = 0;
            } else {
                ext[0] = 0;
            }

            while (access(new_path, F_OK) == 0) {
                if (snprintf(new_path, sizeof(new_path), "%s/%s_%d%s",
                             target_path, base, counter, ext)
                    >= sizeof(new_path)) {
                    fprintf(stderr, "Filename too long.\n");
                    break;
                }
                counter++;
            }

            if (rename(filepath, new_path) == 0)
                printf("  Moved to: %s\n", new_path);
            else
                perror("Move failed");
        }

        files_processed++;
    }

    closedir(dir);
    return files_processed;
}
