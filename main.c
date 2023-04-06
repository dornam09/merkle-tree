#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <openssl/evp.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#define BUFSIZE 100
#define DIGESTSIZE 20

int isDir(const char* filename);

int main(int argc, char *argv[])
{
    DIR *d;
    FILE *fp;
    struct dirent *dir;
    unsigned char buf[BUFSIZE];
    char final_digest[DIGESTSIZE];
    unsigned char top_digest[DIGESTSIZE];
    unsigned char tmp_digest[DIGESTSIZE];
    int size = 0;
    unsigned int digest_len = 0;
    int i = 0;
    int id = 0;
    int hash_count = 0;
    int file_count = 0;

    OpenSSL_add_all_algorithms();
    EVP_MD_CTX *hashctx;
    EVP_MD_CTX *topctx;
    const EVP_MD *hashptr = EVP_get_digestbyname("SHA1");

    // Initialize Top Hash Context
    topctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(topctx, hashptr, NULL);

    // Check for the correct number of arguments
    if(argc==2)
    {
        char* path = argv[1];
        d = opendir(path);
        if(d == NULL)
        {
            printf("Could not open: %s\r\n", path);
        }
        else
        {
            // Iterate over each file on the path and compute hash
            while((dir = readdir(d)) != NULL)
            {
                id = isDir(dir->d_name);
                if(id == 0)
                {
                    printf("%s is a directory\r\n", dir->d_name);
                }
                else
                {
                    file_count++;
                    hashctx = EVP_MD_CTX_new();
                    EVP_DigestInit_ex(hashctx, hashptr, NULL);

                    fp = fopen(dir->d_name, "r");
                    do{
                        size = fread(buf, BUFSIZE, 1, fp);
                        EVP_DigestUpdate(hashctx, buf, size);
                    }while(size == BUFSIZE);

                    EVP_DigestFinal_ex(hashctx, buf, &digest_len);
                    printf("File: %s Hash:", dir->d_name);
                    for(i=0; i<digest_len; i++)
                        sprintf(final_digest+i, "%02x", buf[i]);
                    printf("%s\n", final_digest);

                    hash_count++;
                    if(hash_count == 2)
                    {
                        EVP_DigestUpdate(topctx, buf, DIGESTSIZE);
                        EVP_DigestUpdate(topctx, tmp_digest, DIGESTSIZE);
                        hash_count = 0;
                    }
                    memcpy(tmp_digest, buf, DIGESTSIZE);
                    EVP_MD_CTX_free(hashctx);
                }
            }
        }
    }
    else
    {
        printf("Wrong number of arguments\r\n");
    }

    // Check for odd file number
    if(file_count % 2)
    {
        EVP_DigestUpdate(topctx, tmp_digest, DIGESTSIZE);
    }

    EVP_DigestFinal_ex(topctx, top_digest, &digest_len);
    for(i=0; i<digest_len; i++)
        sprintf(final_digest+i, "%02x", top_digest[i]);
    printf("Top Digest: %s\n", final_digest);
    EVP_MD_CTX_free(topctx);

    return 0;
}


int isDir(const char* fileName)
{
    struct stat path;

    stat(fileName, &path);

    return S_ISREG(path.st_mode);
}

