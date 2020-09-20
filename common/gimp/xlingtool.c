/*-
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This file is part of xlingtool, a plug-in for GIMP to generate header and
 * source code files with graphics for Xling, a tamagotchi-like toy.
 *
 * Copyright (c) 2020 Dmitry Salychev
 *
 * Xling firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Xling firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgimp/gimp.h>

#include <openssl/sha.h> /* to calculate SHA-1 hash */

/******************************************************************************
 * Plugin information.
 ******************************************************************************/

#define PLUGIN_NAME		"xlingtool"
#define PLUGIN_SHORT_DESC 	"Helper tool for Xling, a pocket demon"
#define PLUGIN_LONG_DESC  	"Converts layers of the scene image into "    \
				"headers and source code files for Xling."
#define PLUGIN_AUTHOR		"Dmitry Salychev <dsl@mcusim.org>"
#define PLUGIN_COPYRIGHT	"Dmitry Salychev"
#define PLUGIN_YEAR		"2020"
#define PLUGIN_IMG_TYPES	"RGB*, GRAY*"
#define PLUGIN_MENU_ITEM	"_xlingtool..."
#define PLUGIN_MENU_PATH	"<Image>/Filters/Tools"

/******************************************************************************
 * Configuration of the animations.
 ******************************************************************************/

#define ANIM_MAX_NAME		(64u)  /* Max. length of the animation name */
#define ANIM_MAX		(128u) /* Max. # of animations per scene. */
#define ANIM_MAX_PATHS		(16u)  /* Max. # of paths per animation. */
#define ANIM_MAX_FRAMES		(64u)  /* Max. # of frames per path */
#define ANIM_TAG_FORMAT		"!anim(%63[a-zA-Z0-9]%63[#,]%63[a-zA-Z0-9])"
#define ANIM_TAG_PARTS		(3)    /* # of the animation tag parts */
#define ANIM_GO_TAG_FORMAT	"!go(%63[a-zA-Z0-9]%63[#,]%d%1[%])"
#define ANIM_GO_TAG_PARTS	(4)

/******************************************************************************
 * Basic configuration.
 ******************************************************************************/

#define CONVERTER_PATH          "/home/dsl/git/lcd-image-converter/release/" \
                                "linux/output/lcd-image-converter"
#define XLING_CFG_DIR           "/home/dsl/git/Xling/common/lcd-image-converter/"
#define PRESETS_DIR             XLING_CFG_DIR "presets/"
#define TEMPLATES_DIR           XLING_CFG_DIR "templates/"
#define CONV_CONFIG             XLING_CFG_DIR "lcd-image-converter.conf"
#define OUTPUT_DIR              "/home/dsl/xling"
#define DATA_DIR		OUTPUT_DIR "/data"
#define SCENES_FILE             OUTPUT_DIR "/scenes.h"
#define ANIM_FILE               OUTPUT_DIR "/anim.h"

#define DISPLAY_PRESET          "Xling_SH1106_display_preset"
#define DISPLAY_PRESET_ALPHA    "Xling_SH1106_display_preset_alpha_channel"
#define IMG_TEMPLATE            TEMPLATES_DIR "image.tmpl"
#define IMGA_TEMPLATE           TEMPLATES_DIR "image_alpha.tmpl"
#define ALPHA_TEMPLATE          TEMPLATES_DIR "alpha.tmpl"

#define TEXT_BUFSZ		(10240u)
#define HASH_SZ                 (20u)
#define LAYER_MAX_NAME		(128u)
#define LAYERS_MAX		(2 * ANIM_MAX * ANIM_MAX_PATHS * ANIM_MAX_FRAMES)

/******************************************************************************
 * Local function-like macros.
 ******************************************************************************/

#define IMGA_CMD(buf, bufsz, name, img_path) do {			\
	snprintf((buf), (bufsz),					\
	    "%s "							\
	    "--mode=convert-image "					\
	    "--doc-name=%s "						\
	    "--config-application=%s "					\
	    "--preset-name=%s "						\
	    "--template=%s "						\
	    "--input=%s "						\
	    "--output=%s/%s.h",						\
	    CONVERTER_PATH, (name), CONV_CONFIG, DISPLAY_PRESET,	\
	    IMGA_TEMPLATE, (img_path), OUTPUT_DIR, (name));		\
} while(0)

#define ALPHA_CMD(buf, bufsz, name, img_path) do {			\
	snprintf((buf), (bufsz),					\
	    "%s "							\
	    "--mode=convert-image "					\
	    "--doc-name=%s "						\
	    "--config-application=%s "					\
	    "--preset-name=%s "						\
	    "--template=%s "						\
	    "--input=%s "						\
	    "--output=%s/%s_a.h",					\
	    CONVERTER_PATH, (name), CONV_CONFIG, DISPLAY_PRESET_ALPHA,	\
	    ALPHA_TEMPLATE, (img_path), OUTPUT_DIR, (name));		\
} while(0)

#define IMG_CMD(buf, bufsz, name, img_path) do {		 \
	snprintf((buf), (bufsz),				 \
	    "%s "						 \
	    "--mode=convert-image "				 \
	    "--doc-name=%s "					 \
	    "--config-application=%s "				 \
	    "--preset-name=%s "					 \
	    "--template=%s "					 \
	    "--input=%s "					 \
	    "--output=%s/%s.h",					 \
	    CONVERTER_PATH, (name), CONV_CONFIG, DISPLAY_PRESET, \
	    IMG_TEMPLATE, (img_path), OUTPUT_DIR, (name));	 \
} while(0)

#define IS_SAME_NAME(name1, name2)	(strcmp((name1), (name2)) == 0)
#define IS_NOT_SAME_NAME(name1, name2)	(strcmp((name1), (name2)) != 0)

#define REPLACE_STR(str, substr, ch) do {				\
	char *__rep_subst_pos = NULL;					\
	while ((__rep_subst_pos = strstr((str), (substr))) != NULL) {	\
		(*__rep_subst_pos) = (ch);				\
	}								\
} while(0)

/******************************************************************************
 * Local types.
 ******************************************************************************/

typedef struct point_t point_t;
typedef struct scene_layer_t scene_layer_t;
typedef struct layer_chk_t layer_chk_t;
typedef struct layer_ctx_t layer_ctx_t;
typedef struct anim_t anim_t;
typedef struct anim_frame_t anim_frame_t;
typedef struct anim_path_t anim_path_t;

typedef void (*layer_callback_t)(layer_ctx_t *ctx);

struct point_t {
	int32_t	 x;
	int32_t	 y;
};

/* Different types of checks for layers. */
typedef enum layer_chk_kind_t {
	CHECK_BEFORE_ALL,
	CHECK_AFTER_ALL,
	CHECK_PER_LAYER,
	CHECK_INACTIVE
} layer_chk_kind_t;

/* Helps to describe checks for layers. */
struct layer_chk_t {
	layer_chk_kind_t kind;
	layer_callback_t cbk;
};

/*
 * Layer check context.
 *
 * Helps to de-duplicate operations between different checks applied to the same
 * layer like calculating hash, obtaining coordinates, etc.
 */
struct layer_ctx_t {
	char		 anim_alt_path_name[ANIM_MAX_NAME];
	uint8_t		 hash[HASH_SZ];
	point_t		 base_pt;
	gint		 layer_id;
	anim_t		*anim;
	anim_path_t	*anim_path;
	uint8_t		 anim_alt_path_chance;

	gboolean	 has_hash;
	gboolean	 has_base_pt;
	gboolean	 has_layer_id;
	gboolean	 is_anim_frame;
};

typedef enum layer_obj_t {
	OT_IMAGE,
	OT_ANIMATION
} layer_obj_t;

struct scene_layer_t {
	char		 name[LAYER_MAX_NAME];
	gint		 layer_id;
	layer_obj_t	 obj_type;
};

/*
 * Animation frame.
 *
 * hash		SHA-1 hash of the frame image.
 * base_pt	Coordinates of the top-left corner of the frame.
 * anim_i	Index of the animation this frame belongs to.
 * path_i	Index of the animation's path this frame belongs to.
 * next_i	Index of the next animation frame.
 * alt_i	Index of the alternative next animation frame.
 * chance	Chance to draw the alternative frame, in percent.
 */
struct anim_frame_t {
	char		alt_path_name[ANIM_MAX_NAME];
	uint8_t		hash[HASH_SZ];
	point_t		base_pt;
	uint32_t	anim_idx;
	uint32_t	path_idx;
	uint32_t	alt_path_idx;
	uint8_t		alt_path_chance;
};

/*
 * Animation.
 *
 * name		Name of the animation.
 * paths_i	Indexes of the animation's paths.
 */
struct anim_t {
	char		name[ANIM_MAX_NAME];
	uint32_t	paths_idx[ANIM_MAX_PATHS];
	uint32_t	paths_n;
	uint32_t	anim_idx;
};

/* Animation path. */
struct anim_path_t {
	char		name[ANIM_MAX_NAME];
 	uint32_t	frames_idx[ANIM_MAX_FRAMES];
	uint32_t	frames_n;
	uint32_t	anim_idx;
	uint32_t	path_idx;
};

/******************************************************************************
 * Prototypes of the local functions.
 ******************************************************************************/

/* Check functions to be applied to layer/layers. */
static void	 chk_parse_frame(layer_ctx_t *ctx);
static void	 chk_parse_anim_tag(layer_ctx_t *ctx);
static void	 chk_parse_anim_frame(layer_ctx_t *ctx);
static void	 chk_parse_go_tag(layer_ctx_t *ctx);
static void	 chk_link_animation_frames(layer_ctx_t *ctx);
static void	 chk_print_animations(layer_ctx_t *ctx);

/* Plugin lifecycle functions. */
static void      query(void);
static void      run(const gchar *name, gint nparams, const GimpParam *param,
                     gint *nreturn_vals, GimpParam **return_vals);

/* Hash helper functions. */
static uint8_t   util_calc_sha1(GimpDrawable *drawable, uint8_t *hash,
                     uint32_t hash_sz);
static char	*util_sha1_to_text(uint8_t *hash, uint32_t hash_sz, char *hasht,
                     uint32_t hasht_sz);

/* Files helper functions. */
static FILE	*util_open_scenes_file(void);
static void	 util_close_scenes_file(void);
static FILE	*util_open_anim_file(void);
static void	 util_close_anim_file(void);

static void	 util_reset_layer_context(layer_ctx_t *ctx);

/******************************************************************************
 * Plugin-wide variables.
 ******************************************************************************/

GimpPlugInInfo PLUG_IN_INFO = { NULL, NULL, query, run };

/* Arguments for the GIMP procedure. */
static GimpParamDef _proc_args[] = {
	{ GIMP_PDB_INT32,    "run-mode", "Run mode" },
	{ GIMP_PDB_IMAGE,    "image",    "Input image" },
	{ GIMP_PDB_DRAWABLE, "drawable", "Input drawable" }
};

/* List of check functions to be applied to layer/layers. */
static layer_chk_t _layer_checks[] = {
	{ .kind = CHECK_PER_LAYER,	.cbk = &chk_parse_frame },
	{ .kind = CHECK_PER_LAYER,	.cbk = &chk_parse_anim_tag },
	{ .kind = CHECK_PER_LAYER,	.cbk = &chk_parse_go_tag },
	{ .kind = CHECK_PER_LAYER,	.cbk = &chk_parse_anim_frame },
	{ .kind = CHECK_AFTER_ALL,	.cbk = &chk_link_animation_frames },
	{ .kind = CHECK_AFTER_ALL,	.cbk = &chk_print_animations }
};
static uint32_t _layer_checks_n =
    sizeof(_layer_checks)/sizeof(_layer_checks[0]);

/*
 * Information about scane layers.
 *
 * NOTE: Scene layers aren't usually the same as image layers.
 *       For example, a single animation occupies one scene
 *       layer, but might contain several frames (each per
 *       single image layer).
 */
static scene_layer_t _scene_layers[LAYERS_MAX];
static uint32_t _scene_layers_n;

/* Animations in the scene. */
static anim_t _animations[ANIM_MAX];
static uint32_t _animations_n;

/* Animation paths in the scene. */
static anim_path_t _paths[ANIM_MAX * ANIM_MAX_PATHS];
static uint32_t _paths_n;

/* Animation frames in the scene. */
static anim_frame_t _frames[ANIM_MAX * ANIM_MAX_PATHS * ANIM_MAX_FRAMES];
static uint32_t _frames_n;

static FILE *_scenes_f;
static FILE *_anim_f;
static gchar _text_buf[TEXT_BUFSZ];

/******************************************************************************
 * Implementation.
 ******************************************************************************/

/*
 * Entry point for Xlingtool which hides a bit of dark magic to
 * initialize arguments and calls appropriate PLUG_IN_INFO functions.
 */
MAIN();

static void
query(void)
{
	/*
	 * GIMP_PLUGIN argument means that it is an external procedure which
	 * isn't to be executed in the GIMP core.
	 */
	gimp_install_procedure(
	    PLUGIN_NAME,
	    PLUGIN_SHORT_DESC, PLUGIN_LONG_DESC,
	    PLUGIN_AUTHOR, PLUGIN_COPYRIGHT, PLUGIN_YEAR,
	    PLUGIN_MENU_ITEM, PLUGIN_IMG_TYPES,
	    GIMP_PLUGIN,
	    G_N_ELEMENTS(_proc_args), 0, _proc_args, NULL);

	gimp_plugin_menu_register(PLUGIN_NAME, PLUGIN_MENU_PATH);
}

static void
run(const gchar *name, gint nparams, const GimpParam *param,
    gint *nreturn_vals, GimpParam **return_vals)
{
	static GimpParam values[1];

	GimpPDBStatusType status = GIMP_PDB_SUCCESS;
	GimpRunMode run_mode;
	gint *layer;
	gint layers_n;
	gint image_id;
	gchar *image_name;
	char *pos;
	layer_ctx_t ctx;

	/* Setting mandatory output values. */
	(*nreturn_vals) = 1;
	(*return_vals)  = values;
	values[0].type = GIMP_PDB_STATUS;
	values[0].data.d_status = status;

	/* Getting run_mode. */
	run_mode = param[0].data.d_int32;

	/* Obtain information about an image. */
	image_id = param[1].data.d_image;
	image_name = gimp_image_get_name(image_id);

	/* Get rid of the image file extension. */
	pos = strstr(image_name, ".");
	if (pos != NULL) {
		image_name[pos - image_name] = '\0';
	}

	/* Find a number of the image layers. */
	layer = gimp_image_get_layers(image_id, &layers_n);

	/* 1. Call checks before all layers. */
	util_reset_layer_context(&ctx);
	for (uint32_t j = 0; j < _layer_checks_n; j++) {
		switch (_layer_checks[j].kind) {
		case CHECK_BEFORE_ALL:
			_layer_checks[j].cbk(&ctx);
			break;
		default:
			break;
		}
	}

	/* 2. Process all layers. */
	util_reset_layer_context(&ctx);
	for (gint i = 0; i < layers_n; i++) {
		/* Setup context before start of the next layer. */
		ctx.is_anim_frame = FALSE;
		ctx.has_hash = FALSE;
		ctx.has_base_pt = FALSE;
		ctx.has_layer_id = TRUE;
		ctx.layer_id = layer[i];
		ctx.anim_alt_path_chance = 0;

		/* Call all per-layer checks for the current layer. */
		for (uint32_t j = 0; j < _layer_checks_n; j++) {
			switch (_layer_checks[j].kind) {
			case CHECK_PER_LAYER:
				_layer_checks[j].cbk(&ctx);
				break;
			default:
				break;
			}
		}
	}

	/* 3. Call checks after all layers. */
	util_reset_layer_context(&ctx);
	for (uint32_t j = 0; j < _layer_checks_n; j++) {
		switch (_layer_checks[j].kind) {
		case CHECK_AFTER_ALL:
			_layer_checks[j].cbk(&ctx);
			break;
		default:
			break;
		}
	}
}

/*
 * Exports image data of the layer as a separate header file named
 * as <SHA-1>.h (and <SHA-1>_a.h if layer has an alpha channel).
 */
static void
chk_parse_frame(layer_ctx_t *ctx)
{
	gboolean visible, has_alpha, stat;
	gint32 img_id;
	gchar *img_file_name = NULL;
	GimpDrawable *dwb = NULL;
	point_t pt;
	uint8_t hash[HASH_SZ];
	char hasht[(HASH_SZ * 2) + 1]; /* 2 chars/byte + '\0' */
	int rc = 0;

	if (ctx->has_layer_id) {
		/* Export image data of the visible layers only. */
		visible = gimp_item_get_visible(ctx->layer_id);

		/* Get a drawable for this layer. */
		dwb = gimp_drawable_get(ctx->layer_id);
	} else {
		/* Can't process a layer without its id. */
		rc = 1;
	}

	/*
	 * Ignore invisible layers and layers which
	 * drawables can't be obtained.
	 */
	if ((rc == 0) && (visible) && (dwb != NULL)) {
		/* Prepare a temporary image file. */
		img_file_name = gimp_temp_name("png");

		/*
		 * Get coordinates of the top-left corner of
		 * the drawable on the image.
		 */
		gimp_drawable_offsets(dwb->drawable_id, &pt.x, &pt.y);
		if (!ctx->has_base_pt) {
			ctx->base_pt = pt;
			ctx->has_base_pt = TRUE;
		}

		/* Drawable might have transparency channel. */
		has_alpha = gimp_drawable_has_alpha(dwb->drawable_id);

		/* Calculate SHA-1 based on the drawable data. */
		util_calc_sha1(dwb, hash, sizeof(hash));
		util_sha1_to_text(hash, sizeof(hash), hasht, sizeof(hasht));
		if (!ctx->has_hash) {
			/* Save hash to the context. */
			memcpy(ctx->hash, hash, HASH_SZ);
		}

		/* Copy content of the current drawable. */
		gimp_edit_copy(dwb->drawable_id);

		/* Create a temporary image from edit buffer. */
		img_id = gimp_edit_paste_as_new();

		/* Save the temporary image to a file. */
		stat = gimp_file_save(GIMP_RUN_NONINTERACTIVE, img_id,
		    dwb->drawable_id, img_file_name, hasht);

		if (has_alpha) {
			/* Export image with alpha channel. */
			IMGA_CMD(_text_buf, TEXT_BUFSZ, hasht, img_file_name);
			rc = system(_text_buf);

			ALPHA_CMD(_text_buf, TEXT_BUFSZ, hasht, img_file_name);
			rc = system(_text_buf);
		} else {
			/* Export image without alpha channel. */
			IMG_CMD(_text_buf, TEXT_BUFSZ, hasht, img_file_name);
			rc = system(_text_buf);
		}

		/* Remove temporary image. */
		gimp_image_delete(img_id);
	}
}

/*
 * Parses !anim(name, path) tag from the layer name.
 */
static void
chk_parse_anim_tag(layer_ctx_t *ctx)
{
	char layer_name[LAYER_MAX_NAME];
	char anim_name[ANIM_MAX_NAME];
	char path_name[ANIM_MAX_NAME];
	char name_buf[(2 * ANIM_MAX_NAME) + 1];
	gboolean visible;
	gchar *gpos;
	char *token, *pos;
	int rc = 0;

	if (ctx->has_layer_id) {
		/* Export image data of the visible layers only. */
		visible = gimp_item_get_visible(ctx->layer_id);
	} else {
		/* Can't process a layer without its id. */
		rc = 1;
	}

	if ((rc == 0) && (visible)) {
		/* Get a copy of the layer name. */
		gpos = gimp_item_get_name(ctx->layer_id);
		strncpy(layer_name, gpos, LAYER_MAX_NAME);

		/* Parse layer name by token. */
		token = strtok(layer_name, "_");
		while (token != NULL) {
			/* Replace all spaces */
			REPLACE_STR(token, " ", '#');

			/* An attempt to parse the animation tag. */
			rc = sscanf(token, ANIM_TAG_FORMAT, anim_name,
			    name_buf, path_name);
			if (rc == ANIM_TAG_PARTS) {
				break;
			}

			/* Go to the next token. */
			token = strtok(NULL, "_");
		}

		if (rc == ANIM_TAG_PARTS) {
			/* Animation tag found! */
			ctx->is_anim_frame = TRUE;

			if ((ctx->anim == NULL) ||
			    (IS_NOT_SAME_NAME(anim_name, ctx->anim->name))) {
				/* Update current animation in the context. */
				ctx->anim = &_animations[_animations_n];
				ctx->anim->anim_idx = _animations_n;

				/* Copy name of the animation */
				strncpy(ctx->anim->name, anim_name,
				    ANIM_MAX_NAME);

				/* Increase number of the known animations. */
				_animations_n++;
			}

			/*
			 * Animation tag should contain a path for the current
			 * frame also.
			 */
			sprintf(name_buf, "%s@%s", path_name, anim_name);
			if ((ctx->anim_path == NULL) ||
			    (IS_NOT_SAME_NAME(name_buf, ctx->anim_path->name))) {
				/*
				 * Update current animation path in the
				 * context.
				 */
				ctx->anim_path = &_paths[_paths_n];
				ctx->anim_path->path_idx = _paths_n;
				ctx->anim_path->anim_idx = ctx->anim->anim_idx;

				/* Append current path to the animation. */
				ctx->anim->paths_idx[ctx->anim->paths_n] =
				    _paths_n;
				ctx->anim->paths_n++;

				/* Copy name of the path. */
				strncpy(ctx->anim_path->name, name_buf,
				    ANIM_MAX_NAME);

				/* Increment number of the known paths. */
				_paths_n++;
			}
		}
	}
}

/*
 * Parses !go(path, chance) tag from the layer name.
 */
static void
chk_parse_go_tag(layer_ctx_t *ctx)
{
	char layer_name[LAYER_MAX_NAME];
	char path_name[ANIM_MAX_NAME];
	char buf1[ANIM_MAX_NAME];
	char buf2[ANIM_MAX_NAME];
	int chance;
	gchar *gpos;
	char *token, *pos;
	int rc = 0;

	if (ctx->is_anim_frame) {
		/* An animation frame. */

		/* Get a copy of the layer name. */
		gpos = gimp_item_get_name(ctx->layer_id);
		strncpy(layer_name, gpos, LAYER_MAX_NAME);

		/* Parse layer name by token. */
		token = strtok(layer_name, "_");
		while (token != NULL) {
			/* Replace all spaces */
			REPLACE_STR(token, " ", '#');

			/* An attempt to parse the animation tag. */
			rc = sscanf(token, ANIM_GO_TAG_FORMAT, path_name,
			    buf1, &chance, buf2);
			if (rc == ANIM_GO_TAG_PARTS) {
				break;
			}

			/* Go to the next token. */
			token = strtok(NULL, "_");
		}

		if (rc == ANIM_GO_TAG_PARTS) {
			/* Go tag found! */
			snprintf(ctx->anim_alt_path_name, ANIM_MAX_NAME,
			    "%s@%s", path_name, ctx->anim->name);
			ctx->anim_alt_path_chance = chance;
		}
	} else {
		/* Not an animation frame - do nothing. */
	}
}

/*
 * Adds current layer's image as an animation frame to the list of frames
 * and updates its animation path.
 */
static void
chk_parse_anim_frame(layer_ctx_t *ctx)
{
	anim_frame_t *frame;

	if (ctx->is_anim_frame) {
		/* An animation frame. */
		frame = &_frames[_frames_n];
		frame->anim_idx = ctx->anim->anim_idx;
		frame->path_idx = ctx->anim_path->path_idx;
		frame->base_pt = ctx->base_pt;

		/* Alternative path for this frame */
		if (ctx->anim_alt_path_chance > 0u) {
			strncpy(frame->alt_path_name, ctx->anim_alt_path_name,
			    ANIM_MAX_NAME);
			frame->alt_path_chance = ctx->anim_alt_path_chance;
		}

		/* Append frame to the path */
		ctx->anim_path->frames_idx[ctx->anim_path->frames_n] =
		    _frames_n;
		ctx->anim_path->frames_n++;

		/* Copy hash of the frame. */
		memcpy(frame->hash, ctx->hash, HASH_SZ);

		/* Increment number of the known animation frames. */
		_frames_n++;
	} else {
		/* Not an animation frame - do nothing. */
	}
}

/* Resolve alternative path names to path indexes. */
static void
chk_link_animation_frames(layer_ctx_t *ctx)
{
	anim_frame_t *frame;
	char hasht[(2 * HASH_SZ) + 1];

	for (uint32_t i = 0; i < _frames_n; i++) {
		frame = &_frames[i];

		if (frame->alt_path_chance > 0u) {
			/* Frame has an alternative path. */
			for (uint32_t j = 0; j < _paths_n; j++) {
				if (IS_SAME_NAME(frame->alt_path_name,
				    _paths[j].name)) {
					frame->alt_path_idx = j;
					break;
				}
			}
		}
	}
}

static void
chk_print_animations(layer_ctx_t *ctx)
{
	anim_t *anim;
	anim_path_t *path;
	anim_frame_t *frame;
	char hasht[(2 * HASH_SZ) + 1];
	char hasht2[(2 * HASH_SZ) + 1];

	printf("\n--- Animations ---\n");

	/* Animations */
	for (uint32_t i = 0; i < _animations_n; i++) {
		anim = &_animations[i];
		printf("%s\n", anim->name);

		/* Paths */
		for (uint32_t j = 0; j < anim->paths_n; j++) {
			path = &_paths[anim->paths_idx[j]];
			printf("\t%s\n", path->name);

			/* Frames */
			for (uint32_t k = 0; k < path->frames_n; k++) {
				frame = &_frames[path->frames_idx[k]];

				util_sha1_to_text(frame->hash, HASH_SZ, hasht,
				    sizeof(hasht));

				if (frame->alt_path_chance > 0u) {
					/* There's an alternative path */
					util_sha1_to_text(
					    _frames[_paths[frame->alt_path_idx].frames_idx[0]].hash,
					    HASH_SZ, hasht2, sizeof(hasht2));

					printf("\t\t%s ---> %s (%s), %d%%\n",
					    hasht, hasht2,
					    _paths[frame->alt_path_idx].name,
					    frame->alt_path_chance);
				} else {
					printf("\t\t%s\n", hasht);
				}
			}
		}
	}
}

static uint8_t
util_calc_sha1(GimpDrawable *drawable, uint8_t *hash, uint32_t hash_sz)
{
	uint32_t i;
	gint width, height;
	gint channels;
	guchar *row;
	GimpPixelRgn rgn;
	SHA_CTX hash_ctx;
	uint8_t rc = 0;

	/* There should be enough space for SHA-1 (20 bytes). */
	if (hash_sz < 20) {
		rc = 1;
	}

	if (rc == 0) {
		/* Obtain pieces of information about the given drawable. */
		width = drawable->width;
		height = drawable->height;
		channels = gimp_drawable_bpp(drawable->drawable_id);

		/* Initialize pixel region to access drawable data. */
		gimp_pixel_rgn_init(&rgn, drawable, 0, 0, width, height,
		    FALSE, FALSE);

		/* Allocate memory to store image data of a whole row. */
		row = g_new(guchar, channels * width);

		/* Initialize context to calculate hash. */
		SHA1_Init(&hash_ctx);

		/* Calculate SHA-1 hash using image rows. */
		for (i = 0; i < height; i++) {
			/* Get the next image row. */
			gimp_pixel_rgn_get_row(&rgn, row, 0, i, width);

			/* Append data to be hashed. */
			SHA1_Update(&hash_ctx, row, channels * width);
		}
		SHA1_Final(hash, &hash_ctx);

		/* Return allocated memory back. */
		g_free(row);
	}

	return (rc);
}

static char *
util_sha1_to_text(uint8_t *hash, uint32_t hash_sz, char *hasht, uint32_t hasht_sz)
{
	uint32_t i;
	char *val = NULL;

	/*
	 * Do we have enough buffer space to keep text representation
	 * of the hash?
	 */
	if (hasht_sz >= ((hash_sz * 2) + 1)) {
		/* Print hash byte by byte. */
		for (i = 0; i < hash_sz; i++) {
			snprintf(&hasht[i * 2], 3, "%02x", hash[i]);
		}
		/* Terminate hash string */
		hasht[(i * 2) + 1] = 0;

		/* Update return value */
		val = hasht;
	} else {
		/* No buffer space to store text representation of the hash. */
	}

	return (val);
}

static void
util_reset_layer_context(layer_ctx_t *ctx)
{
	ctx->has_hash = FALSE;
	ctx->has_base_pt = FALSE;
	ctx->has_layer_id = FALSE;
	ctx->is_anim_frame = FALSE;
	ctx->anim_alt_path_chance = 0;

	ctx->anim = NULL;
	ctx->anim_path = NULL;
}
