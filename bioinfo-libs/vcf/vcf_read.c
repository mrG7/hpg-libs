#include "vcf_read.h"

//====================================================================================
// vcf reading functions
//====================================================================================


/* ************ Header management functions **********************/

void set_file_format(char *fileformat, int length, vcf_file_t *vcf_file)
{
    vcf_file->format = fileformat;
    vcf_file->format_len = length;
//     LOG_DEBUG_F("set format = %s\n", vcf_file->format);
}

void set_header_entry_name(char *name, int length, vcf_header_entry_t *entry)
{
    entry->name = name;
    entry->name_len = length;
//     LOG_DEBUG_F("set name: %s\n", entry->name);
}

void add_header_entry_key(char *key, int length, vcf_header_entry_t *entry)
{
//     list_item_t *item = list_item_new(entry->num_keys, 1, key);
    list_item_t *item = list_item_new(entry->num_keys, 1, strndup(key, length));
    int result = list_insert_item(item, entry->keys);
    if (result) {
        entry->num_keys++;
        LOG_DEBUG_F("key %zu = %s\n", entry->num_keys, (char*) item->data_p);
    } else {
        LOG_DEBUG_F("key %zu not inserted\n", entry->num_keys);
    }
}

void add_header_entry_value(char *value, int length, vcf_header_entry_t *entry)
{
//     list_item_t *item = list_item_new(entry->num_values, 1, value);
    list_item_t *item = list_item_new(entry->num_values, 1, strndup(value, length));
    int result = list_insert_item(item, entry->values);
    if (result) {
        entry->num_values++;
        LOG_DEBUG_F("value %zu = %s\n", entry->num_values, (char*) item->data_p);
    } else {
        LOG_DEBUG_F("value %zu not inserted\n", entry->num_values);
    }
}


/* ************ Record management functions **********************/

void set_record_chromosome(char* chromosome, int length, vcf_record_t* vcf_record)
{
    if (starts_with_n(chromosome, "chrom", 5)) {
        vcf_record->chromosome = chromosome + 5;
        vcf_record->chromosome_len = length - 5;
    } else if (starts_with_n(chromosome, "chr", 3)) {
        vcf_record->chromosome = chromosome + 3;
        vcf_record->chromosome_len = length - 3;
    } else {
        vcf_record->chromosome = chromosome;
        vcf_record->chromosome_len = length;
    }
//     LOG_DEBUG_F("set chromosome: '%.*s'\n", vcf_record->chromosome_len, vcf_record->chromosome);
}

void set_record_position(long position, vcf_record_t* vcf_record) 
{
    vcf_record->position = position;
    LOG_DEBUG_F("set position: %ld\n", vcf_record->position);
}

void set_record_id(char* id, int length, vcf_record_t* vcf_record) 
{
    vcf_record->id = id;
    vcf_record->id_len = length;
//     LOG_DEBUG_F("set id: %s\n", vcf_record->id);
}

void set_record_reference(char* reference, int length, vcf_record_t* vcf_record) 
{
    vcf_record->reference = reference;
    vcf_record->reference_len = length;
//     LOG_DEBUG_F("set reference: %s\n", vcf_record->reference);
}

void set_record_alternate(char* alternate, int length, vcf_record_t* vcf_record)
{
    vcf_record->alternate = alternate;
    vcf_record->alternate_len = length;
//     LOG_DEBUG_F("set alternate: %s\n", vcf_record->alternate);
}

void set_record_quality(float quality, vcf_record_t* vcf_record)
{
    vcf_record->quality = quality;
    LOG_DEBUG_F("set quality: %f\n", vcf_record->quality);
}

void set_record_filter(char* filter, int length, vcf_record_t* vcf_record)
{
    vcf_record->filter = filter;
    vcf_record->filter_len = length;
//     LOG_DEBUG_F("set filter: %s\n", vcf_record->filter);
}

void set_record_info(char* info, int length, vcf_record_t* vcf_record)
{
    vcf_record->info = info;
    vcf_record->info_len = length;
//     LOG_DEBUG_F("set info: %s\n", vcf_record->info);
}

void set_record_format(char* format, int length, vcf_record_t* vcf_record)
{
    vcf_record->format = format;
    vcf_record->format_len = length;
//     LOG_DEBUG_F("set format: %s\n", vcf_record->format);
}

void add_record_sample(char* sample, int length, vcf_record_t* vcf_record)
{
//     int result = array_list_insert(sample, vcf_record->samples);
    int result = array_list_insert(strndup(sample, length), vcf_record->samples);
    if (result) {
        LOG_DEBUG_F("sample %s inserted\n", sample);
    } else {
        LOG_DEBUG_F("sample %s not inserted\n", sample);
    }
}
