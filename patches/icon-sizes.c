/* Icon Sizes:
 *
 * Allows for the configuration of stock icon sizes via the GTK setting
 * 'gtk-icon-sizes'.  GTK (2) used to allow themes to choose the sizes of
 * stock icons for various purposes; by default, button icons were 20x20 pixels
 * (in reality, 24x24 icons usually were what GTK ended up using), whereas
 * menu items used 16x16 icons.  Some themes overrode these defaults with the
 * 'gtk-icon-sizes' setting.  Now, GTK ignores this setting entirely, and
 * always uses 16x16 icons for buttons, which may not be such a bad thing if
 * the icons weren't so small as to be nearly incomprehensible.
 *
 * You can change the size of the icons by adding to your settings.ini file:
 *
 *    gtk-icon-sizes = gtk-button=24,24
 *
 * More details regarding the exact syntax of this setting are available in
 * the GTK Settings documentation, currently available at:
 * https://docs.gtk.org/gtk3/property.Settings.gtk-icon-sizes.html
 */


#include <gtk/gtk.h>
#include <glib.h>

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <gtk3-classic.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic::Icon-Sizes"


static GQuark	icon_sizes_table_quark;


static void	free_icon_sizes_table	(gpointer	data);
static void	parse_icon_sizes_string	(char *	icon_sizes_string,
					 int *	icon_sizes_table);


INTERCEPTED_CLASS_METHOD (gtk_settings, notify,
			  (GObject *	object,
			   GParamSpec *	pspec),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_image, notify,
			  (GObject *	object,
			   GParamSpec *	pspec),
			  void)


void icon_sizes_init ()
{
	{
		GtkSettingsClass * gtk_settings_class =
		 g_type_class_ref (GTK_TYPE_SETTINGS);

		INTERCEPT_CLASS_METHOD (gtk_settings, G_OBJECT_CLASS, notify)

		g_type_class_unref (gtk_settings_class);
	}


	{
		GtkImageClass * gtk_image_class =
		 g_type_class_ref (GTK_TYPE_IMAGE);

		INTERCEPT_CLASS_METHOD (gtk_image, G_OBJECT_CLASS, notify)

		g_type_class_unref (gtk_image_class);
	}


	icon_sizes_table_quark = g_quark_from_string ("icon-sizes-table");
}


static void
new_gtk_settings_notify
(GObject *	object,
 GParamSpec *	pspec)
{
	int *	icon_sizes_table;
	int	i;
	char *	icon_sizes_string;


	CALL_ORIGINAL_CLASS_METHOD (gtk_settings, notify, (object, pspec));

	if (strcmp (pspec->name, "gtk-icon-sizes") != 0)
		return;

	icon_sizes_table = g_object_get_qdata (object, icon_sizes_table_quark);
	if (icon_sizes_table == NULL)
	{
		icon_sizes_table = calloc (GTK_ICON_SIZE_DIALOG + 1,
					   sizeof (int));
		g_object_set_qdata_full (object, icon_sizes_table_quark,
					 icon_sizes_table, free_icon_sizes_table);
	}

	/* Initialize the icon sizes table */
	for (i = 0; i <= GTK_ICON_SIZE_DIALOG; i++)
		icon_sizes_table[i] = -1;

	g_object_get (object, "gtk-icon-sizes", &icon_sizes_string, NULL);
	if (icon_sizes_string)
		parse_icon_sizes_string (icon_sizes_string, icon_sizes_table);
}


static void
new_gtk_image_notify
(GObject *	object,
 GParamSpec *	pspec)
{
	GtkSettings *	settings;
	GtkImage *	image;
	GtkWidget *	image_parent;
	char *		stock_id;
	int		pixel_size;
	GtkIconSize	icon_size;
	int *		icon_sizes_table;
	int		i;
	char *		icon_sizes_string;


	CALL_ORIGINAL_CLASS_METHOD (gtk_image, notify, (object, pspec));


	image = GTK_IMAGE (object);

	if (strcmp (pspec->name, "stock") == 0)
	{
		g_object_get (object, "stock", &stock_id, NULL);
		g_object_get (object, "icon-size", &icon_size, NULL);
		if (stock_id != NULL)
		{
			g_object_freeze_notify (object);
			gtk_image_set_from_icon_name (image, stock_id, icon_size);
			g_object_thaw_notify (object);
		}
		return;
	}
	else if (strcmp (pspec->name, "pixel-size") == 0)
	{
		pixel_size = gtk_image_get_pixel_size (image);
		if (pixel_size == 0)
			gtk_image_set_pixel_size (image, -1);
		else if (pixel_size != 16)
			return;
		image_parent = gtk_widget_get_parent (GTK_WIDGET (object));
		G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
		if (image_parent != NULL &&
		    GTK_IS_IMAGE_MENU_ITEM (image_parent))
		{
			g_object_set (object,
				      "icon-size", GTK_ICON_SIZE_MENU,
				      NULL);
		}
		else
			return;
		G_GNUC_END_IGNORE_DEPRECATIONS;
	}
	else if (strcmp (pspec->name, "icon-size") != 0)
		return;

	g_object_get (object, "icon-size", &icon_size, NULL);
	if (icon_size <= GTK_ICON_SIZE_INVALID ||
	    icon_size > GTK_ICON_SIZE_DIALOG)
		return;

	icon_sizes_table = g_object_get_qdata (object, icon_sizes_table_quark);
	if (icon_sizes_table == NULL)
	{
		icon_sizes_table = calloc (GTK_ICON_SIZE_DIALOG + 1,
					   sizeof (int));
		g_object_set_qdata_full (object, icon_sizes_table_quark,
					 icon_sizes_table, free_icon_sizes_table);

		/* Initialize the icon sizes table */
		for (i = 0; i <= GTK_ICON_SIZE_DIALOG; i++)
			icon_sizes_table[i] = -1;

		settings = gtk_widget_get_settings (GTK_WIDGET (image));
		g_object_get (G_OBJECT (settings),
			      "gtk-icon-sizes", &icon_sizes_string,
			      NULL);
		if (icon_sizes_string)
			parse_icon_sizes_string (icon_sizes_string, icon_sizes_table);
	}

	gtk_image_set_pixel_size (image, icon_sizes_table[icon_size]);
}


/* Helper functions */

static void free_icon_sizes_table (gpointer data)
{
	free (data);
}


static void
parse_error
(const char *	icon_sizes_string,
 int		position,
 const char *	error_message)
{
	g_warning (
"Failed to parse GTK setting \'gtk-icon-sizes\' at character offset %d:\n"
"%s\n"
"   %s\n"
"   %*.1s\n",
		   position, error_message, icon_sizes_string, position, "^");
}


static void
parse_icon_sizes_string
(char *	icon_sizes_string,
 int *	icon_sizes_table)
{
	const char *	size_names_table[] =
	{
		NULL,
		"gtk-menu",
		"gtk-small-toolbar",
		"gtk-large-toolbar",
		"gtk-button",
		"gtk-dnd",
		"gtk-dialog",
		NULL
	};

	int		i;
	int		j;
	char *		size_name;
	int		size_name_length;
	char *		p;
	long		width;
	long		height;


	i = 0;
	while (icon_sizes_string[i] != '\0')
	{
		/* Skip over any spaces preceding the icon size name */
		while (isspace (icon_sizes_string[i]))
			i++;

		/* Collect the size name */
		size_name = &icon_sizes_string[i];
		size_name_length = 0;
		while (isalnum (size_name[size_name_length]) ||
		       size_name[size_name_length] == '-' ||
		       size_name[size_name_length] == '_')
			size_name_length++;
		size_name = strndup (size_name, size_name_length);

		/* Skip over any spaces following the icon size name */
		i += size_name_length;
		while (isspace (icon_sizes_string[i]))
			i++;

		if (icon_sizes_string[i] != '=')
		{
			parse_error (icon_sizes_string, i, "Expected \'=\'");
			free (size_name);
			return;
		}
		else
			i++;

		/* Read the icon width and height */
		width = 0;
		height = 0;
		width = strtol (&icon_sizes_string[i], &p, 0);
		while (isspace (*p))
			p++;
		if (*p == ',')
			height = strtol (p+1, &p, 0);
		if (height <= 0)
			height = width;

		if (*p == ':')
			p++;
		i = p - icon_sizes_string;


		for (j = 1; size_names_table[j] != NULL; j++)
			if (strcmp (size_name, size_names_table[j]) == 0)
				break;

		if (size_names_table[j] == NULL)
		{
			parse_error (icon_sizes_string, i,
				     "Unknown icon size name");
		}
		else if (width <= 0 || width > 256)
		{
			parse_error (icon_sizes_string, i,
				     "Icon width is suspicious, should be"
				     " between 1 and 256 px");
		}
		else if (height <= 0 || height > 256)
		{
			parse_error (icon_sizes_string, i,
				     "Icon height is suspicious, should be"
				     " between 1 and 256 px");
		}
		else
		{
			icon_sizes_table[j] = (int) MAX (width, height);
		}


		free (size_name);
	}
}
