/* No Emojis:
 * (from gtk3-classic patch 'other__hide-insert-emoji.patch')
 *
 * Hides the 'Insert Emoji' menu item found at the bottom of the context menus
 * for GtkEntry and GtkTextView widgets.
 *
 * Set the environment variable GTKM_INSERT_EMOJI=1 to restore the menu items.
 */


#include <gtk/gtk.h>
#include <glib.h>

#include <stdlib.h>
#include <string.h>

#include <gtk3-classic.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic::No-Emojis"


INTERCEPTED_CLASS_METHOD (gtk_menu_item, show,
			  (GtkWidget *	widget),
			  void)


void no_emojis_init ()
{
	GtkMenuItemClass * gtk_menu_item_class =
	 g_type_class_ref (GTK_TYPE_MENU_ITEM);

	INTERCEPT_CLASS_METHOD (
	 gtk_menu_item, GTK_WIDGET_CLASS, show)

	g_type_class_unref (gtk_menu_item_class);
}


static void
new_gtk_menu_item_show
(GtkWidget *	widget)

{
	const char *	gtkm_insert_emoji;
	const char *	label;


	CALL_ORIGINAL_CLASS_METHOD (gtk_menu_item, show,
				    (widget));


	gtkm_insert_emoji = getenv ("GTKM_INSERT_EMOJI");
	if (gtkm_insert_emoji != NULL && strcmp (gtkm_insert_emoji, "1") == 0)
		return;

	label = gtk_menu_item_get_label (GTK_MENU_ITEM (widget));
	if (label != NULL && strcmp (label, _("Insert _Emoji")) == 0)
		gtk_widget_hide (widget);
}
