#include "procFS.h"

fs procfs;

vnode *procfs_root = 0;

static proc* currentprocs[5];
static int currPID = 0;

fs procfs =
{
    .name  = "procfs",
    .init  = procfs_init,

    .fops =
		{
        .open    = procfs_open,
        .read    = procfs_read,
        .close   = procfs_close,
    },
};

static int procfs_init()
{
    procfs_root = (vnode*)kmalloc(sizeof(struct vnode));

    procfs_root->fs = &procfs;

    vfs_install(&procfs);
    return 0;
}

int procfs_open(file *file)
{
	proc* newProc = (proc*)kmalloc(sizeof(proc*));
	newProc->name = (char*)(file->vnode->p);
	currentprocs[currPID] = newProc;
	currPID++;

	struct file* newPFile = (struct file*)kmalloc(sizeof(struct file*));
	newPFile->vnode = (vnode*)kmalloc(sizeof(vnode*));
	newPFile->vnode->fs = &procfs;
	newPFile->vnode->p = (void*)currPID;

	procfs.files[currPID] = newPFile;
}

uint32 procfs_read(file *file, void *buf, uint32 size)
{
	strCpy(buf, currentprocs[(int)(file->vnode->p)]->name);
	return 0;
}

int procfs_close(file *file)
{
	kfree(currentprocs[(int)(file->vnode->p)]->name);
	kfree(currentprocs[(int)(file->vnode->p)]);
	kfree(procfs.files[(int)(file->vnode->p)]->vnode);
	kfree(procfs.files[(int)(file->vnode->p)]);
}
