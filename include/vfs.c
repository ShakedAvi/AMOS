#include "vfs.h"

struct fs_list *registered_fs;
vnode *vfs_root = 0;

int vfs_mount_root(vnode *vnode)
{
    vfs_root = vnode;

    return 0;
}

int vfs_install(fs *fs)
{
    fs_list *node = (fs_list*)kmalloc(sizeof(struct fs_list));

    node->name = fs->name;
    node->fs   = fs;

    node->next = registered_fs;
    registered_fs = node;

    return 0;
}

int vfs_file_open(file *file)
{
    return file->vnode->fs->fops.open(file);
}

uint32 vfs_file_read(file *file, void *buf, uint32 nbytes)
{
    return file->vnode->fs->fops.read(file, buf, nbytes);
}

uint32 vfs_file_write(file *file, void *buf, uint32 nbytes)
{
    return file->vnode->fs->fops.write(file, buf, nbytes);
}

uint32 vfs_file_close(file *file)
{
    return file->vnode->fs->fops.close(file);
}
