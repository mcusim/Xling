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
#include <libgimp/gimp.h>

/*
 * OpenSSL headers to calculate SHA-1 hash.
 */
#include <openssl/sha.h>

#define PLUGIN_NAME		"xlingtool"
#define PLUGIN_SHORT_DESC 	"Helper tool for Xling, a tamagotchi-like toy"
#define PLUGIN_LONG_DESC  	"Converts layers of the scene image into "    \
				"headers and source code files for Xling, a " \
				"tamagotchi-like toy."
#define PLUGIN_AUTHOR		"Dmitry Salychev <dsl@mcusim.org>"
#define PLUGIN_COPYRIGHT	"Dmitry Salychev"
#define PLUGIN_YEAR		"2020"
#define PLUGIN_IMG_TYPES	"RGB*, GRAY*"
#define PLUGIN_MENU_ITEM	"_xlingtool..."
#define PLUGIN_MENU_PATH	"<Image>/Filters/Tools"

#define TEXT_BUFSZ		10240

#define IMG_EXT                 "png"

#define CONVERTER_PATH          "/home/dsl/git/lcd-image-converter/release/" \
                                "linux/output/lcd-image-converter"
#define XLING_CFG_DIR           "/home/dsl/git/Xling/common/lcd-image-converter/"
#define PRESETS_DIR             XLING_CFG_DIR "presets/"
#define TEMPLATES_DIR           XLING_CFG_DIR "templates/"
#define CONV_CONFIG             XLING_CFG_DIR "lcd-image-converter.conf"
#define OUTPUT_DIR              "/home/dsl/xling"
#define SCENES_FILE             OUTPUT_DIR "/scenes.h"

#define DISPLAY_PRESET          "Xling_SH1106_display_preset"
#define DISPLAY_PRESET_ALPHA    "Xling_SH1106_display_preset_alpha_channel"
#define IMG_TEMPLATE            TEMPLATES_DIR "image.tmpl"
#define IMGA_TEMPLATE           TEMPLATES_DIR "image_alpha.tmpl"
#define ALPHA_TEMPLATE          TEMPLATES_DIR "alpha.tmpl"

#define IMGA_CMD(buf, bufsz, name, img_path) do { \
	snprintf((buf), (bufsz), \
	    "%s " \
	    "--mode=convert-image " \
	    "--doc-name=%s " \
	    "--config-application=%s " \
	    "--preset-name=%s " \
	    "--template=%s " \
	    "--input=%s " \
	    "--output=%s/%s.h", \
	    CONVERTER_PATH, (name), CONV_CONFIG, DISPLAY_PRESET, \
	    IMGA_TEMPLATE, (img_path), OUTPUT_DIR, (name)); \
} while(0)

#define ALPHA_CMD(buf, bufsz, name, img_path) do { \
	snprintf((buf), (bufsz), \
	    "%s " \
	    "--mode=convert-image " \
	    "--doc-name=%s " \
	    "--config-application=%s " \
	    "--preset-name=%s " \
	    "--template=%s " \
	    "--input=%s " \
	    "--output=%s/%s_a.h", \
	    CONVERTER_PATH, (name), CONV_CONFIG, DISPLAY_PRESET_ALPHA, \
	    ALPHA_TEMPLATE, (img_path), OUTPUT_DIR, (name)); \
} while(0)

#define IMG_CMD(buf, bufsz, name, img_path) do { \
	snprintf((buf), (bufsz), \
	    "%s " \
	    "--mode=convert-image " \
	    "--doc-name=%s " \
	    "--config-application=%s " \
	    "--preset-name=%s " \
	    "--template=%s " \
	    "--input=%s " \
	    "--output=%s/%s.h", \
	    CONVERTER_PATH, (name), CONV_CONFIG, DISPLAY_PRESET, \
	    IMG_TEMPLATE, (img_path), OUTPUT_DIR, (name)); \
} while(0)

/*
 * Arguments for the procedure.
 */
static GimpParamDef _proc_args[] = {
	{ GIMP_PDB_INT32,    "run-mode", "Run mode" },
	{ GIMP_PDB_IMAGE,    "image",    "Input image" },
	{ GIMP_PDB_DRAWABLE, "drawable", "Input drawable" }
};
static gchar _text_buf[TEXT_BUFSZ];

/*
 * Local functions.
 */
static void     query(void);
static void     run(const gchar *name, gint nparams, const GimpParam *param,
                    gint *nreturn_vals, GimpParam **return_vals);
static uint8_t  calc_sha1(GimpDrawable *drawable,
                          uint8_t *hash, uint32_t hash_sz,
                          char *hasht, uint32_t hasht_sz);

GimpPlugInInfo PLUG_IN_INFO = {
	NULL,		/* No init function. */
	NULL,		/* No quit function. */
	query,
	run
};

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

	FILE *f_scenes = NULL;
	GimpPDBStatusType status = GIMP_PDB_SUCCESS;
	GimpRunMode run_mode;
	gint32 image_id;
	gint32 tmp_image_id;
	gint32 layer_id;
	gint *layer;
	gint l_num, vl_num;
	gint off_x, off_y;
	gchar *img_file_name;
	gchar *layer_name;
	gchar *image_name;
	GimpDrawable *drawable;
	gboolean stat, has_alpha, is_visible;
	uint8_t hash[20];
	char hash_txt[41];
	char *pos;
	int rc;

	/* Setting mandatory output values. */
	(*nreturn_vals) = 1;
	(*return_vals)  = values;

	values[0].type = GIMP_PDB_STATUS;
	values[0].data.d_status = status;

	/*
	 * Getting run_mode - we won't display a dialog if we are in
	 * NONINTERACTIVE mode.
	 */
	run_mode = param[0].data.d_int32;

	printf("Xlingtool has started processing image...\n");

	image_id = param[1].data.d_image;
	image_name = gimp_image_get_name(image_id);
	pos = strstr(image_name, ".");
	if (pos != NULL) {
		image_name[pos - image_name] = '\0';
	}

	/* Find a number of layers. */
	layer = gimp_image_get_layers(image_id, &l_num);
	vl_num = 0;

	/* Write scenes.h */
	f_scenes = fopen(SCENES_FILE, "w");
	if (f_scenes) {
		fprintf(f_scenes, "#ifndef XG_SCENES_H_\n");
		fprintf(f_scenes, "#define XG_SCENES_H_ 1\n");
		fprintf(f_scenes, "\n");
		fprintf(f_scenes, "#include \"xling/graphics.h\"\n");
		fprintf(f_scenes, "\n");
	}

	/* Iterate over all of the layers in the current image. */
	for (gint i = 0; i < l_num; i++) {
		/* Check whether it's a layer or something else. */
		if (gimp_item_is_layer(layer[i])) {
			/* Skip layer if it isn't visible. */
			is_visible = gimp_item_get_visible(layer[i]);
			if (!is_visible) {
				continue;
			}

			/* Count visible layers. */
			vl_num++;

		        /* Get layer name. */
			layer_name = gimp_item_get_name(layer[i]);

			/* Prepare a full filename with path. */
		        img_file_name = gimp_temp_name(IMG_EXT);

			/* Get a drawable for this layer. */
			drawable = gimp_drawable_get(layer[i]);

			if (drawable != NULL) {
				/*
				 * Get coordinates of the top-left corner of
				 * the drawable on the image.
				 */
				gimp_drawable_offsets(drawable->drawable_id,
				        &off_x, &off_y);

				/* Calculate hash based on drawable's data. */
				calc_sha1(drawable, hash, sizeof(hash),
				    hash_txt, sizeof(hash_txt));

				has_alpha = gimp_drawable_has_alpha(
				    drawable->drawable_id);

				/* Copy content of the current drawable. */
				gimp_edit_copy(drawable->drawable_id);

				/* Create a temporary image from edit buffer. */
				tmp_image_id = gimp_edit_paste_as_new();

				/* Save the temporary image to a file. */
				stat = gimp_file_save(
				    GIMP_RUN_NONINTERACTIVE, tmp_image_id,
				    drawable->drawable_id, img_file_name,
				    hash_txt);

				if (has_alpha) {
					/*
					 * Convert image with alpha channel.
					 */
					IMGA_CMD(_text_buf, TEXT_BUFSZ,
					    hash_txt, img_file_name);

					rc = system(_text_buf);

					ALPHA_CMD(_text_buf, TEXT_BUFSZ,
					    hash_txt, img_file_name);

					rc = system(_text_buf);
				} else {
					/*
					 * Convert image without alpha channel.
					 */
					IMG_CMD(_text_buf, TEXT_BUFSZ,
					    hash_txt, img_file_name);

					rc = system(_text_buf);
				}

				/* Delete temporary image. */
				gimp_image_delete(tmp_image_id);

				/* Append header file for a particular image. */
				if (f_scenes) {
					fprintf(f_scenes, "#include \"xling/scenes/%s.h\"\n", hash_txt);
				}
			}
		} else {
			/* Only layer can be saved as an image. */
		}
	}

	if (f_scenes) {
		fprintf(f_scenes, "\n");
		fprintf(f_scenes, "const XG_Layer_t XG_SCN_%s_LAYERS[] = {\n",
		    image_name);
	}

	/* Save layers to scenes.h */
	for (gint i = 0; i < l_num; i++) {
		/* Check whether it's a layer or something else. */
		if (gimp_item_is_layer(layer[i])) {
			/* Skip layer if it isn't visible. */
			is_visible = gimp_item_get_visible(layer[i]);
			if (!is_visible) {
				continue;
			}

		        /* Get layer name. */
			layer_name = gimp_item_get_name(layer[i]);

			/* Prepare a full filename with path. */
		        img_file_name = gimp_temp_name(IMG_EXT);

			/* Get a drawable for this layer. */
			drawable = gimp_drawable_get(layer[i]);

			if ((drawable != NULL) && (f_scenes)) {
				/*
				 * Get coordinates of the top-left corner of
				 * the drawable on the image.
				 */
				gimp_drawable_offsets(drawable->drawable_id,
				        &off_x, &off_y);

				/* Calculate hash based on drawable's data. */
				calc_sha1(drawable, hash, sizeof(hash),
				    hash_txt, sizeof(hash_txt));

				/* Append layer to scenes.h */
				fprintf(f_scenes, "\t{ "
				    ".obj = &XG_IMG_%s, "
				    ".base_pt = { %d, %d }, "
				    ".obj_type = XG_OT_Image },\n",
				    hash_txt, off_x, off_y);
			}
		} else {
			/* Only layer can be saved as an image. */
		}
	}

	/* Close scenes.h if it was opened. */
	if (f_scenes) {
		fprintf(f_scenes, "};\n\n");
		fprintf(f_scenes, "const XG_Scene_t XG_SCN_%s = {\n", image_name);
		fprintf(f_scenes, "\t.layers = XG_SCN_%s_LAYERS,\n", image_name);
		fprintf(f_scenes, "\t.layers_n = %d,\n", vl_num);
		fprintf(f_scenes, "};\n\n");
		fprintf(f_scenes, "#endif /* XG_SCENES_H_ */");

		fclose(f_scenes);
	}

	printf("Done\n");
}

static uint8_t
calc_sha1(GimpDrawable *drawable,
    uint8_t *hash, uint32_t hash_sz,
    char *hasht, uint32_t hasht_sz)
{
	uint32_t i;
	gint width, height;
	gint channels;
	guchar *row;
	GimpPixelRgn rgn;
	SHA_CTX hash_ctx;
	uint8_t rc = 0;

	/*
	 * SHA-1 hash is 20 bytes long and its text representation occupies
	 * 41 bytes (including a null character).
	 */
	if ((hash_sz < 20) || (hasht_sz < 41)) {
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

		/*
		 * Calculate SHA-1 hash in a row-based manner.
		 */
		for (i = 0; i < height; i++) {
			/* Get the next image row. */
			gimp_pixel_rgn_get_row(&rgn, row, 0, i, width);

			/* Append data to be hashed. */
			SHA1_Update(&hash_ctx, row, channels * width);
		}
		SHA1_Final(hash, &hash_ctx);

		/*
		 * Obtain a text representation of the hash.
		 */
		for (i = 0; i < hash_sz; i++) {
			snprintf(&hasht[i*2], 3, "%02x", hash[i]);
		}
		hasht[(i * 2) + 1] = 0;

		/* Return allocated memory back. */
		g_free(row);
	}

	return (rc);
}
