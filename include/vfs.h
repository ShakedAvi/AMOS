#ifndef VFS_H
#define VFS_H

#include "util.h"

typedef struct fs fs_t;
typedef struct vnode vnode_t;
typedef struct file file_t;

typedef struct fs
{
    char *name;

    file_t* files[5];
}fs_t;

typedef struct vnode
{
    uint32      size;

    struct fs   *fs;

    void *p;

    uint32 ref;
}vnode_t;

typedef struct file
{
    vnode_t* vnode;
    char name[100];
    int flags;
}file_t;

#endif
