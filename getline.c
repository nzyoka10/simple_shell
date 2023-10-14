#include "shell.h"

/**
 * input_buf - Read input from STDIN into a buffer and process chained commands.
 * @info: Parameter struct.
 * @buf: Address of the buffer.
 * @len: Address of the length variable.
 *
 * Return: Bytes read.
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
    ssize_t read_bytes = 0;
    size_t buffer_size = 0;

    if (!*len)
    {
        free(*buf);
        *buf = NULL;
        signal(SIGINT, sigintHandler);

#if USE_GETLINE
        read_bytes = getline(buf, &buffer_size, stdin);
#else
        read_bytes = _getline(info, buf, &buffer_size);
#endif

        if (read_bytes > 0)
        {
            if ((*buf)[read_bytes - 1] == '\n')
            {
                (*buf)[read_bytes - 1] = '\0';
                read_bytes--;
            }

            info->linecount_flag = 1;
            remove_comments(*buf);
            build_history_list(info, *buf, info->histcount++);

            // Check if it's a command chain
            {
                *len = read_bytes;
                info->cmd_buf = buf;
            }
        }
    }

    return read_bytes;
}

/**
 * get_input - Retrieve a line without the newline character, handling command chaining.
 * @info: Parameter struct.
 *
 * Return: Bytes read.
 */
ssize_t get_input(info_t *info)
{
    static char *buf; // Command chain buffer
    static size_t i, j, len;
    ssize_t read_bytes = 0;
    char **buf_p = &(info->arg), *p;

    _putchar(BUF_FLUSH);
    read_bytes = input_buf(info, &buf, &len);

    if (read_bytes == -1)
        return -1;

    if (len)
    {
        j = i;
        p = buf + i;

        check_chain(info, buf, &j, i, len);

        while (j < len)
        {
            if (is_chain(info, buf, &j))
                break;
            j++;
        }

        i = j + 1;

        if (i >= len)
        {
            i = len = 0;
            info->cmd_buf_type = CMD_NORM;
        }

        *buf_p = p;
        return _strlen(p);
    }

    *buf_p = buf;
    return read_bytes;
}

/**
 * read_buf - Read a buffer.
 * @info: Parameter struct.
 * @buf: Buffer.
 * @i: Size.
 *
 * Return: Bytes read.
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
    ssize_t read_bytes = 0;

    if (*i)
        return 0;

    read_bytes = read(info->readfd, buf, READ_BUF_SIZE);

    if (read_bytes >= 0)
        *i = read_bytes;

    return read_bytes;
}

/**
 * _getline - Get the next line of input from STDIN.
 * @info: Parameter struct.
 * @ptr: Address of pointer to buffer, preallocated or NULL.
 * @length: Size of preallocated ptr buffer if not NULL.
 *
 * Return: Bytes read.
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
    static char buf[READ_BUF_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t read_bytes = 0, total_bytes = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;

    if (p && length)
        total_bytes = *length;

    if (i == len)
        i = len = 0;

    read_bytes = read_buf(info, buf, &len);

    if (read_bytes == -1 || (read_bytes == 0 && len == 0))
        return -1;

    c = _strchr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;

    new_p = _realloc(p, total_bytes, total_bytes ? total_bytes + k : k + 1);

    if (!new_p)
        return p ? (free(p), -1) : -1;

    if (total_bytes)
        _strncat(new_p, buf + i, k - i);
    else
        _strncpy(new_p, buf + i, k - i + 1);

    total_bytes += k - i;
    i = k;
    p = new_p;

    if (length)
        *length = total_bytes;

    *ptr = p;
    return total_bytes;
}

/**
 * sigintHandler - Handle the interrupt signal (Ctrl-C).
 * @sig_num: Signal number.
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused)) int sig_num)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}
