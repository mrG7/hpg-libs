#include "gff_file.h"


//====================================================================================
//  gff_file.c
//  gff file management functions
//====================================================================================

//-----------------------------------------------------
// gff_open
//-----------------------------------------------------


gff_file_t *gff_open(char *filename) 
{
    size_t len;
    char *data = mmap_file(&len, filename);

    gff_file_t *gff_file = (gff_file_t *) malloc(sizeof(gff_file_t));
    gff_file->filename = filename;
    gff_file->data = data;
    gff_file->data_len = len;
    gff_file->header_entries = cp_list_create_list(COLLECTION_MODE_DEEP,
                                                   NULL,
                                                   NULL,
                                                   gff_header_entry_free
                                                  );
    gff_file->records = cp_list_create(COLLECTION_MODE_DEEP,
                                       NULL,
                                       NULL,
                                       gff_header_entry_free
                                      );
    return gff_file;
}


//-----------------------------------------------------
// gff_close and memory freeing
//-----------------------------------------------------

void gff_close(gff_file_t *gff_file, int free_records) 
{
    // Free string members
    free(gff_file->filename);
    free(gff_file->mode);
   
    // Free header entries
    cp_list_destroy(gff_file->header_entries);
    
    // Free records list if asked to
    if (free_records) {
        cp_list_destroy(gff_file->records);
    }
    
    munmap((void*) gff_file->data, gff_file->data_len);
    free(gff_file);
}

void gff_header_entry_free(gff_header_entry_t *gff_header_entry)
{
    free(gff_header_entry->text);
    free(gff_header_entry);
}

void gff_record_free(gff_record_t *gff_record)
{
    free(gff_record->sequence);
    free(gff_record->source);
    free(gff_record->feature);
    free(gff_record->attribute);
    free(gff_record);
}


//-----------------------------------------------------
// I/O operations (read and write) in various ways
//-----------------------------------------------------

int gff_read(gff_file_t *gff_file)  {
    return gff_ragel_read(NULL, 1, gff_file, 0);
}

int gff_read_batches(list_t *batches_list, size_t batch_size, gff_file_t *gff_file, int read_samples) {
    return gff_ragel_read(batches_list, batch_size, gff_file, read_samples);
}

int gff_write(gff_file_t *gff_file, char *filename) {
    FILE *fd = fopen(filename, "w");
    if (fd < 0) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    
    if (gff_write_to_file(gff_file, fd)) {
        fprintf(stderr, "Error writing file: %s\n", filename);
        fclose(fd);
        return 2;
    }
    
    fclose(fd);
    return 0;
}

//-----------------------------------------------------
// load data into the gff_file_t
//-----------------------------------------------------

int add_header_entry(gff_header_entry_t *header_entry, gff_file_t *gff_file)
{
    int result = cp_list_insert(gff_file->header_entries, header_entry) != NULL;
    if (result) {
        dprintf("header entry %zu\n", cp_list_item_count(gff_file->header_entries));
    } else {
        dprintf("header entry %zu not inserted\n", cp_list_item_count(gff_file->header_entries));
    }
    return result;
}

int add_record(gff_record_t* record, gff_file_t *gff_file)
{
    int result = cp_list_insert(gff_file->records, header_entry) != NULL;
    if (result) {
        dprintf("record %zu\n", cp_list_item_count(gff_file->records));
    } else {
        dprintf("record %zu not inserted\n", cp_list_item_count(gff_file->records));
    }
    return result;
}

