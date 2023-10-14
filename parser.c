#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(char *path)
{
    struct stat st;

    if (!path || stat(path, &st) != 0)
        return 0;

    return (st.st_mode & S_IFREG) != 0;
}

/**
 * extract_substring - extracts a substring from pathstr
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: dynamically allocated buffer with the extracted substring
 */
char *extract_substring(const char *pathstr, int start, int stop)
{
    char *buf = malloc(stop - start + 1);
    if (!buf)
        return NULL;

    int k = 0;
    for (int i = start; i < stop; i++)
    {
        if (pathstr[i] != ':')
            buf[k++] = pathstr[i];
    }
    buf[k] = '\0';
    return buf;
}

/**
 * find_path - finds this cmd in the PATH string
 * @info: the info struct (currently unused)
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_path(info_t *info, const char *pathstr, const char *cmd)
{
    if (!pathstr)
        return NULL;

    if (_strlen(cmd) > 2 && starts_with(cmd, "./") && is_cmd(cmd))
        return _strdup(cmd);

    int i = 0, curr_pos = 0;
    char *path = NULL;

    while (pathstr[i] != '\0')
    {
        if (pathstr[i] == ':')
        {
            path = extract_substring(pathstr, curr_pos, i);
            if (*path != '\0')
            {
                _strcat(path, "/");
                _strcat(path, cmd);
            }
            if (is_cmd(path))
                return path;

            curr_pos = i + 1;
        }
        i++;
    }

    return NULL;
}
