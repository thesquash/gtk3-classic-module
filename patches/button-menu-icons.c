/* Button / Menu Icons:
 * (from gtk3-classic patch 'appearance__buttons-menus-icons.patch')
 *
 * Applies appropriate icons to buttons and menus as if they were stock buttons
 * or menu items.
 *
 * Enable the icons by adding to your settings.ini file:
 *
 *    gtk-button-images = 1
 *    gtk-menu-images = 1
 *
 * Some desktop environments may set these properties via their own appearance
 * preferences configuration tool; check for such an option or set of options
 * first.
 */


#include <gtk/gtk.h>
#include <glib.h>

#include <stdlib.h>
#include <string.h>

#include <gtk3-classic.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic::Button-Menu-Icons"


/*
 * Most of the button / menu labels and their associated icons listed herein
 * are taken from the gtkstock.h header file which comes with GTK, with the
 * exception that most of the  GTK-specific icon names have been replaced with
 * their generic FreeDesktop equivalents.  Other entries in this table may have
 * been extracted from the original patch from gtk3-classic.
 */

#define ICON(_label, _icon_name) \
 { .label = _label, .icon_name = _icon_name }

typedef struct
{
	const char *	label;
	const char *	translated_label;
	const char *	icon_name;
} Icon;

static int	icons_list_length;
static Icon	icons[] =
{
	ICON ("_About",		"help-about"),
	ICON ("_Add",		"list-add"),
	ICON ("_Apply",		"gtk-apply"),
	ICON ("_Ascending",	"view-sort-ascending"),
	ICON ("Authentication",	"dialog-authentication"),
	ICON ("_Back",		"back"),
	ICON ("Best _Fit",	"zoom-fit-best"),
	ICON ("_Bold",		"format-text-bold"),
	ICON ("_Bottom",	"bottom"),
	ICON ("_Cancel",	"gtk-cancel"),
	ICON ("_CD-ROM",	"media-optical"),
	ICON ("_Center",	"format-justify-center"),
	ICON ("_Clear",		"edit-clear"),
	ICON ("_Close",		"window-close"),
	ICON ("_Color",		"gtk-select-color"),
	ICON ("Color Selection","gtk-select-color"),
	ICON ("_Connect",	"gtk-connect"),
	ICON ("_Convert",	"gtk-convert"),
	ICON ("_Copy",		"edit-copy"),
	ICON ("Copy _Link Address", "edit-copy"),
	ICON ("Copy URL",	"edit-copy"),
	ICON ("_Credits",	"help-about"),
	ICON ("C_ustomize",	"gtk-edit"),
	ICON ("Cu_t",		"edit-cut"),
	ICON ("_Delete",	"edit-delete"),
	ICON ("_Descending",	"view-sort-descending"),
	ICON ("_Discard",	"edit-delete"),
	ICON ("_Disconnect",	"gtk-disconnect"),
	ICON ("_Down",		"down"),
	ICON ("_Edit",		"gtk-edit"),
	ICON ("Error",		"dialog-error"),
	ICON ("_Execute",	"gtk-execute"),
	ICON ("_File",		"text-x-generic"),
	ICON ("_Fill",		"format-justify-fill"),
	ICON ("_Find",		"edit-find"),
	ICON ("Find and _Replace", "edit-find-replace"),
	ICON ("_First",		"go-first"),
	ICON ("Fol_der",	"folder"),
	ICON ("_Font",		"gtk-select-font"),
	ICON ("Font Selection","gtk-select-font"),
	ICON ("_Forward",	"forward"),
	ICON ("_Fullscreen",	"view-fullscreen"),
	ICON ("_Hard Disk",	"media-harddisk"),
	ICON ("_Help",		"help-contents"),
	ICON ("_Home",		"go-home"),
	ICON ("Image",		"image-x-generic"),
	ICON ("Indent",		"format-indent-more"),
	ICON ("_Info",		"dialog-information"),
	ICON ("_Information",	"dialog-information"),
	ICON ("Insert _Emoji",	"face-smile"),
	ICON ("_Italic",	"format-text-italic"),
	ICON ("_Jump to",	"go-jump"),
	ICON ("Landscape",	"gtk-orientation-landscape"),
	ICON ("_Last",		"go-last"),
	ICON ("_Leave Fullscreen", "view-restore"),
	ICON ("_Left",		"format-justify-left"),
	ICON ("_License",	"text-x-generic"),
	ICON ("Lock",		"changes-prevent"),
	ICON ("_Network",	"network"),
	ICON ("Networks",	"network"),
	ICON ("_New",		"document-new"),
	ICON ("_Next",		"forward"),
	ICON ("_No",		"gtk-no"),
	ICON ("_Normal Size",	"zoom-original"),
	ICON ("_OK",		"gtk-ok"),
	ICON ("_Open",		"document-open"),
	ICON ("_Open Link",	"go-jump"),
	ICON ("Page Set_up",	"document-page-setup"),
	ICON ("_Password",	"dialog-authentication"),
	ICON ("_Paste",		"edit-paste"),
	ICON ("P_ause",		"media-playback-pause"),
	ICON ("Pick a Color",	"gtk-select-color"),
	ICON ("Pick a Font",	"gtk-select-font"),
	ICON ("_Play",		"media-playback-play"),
	ICON ("Portrait",	"gtk-orientation-portrait"),
	ICON ("_Preferences",	"preferences-system"),
	ICON ("Pre_view",	"document-print-preview"),
	ICON ("Pre_vious",	"back"),
	ICON ("_Print",		"document-print"),
	ICON ("Print Pre_view",	"document-print-preview"),
	ICON ("_Properties",	"document-properties"),
	ICON ("_Question",	"dialog-question"),
	ICON ("_Quit",		"application-exit"),
	ICON ("_Record",	"media-record"),
	ICON ("_Redo",		"edit-redo"),
	ICON ("_Refresh",	"view-refresh"),
	ICON ("_Remove",	"list-remove"),
	ICON ("_Replace",	"edit-find-replace"),
	ICON ("Reverse landscape", "gtk-orientation-reverse-landscape"),
	ICON ("Reverse portrait", "gtk-orientation-reverse-portrait"),
	ICON ("_Revert",	"document-revert"),
	ICON ("R_ewind",	"media-seek-backward"),
	ICON ("_Right",		"format-justify-right"),
	ICON ("_Save",		"document-save"),
	ICON ("Save _As",	"document-save-as"),
	ICON ("Search",		"system-search"),
	ICON ("_Select",	"gtk-ok"),
	ICON ("Select _All",	"edit-select-all"),
	ICON ("Select a Color",	"gtk-select-color"),
	ICON ("Select a Font",	"gtk-select-font"),
	ICON ("_Spell Check",	"tools-check-spelling"),
	ICON ("_Stop",		"process-stop"),
	ICON ("_Strikethrough",	"format-text-strikethrough"),
	ICON ("_Top",		"top"),
	ICON ("_Undelete",	"gtk-undelete"),
	ICON ("_Underline",	"format-text-underline"),
	ICON ("_Undo",		"edit-undo"),
	ICON ("Unindent",	"format-indent-less"),
	ICON ("Unlock",		"changes-allow"),
	ICON ("_Up",		"up"),
	ICON ("Warning",	"dialog-warning"),
	ICON ("_Yes",		"gtk-yes"),
	ICON ("Zoom _In",	"zoom-in"),
	ICON ("Zoom _Out",	"zoom-out"),
	{ NULL, NULL, NULL }
};

static int sort_icon_labels	(const void * icon1, const void * icon2);
static int search_icon_labels	(const void * key, const void * icon);


INTERCEPTED_CLASS_METHOD (gtk_button, notify,
			  (GObject *	object,
			   GParamSpec *	pspec),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_menu_item, constructor,
			  (GType			type,
			   guint			n_construct_properties,
			   GObjectConstructParam *	construct_params),
			  GObject *)

INTERCEPTED_CLASS_METHOD (gtk_image_menu_item, parent_set,
			  (GtkWidget *	widget,
			   GtkWidget *	previous_parent),
			  void)

INTERCEPTED_CLASS_METHOD (gtk_image_menu_item, set_label,
			  (GtkMenuItem *	menu_item,
			   const gchar *	label),
			  void)


void button_menu_icons_init ()
{
	{
		GtkButtonClass * gtk_button_class =
		 g_type_class_ref (GTK_TYPE_BUTTON);

		INTERCEPT_CLASS_METHOD (gtk_button, G_OBJECT_CLASS, notify)

		g_type_class_unref (gtk_button_class);
	}


	{
		GtkMenuItemClass * gtk_menu_item_class =
		 g_type_class_ref (GTK_TYPE_MENU_ITEM);
		G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
		GtkImageMenuItemClass * gtk_image_menu_item_class =
		 g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM);
		G_GNUC_END_IGNORE_DEPRECATIONS;

		INTERCEPT_CLASS_METHOD (
		 gtk_menu_item, G_OBJECT_CLASS, constructor)
		INTERCEPT_CLASS_METHOD (
		 gtk_image_menu_item, GTK_WIDGET_CLASS, parent_set)
		INTERCEPT_CLASS_METHOD (
		 gtk_image_menu_item, GTK_MENU_ITEM_CLASS, set_label)

		g_type_class_unref (gtk_menu_item_class);
		g_type_class_unref (gtk_image_menu_item_class);
	}


	for (icons_list_length = 0;
	     icons[icons_list_length].label != NULL;
	     icons_list_length++)
	{
		icons[icons_list_length].translated_label =
		 g_dgettext (GTK_GETTEXT_DOMAIN,
			     icons[icons_list_length].label);
	}

	qsort (icons, icons_list_length, sizeof (Icon), sort_icon_labels);
}


static void
new_gtk_button_notify
(GObject *	object,
 GParamSpec *	pspec)
{
	GtkSettings *	settings;
	gboolean	buttons_have_icons;
	GtkButton *	button;
	GtkWidget *	image;
	const char *	label_text;
	Icon *		icon;


	CALL_ORIGINAL_CLASS_METHOD (gtk_button, notify, (object, pspec));

	if (strcmp (pspec->name, "label") != 0)
		return;

	button = GTK_BUTTON (object);
	label_text = gtk_button_get_label (button);
	image = gtk_button_get_image (button);
	if (! label_text || image)
		return;

	settings = gtk_widget_get_settings (GTK_WIDGET (object));
	g_object_get (settings,
		      "gtk-button-images", &buttons_have_icons,
		      NULL);
	if (! buttons_have_icons)
		return;

	icon = bsearch (label_text, icons,
			icons_list_length, sizeof (Icon),
			search_icon_labels);
	if (icon)
	{
		image = gtk_image_new_from_icon_name (icon->icon_name,
						      GTK_ICON_SIZE_BUTTON);
		gtk_button_set_image (button, image);
	}
}


static GObject *
new_gtk_menu_item_constructor
(GType				type,
 guint				n_construct_properties,
 GObjectConstructParam *	construct_params)
{
	if (type == GTK_TYPE_MENU_ITEM)
	{
		G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
		type = GTK_TYPE_IMAGE_MENU_ITEM;
		G_GNUC_END_IGNORE_DEPRECATIONS;
	}

	return CALL_ORIGINAL_CLASS_METHOD_UNCHECKED (gtk_menu_item,
						     constructor,
						     (type,
						      n_construct_properties,
						      construct_params))
}


static void
new_gtk_image_menu_item_parent_set
(GtkWidget *	widget,
 GtkWidget *	previous_parent)

{
	GtkWidget *		parent;


	CALL_ORIGINAL_CLASS_METHOD (gtk_image_menu_item, parent_set,
				    (widget, previous_parent));


	parent = gtk_widget_get_parent (widget);
	if (parent && GTK_IS_MENU_BAR (parent))
	{
		G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
		gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (widget),
					       NULL);
		G_GNUC_END_IGNORE_DEPRECATIONS;
	}
}


static void
new_gtk_image_menu_item_set_label
(GtkMenuItem *	menu_item,
 const gchar *	label)

{
	GtkWidget *		widget;
	GtkImageMenuItem *	image_menu_item;
	GtkWidget *		parent;
	GtkSettings *		settings;
	gboolean		menus_have_icons;
	Icon *			icon;
	GtkWidget *		image;


	CALL_ORIGINAL_CLASS_METHOD (gtk_image_menu_item, set_label,
				    (menu_item, label));


	widget = GTK_WIDGET (menu_item);
	parent = gtk_widget_get_parent (widget);
	if (parent && GTK_IS_MENU_BAR (parent))
		return;

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
	image_menu_item = GTK_IMAGE_MENU_ITEM (menu_item);
	image = gtk_image_menu_item_get_image (image_menu_item);
	G_GNUC_END_IGNORE_DEPRECATIONS;
	if (! label || image)
		return;

	settings = gtk_widget_get_settings (widget);
	g_object_get (settings,
		      "gtk-menu-images", &menus_have_icons,
		      NULL);
	if (! menus_have_icons)
		return;

	icon = bsearch (label, icons,
			icons_list_length, sizeof (Icon),
			search_icon_labels);
	if (icon)
	{
		image = gtk_image_new_from_icon_name (icon->icon_name,
						      GTK_ICON_SIZE_MENU);
		G_GNUC_BEGIN_IGNORE_DEPRECATIONS;
		gtk_image_menu_item_set_image (image_menu_item, image);
		G_GNUC_END_IGNORE_DEPRECATIONS;
	}
}


/* Helper functions */

static int sort_icon_labels (const void * icon1, const void * icon2)
{
	const char *	s1 = ((Icon *) icon1)->translated_label;
	const char *	s2 = ((Icon *) icon2)->translated_label;
	int		i;
	int		j;


	i = 0;
	j = 0;
	do
	{
		if (s1[i] == '_')
			i++;
		if (s2[j] == '_')
			j++;

		if (s1[i] != s2[j])
			return s1[i] - s2[j];

		if (s1[i] == '\0')
			break;

		i++;
		j++;
	}
	while (1);


	return 0;
}


static int search_icon_labels (const void * key, const void * icon)
{
	const char *	key_string = key;
	const char *	label = ((Icon *) icon)->translated_label;
	int		k;
	int		l;

	k = 0;
	l = 0;
	do
	{
		if (key_string[k] == '_')
			k++;
		if (label[l] == '_')
			l++;

		if (key_string[k] != label[l])
			return key_string[k] - label[l];

		if (key_string[k] == '\0')
			break;

		k++;
		l++;

		/* Ignore ellipses following the label text, so that buttons /
		 * menu items such as 'Open...' will be themed as well.
		 */
		if (strcmp (&key_string[k], "...") == 0)
			break;
	}
	while (1);


	return 0;
}
