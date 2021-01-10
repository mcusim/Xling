#include <stdio.h>

/* Xling headers. */
#include "xling/graphics.h"
#include "xling/msg.h"

void
XG_SCNKBD_smoking_02(XG_ButtonState_e stat, void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	XG_SceneCtx_t *scene_ctx = (XG_SceneCtx_t *)arg;
	XG_Scene_t *scene = scene_ctx->scene;
	XG_Text_t *text = scene_ctx->text;
	XG_Point_t pt = { 0, 0 };
	XG_Animation_t *anim;
	XG_Animation_t *dots = NULL;
	XG_Animation_t *thought_bubble = NULL;

	if (scene->layers[6].obj_type == XG_OT_Animation) {
		dots = (XG_Animation_t *) scene->layers[6].obj;
	}
	if (scene->layers[7].obj_type == XG_OT_Animation) {
		thought_bubble = (XG_Animation_t *) scene->layers[7].obj;
	}

	switch (stat) {
	case XG_BTN_LEFT_PRESSED:
		for (uint8_t i = 0; i <= 7; i++) {
			if (scene->layers[i].obj_type == XG_OT_Animation) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
			} else {
				continue;
			}

			for (uint8_t j = 0; j < anim->frames_n; j++) {
				anim->frames[j].base_pt.x--;
			}
		}

		if (scene->layers[3].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[3].obj;
			anim->active = 0;
		}
		if (scene->layers[4].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[4].obj;
			anim->active = 1;
		}
		break;
	case XG_BTN_LEFT_RELEASED:
		if (scene->layers[3].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[3].obj;
			anim->active = 1;
		}
		if (scene->layers[4].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[4].obj;
			anim->active = 0;
		}
		break;
	case XG_BTN_RIGHT_PRESSED:
		for (uint8_t i = 0; i <= 7; i++) {
			if (scene->layers[i].obj_type == XG_OT_Animation) {
				anim = (XG_Animation_t *) scene->layers[i].obj;
			} else {
				continue;
			}

			for (uint8_t j = 0; j < anim->frames_n; j++) {
				anim->frames[j].base_pt.x++;
			}
		}

		if (scene->layers[3].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[3].obj;
			anim->active = 0;
		}
		if (scene->layers[4].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[4].obj;
			anim->active = 1;
		}
		break;
	case XG_BTN_RIGHT_RELEASED:
		if (scene->layers[3].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[3].obj;
			anim->active = 1;
		}
		if (scene->layers[4].obj_type == XG_OT_Animation) {
			anim = (XG_Animation_t *) scene->layers[4].obj;
			anim->active = 0;
		}
		break;
	case XG_BTN_CENTER_PRESSED:
		if (stat_lock == 0) {
			stat_lock = 1;
			show_stat = (show_stat == 0) ? 1 : 0;

			if (dots != NULL) {
				dots->active = dots->active == 1 ? 0 : 1;
			}
			if (thought_bubble != NULL) {
				thought_bubble->active =
				    thought_bubble->active == 1 ? 0 : 1;
			}
		}
		break;
	case XG_BTN_CENTER_RELEASED:
		if (stat_lock == 1) {
			stat_lock = 0;
		}
		break;
	default:
		/* All of the other keyboard events are ignored silently. */
		break;
	}

	if (show_stat == 1) {
		snprintf(text->text, text->text_sz, "%lu ms",
		    scene_ctx->frame_delay);
		pt.x = 85;
		pt.y = 20;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "%u%%", scene_ctx->bat_lvl);
		pt.y = 31;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, (scene_ctx->bat_stat == 1)
		    ? "yes" : "no");
		pt.y = 42;
		XG_Print(scene_ctx->canvas, text, pt);
	}
}

void
XG_SCNKBD_walking_01(XG_ButtonState_e stat, void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	XG_SceneCtx_t *scene_ctx = (XG_SceneCtx_t *)arg;
	XG_Text_t *text = scene_ctx->text;
	XG_Point_t pt = { 0, 0 };

	switch (stat) {
	case XG_BTN_CENTER_PRESSED:
		if (stat_lock == 0) {
			stat_lock = 1;
			show_stat = (show_stat == 0) ? 1 : 0;
		}
		break;
	case XG_BTN_CENTER_RELEASED:
		if (stat_lock == 1) {
			stat_lock = 0;
		}
		break;
	default:
		break;
	}

	if (show_stat == 1) {
		snprintf(text->text, text->text_sz, "%lu ms",
		    scene_ctx->frame_delay);
		pt.x = 85;
		pt.y = 20;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "%u%%", scene_ctx->bat_lvl);
		pt.y = 31;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, (scene_ctx->bat_stat == 1)
		    ? "yes" : "no");
		pt.y = 42;
		XG_Print(scene_ctx->canvas, text, pt);
	}
}

void
XG_SCNKBD_test_brick(XG_ButtonState_e stat, void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	static uint8_t y_lock = 0;
	XG_SceneCtx_t *scene_ctx = (XG_SceneCtx_t *)arg;
	XG_Scene_t *scene = scene_ctx->scene;
	XG_Text_t *text = scene_ctx->text;
	XG_Point_t pt = { 0, 0 };

	switch (stat) {
	case XG_BTN_LEFT_PRESSED:
		if (y_lock == 0) {
			y_lock = 1;
			scene->layers[0].base_pt.x--;
		}
		break;
	case XG_BTN_LEFT_RELEASED:
		if (y_lock == 1) {
			y_lock = 0;
		}
		break;
	case XG_BTN_RIGHT_PRESSED:
		if (y_lock == 0) {
			y_lock = 1;
			scene->layers[0].base_pt.y--;
		}
		break;
	case XG_BTN_RIGHT_RELEASED:
		if (y_lock == 1) {
			y_lock = 0;
		}
		break;
	case XG_BTN_CENTER_PRESSED:
		if (stat_lock == 0) {
			stat_lock = 1;
			show_stat = (show_stat == 0) ? 1 : 0;
		}
		break;
	case XG_BTN_CENTER_RELEASED:
		if (stat_lock == 1) {
			stat_lock = 0;
		}
		break;
	default:
		/* All of the other keyboard events are ignored silently. */
		break;
	}

	if (show_stat == 1) {
		snprintf(text->text, text->text_sz, "%lu ms",
		    scene_ctx->frame_delay);
		pt.x = 85;
		pt.y = 10;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "%u%%", scene_ctx->bat_lvl);
		pt.y = 21;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, (scene_ctx->bat_stat == 1)
		    ? "yes" : "no");
		pt.y = 32;
		XG_Print(scene_ctx->canvas, text, pt);

		snprintf(text->text, text->text_sz, "(%d,%d)",
		    scene->layers[0].base_pt.x, scene->layers[0].base_pt.y);
		pt.y = 43;
		XG_Print(scene_ctx->canvas, text, pt);
	}
}
