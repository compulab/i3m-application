#include "config_block.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "gfx/gfx_components/gfx_information.h"
#include "gfx/gfx_components/gfx_label.h"
#include "gfx/gfx_components/gfx_image.h"
#include "gfx/gfx_gui_control.h"
#include "lib/memory.h"
#include <string.h>

#define CONFIG_SECTION_ADDRESS 0xA000

struct gfx_graphic_menu **graphic_menus;

struct gfx_frame *dashboard;

uint8_t size_of_menus;
uint8_t new_fonts_size;

static void memcpy_config(void *dst, void *src_addr, size_t size)
{
	memcpy_PF(dst, (uint_farptr_t) (0x00010000 + (uint16_t)src_addr), size);
}

static struct gfx_image_node *load_frame_images(struct cnf_image_node *cnf_image_pgmem)
{
	struct gfx_image_node *frame_image_last = 0;
	struct gfx_image_node *image_head = NULL;
	while (cnf_image_pgmem) {
		struct cnf_image_node cnf_image_node;
		struct gfx_image_node *gfx_image_node = (struct gfx_image_node *)malloc_locked(sizeof(struct gfx_image_node));
		if (gfx_image_node == NULL)
			return NULL;

		memcpy_config(&cnf_image_node, cnf_image_pgmem, sizeof(struct cnf_image_node));
		gfx_image_node->image.bitmap = (struct gfx_mono_bitmap *)malloc_locked(sizeof(struct gfx_mono_bitmap));
		//TODO: Possible memory leak here fix later
		if (gfx_image_node->image.bitmap == NULL)
			return NULL;

		gfx_image_init(&gfx_image_node->image, cnf_image_node.image.bitmap_progmem, cnf_image_node.image.height,
					   cnf_image_node.image.width, cnf_image_node.image.x, cnf_image_node.image.y);

		gfx_image_node->next = 0;
		if (image_head == NULL)
			image_head = gfx_image_node;
		else
			frame_image_last->next = gfx_image_node;
		frame_image_last = gfx_image_node;
		cnf_image_pgmem = cnf_image_node.next;
	}
	return image_head;
}

static bool is_valid_char(char ch)
{
	return ch != '\0' && ch != '\n';
}

static uint8_t length_P(char *str)
{
	uint8_t count = 0;
	uint8_t temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	while (is_valid_char(temp_char)) {
		count++;
		temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	}
	return count;
}

static struct gfx_label_node *load_frame_labels(struct cnf_label_node *cnf_label_pgmem)
{
	struct gfx_label_node *frame_label_last = 0;
	struct gfx_label_node *label_head = NULL;
	while (cnf_label_pgmem) {
		struct cnf_label_node cnf_label_node;
		struct gfx_label_node *gfx_label_node = (struct gfx_label_node *)malloc_locked(sizeof(struct gfx_label_node));
		if (gfx_label_node == NULL)
			return NULL;

		memcpy_config(&cnf_label_node, cnf_label_pgmem, sizeof(struct cnf_label_node));
		int label_length = length_P(cnf_label_node.label.text);
		char *label = malloc_locked(sizeof(char) * (label_length + 1));
		memset(label, 0, label_length + 1);
		memcpy_config(label, cnf_label_node.label.text, length_P(cnf_label_node.label.text));
		gfx_label_init(&gfx_label_node->label, label, cnf_label_node.label.x, cnf_label_node.label.y, cnf_label_node.font_id);
		gfx_label_node->next = 0;
		if (label_head == 0)
			label_head = gfx_label_node;
		else
			frame_label_last->next = gfx_label_node;
		frame_label_last = gfx_label_node;
		cnf_label_pgmem = cnf_label_node.next;
	}
	return label_head;
}

static struct gfx_information_node *load_frame_infos(struct cnf_info_node *cnf_info_pgmem)
{
	struct gfx_information_node *frame_information_last = 0;
	struct gfx_information_node *information_head = NULL;
	while (cnf_info_pgmem) {
		struct cnf_info_node cnf_info_node;
		struct gfx_information_node *gfx_information_node = (struct gfx_information_node *)malloc_locked(sizeof(struct gfx_information_node));
		if (gfx_information_node == NULL)
			return NULL;

		memcpy_config(&cnf_info_node, (void *)cnf_info_pgmem, sizeof(struct cnf_info_node));
		struct cnf_info cnf_info = cnf_info_node.info;
		if (gfx_information_init_config(&gfx_information_node->information, &cnf_info, cnf_info_node.font_id))
			return NULL;

		gfx_information_node->next = 0;
		if (information_head)
			frame_information_last->next = gfx_information_node;
		else
			information_head = gfx_information_node;
		frame_information_last = gfx_information_node;
		cnf_info_pgmem = cnf_info_node.next;
	}
	return information_head;
}

static void action_types_init(void)
{
	for (int i = 0; i < size_of_menus; i++) {
		struct gfx_graphic_menu *menu = graphic_menus[i];
		for (int j = 0; j < menu->menu->num_elements; j++) {
			struct gfx_graphic_menu_action *action = &menu->actions[j];
			if (action->type == ACTION_TYPE_SHOW_MENU) {
				if (action->menu_id < size_of_menus) {
					action->menu = graphic_menus[action->menu_id];
				}
			}
		}
	}
}

static bool is_debug_frame(struct cnf_frame *cnf_frame)
{
	struct cnf_info_node cnf_info_node;
	memcpy_config(&cnf_info_node, (void *)cnf_frame->infos_head, sizeof(struct cnf_info_node));
	enum information_type info_type = cnf_info_node.info.info_type;

	return (info_type == SHOW_USB_SERIAL_INPUT);
}

static bool is_generic_frame(struct cnf_frame *cnf_frame)
{
	struct cnf_info_node cnf_info_node;
	memcpy_config(&cnf_info_node, (void *)cnf_frame->infos_head, sizeof(struct cnf_info_node));
	enum information_type info_type = cnf_info_node.info.info_type;

	return (info_type == SHOW_POWER_STATE);
}

static int load_action(struct gfx_graphic_menu_action *action, struct cnf_action config_action)
{
	action->type = config_action.type;
	switch(config_action.type){
	case ACTION_TYPE_SHOW_FRAME:
		action->frame = (struct gfx_frame *)malloc_locked(sizeof(struct gfx_frame));
		if (action->frame == NULL)
			return -1;

		struct cnf_frame cnf_frame;
		memcpy_config(&cnf_frame, config_action.frame, sizeof(cnf_frame));
		if (is_generic_frame(&cnf_frame)) {
			gfx_frame_init(action->frame, load_frame_images(cnf_frame.images_head),
						   load_frame_labels(cnf_frame.labels_head),
						   load_frame_infos(cnf_frame.infos_head));
		} else if (is_debug_frame(&cnf_frame)) {
			gfx_action_frame_init(action->frame, load_frame_images(cnf_frame.images_head),
						   load_frame_labels(cnf_frame.labels_head),
						   load_frame_infos(cnf_frame.infos_head));
		} else {
			gfx_context_frame_init(action->frame, load_frame_images(cnf_frame.images_head),
								   load_frame_labels(cnf_frame.labels_head),
								   load_frame_infos(cnf_frame.infos_head));
		}

		return 0;
	case ACTION_TYPE_SHOW_MENU:
		action->menu_id = config_action.menu_id;
		return 0;
	default:
		return 0;
	}
}

static int graphic_item_init(struct gfx_image *menu_image, struct cnf_image * image_node)
{
	menu_image->bitmap = (struct gfx_mono_bitmap *)malloc_locked(sizeof(struct gfx_mono_bitmap));
	if (menu_image->bitmap == NULL)
		return -1;

	menu_image->bitmap->height = image_node->height;
	menu_image->bitmap->width = image_node->width;
	menu_image->bitmap->data.progmem = image_node->bitmap_progmem;
	menu_image->bitmap->type = GFX_MONO_BITMAP_SECTION;
	return 0;
}

static int load_fonts(struct cnf_font_node *cnf_font_node)
{
	struct cnf_font_node font_node;
	struct glcd_FontConfig_t *font;
	if (fonts == NULL)
		return -1;

	while (cnf_font_node != 0) {
		memcpy_config(&font_node, cnf_font_node, sizeof(struct cnf_font_node));
		font = (struct glcd_FontConfig_t *)malloc_locked(sizeof(struct glcd_FontConfig_t));
		if (font == NULL)
			return -1;

		font->font_table = font_node.font.font_table;
		font->width = font_node.font.width;
		font->height = font_node.font.height;
		font->start_char = font_node.font.start_char;
		font->end_char = font_node.font.end_char;
		font->table_type = font_node.font.table_type;
		glcd_add_font(font, font_node.id);
		cnf_font_node = font_node.next;
	}
	return 0;
}

static struct gfx_image_node *load_graphic_view(int num_elements, struct cnf_image_node *cnf_graphic_item_node)
{
	struct cnf_image_node cnf_image;
	if (cnf_graphic_item_node == 0)
		return NULL;

	struct gfx_image_node *graphic_items_head = (struct gfx_image_node *)malloc_locked(sizeof(struct gfx_image_node));
	if (graphic_items_head == NULL)
		return NULL;

	struct gfx_image_node *image_node = graphic_items_head;
	for (uint8_t i = 0; i < num_elements; i++){
		if (cnf_graphic_item_node == 0)
			break;
		memcpy_config(&cnf_image, cnf_graphic_item_node, sizeof(struct cnf_image_node));
		if (graphic_item_init(&image_node->image, &cnf_image.image))
			return NULL;

		cnf_graphic_item_node = cnf_image.next;
		if (i < num_elements - 1){
			image_node->next = (struct gfx_image_node *)malloc_locked(sizeof(struct gfx_image_node));
			if (image_node->next == NULL)
				return NULL;

			image_node = image_node->next;
		} else {
			image_node->next = 0;
		}
	}

	return graphic_items_head;
}

static struct gfx_mono_menu *load_mono_menu(struct gfx_mono_menu *menu)
{
	struct gfx_mono_menu *mono_menu = (struct gfx_mono_menu *)malloc_locked(sizeof(struct gfx_mono_menu));
	if (mono_menu == NULL)
		return NULL;

	memcpy_config(mono_menu, menu, sizeof(struct gfx_mono_menu));

	return mono_menu;
}

static struct gfx_graphic_menu_action *load_menu_actions(int num_of_actions, struct cnf_action_node *cnf_action_node)
{
	struct cnf_action_node action_node;
	struct gfx_graphic_menu_action *actions = (struct gfx_graphic_menu_action *)malloc_locked(sizeof(struct gfx_graphic_menu_action) * num_of_actions);
	if (actions == NULL)
		return NULL;

	for (uint8_t action_index = 0; cnf_action_node; action_index++, cnf_action_node = action_node.next) {
		memcpy_config(&action_node, cnf_action_node, sizeof(struct cnf_action_node));
		if (load_action(&(actions[action_index]), action_node.action))
			return NULL;
	}

	return actions;
}

int load_config_block(void)
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	struct cnf_menu_node cnf_menu;

	memcpy_config(&config_block, (void *)CONFIG_SECTION_ADDRESS, sizeof(struct cnf_blk));
	size_of_menus = config_block.menu_size;
	new_fonts_size = config_block.font_size;
	glcd_fonts_init(new_fonts_size);

	if (new_fonts_size > 0 && config_block.fonts_head && load_fonts(config_block.fonts_head))
			return -1;

	graphic_menus = (struct gfx_graphic_menu **)malloc_locked(sizeof(struct gfx_graphic_menu *) * size_of_menus);
	if (graphic_menus == NULL)
		return -1;

	struct cnf_menu_node *cnf_menu_node = config_block.menus_head;
	for (int i = 0; i < size_of_menus; i++) {
		graphic_menus[i] = (struct gfx_graphic_menu *)malloc_locked(sizeof(struct gfx_graphic_menu));
		if (graphic_menus[i] == NULL)
			return -1;
	}

	for (int i = 0; i < size_of_menus; i++) {
		if (cnf_menu_node == 0)
			break;

		memcpy_config(&cnf_menu, cnf_menu_node, sizeof(struct cnf_menu_node));
		memcpy_config(&config_menu, cnf_menu.menu, sizeof(struct cnf_menu));
		struct gfx_mono_menu *mono_menu = load_mono_menu(config_menu.menu);
		gfx_graphic_menu_init(graphic_menus[config_menu.id], config_menu.id, mono_menu,
							 load_menu_actions(mono_menu->num_elements, config_menu.actions_head),
							 load_graphic_view(mono_menu->num_elements, config_menu.images_items_head));
		cnf_menu_node = cnf_menu.next;
	}
	action_types_init();
	gfx_gui_init(graphic_menus, graphic_menus[0]->menu->num_elements);
	return 0;
}
