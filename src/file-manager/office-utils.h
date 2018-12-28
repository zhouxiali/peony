#include <glib-2.0/glib.h>
#include <glib/gstdio.h>
#include "navigation-window-interface.h"

//#include "libpeony-private/peony-window-info.h"

//get the office file's of pdf one's name.
//char* office2pdf(char* filename);
//char* excel2html(char* filename);

//when previewing an office type, we must do a delay op to ensure that the office trans op is not busy when changing the selection quickly.
GPid child_prog_sleep_and_preview_office (char* second, PeonyWindowInfo *window, char* filename, char* filename2);
char* get_pending_preview_filename (char* filename);
gboolean is_office_busy();

void office_utils_conncet_window_info (PeonyWindowInfo *window_info);
void office_utils_disconnect_window_info (PeonyWindowInfo *window_info);

void clean_cache_files_anyway (); 

/*
void     peony_navigation_window_set_current_preview_file_by_window_info (PeonyWindowInfo *window_info, char* filename);
char*    peony_navigation_window_get_current_preview_file_by_window_info (PeonyWindowInfo *window_info);
void     peony_navigation_window_set_current_previewing_office_file_by_window_info (PeonyWindowInfo *window_info, char* filename);
char*    peony_navigation_window_get_current_previewing_office_file_by_window_info (PeonyWindowInfo *window_info);
void     peony_navigation_window_set_pending_preview_file_by_window_info (PeonyWindowInfo *window_info, char* filename);
char*    peony_navigation_window_get_pending_preview_file_by_window_info (PeonyWindowInfo *window_info);
void     peony_navigation_window_set_loading_office_file_by_window_info (PeonyWindowInfo *window_info, char* filename);
char*    peony_navigation_window_get_loading_office_file_by_window_info (PeonyWindowInfo *window_info);
*/

static char* office_mime_types[] = {
    "application/wps-office.doc",
    "application/msword",
    "application/vnd.ms-word",
    "application/x-msword",
    "application/vnd.ms-word.document.macroenabled.12",
    "application/wps-office.dot",
    "application/msword-template",
    "application/vnd.ms-word.template.macroenabled.12",
    "application/wps-office.dotx",
    "application/vnd.openxmlformats-officedocument.wordprocessingml.template",
    "application/wps-office.docx",
    "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
    "application/wps-office.wps",
    "application/vnd.ms-works",
    "application/vnd.oasis.opendocument.text",
    "application/vnd.openxmlformats-officedocument.wordprocessingml.document",

    "application/wps-office.wpt",
    "application/wps-office.ppt",
    "application/vnd.ms-powerpoint",
    "application/powerpoint",
    "application/mspowerpoint",
    "application/x-mspowerpoint",
    "application/vnd.ms-powerpoint.presentation.macroenabled.12",
    "application/wps-office.pot",
    "application/vnd.ms-powerpoint.template.macroenabled.12",
    "application/wps-office.potx",
    "application/vnd.openxmlformats-officedocument.presentationml.template",
    "application/wps-office.pptx",
    "application/vnd.openxmlformats-officedocument.presentationml.presentation",
    "application/vnd.openxmlformats-officedocument.presentationml.slideshow",
    "application/wps-office.dps",
    "application/wps-office.dpt",
    "application/vnd.oasis.opendocument.presentation",
    "application/vnd.openxmlformats-officedocument.presentationml.presentation",
    
    "application/wps-office.xlsx",
    "application/wps-office.xls",
    "application/vnd.ms-excel",
    "application/msexcel",
    "application/x-msexcel",
    "application/vnd.ms-excel.sheet.macroenabled.12",
    "application/vnd.ms-excel.template.macroenabled.12",
    "application/wps-office.xlt",
    "application/wps-office.xltx",
    "application/vnd.openxmlformats-officedocument.spreadsheetml.template",
    "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
    "application/wps-office.et",
    "application/wps-office.ett"
};

static char* excel_mime_types[] = {
    "application/wps-office.xlsx",
    "application/wps-office.xls",
    "application/vnd.ms-excel",
    "application/msexcel",
    "application/x-msexcel",
    "application/vnd.ms-excel.sheet.macroenabled.12",
    "application/vnd.ms-excel.template.macroenabled.12",
    "application/wps-office.xlt",
    "application/wps-office.xltx",
    "application/vnd.openxmlformats-officedocument.spreadsheetml.template",
    "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
    "application/wps-office.et",
    "application/wps-office.ett",
    "application/vnd.oasis.opendocument.spreadsheet",
    "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
};
/*
		office_mime_types[0] = "application/wps-office.doc";
		office_mime_types[1] = "application/msword";
		office_mime_types[2] = "application/vnd.ms-word";
		office_mime_types[3] = "application/x-msword";
		office_mime_types[4] = "application/vnd.ms-word.document.macroenabled.12";
		office_mime_types[5] = "application/wps-office.dot";
		office_mime_types[6] = "application/msword-template";
		office_mime_types[7] = "application/vnd.ms-word.template.macroenabled.12";
		office_mime_types[8] = "application/wps-office.dotx";
		office_mime_types[9] = "application/vnd.openxmlformats-officedocument.wordprocessingml.template";
		office_mime_types[10] = "application/wps-office.docx";
		office_mime_types[11] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
		office_mime_types[12] = "application/wps-office.wps";
		office_mime_types[13] = "application/vnd.ms-works";
		//ppt
		office_mime_types[14] = "application/wps-office.wpt";
		office_mime_types[15] = "application/wps-office.ppt";
		office_mime_types[16] = "application/vnd.ms-powerpoint";
		office_mime_types[17] = "application/powerpoint";
		office_mime_types[18] = "application/mspowerpoint";
		office_mime_types[19] = "application/x-mspowerpoint";
		office_mime_types[20] = "application/vnd.ms-powerpoint.presentation.macroenabled.12";
		office_mime_types[21] = "application/wps-office.pot";
		office_mime_types[22] = "application/vnd.ms-powerpoint.template.macroenabled.12";
		office_mime_types[23] = "application/wps-office.potx";
		office_mime_types[25] = "application/vnd.openxmlformats-officedocument.presentationml.template";
		office_mime_types[26] = "application/wps-office.pptx";
		office_mime_types[27] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
		office_mime_types[28] = "application/vnd.openxmlformats-officedocument.presentationml.slideshow";
		office_mime_types[29] = "application/wps-office.dps";
		office_mime_types[30] = "application/wps-office.dpt";
		//excel
		office_mime_types[31] = "application/wps-office.xls";
		office_mime_types[32] = "application/vnd.ms-excel";
		office_mime_types[33] = "application/msexcel";
		office_mime_types[34] = "application/x-msexcel";
		office_mime_types[35] = "application/vnd.ms-excel.sheet.macroenabled.12";
		office_mime_types[36] = "application/vnd.ms-excel.template.macroenabled.12";
		office_mime_types[37] = "application/wps-office.xlt";
		office_mime_types[38] = "application/wps-office.xltx";
		office_mime_types[39] = "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
		office_mime_types[40] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
		office_mime_types[41] = "application/wps-office.et";
		office_mime_types[42] = "application/wps-office.ett";
        */