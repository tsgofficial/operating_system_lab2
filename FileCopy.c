#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 4096

int main() {
    char src[256], dest[256];

    printf("Эх файлын нэрийг оруулна уу: ");
    scanf("%255s", src);
    printf("Хуулах файлын нэрийг оруулна уу: ");
    scanf("%255s", dest);

    // Check file extension (e.g., only allow .txt files)
    const char *ext = strrchr(src, '.');
    if (!ext || strcmp(ext, ".txt") != 0) {
        fprintf(stderr, "Алдаа: зөвхөн .txt өргөтгөлтэй файлыг оруулна уу.\n");
        return 1;
    }

    // Check that destination is not the same as source
    if (strcmp(src, dest) == 0) {
        fprintf(stderr, "Алдаа: эх файл болон хуулах файл ижил байна.\n");
        return 1;
    }


    int fd_src = open(src, O_RDONLY);
    if (fd_src < 0) {
        fprintf(stderr, "Алдаа: %s файлыг нээж чадсангүй (%s)\n", src, strerror(errno));
        return 1;
    }

    // Check if file has content (not empty)
    off_t file_size = lseek(fd_src, 0, SEEK_END);
    if (file_size == 0) {
        fprintf(stderr, "Алдаа: %s файл хоосон байна.\n", src);
        close(fd_src);
        return 1;
    }
    lseek(fd_src, 0, SEEK_SET); // Reset file pointer

    int fd_dest = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest < 0) {
        fprintf(stderr, "Алдаа: %s файлыг нээж чадсангүй (%s)\n", dest, strerror(errno));
        close(fd_src);
        return 1;
    }

    char buffer[BUF_SIZE];
    ssize_t bytes;
    while ((bytes = read(fd_src, buffer, BUF_SIZE)) > 0) {
        if (write(fd_dest, buffer, bytes) != bytes) {
            fprintf(stderr, "Алдаа: бичих үед алдаа гарлаа (%s)\n", strerror(errno));
            close(fd_src);
            close(fd_dest);
            return 1;
        }
    }

    if (bytes < 0) {
        fprintf(stderr, "Алдаа: унших үед алдаа гарлаа (%s)\n", strerror(errno));
    }

    close(fd_src);
    close(fd_dest);

    printf("Файл амжилттай хуулагдлаа!\n");
    return 0;
}
