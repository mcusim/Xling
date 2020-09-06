#ifndef XG_SCENES_H_
#define XG_SCENES_H_ 1

#include "xling/graphics.h"

#include "xling/scenes/769b30964a8bffbd8dd40252e33b991168bac545.h"
#include "xling/scenes/55ed9bac44d7150c300891012bf787bf30aad4f8.h"
#include "xling/scenes/5e62d51889446e4ab43915294297563486a73008.h"
#include "xling/scenes/7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54.h"
#include "xling/scenes/9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc.h"
#include "xling/scenes/e3126a36c898fa1cd9791074e68630a7970d7d38.h"

const XG_Layer_t XG_SCN_smoking_02_LAYERS[] = {
	{ .obj = &XG_IMG_769b30964a8bffbd8dd40252e33b991168bac545, .base_pt = { 6, 6 }, .obj_type = XG_OT_Image },
	{ .obj = &XG_IMG_55ed9bac44d7150c300891012bf787bf30aad4f8, .base_pt = { 62, 2 }, .obj_type = XG_OT_Image },
	{ .obj = &XG_IMG_5e62d51889446e4ab43915294297563486a73008, .base_pt = { 30, 61 }, .obj_type = XG_OT_Image },
	{ .obj = &XG_IMG_7bc02a5b6a2d0f2c0f12acd04dab7e7ac2340a54, .base_pt = { 6, 45 }, .obj_type = XG_OT_Image },
	{ .obj = &XG_IMG_9ac80a2b3f33ed0ceeff03b0bb22aac84b5ca1bc, .base_pt = { 14, 31 }, .obj_type = XG_OT_Image },
	{ .obj = &XG_IMG_e3126a36c898fa1cd9791074e68630a7970d7d38, .base_pt = { 23, 43 }, .obj_type = XG_OT_Image },
};

const XG_Scene_t XG_SCN_smoking_02 = {
	.layers = XG_SCN_smoking_02_LAYERS,
	.layers_n = 6,
};

#endif /* XG_SCENES_H_ */