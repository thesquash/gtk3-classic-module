/* Dialogs:
 * From gtk3-classic patches:
 *    appearance__message-dialogs.patch
 *    appearance__file-chooser.patch
 *    appearance__print-dialog.patch
 *    fixes__wayland_dialogs_header_setting.patch
 *    file-chooser__eject-button.patch
 *    file-chooser__typeahead.patch
 *
 * Restores the classic appearance of message, print, and file dialogs.
 * Forces dialogs to use the traditional action area for their buttons,
 * instead of putting the buttons in a headerbar -- even on Wayland.
 * Also makes some behavioural changes to file chooser widgets (and thus
 * by extension file dialogs).
 */


#include <gtk/gtk.h>
#include <gtk/gtkunixprint.h>
#include <gdk/gdk.h>
#include <glib.h>

#include <string.h>

#include <gtk3-classic.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic::Dialogs"


#define MESSAGE_DIALOG_CSS \
 "messagedialog > *,\n" \
 "messagedialog buttonbox.dialog-action-area,\n" \
 "messagedialog box.dialog-action-box\n" \
 "{\n" \
 "	margin: 	0;\n" \
 "	margin-top: 	0;\n" \
 "	padding: 	0;\n" \
 "}\n"

#define PRINT_DIALOG_CSS \
 "printdialog box.dialog-action-box\n" \
 "{\n" \
 "	margin:	0;\n" \
 "}\n"

#define FILE_CHOOSER_CSS \
 "/* Prefer full-color icons to symbolic ones */\n" \
 "filechooser .sidebar-icon,\n" \
 "filechooser .path-bar,\n" \
 "filechooser .path-bar + button\n" \
 "{\n" \
 "	opacity:		1;\n" \
 "	-gtk-icon-style:	regular;\n" \
 "}\n" \
 "\n" \
 "/* Hide the Eject / Unmount buttons next to volumes in the sidebar */\n" \
 "filechooser .sidebar-button\n" \
 "{\n" \
 "	opacity:	0;\n" \
 "}\n"


static void		apply_dialog_css	(GtkWidget *	dialog);
static GtkWidget *	find_widget_by_name	(GtkWidget *	start,
						 const char *	name);


INTERCEPTED_CLASS_METHOD (gtk_settings, constructed,
			  (GObject *	object),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_message_dialog, constructed,
			  (GObject *	object),
			  void)

INTERCEPTED_CLASS_METHOD (gtk_message_dialog, notify,
			  (GObject *	object,
			   GParamSpec *	pspec),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_print_unix_dialog, constructed,
			  (GObject *	object),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_file_chooser_widget, constructed,
			  (GObject *	object),
			  void)

static void
filechooser_treeview_search_column_changed	(GObject *	object,
						 GParamSpec *	pspec,
						 gpointer	data);

static void
filechooser_search_entry_map			(GtkWidget *	widget,
						 gpointer	data);

static void
filechooser_search_entry_unmap			(GtkWidget *	widget,
						 gpointer	data);


void dialogs_init ()
{
	{
		GtkSettingsClass * gtk_settings_class =
		 g_type_class_ref (GTK_TYPE_SETTINGS);

		INTERCEPT_CLASS_METHOD (gtk_settings, G_OBJECT_CLASS,
					constructed)

		g_type_class_unref (gtk_settings_class);
	}


	{
		GtkMessageDialogClass * gtk_message_dialog_class =
		 g_type_class_ref (GTK_TYPE_MESSAGE_DIALOG);

		INTERCEPT_CLASS_METHOD (gtk_message_dialog, G_OBJECT_CLASS,
					constructed)
		INTERCEPT_CLASS_METHOD (gtk_message_dialog, G_OBJECT_CLASS,
					notify);

		g_type_class_unref (gtk_message_dialog_class);
	}


	{
		GtkPrintUnixDialogClass * gtk_print_unix_dialog_class =
		 g_type_class_ref (GTK_TYPE_PRINT_UNIX_DIALOG);

		INTERCEPT_CLASS_METHOD (gtk_print_unix_dialog, G_OBJECT_CLASS,
					constructed)

		g_type_class_unref (gtk_print_unix_dialog_class);
	}


	{
		GtkFileChooserWidgetClass * gtk_file_chooser_widget_class =
		 g_type_class_ref (GTK_TYPE_FILE_CHOOSER_WIDGET);

		INTERCEPT_CLASS_METHOD (gtk_file_chooser_widget,
					G_OBJECT_CLASS, constructed)

		g_type_class_unref (gtk_file_chooser_widget_class);
	}
}


static void
new_gtk_settings_constructed
(GObject *	object)
{
	CALL_ORIGINAL_CLASS_METHOD (gtk_settings, constructed, (object));

	g_object_set (object, "gtk-dialogs-use-header", FALSE, NULL);
}


static void
new_gtk_message_dialog_constructed
(GObject *	object)
{
	GtkWidget *		internal_vbox;
	GtkWidget *		box;
	GtkWidget *		message_area;
	GtkWidget *		label;
	GtkWidget *		secondary_label;
	GtkWidget *		action_area;


	CALL_ORIGINAL_CLASS_METHOD (gtk_message_dialog, constructed, (object));

	apply_dialog_css (GTK_WIDGET (object));

	gtk_container_set_border_width (GTK_CONTAINER (object), 5);

	internal_vbox = find_widget_by_name (GTK_WIDGET (object),
					     "dialog-vbox1");
	gtk_container_set_border_width (GTK_CONTAINER (internal_vbox), 2);
	gtk_box_set_spacing (GTK_BOX (internal_vbox), 8);

	box = find_widget_by_name (internal_vbox, "box");
	gtk_widget_set_margin_start (box, 2);
	gtk_widget_set_margin_end (box, 2);
	gtk_container_set_border_width (GTK_CONTAINER (box), 5);
	gtk_box_set_spacing (GTK_BOX (box), 12);

	message_area = find_widget_by_name (box, "message_area");
	gtk_box_set_spacing (GTK_BOX (message_area), 12);

	label = find_widget_by_name (message_area, "label");
	secondary_label = find_widget_by_name (message_area,
					       "secondary_label");
	gtk_widget_set_halign (label, GTK_ALIGN_START);
	gtk_widget_set_halign (secondary_label, GTK_ALIGN_START);
	gtk_label_set_xalign (GTK_LABEL (label), 0);
	gtk_label_set_xalign (GTK_LABEL (secondary_label), 0);

	action_area = find_widget_by_name (GTK_WIDGET (object), "action_area");
	gtk_container_set_border_width (GTK_CONTAINER (action_area), 5);
	gtk_box_set_spacing (GTK_BOX (action_area), 6);
	gtk_button_box_set_layout (GTK_BUTTON_BOX (action_area),
				   GTK_BUTTONBOX_END);
}


static void
new_gtk_message_dialog_notify
(GObject *	object,
 GParamSpec *	pspec)
{
	GtkMessageType	message_type;
	const char *	icon_name;
	GtkWidget *	image;
	GtkMessageType	image_message_type;


	CALL_ORIGINAL_CLASS_METHOD (gtk_message_dialog,
				    notify, (object, pspec));


	if (strcmp (pspec->name, "message-type") != 0)
		return;

	g_object_get (object, "message-type", &message_type, NULL);
	g_object_get (object, "image", &image, NULL);

	if (image != NULL)
	{
		image_message_type = GPOINTER_TO_INT (
		 g_object_get_data (G_OBJECT (image), "message-type")) - 1;
		if (image_message_type == message_type)
			return;
	}

	switch (message_type)
	{
	case GTK_MESSAGE_INFO:
		icon_name = "dialog-information";
		break;
	case GTK_MESSAGE_WARNING:
		icon_name = "dialog-warning";
		break;
	case GTK_MESSAGE_QUESTION:
		icon_name = "dialog-question";
		break;
	case GTK_MESSAGE_ERROR:
		icon_name = "dialog-error";
		break;
	default:
		return;
	}

	image = gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_DIALOG);
	gtk_widget_show (image);
	g_object_set_data (G_OBJECT (image), "message-type",
			   GINT_TO_POINTER (message_type + 1));

	g_object_freeze_notify (object);
	g_object_set (object, "image", image, NULL);
	g_object_set (object, "message-type", message_type, NULL);
	g_object_thaw_notify (object);
}


static void
new_gtk_print_unix_dialog_constructed
(GObject *	object)
{
	GtkWidget *		internal_vbox;
	GtkWidget *		box;
	GtkWidget *		notebook;


	CALL_ORIGINAL_CLASS_METHOD (gtk_print_unix_dialog,
				    constructed, (object));

	apply_dialog_css (GTK_WIDGET (object));

	gtk_container_set_border_width (GTK_CONTAINER (object), 5);

	internal_vbox = find_widget_by_name (GTK_WIDGET (object),
					     "dialog-vbox1");
	gtk_container_set_border_width (GTK_CONTAINER (internal_vbox), 2);

	box = find_widget_by_name (internal_vbox, "box1");
	gtk_container_set_border_width (GTK_CONTAINER (box), 5);

	notebook = find_widget_by_name (box, "notebook");
	gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook), TRUE);
}


static void
new_gtk_file_chooser_widget_constructed
(GObject *	object)
{
	GtkWidget *	treeview;
	GtkWidget *	search_entry;


	CALL_ORIGINAL_CLASS_METHOD (gtk_file_chooser_widget,
				    constructed, (object));


	apply_dialog_css (GTK_WIDGET (object));


	treeview = find_widget_by_name (GTK_WIDGET (object),
					"browse_files_tree_view");

	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (treeview), TRUE);

	g_signal_connect (G_OBJECT (treeview), "notify::search-column",
	 G_CALLBACK (filechooser_treeview_search_column_changed), NULL);

	search_entry = find_widget_by_name (GTK_WIDGET (object),
					    "search_entry");

	g_signal_connect (G_OBJECT (search_entry), "map",
			  G_CALLBACK (filechooser_search_entry_map),
			  treeview);

	g_signal_connect (G_OBJECT (search_entry), "unmap",
			  G_CALLBACK (filechooser_search_entry_unmap),
			  treeview);
}


static void
filechooser_treeview_search_column_changed
(GObject *	object,
 GParamSpec *	pspec,
 gpointer	data)
{
	(void) pspec;
	(void) data;


	if (gtk_tree_view_get_search_column (GTK_TREE_VIEW (object)) == -1)
		gtk_tree_view_set_search_column (GTK_TREE_VIEW (object), 0);
}


static void filechooser_search_entry_map
(GtkWidget *	widget,
 gpointer	data)
{
	(void) widget;
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (data), FALSE);
}

static void filechooser_search_entry_unmap
(GtkWidget *	widget,
 gpointer	data)
{
	(void) widget;
	gtk_tree_view_set_enable_search (GTK_TREE_VIEW (data), TRUE);
}


/* Helper functions */

static void apply_dialog_css (GtkWidget * dialog)
{
	GdkScreen *		screen;
	gboolean		dialog_css_applied;
	GtkCssProvider *	provider;


	screen = gtk_widget_get_screen (dialog);
	dialog_css_applied =
	 GPOINTER_TO_INT (g_object_get_data (G_OBJECT (screen),
					     "dialog-css-applied"));
	if (! dialog_css_applied)
	{
		provider = gtk_css_provider_new ();
		gtk_css_provider_load_from_data (provider,
						 MESSAGE_DIALOG_CSS
						 PRINT_DIALOG_CSS
						 FILE_CHOOSER_CSS,
						 -1, NULL);
		gtk_style_context_add_provider_for_screen (screen,
		 GTK_STYLE_PROVIDER (provider),
		 GTK_STYLE_PROVIDER_PRIORITY_SETTINGS);
		g_object_set_data (G_OBJECT (screen),
				   "dialog-css-applied",
				   GINT_TO_POINTER (TRUE));
	}
}


static void add_widget_to_list (GtkWidget * widget, gpointer data)
{
	GSList **	list	= data;


	*list = g_slist_append (*list, widget);
}


static GtkWidget *
find_widget_by_name
(GtkWidget *	start,
 const char *	name)
{
	const char *	widget_name;
	GtkWidget *	widget;
	GSList *	list;
	GSList *	list_iter;


	if (GTK_IS_BUILDABLE (start))
	{
		widget_name = gtk_buildable_get_name (GTK_BUILDABLE (start));
		if (widget_name != NULL && strcmp (widget_name, name) == 0)
			return start;
	}

	if (GTK_IS_CONTAINER (start))
	{
		list = NULL;
		gtk_container_forall (GTK_CONTAINER (start),
				      add_widget_to_list,
				      &list);
		for (list_iter = list;
		     list_iter != NULL;
		     list_iter = list_iter->next)
		{
			widget =
			 find_widget_by_name (GTK_WIDGET (list_iter->data),
					      name);
			if (widget != NULL)
				break;
		}

		g_slist_free (list);
		return widget;
	}


	return NULL;
}
