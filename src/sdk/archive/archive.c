/* Parser and relocation layout follow
 * https://github.com/doldecomp/melee/blob/0f5c8af07668b879ac7d62dcb4e2b8a98919e249/src/sysdolphin/baselib/archive.c.
 */
#include "archive.h"
#define NULL 0
typedef struct DVDFileInfo {
    u8 command_block[0x30];
    u32 start_addr;
    u32 length;
    void (*callback)(s32, struct DVDFileInfo *);
} DVDFileInfo;
extern s32 DVDOpen(char *, DVDFileInfo *);
extern s32 DVDReadPrio(DVDFileInfo *, void *, s32, s32, s32);
extern s32 DVDClose(DVDFileInfo *);
extern void *HSD_Alloc(u32);
extern void HSD_Free(void *);
extern void OSReport(const char *, ...);
extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);
extern int strcmp(const char *, const char *);
extern u32 strlen(const char *);
extern char *strcpy(char *, const char *);
extern char *strchr(const char *, s32);
static inline void Locate(HSD_Archive *archive)
{
    u32 i;
    u32 *ptr;

    for (i = 0; i < archive->header.nb_reloc; i++) {
        ptr = (u32 *)(archive->data + archive->reloc_info[i].offset);
        *ptr += (u32)archive->data;
    }
}

s32 HSD_ArchiveParse(HSD_Archive *archive, u8 *src, u32 file_size)
{
    u32 offset;

    if (archive == NULL) {
        return -1;
    }

    memset(archive, 0, sizeof(HSD_Archive));
    archive->flags |= HSD_ARCHIVE_DONT_FREE;
    memcpy(archive, src, sizeof(HSD_ArchiveHeader));

    if (archive->header.file_size != file_size) {
        OSReport("HSD_ArchiveParse: byte-order mismatch! Please check data format\n");
        return -1;
    }

    offset = sizeof(HSD_ArchiveHeader);
    if (archive->header.data_size != 0) {
        archive->data = src + sizeof(HSD_ArchiveHeader);
        offset = archive->header.data_size + sizeof(HSD_ArchiveHeader);
    }
    if (archive->header.nb_reloc != 0) {
        archive->reloc_info = (HSD_ArchiveRelocationInfo *)((s32)src + offset);
        offset = offset + archive->header.nb_reloc * sizeof(HSD_ArchiveRelocationInfo);
    }
    if (archive->header.nb_public != 0) {
        archive->public_info = (HSD_ArchivePublicInfo *)((s32)src + offset);
        offset = offset + archive->header.nb_public * sizeof(HSD_ArchivePublicInfo);
    }
    if (archive->header.nb_extern != 0) {
        archive->extern_info = (HSD_ArchiveExternInfo *)((s32)src + offset);
        offset = offset + archive->header.nb_extern * sizeof(HSD_ArchiveExternInfo);
    }
    if (offset < archive->header.file_size) {
        archive->symbols = (char *)((s32)src + offset);
    }

    archive->top_ptr = (void *)src;
    Locate(archive);

    return 0;
}

void *HSD_ArchiveGetPublicAddress(HSD_Archive *archive, char *symbols)
{
    u32 i;

    for (i = 0; i < archive->header.nb_public; i++) {
        int comparison =
            strcmp(archive->symbols + archive->public_info[i].symbol, symbols);

        if (comparison == 0) {

            return archive->data + archive->public_info[i].offset;
        }
    }

    return NULL;
}

static inline char *HSD_ArchiveGetExtern(HSD_Archive *archive, int offset)
{
    if (offset < 0 || archive->header.nb_extern <= (unsigned)offset) {
        return NULL;
    }

    return archive->symbols + archive->extern_info[offset].symbol;
}

void HSD_ArchiveLocateExtern(HSD_Archive *archive, char *symbols, void *addr)
{
    u32 next;
    u32 offset = -1;
    u32 i;

    for (i = 0; i < archive->header.nb_extern; i++) {
        int comparison =
            strcmp(symbols, archive->symbols + archive->extern_info[i].symbol);

        if (comparison == 0) {
            offset = archive->extern_info[i].offset;
            break;
        }
    }

    if (offset == -1U) {
        return;
    }

    while (offset != -1U && offset < archive->header.data_size) {
        next = *(u32 *)((u32)archive->data + offset);
        *(u32 *)((u32)archive->data + offset) = (u32)addr;
        offset = next;
    }
}

HSD_Archive *HSD_ArchiveLoad(char *filename)
{
    HSD_Archive *archive;
    DVDFileInfo info;
    int has_archive;
    u32 data_size;
    u32 size, offset, name_size;

    OSReport("Trying to open file %s\n", filename);
    if (!DVDOpen(filename, &info)) {
        OSReport("Cannot open file.\n");
        return NULL;
    }
    archive = HSD_Alloc(sizeof(HSD_Archive));
    has_archive = archive != NULL;
    if (!has_archive)
        goto error;
    memset(archive, 0, sizeof(HSD_Archive));
    if (DVDReadPrio(&info, archive, sizeof(HSD_ArchiveHeader), 0, 2) !=
        sizeof(HSD_ArchiveHeader)) {
        OSReport("Error occured when reading file (header).");
        goto error;
    }
    if (archive->header.file_size != info.length) {
        OSReport("Unexpected binary file size. Broken file or binary order mismatch\n");
        goto error;
    }
    data_size = archive->header.data_size;
    archive->data = HSD_Alloc(((data_size + 31) & ~31));
    if (archive->data == NULL)
        goto error;
    archive->top_ptr = NULL;
    if (DVDReadPrio(&info, archive->data, ((data_size + 31) & ~31),
                    sizeof(HSD_ArchiveHeader), 2) < data_size) {
        OSReport("Error occured when reading file (data body).");
        goto error;
    }
    offset = sizeof(HSD_ArchiveHeader) + data_size;
    if (archive->header.nb_reloc != 0) {
        size = archive->header.nb_reloc * sizeof(HSD_ArchiveRelocationInfo);
        archive->reloc_info = HSD_Alloc(((size + 31) & ~31));
        if (archive->reloc_info == NULL)
            goto error;
        if (DVDReadPrio(&info, archive->reloc_info, ((size + 31) & ~31), offset, 2) <
            size) {
            OSReport("Error occured when reading file (reloc table).");
            goto error;
        }
        offset += size;
    }
    if (archive->header.nb_public != 0) {
        size = archive->header.nb_public * sizeof(HSD_ArchivePublicInfo);
        archive->public_info = HSD_Alloc(((size + 31) & ~31));
        if (archive->public_info == NULL)
            goto error;
        if (DVDReadPrio(&info, archive->public_info, ((size + 31) & ~31), offset, 2) <
            size) {
            OSReport("Error occured when reading file (public table).");
            goto error;
        }
        offset += size;
    }
    if (archive->header.nb_extern != 0) {
        size = archive->header.nb_extern * sizeof(HSD_ArchiveExternInfo);
        archive->extern_info = HSD_Alloc(((size + 31) & ~31));
        if (archive->extern_info == NULL)
            goto error;
        if (DVDReadPrio(&info, archive->extern_info, ((size + 31) & ~31), offset, 2) <
            size) {
            OSReport("Error occured when reading file (extern table).");
            goto error;
        }
        offset += size;
    }
    size = archive->header.file_size - offset;
    if (size != 0) {
        archive->symbols = HSD_Alloc(((size + 31) & ~31));
        if (archive->symbols == NULL)
            goto error;
        if (DVDReadPrio(&info, archive->symbols, ((size + 31) & ~31), offset, 2) <
            size) {
            OSReport("Error occured when reading file (symbol).");
            goto error;
        }
    }
    name_size = strlen(filename);
    archive->name = HSD_Alloc(name_size + 1);
    if (archive->name == NULL)
        goto error;
    memcpy(archive->name, filename, name_size + 1);
    Locate(archive);
    return archive;
error:
    DVDClose(&info);
    if (archive != NULL)
        HSD_ArchiveFree(archive);
    return NULL;
}
void HSD_ArchiveFree(HSD_Archive *archive)
{
    if (archive != NULL) {
        if (!(archive->flags & HSD_ARCHIVE_DONT_FREE)) {
            if (archive->data != NULL)
                HSD_Free(archive->data);
            if (archive->reloc_info != NULL)
                HSD_Free(archive->reloc_info);
            if (archive->public_info != NULL)
                HSD_Free(archive->public_info);
            if (archive->extern_info != NULL)
                HSD_Free(archive->extern_info);
            if (archive->symbols != NULL)
                HSD_Free(archive->symbols);
            if (archive->name != NULL)
                HSD_Free(archive->name);
        }
        HSD_Free(archive);
    }
}

HSD_Archive *HSD_ArchiveDefaultLoader(char *filename, void *argument)
{
    return HSD_ArchiveLoad(filename);
}
void HSD_ArchiveLoadDepends(HSD_Archive *archive, HSD_Archive *head,
                            HSD_ArchiveLoader loader, void *argument)
{
    static char buf[1024];
    HSD_Archive *current, *previous;
    char *name, *separator;
    HSD_Archive *loaded;
    if (head == NULL)
        head = archive;
    if (loader == NULL) {
        loader = HSD_ArchiveDefaultLoader;
        argument = head;
    }
    {
        s32 i = 0;
        for (; i < archive->header.nb_extern; i++) {
            name = HSD_ArchiveGetExtern(archive, i);
            if (name == NULL)
                continue;
            strcpy(buf, name);
            separator = strchr(buf, ':');
            if (separator != NULL)
                *separator = 0;
            previous = NULL;
            for (current = head; current != NULL; current = current->next) {
                if (strcmp(current->name, buf) == 0)
                    break;
                previous = current;
            }
            if (current == NULL) {
                loaded = loader(buf, argument);
                if (loaded != NULL) {
                    loaded->next = NULL;
                    if (previous != NULL)
                        previous->next = loaded;
                    HSD_ArchiveLoadDepends(loaded, head, loader, argument);
                }
            }
        }
    }
}

void HSD_ArchiveResolveReference(HSD_Archive *archive, HSD_Archive *head)
{
    static char buf[1024];
    char *sym;
    void *addr;
    s32 i;
    HSD_Archive *ref;
    char *name;

    if (head == NULL)
        head = archive;
    for (; archive != NULL; archive = archive->next) {
        for (i = 0; i < archive->header.nb_extern; i++) {
            name = HSD_ArchiveGetExtern(archive, i);
            if (name == NULL)
                continue;
            strcpy(buf, name);
            sym = strchr(buf, ':');
            if (sym != NULL) {
                *sym = 0;
                sym++;
            }
            for (ref = head; ref != NULL; ref = ref->next) {
                if (strcmp(ref->name, buf) == 0) {
                    if (sym != NULL) {
                        addr = HSD_ArchiveGetPublicAddress(ref, sym);
                    } else {
                        addr = ref->data;
                    }
                    HSD_ArchiveLocateExtern(archive, name, addr);
                }
            }
        }
    }
}
