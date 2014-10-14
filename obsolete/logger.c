#include "logger.h"

#define FILE_PERMS ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )
#define OPEN_FLAGS ( O_WRONLY | O_APPEND | O_CREAT )

typedef struct list {
   char *entry;
   int len;
   struct list *next;
} list;

static int fd = -1;
static list *first = NULL;
static list *last = NULL;

static ssize_t doggedwrite(int filedes, void *buffer, size_t size) {
   ssize_t bytes;

   while (((bytes = write(filedes, buffer, size)) == -1) && (errno == EINTR));
   return bytes;
}

static list *insert_new_entry(int len, int extra) {
   char *new_str;
   list *new_entry;

   new_entry = (list *)malloc(sizeof(list)+len+extra);
   if (new_entry == NULL)
      return NULL;
   new_str = (char *)new_entry+sizeof(list);
   new_entry->entry = new_str;
   new_entry->next = NULL;
   new_entry->len = len;
   if (last == NULL)
      first = new_entry;
   else
      last->next = new_entry;
   last = new_entry;
   return new_entry;
}

static int get_length() {
   int len = 0;
   list *current;

   current = first;
   while (current != NULL) {
      len += current->len;
      current = current->next;
   }
   return len;
}

static void clear() {
   list *current;
   list *free_entry;

   current = first;
   while (current != NULL) {
      free_entry = current;
      current = current->next;
      free(free_entry);
   }
   first = NULL;
   last = NULL;
}

int logopen(char *fn) {
   while (fd = open(fn, OPEN_FLAGS, FILE_PERMS), fd == -1 && errno == EINTR);
   if (fd < 0)
      return -1;
   return 0;
}

int logprintf(char *fmt, ...) {
   va_list ap;
   char ch;
   int len;
   list *new_entry;

   if (fd < 0) {
      errno = EINVAL;
      return -1;
   }
   va_start(ap, fmt);
   len = vsnprintf(&ch, 1, fmt, ap);
   new_entry = insert_new_entry(len, 1);
   if (new_entry == NULL)
      return -1;
   vsprintf(new_entry->entry, fmt, ap);
   return 0;
}

int logsend() {
   char *buf;
   list *current;
   int len;

   if (fd < 0) {
      errno = EINVAL;
      return -1;
   }
   len = get_length();
   if (len == 0)
      return 0;
   buf = (char *)malloc(len);
   if (buf == NULL)
      return -1;
   current = first;
   len = 0;
   while (current != NULL) {
      (void)memcpy(buf+len, current->entry, current->len);
      len += current->len;
      current = current->next;
   }
   if (doggedwrite(fd, buf, len) != len) {
      free(buf);
      errno = EAGAIN;
      return -1;
   }
   free(buf);
   clear();
   return 0;
}

int logclear() {
   clear();
   return 0;
}

int logclose() {
   int retval;
   clear();
   while (retval = close(fd), retval == -1 && errno == EINTR) ;
   return retval;
}
