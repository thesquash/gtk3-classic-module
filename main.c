#include <glib.h>
#include <gmodule.h>


#undef  G_LOG_DOMAIN
#define G_LOG_DOMAIN	"Gtk3-Classic-Module"


typedef void (* InitFunc) ();

void	button_menu_icons_init ();
void	dialogs_init ();
void	icon_sizes_init ();
void	no_emojis_init ();
void	persistent_mnemonics_init ();
void	smaller_widgets_init ();
void	treeview_zebra_stripes_init ();

static const InitFunc	init_funcs[] =
{
	treeview_zebra_stripes_init,
	smaller_widgets_init,
	/* No Emojis must be loaded before Button/Menu Icons since the former
	 * overrides GtkMenuItem, the superclass of the GtkImageMenuItem class
	 * which Button/Menu Icons taps into.
	 */
	no_emojis_init,
	button_menu_icons_init,
	persistent_mnemonics_init,
	dialogs_init,
	icon_sizes_init,
	NULL
};


G_MODULE_EXPORT
const char * g_module_check_init (GModule *module)
{
	g_module_make_resident (module);
	return NULL;
}

G_MODULE_EXPORT
int gtk_module_init (gint * argc, char *** argv)
{
	int	i;


	(void) argc;
	(void) argv;


	for (i = 0; init_funcs[i] != NULL; i++)
		init_funcs[i] ();


	return 0;
}
