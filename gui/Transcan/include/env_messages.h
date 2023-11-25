#ifndef ENV_MESSAGES_H
#define ENV_MESSAGES_H

#define ERR_FATAL_LOAD_PROJECT tr("Fatal error: Unable to load project")
#define ERR_FATAL_SWITCH_TAB tr("Fatal error: Failed to switch tab")
#define ERR_FATAL_LOAD_PAGES tr("Fatal error: failed to load pages")
#define ERR_SEND_CORE_REQ tr("Error: failed launch AI process, please retry")

#define ERR_NET_CORE tr("Failed to communicate with CORE daemon")
#define ERR_FORM_REQ_PROJECT_NAME tr("Project name required")

#define INFO_CANT_SWITCH_TAB tr("You cannot skip any step of the process")
#define INFO_TITLE_CONFIRMATION tr("Action confirmation")
#define WARN_QUESTION_OVERWRITE tr("This action will overwrite the current modifications. Continue?")

//

#define TXT_IMG_DIR tr("Image folder")
#define TXT_SRC_IMG_DIR tr("Source image folder")
#define TXT_DEST_IMG_DIR tr("Destination image folder")

#define TXT_SELECT_FILE tr("Select file")
#define TXT_REPLACE_PAGE tr("Replace current page: ")
#define TXT_SELECT_IMG tr("Select image")

#define TXT_TRANSLATE tr("Translate")
#define TXT_DEF_CONFIG tr("Default configuration:") // Edit text style box title
#define TXT_SELECT_BOX tr("Selected box:") // Edit text style box title

#define TXT_EXTRACT_TXT tr("Extract text again")
#define TXT_DESTINATION tr("Destination:")
#define TXT_GO_BACK tr("Go Back")
#define TXT_CHAPTER_VIEWER tr("Chapter viewer")
#define TXT_OPEN_FOLDER tr("Open folder")

/// Help

#define CLEAN_HELP tr( \
    "Clean:\n" \
    "\n" \
    "This software only allow very easy cleaning but" \
    "you can import cleaned page from another " \
    "software.\n" \
    "\n" \
    "Actions:\n" \
    "- Enable and disable clean boxes.\n" \
    "\n" \
    "Controls:\n" \
    "- Click on a box to change it state\n" \
    "- Click on a page to select it\n" \
    )

#endif // ENV_MESSAGES_H
