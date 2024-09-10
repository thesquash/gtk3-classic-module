/* Smaller Widgets:
 * (based on gtk3-classic CSS 'smaller-adwaita.css', and gtk3-classic patch
 * 'appearance__smaller-statusbar.patch'):
 *
 * Reduces the size of certain widgets, especially when the built-in GTK themes
 * (Adwaita and HighContrast) are in use, in the spirit of more traditional
 * GTK themes for prior versions of GTK.
 */


#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>

#include <string.h>

#include <gtk3-classic.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic::Smaller-Widgets"


#define CSS \
 "button,\n" \
 "button.titlebutton,\n" \
 "button.image-button,\n" \
 "button.text-button,\n" \
 "button.image-button.text-button,\n" \
 ".stack-switcher > button.text-button {\n" \
 "	padding: 3px 6px;\n" \
 "	min-height: 20px;\n" \
 "	min-width: 0;\n" \
 "}\n" \
 ".stack-switcher > button.text-button {\n" \
 "	min-width: 80px;\n" \
 "}\n" \
 "button.text-button.image-button label {\n" \
 "	padding: 0 2px;\n" \
 "}\n" \
 "notebook > header.top tab,\n" \
 "notebook > header.bottom tab {\n" \
 "	padding: 0 5px;\n" \
 "}\n" \
 "notebook > header.left tab,\n" \
 "notebook > header.right tab {\n" \
 "	padding: 0 8px;\n" \
 "}\n" \
 "notebook tab button {\n" \
 "	padding: 0px;\n" \
 "	min-height: 15px;\n" \
 "}\n" \
 "entry,\n" \
 "spinbutton:not(.vertical) {\n" \
 "	min-height: 26px;\n" \
 "}\n" \
 "treeview button {\n" \
 "	padding: 0 7px;\n" \
 "	min-height: 23px;\n" \
 "}\n" \
 "progressbar.horizontal trough,\n" \
 "progressbar.horizontal progress,\n" \
 "progressbar.vertical trough,\n" \
 "progressbar.vertical progress,\n" \
 "levelbar trough {\n" \
 "	min-height: 9px;\n" \
 "	min-width: 9px;\n" \
 "}\n" \
 "scale {\n" \
 "	padding: 10px 0;\n" \
 "}\n" \
 "headerbar, switch slider {\n" \
 "	min-height: 0;\n" \
 "}\n" \
 "popover:not(.emoji-picker) > *:not(grid) {\n" \
 "	margin: -3px;\n" \
 "}\n" \
 "tooltip * {\n" \
 "	padding: 0px;\n" \
 "}\n" \
 "placessidebar row {\n" \
 "	min-height: 29px;\n" \
 "}\n" \
 "placessidebar row button.sidebar-button {\n" \
 "	min-height: 20px;\n" \
 "	min-width: 20px;\n" \
 "	padding: 0;\n" \
 "}\n" \
 "filechooser > box.search-bar, filechooser > box:last-child {\n" \
 "	border-bottom: 1px solid alpha(#000, 0.15);\n" \
 "}\n" \
 "filechooser > actionbar > revealer > box {\n" \
 "	border-top: 1px solid alpha(#000, 0.15);\n" \
 "}\n" \
 "* {\n" \
 "	-GtkDialog-action-area-border: 5;\n" \
 "}\n" \
 "messagedialog > box {\n" \
 "	padding: 7px;\n" \
 "}\n" \
 "messagedialog buttonbox {\n" \
 "	margin-top: -17px;\n" \
 "}\n" \
 "menu, .menu, .context-menu, tooltip {\n" \
 "	border-radius: 0;\n" \
 "}\n" \
 ".background:not(.csd):not(.solid-csd) .titlebar:not(.default-decoration),\n" \
 ".background:not(.csd):not(.solid-csd) .titlebar:not(.default-decoration) headerbar {\n" \
 "	background: inherit;\n" \
 "}\n" \
 "entry undershoot.left,\n" \
 "entry undershoot.right,\n" \
 "spinbutton:not(.vertical) undershoot.left,\n" \
 "spinbutton:not(.vertical) undershoot.right,\n" \
 "scrolledwindow undershoot.left,\n" \
 "scrolledwindow undershoot.right,\n" \
 "scrolledwindow undershoot.top,\n" \
 "scrolledwindow undershoot.bottom {\n" \
 "	background-image: none;\n" \
 "}\n"


INTERCEPTED_CLASS_METHOD (gtk_settings, notify,
			  (GObject *	object,
			   GParamSpec *	pspec),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_window, constructed,
			  (GObject *	object),
			  void)


INTERCEPTED_CLASS_METHOD (gtk_statusbar, constructed,
			  (GObject *	object),
			  void)


void smaller_widgets_init ()
{
	{
		GtkSettingsClass * gtk_settings_class =
		 g_type_class_ref (GTK_TYPE_SETTINGS);

		INTERCEPT_CLASS_METHOD (gtk_settings, G_OBJECT_CLASS,
					notify)

		g_type_class_unref (gtk_settings_class);
	}


	{
		GtkWindowClass * gtk_window_class =
		 g_type_class_ref (GTK_TYPE_WINDOW);

		INTERCEPT_CLASS_METHOD (gtk_window, G_OBJECT_CLASS,
					constructed)

		g_type_class_unref (gtk_window_class);
	}


	{
		GtkStatusbarClass * gtk_statusbar_class =
		 g_type_class_ref (GTK_TYPE_STATUSBAR);

		INTERCEPT_CLASS_METHOD (gtk_statusbar, G_OBJECT_CLASS,
					constructed)

		g_type_class_unref (gtk_statusbar_class);
	}
}


static void
new_gtk_settings_notify
(GObject *	object,
 GParamSpec *	pspec)
{
	const char *		theme_name;
	GdkScreen *		screen;
	GtkCssProvider *	provider;


	CALL_ORIGINAL_CLASS_METHOD (gtk_settings, notify, (object, pspec));


	if (strcmp (pspec->name, "gtk-theme-name") != 0)
		return;

	g_object_get (object, "gtk-theme-name", &theme_name, NULL);

	screen = g_object_get_data (object, "screen");
	if (screen == NULL)
		return;

	provider = g_object_get_data (G_OBJECT (screen),
				      "smaller-adwaita-css");
	if (provider == NULL)
		return;

	if (strcmp (theme_name, "Adwaita") == 0 ||
	    strcmp (theme_name, "Adwaita-Dark") == 0 ||
	    strcmp (theme_name, "HighContrast") == 0 ||
	    strcmp (theme_name, "HighContrastInverse") == 0)
		gtk_style_context_add_provider_for_screen (screen,
		 GTK_STYLE_PROVIDER (provider),
		 GTK_STYLE_PROVIDER_PRIORITY_SETTINGS);
	else
		gtk_style_context_remove_provider_for_screen (screen,
		 GTK_STYLE_PROVIDER (provider));
}


static void
new_gtk_window_constructed
(GObject *	object)
{
	GdkScreen *		screen;
	GtkCssProvider *	provider;
	GtkSettings *		settings;


	CALL_ORIGINAL_CLASS_METHOD (gtk_window, constructed, (object));


	screen = gtk_widget_get_screen (GTK_WIDGET (object));
	provider = g_object_get_data (G_OBJECT (screen),
				      "smaller-adwaita-css");
	if (provider == NULL)
	{
		provider = gtk_css_provider_new ();
		gtk_css_provider_load_from_data (provider, CSS, -1, NULL);
		g_object_set_data (G_OBJECT (screen),
				   "smaller-adwaita-css",
				   provider);

		settings = gtk_settings_get_for_screen (screen);
		g_object_set_data (G_OBJECT (settings), "screen", screen);
		g_object_notify (G_OBJECT (settings),
				 "gtk-theme-name");
	}

}


static void
new_gtk_statusbar_constructed
(GObject *	object)
{
	GtkStyleContext *	style_context;


	CALL_ORIGINAL_CLASS_METHOD (gtk_statusbar, constructed, (object));


	style_context = gtk_widget_get_style_context (GTK_WIDGET (object));
	gtk_style_context_add_class (style_context, "classic");

	gtk_widget_set_margin_top (GTK_WIDGET (object), 1);
	gtk_widget_set_margin_bottom (GTK_WIDGET (object), 1);
}
