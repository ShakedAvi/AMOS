#ifndef VFS_H
#define VFS_H

#include "util.h"
#include "kheap.h"
#include "paging.h"

typedef struct fs fs;
typedef struct vnode vnode;
typedef struct file file;
typedef struct fops fops;
typedef struct fs_list fs_list;

typedef struct fs_list
{
    const char *name;

    fs  *fs;

    fs_list *next;
}fs_list;

typedef struct fops
{
    int         (*open)    (file *file);
    uint32     (*read)    (file *file, void *buf, uint32 size);
    uint32     (*write)   (file *file, void *buf, uint32 size);
    int         (*close)   (file *file);
}fops;

typedef struct fs
{
    char *name;
    int (*init)  ();

    file* files[5];

    fops fops;
}fs;

typedef struct vnode
{
    uint32      size;

    struct fs   *fs;

    void *p;

    uint32 ref;
}vnode;

typedef struct vfs_path
{
    vnode *root;
}vfs_path;

typedef struct file
{
    vnode *vnode;

    int flags;
}file;


extern fs_list *registered_fs;
extern vnode *vfs_root;

int     vfs_install(fs *fs);
int     vfs_mount_root(vnode *vnode);

int     vfs_file_open(file *file);
uint32 vfs_file_read(file *file, void *buf, uint32 size);
uint32 vfs_file_write(file *file, void *buf, uint32 size);
uint32 vfs_file_close(file *file);

#endif
