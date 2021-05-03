/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * This file is part of a firmware for Xling, a pocket demon.
 *
 * Copyright (c) 2021 Dmitry Salychev
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *      * Redistributions of source code must retain the above copyright notice,
 *        this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      * Neither the name of the copyright holder nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>

#include "xling/graphics.h"
#include "xling/msg.h"

#define LEFT_BORDER		((uint8_t) 20u)
#define RIGHT_BORDER		((uint8_t) 90u)
#define COMMENTS_NUM		(5u)

const char *comments[COMMENTS_NUM] = {
	"- It's me.",
	"- It's me again.",
	"- Don't bother me.",
	"- What?!",
	"- Why don't you just go and finish this firmware for everybody who "
	"wanted to play it?!"
};

void
XG_SCNKBD_peasant_house(void *arg)
{
	static uint8_t show_stat = 0;
	static uint8_t stat_lock = 0;
	static uint8_t right = 1;
	xg_scene_ctx_t *scene_ctx = (xg_scene_ctx_t *) arg;
	xg_scene_t *scene = scene_ctx->scene;
	const xg_scene_mode_t scene_mode = scene_ctx->scene_mode;
	xg_text_t *text = scene_ctx->text;
	xg_point_t pt = { 0, 0 };
	xg_anim_t *anim;
	uint16_t rnd;

	switch (scene_ctx->btn_stat) {
	case XM_BTN_LEFT_PRESSED:
		if (scene_mode == XG_SM_SCENE) {
			/* Does Exy need to turn around? */
			if (right) {
				/* Turn around */
				for (uint8_t i = 0; i <= 5; i++) {
					anim = (xg_anim_t *) scene->layers[i].obj;
					anim->active = 0;
				}
				anim = (xg_anim_t *) scene->layers[12].obj;
				anim->active = 0;
				for (uint8_t i = 6; i <= 11; i++) {
					anim = (xg_anim_t *) scene->layers[i].obj;
					anim->active = 1;
				}
				anim = (xg_anim_t *) scene->layers[13].obj;
				anim->active = 1;
			}
			right = 0;

			/* Move scene or all of the Exy animations */
			anim = (xg_anim_t *) scene->layers[0].obj;
			if (anim->frames[0].base_pt.x <= LEFT_BORDER) {
				for (uint8_t i = 14; i <= 25; i++) {
					/* Move all of the other layers */
					scene->layers[i].base_pt.x += 2;
				}
				for (uint8_t i = 26; i < scene->layers_n; i++) {
					/* Move all of the background layers */
					scene->layers[i].base_pt.x += 1;
				}
			} else {
				for (uint8_t i = 0; i <= 13; i++) {
					if (scene->layers[i].obj_type == XG_OT_ANIM) {
						anim = (xg_anim_t *) scene->layers[i].obj;
						for (uint8_t j = 0; j < anim->frames_n; j++) {
							anim->frames[j].base_pt.x -= 2;
						}
					}
				}
			}

			/* Enable legs animation */
			anim = (xg_anim_t *) scene->layers[10].obj;
			anim->active = 0;
			anim = (xg_anim_t *) scene->layers[11].obj;
			anim->active = 1;

			/* Paws up! */
			anim = (xg_anim_t *) scene->layers[8].obj;
			anim->frames[0].alt_chance = 0;
			anim->frames[1].alt_chance = 100;
		}
		break;
	case XM_BTN_LEFT_RELEASED:
		if (scene_mode == XG_SM_SCENE) {
			if (!right) {
				/* Disable legs animation */
				anim = (xg_anim_t *) scene->layers[10].obj;
				anim->active = 1;
				anim = (xg_anim_t *) scene->layers[11].obj;
				anim->active = 0;

				/* Paws down! */
				anim = (xg_anim_t *) scene->layers[8].obj;
				anim->frames[0].alt_chance = 100;
				anim->frames[1].alt_chance = 0;
			}
		}
		break;
	case XM_BTN_RIGHT_PRESSED:
		if (scene_mode == XG_SM_SCENE) {
			/* Does Exy need to turn around? */
			if (!right) {
				/* Turn around */
				for (uint8_t i = 0; i <= 5; i++) {
					anim = (xg_anim_t *) scene->layers[i].obj;
					anim->active = 1;
				}
				anim = (xg_anim_t *) scene->layers[12].obj;
				anim->active = 1;
				for (uint8_t i = 6; i <= 11; i++) {
					anim = (xg_anim_t *) scene->layers[i].obj;
					anim->active = 0;
				}
				anim = (xg_anim_t *) scene->layers[13].obj;
				anim->active = 0;
			}
			right = 1;

			/* Move scene or all of the Exy animations */
			anim = (xg_anim_t *) scene->layers[0].obj;
			if (anim->frames[0].base_pt.x >= RIGHT_BORDER) {
				for (uint8_t i = 14; i <= 25; i++) {
					/* Move all of the other layers */
					scene->layers[i].base_pt.x -= 2;
				}
				for (uint8_t i = 26; i < scene->layers_n; i++) {
					/* Move all of the background layers */
					scene->layers[i].base_pt.x -= 1;
				}
			} else {
				for (uint8_t i = 0; i <= 13; i++) {
					if (scene->layers[i].obj_type == XG_OT_ANIM) {
						anim = (xg_anim_t *) scene->layers[i].obj;
						for (uint8_t j = 0; j < anim->frames_n; j++) {
							anim->frames[j].base_pt.x += 2;
						}
					}
				}
			}

			/* Enable legs animation */
			anim = (xg_anim_t *) scene->layers[4].obj;
			anim->active = 0;
			anim = (xg_anim_t *) scene->layers[5].obj;
			anim->active = 1;

			/* Paws up! */
			anim = (xg_anim_t *) scene->layers[2].obj;
			anim->frames[0].alt_chance = 0;
			anim->frames[1].alt_chance = 100;
		}
		break;
	case XM_BTN_RIGHT_RELEASED:
		if (scene_mode == XG_SM_SCENE) {
			if (right) {
				/* Disable legs animation */
				anim = (xg_anim_t *) scene->layers[4].obj;
				anim->active = 1;
				anim = (xg_anim_t *) scene->layers[5].obj;
				anim->active = 0;

				/* Paws down! */
				anim = (xg_anim_t *) scene->layers[2].obj;
				anim->frames[0].alt_chance = 100;
				anim->frames[1].alt_chance = 0;
			}
		}
		break;
	case XM_BTN_CENTER_PRESSED:
		if (stat_lock == 0) {
			stat_lock = 1;
			/* show_stat = (show_stat == 0) ? 1 : 0; */

			if (scene_mode == XG_SM_SCENE) {
				rnd = (uint16_t)
				    rand() / ((RAND_MAX + 1u) / COMMENTS_NUM);

				text->drawn_pt.x = 0;
				text->drawn_pt.y = 0;
				text->drawn_len = 0;
				text->skip_cycles = 0;
				snprintf(text->text, text->text_sz,
				    comments[rnd]);
				scene_ctx->scene_mode = XG_SM_SPEECH;
			} else {
				scene_ctx->scene_mode = XG_SM_SCENE;
			}
		}
		break;
	case XM_BTN_CENTER_RELEASED:
		if (stat_lock == 1) {
			stat_lock = 0;
		}
		break;
	default:
		/* All of the other keyboard events are ignored silently. */
		break;
	}

	/* if (show_stat == 1) { */
	/* 	snprintf(text->text, text->text_sz, "%u ms", */
	/* 	    scene_ctx->frame_delay << 1); */
	/* 	pt.x = 85; */
	/* 	pt.y = 20; */
	/* 	xg_print(scene_ctx->canvas, text, pt); */

	/* 	snprintf(text->text, text->text_sz, "%u%%", scene_ctx->bat_lvl); */
	/* 	pt.y = 31; */
	/* 	xg_print(scene_ctx->canvas, text, pt); */

	/* 	snprintf(text->text, text->text_sz, (scene_ctx->bat_stat == 1) */
	/* 	    ? "yes" : "no"); */
	/* 	pt.y = 42; */
	/* 	xg_print(scene_ctx->canvas, text, pt); */
	/* } */
}

void
XG_SCNKBD_forest(void *arg)
{
	XG_SCNKBD_peasant_house(arg);
}

void
XG_SCNKBD_town_street(void *arg)
{
	XG_SCNKBD_peasant_house(arg);
}

void
XG_SCNKBD_dungeon(void *arg)
{
	XG_SCNKBD_peasant_house(arg);
}
