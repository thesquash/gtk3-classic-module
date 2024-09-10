/* Persistent mnemonics:
 * (from 'sgm-flexible-mnemonics.c' [https://github.com/thesquash/sgm])
 *
 * Restores the behaviour of the GTK setting 'gtk-auto-mnemonics'.  When this
 * setting is set (TRUE, the default), mnemonic letters on buttons and menu
 * items will be underlined only while the <Alt> key is pressed; when unset
 * (FALSE), mnemonics will always be underlined.
 *
 * Enable the latter behaviour by adding to your settings.ini file:
 *
 *    gtk-auto-mnemonics = 0
 */


#include <gtk/gtk.h>
#include <glib.h>

#include <gtk3-classic.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic::Persistent-Mnemonics"


INTERCEPTED_CLASS_METHOD (gtk_window, notify,
			  (GObject *	object,
			   GParamSpec *	pspec),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_menu_shell, deactivate,
			  (GtkMenuShell *	menu_shell),
			  void)


void persistent_mnemonics_init ()
{
	{
		GtkWindowClass * gtk_window_class =
		 g_type_class_ref (GTK_TYPE_WINDOW);

		INTERCEPT_CLASS_METHOD (gtk_window, G_OBJECT_CLASS, notify)

		g_type_class_unref (gtk_window_class);
	}


	{
		GtkMenuShellClass * gtk_menu_shell_class =
		 g_type_class_ref (GTK_TYPE_MENU_SHELL);

		INTERCEPT_CLASS_METHOD (gtk_menu_shell, GTK_MENU_SHELL_CLASS,
					deactivate)

		g_type_class_unref (gtk_menu_shell_class);
	}
}


static void
new_gtk_window_notify
(GObject *	object,
 GParamSpec *	pspec)
{
	GtkSettings *	settings;
	gboolean	auto_mnemonics;


	CALL_ORIGINAL_CLASS_METHOD (gtk_window, notify, (object, pspec));

	if (strcmp (pspec->name, "mnemonics-visible") != 0)
		return;

	if (! gtk_window_get_mnemonics_visible (GTK_WINDOW (object)))
	{
		settings = gtk_widget_get_settings (GTK_WIDGET (object));
		g_object_get (settings,
			      "gtk-auto-mnemonics", &auto_mnemonics,
			      NULL);
		if (! auto_mnemonics)
			gtk_window_set_mnemonics_visible (GTK_WINDOW (object),
							  TRUE);
	}
}


static void
new_gtk_menu_shell_deactivate
(GtkMenuShell *	menu_shell)
{
	GtkWidget *	toplevel;


	CALL_ORIGINAL_CLASS_METHOD (gtk_menu_shell, deactivate, (menu_shell));


	toplevel = gtk_widget_get_toplevel (GTK_WIDGET (menu_shell));
	if (toplevel)
		g_object_notify (G_OBJECT (toplevel), "mnemonics-visible");
}
