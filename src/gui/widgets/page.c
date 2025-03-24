#include "page.h"

#include <zephyr/sys/__assert.h>

enum page_t {
    PAGE_CONTENT = 0,
    PAGE_FOOTER = 1
};

lv_obj_t *page_create(lv_obj_t *parent)
{
    __ASSERT_NO_MSG(parent != NULL);

    lv_obj_t *obj = lv_obj_create(parent);

    if (obj)
    {
        /* Adjust the page object */
        lv_obj_set_size(obj, lv_obj_get_width(parent), lv_obj_get_height(parent));
        lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(obj, 2, 0);

        /* Follow order within the PAGE_CONTENT */
        lv_obj_t *content = lv_obj_create(obj);

        if (content)
        {
            lv_obj_set_flex_grow(content, 1);
            lv_obj_set_width(content, LV_PCT(100));
        }

        /* Follow order within the PAGE_FOOTER */
        lv_obj_t *footer = lv_obj_create(obj);

        if (footer)
        {
            lv_obj_set_size(footer, LV_PCT(100), 62);
            lv_obj_set_flex_flow(footer, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(footer, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_set_style_pad_column(footer, 15, 0);
        }
    }

    return obj;
}

lv_obj_t *get_page_content(const lv_obj_t *page)
{
    __ASSERT_NO_MSG(page != NULL);

    return lv_obj_get_child(page, PAGE_CONTENT);
}

lv_obj_t *get_page_footer(const lv_obj_t *page)
{
    __ASSERT_NO_MSG(page != NULL);

    return lv_obj_get_child(page, PAGE_FOOTER);
}
